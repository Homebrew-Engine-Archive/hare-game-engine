//***************************************************************
//  File:    Socket.h
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
#ifndef SOCKET_H
#define SOCKET_H

#include "CorePrerequisites.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
    typedef int socklen_t;
    #include <winsock.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <cerrno>
    #include <netdb.h>
    #include <arpa/inet.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR   -1
#endif

// This is the MSW version of SHUT_RDWR for ::shutdown(sock, how=SHUT_RDWR)
// Note that these are defined in winsock2.h, but if you try to include it
// you get errors about redefinitions since it includes winsock.h anyway
#if !defined(SD_RECEIVE) && defined(SHUT_RD)
    #define SD_RECEIVE      SHUT_RD
    #define SD_SEND         SHUT_WR
    #define SD_BOTH         SHUT_RDWR
#elif !defined(SD_RECEIVE)
    #define SD_RECEIVE      0
    #define SD_SEND         1
    #define SD_BOTH         2
#endif // SD_RECEIVE

namespace hare
{
    class CORE_API ISocket
    {
    public:
        ISocket() : portNumber(-1)
        {}

        virtual ~ISocket()
        {}

        // Get the port used for the socket, -1 if not initialized
        int32 getPortNumber() const
        {
            return portNumber;
        }

        // Get the address of the socket, empty string if not initialized
        String getAddress() const
        {
            return address;
        }

        // Is the socket currently connected
        virtual bool isConnected() = 0;

        // Read the number of bytes length into buffer from the socket
        //  the buffer must be large enough to hold the data.
        virtual int read(char *buffer, uint32 length) = 0;

        // Write the whole buffer of number of bytes length to the socket
        virtual int write(const char *buffer, uint32 length) = 0;

        virtual String getErrorMsg(bool clear);

        void appendErrorMsg(const String& msg);

        virtual String getLastErrorMsg() const = 0;

    protected:
        String errorMsg;
        String address;         // The address for the socket to use
        int32 portNumber;         // The port that's used, else -1
    };

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
    typedef SOCKET socket_type;
#else
    typedef int socket_type;
#endif

    class CORE_API Socket : public ISocket
    {
    public:

        enum SocketState
        {
            SOCKET_Closed,
            SOCKET_Listening,
            SOCKET_Accepted,
            SOCKET_Connected,
        };

        Socket();
        // Socket constructor from an accepted socket
        Socket(socket_type socket, sockaddr_in addr);

        virtual ~Socket();

        // Create a listening socket, using the specified port number
        //   server: bind and listen for client connections
        bool listen(uint16 port, int backLog = 100);

        // Accept a connection, returning an accepted socket.
        //   server: block until accepting a connection from a client
        Socket* accept();

        // Connect to a given host and port number
        //   client: connect a client to a server
        bool connect(const String &address, uint16 port);

        // Get the socket state
        SocketState getState() const { return sockState; }

        // Get the socket handle
        socket_type getSocket() const { return sock; }

        // Shutdown the socket in an orderly fashion, ::shutdown(sock, how)
        //  returns true on success
        bool shutdown(int how = SD_BOTH);

        // Close the open socket, returns true on success
        bool close();

        // Is the socket connected?
        //   Overridden from wxLuaSocketBase
        virtual bool isConnected()
        {
            return ((sockState == SOCKET_Connected) || (sockState == SOCKET_Accepted));
        }

        // Read the whole buffer of byte size length into buffer from the socket
        virtual int read(char *buffer, uint32 length);

        // Write the whole buffer of byte size length to the socket
        virtual int write(const char *buffer, uint32 length);

        // Get the last/current error message using the system error
        //   either errno in Unix or WSAGetLastError in MSW, doesn't clear error
        virtual String getLastErrorMsg() const;

    protected:
        socket_type  sock;
        sockaddr_in  sockAddress;
        SocketState  sockState;

    private:
        HARE_DECLARE_NO_COPY_CLASS(Socket)
    };
}

#endif
