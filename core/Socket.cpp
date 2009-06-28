//***************************************************************
//  File:    Socket.cpp
//  Data:    02/19/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "Socket.h"
#include "StringUtil.h"

#ifndef INADDR_NONE
#   define INADDR_NONE 0xffffffff
#endif

namespace hare
{
    // ----------------------------------------------------------------------------
    // Socket
    // ----------------------------------------------------------------------------

    Socket::Socket() : portNumber(-1), sock(0), sockState(SOCKET_Closed)
    {
        memset(&sockAddress, 0, sizeof(sockAddress));
    }

    Socket::Socket(socket_type socket, sockaddr_in addr)
    : sock(socket), sockAddress(addr), sockState(SOCKET_Accepted)
    {
        address = inet_ntoa(sockAddress.sin_addr);
        portNumber = ntohs(sockAddress.sin_port);
    }

    Socket::~Socket()
    {
        // close the socket if not already closed, don't bother with errors
        //  since we should have shut down nicely unless the program is terminating
        if (sockState != SOCKET_Closed)
        {
#if HARE_PLATFORM == HARE_PLATFORM_WIN32
            ::closesocket(sock);
#else
            ::close(sock);
#endif // HARE_PLATFORM == HARE_PLATFORM_WIN32
        }
    }

    String Socket::getErrorMsg(bool clear)
    {
        String str(errorMsg);
        if (clear)
            errorMsg.clear();

        return str;
    }

    void Socket::appendErrorMsg(const String& msg)
    {
        String s(msg);

        if (address.length() != 0)
            s += StringUtil::format(" Address '%s'.", address.c_str());
        if (portNumber > 0)
            s += StringUtil::format(" Port %d.", portNumber);

        String lastErrorMsg = getLastErrorMsg();
        if (lastErrorMsg.length() > 0)
            s += ("\n") + lastErrorMsg;

        if (errorMsg.length() > 0)
            errorMsg += "\n\n";

        errorMsg += s;
    }

    bool Socket::listen(uint16 port, int backLog)
    {
        portNumber = port;

        if (sockState != SOCKET_Closed)
        {
            appendErrorMsg("Failed to create a listening socket, socket already open.");
            return false;
        }

        sock = ::socket(AF_INET, SOCK_STREAM, 0);

        if (sock == INVALID_SOCKET)
        {
            appendErrorMsg("Unable to create a listening socket.");
            return false;
        }

        sockaddr_in localAddr = { 0 };

        localAddr.sin_family      = AF_INET;
        localAddr.sin_port        = htons(portNumber);
        localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (::bind(sock, (sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR)
        {
            appendErrorMsg("Unable to bind to socket to listen for clients.");
            return false;
        }

        if (::listen(sock, backLog) == SOCKET_ERROR)
        {
            appendErrorMsg("Unable to listen to bound socket.");
            return false;
        }

        memset(&sockAddress, 0, sizeof(sockAddress));
        sockState = SOCKET_Listening;

        return true;
    }

    Socket* Socket::accept()
    {
        if (sockState != SOCKET_Listening)
        {
            appendErrorMsg("Unable to accept from a socket that's not listening.");
            return NULL;
        }

        sockaddr_in fromAddr = { 0 };
        socklen_t length = sizeof(fromAddr);

        socket_type acceptedSocket = ::accept(sock, (sockaddr *)&fromAddr, &length);
        if (acceptedSocket == INVALID_SOCKET)
        {
            appendErrorMsg("Unable to accept socket connection.");
            return NULL;
        }

        return new Socket(acceptedSocket, fromAddr);
    }

    bool Socket::connect(const String &addr, uint16 port)
    {
        portNumber = port;
        hostent *host = NULL;

        if (sockState != SOCKET_Closed)
        {
            appendErrorMsg(StringUtil::format("Unable to connect to addr '%s' socket already open.", addr.c_str()));
            return false;
        }

        sock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET)
        {
            appendErrorMsg(StringUtil::format("Unable to create client socket for addr '%s'.", addr.c_str()));
            return false;
        }

        host = ::gethostbyname(addr.c_str());

        if (host == NULL)
        {
            unsigned long address_l = ::inet_addr(addr.c_str());
            if (address_l != INADDR_NONE)
                host = ::gethostbyaddr((const char*)&address_l, 4, AF_INET);
        }

        if (host == NULL)
        {
            appendErrorMsg(StringUtil::format("Unable to get hostbyaddr or gethostbyname for addr '%s'.", addr.c_str()));
            return false;
        }

        if (host->h_addrtype != AF_INET)
        {
            appendErrorMsg(StringUtil::format("Socket for addr '%s' is wrong type, isn't AF_INET.", addr.c_str()));
            return false;
        }

        memset(&sockAddress, 0, sizeof(sockAddress));
        memcpy(&(sockAddress.sin_addr), host->h_addr_list[0], host->h_length);

        sockAddress.sin_family = AF_INET;
        sockAddress.sin_port   = htons(portNumber);

        address = inet_ntoa(sockAddress.sin_addr);
        portNumber = ntohs(sockAddress.sin_port);

        if (::connect(sock, (sockaddr*)&sockAddress, sizeof(sockAddress)) == SOCKET_ERROR)
        {
            appendErrorMsg(StringUtil::format("Unable to connect socket to addr '%s'.", addr.c_str()));
            return false;
        }

        sockState = SOCKET_Connected;
        return true;
    }

    // Write data to an open socket, repeat until all data has been sent.
    int Socket::write(const char *buffer, uint32 length)
    {
        if ((sockState != SOCKET_Connected) && (sockState != SOCKET_Accepted))
        {
            appendErrorMsg("Unable to write to unconnected or unaccepted socket. ");
            return 0;
        }

        uint32 len = length;
        const char *buf = buffer;
        uint32 num_written = 0;

        while (num_written < len)
        {
            int s = ::send(sock, buf, len - num_written, 0);
            if (s == SOCKET_ERROR)
            {
                appendErrorMsg("Got a socket error trying to write to socket.");
                return num_written;
            }

            num_written += s;
            buf += s;
        }

        return num_written;
    }

    // Read data from an open socket, repeat reading until all data has been read
    int Socket::read(char *buffer, uint32 length)
    {
        if ((sockState != SOCKET_Connected) && (sockState != SOCKET_Accepted))
        {
            appendErrorMsg("Unable to read from an unconnected or unaccepted socket.");
            return 0;
        }

        int len = length;
        char *buf = buffer;
        int num_read = 0;

        while (num_read < len)
        {
            int r = ::recv(sock, buf, len - num_read, 0);
            if (r == 0)
                return num_read;

            if (r == SOCKET_ERROR)
            {
                appendErrorMsg("Got a socket error trying to read.");
                return num_read;
            }

            num_read += r;
            buf += r;
        }

        return num_read;
    }

    bool Socket::shutdown(int how)
    {
        if (sockState != SOCKET_Closed)
        {
            return ::shutdown(sock, how) == 0;
        }

        return false;
    }

    bool Socket::close()
    {
        if (sockState != SOCKET_Closed)
        {
#if HARE_PLATFORM == HARE_PLATFORM_WIN32
            if (::closesocket(sock) == SOCKET_ERROR)
            {
                appendErrorMsg("Unable to close socket.");
                return false;
            }
#else
            if (::close(sock))
            {
                appendErrorMsg("Unable to close socket.");
                return false;
            }
#endif
            else
            {
                sockState = SOCKET_Closed;
                return true;
            }
        }

        return false;
    }

    String Socket::getLastErrorMsg() const
    {
        String str;
        int errnum = 0;

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
        errnum = ::WSAGetLastError();
        switch (errnum)
        {
        case WSANOTINITIALISED:
            str = "A successful WSAStartup must occur before using this function.";
            break;
        case WSAENETDOWN:
            str = "The network subsystem or the associated service provider has failed.";
            break;
        case WSAEAFNOSUPPORT:
            str = "The specified address family is not supported.";
            break;
        case WSAEINPROGRESS:
            str = "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
            break;
        case WSAEMFILE:
            str = "No more socket descriptors are available.";
            break;
        case WSAENOBUFS:
            str = "No buffer space is available. The socket cannot be created.";
            break;
        case WSAEPROTONOSUPPORT:
            str = "The specified protocol is not supported.";
            break;
        case WSAEPROTOTYPE:
            str = "The specified protocol is the wrong type for this socket.";
            break;
        case WSAESOCKTNOSUPPORT:
            str = "The specified socket type is not supported in this address family.";
            break;
        }

#else
        str = strerror(errno);
        errnum = errno;

#endif
        if (str.empty())
            str = "Unknown Socket Error.";

        str = StringUtil::format("Socket Error %d : '%s'", errnum, str.c_str());

        return str;
    }
}

