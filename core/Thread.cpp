//***************************************************************
//  File:    Thread.cpp
//  Data:    03/08/2009
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
#include "Thread.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#include <windows.h>
#endif

namespace hare_core
{
#if HARE_PLATFORM == HARE_PLATFORM_WIN32
    
    //---------------------------------------------------------------------
    CriticalSection::CriticalSection()
    {
        privateData = new CRITICAL_SECTION;
        InitializeCriticalSection(static_cast<CRITICAL_SECTION*>(privateData));
    }

    CriticalSection::~CriticalSection()
    {
        DeleteCriticalSection(static_cast<CRITICAL_SECTION*>(privateData));
        delete static_cast<CRITICAL_SECTION*>(privateData);
        privateData = 0;
    }

    void CriticalSection::enter()
    {
        EnterCriticalSection(static_cast<CRITICAL_SECTION*>(privateData));
    }

    void CriticalSection::leave()
    {
        LeaveCriticalSection(static_cast<CRITICAL_SECTION*>(privateData));
    }

    //---------------------------------------------------------------------
    static MutexError mutexLockTimeout(HANDLE mutex, DWORD milliseconds)
    {
        DWORD rc = WaitForSingleObject(mutex, milliseconds);

        if (rc == WAIT_ABANDONED)
        {
            // the previous caller died without releasing the mutex, but now we can
            // really lock it
            // use 0 timeout, normally we should always get it
            rc = WaitForSingleObject(mutex, 0);
        }

        switch (rc)
        {
        case WAIT_OBJECT_0:
            break;

        case WAIT_TIMEOUT:
            return MUTEX_BUSY;

        case WAIT_FAILED:
        case WAIT_ABANDONED:        // checked for above
        default:
            return MUTEX_ERROR;
        }

        return MUTEX_NO_ERROR;
    }

    Mutex::Mutex()
    {
        privateData = CreateMutex(NULL, FALSE, NULL);
    }

    Mutex::~Mutex()
    {
        CloseHandle(static_cast<HANDLE>(privateData));
        privateData = 0;
    }

    bool Mutex::isOk() const
    {
        return privateData != 0;
    }

    MutexError Mutex::lock()
    {
        return mutexLockTimeout(static_cast<HANDLE>(privateData), INFINITE);
    }

    MutexError Mutex::tryLock()
    {
        return mutexLockTimeout(static_cast<HANDLE>(privateData), 0);
    }

    MutexError Mutex::unlock()
    {
        if (!ReleaseMutex(static_cast<HANDLE>(privateData)))
        {
            return MUTEX_ERROR;
        }

        return MUTEX_NO_ERROR;
    }

    //---------------------------------------------------------------------
    Semaphore::Semaphore(int initialCount, int maxCount)
    {
        privateData = CreateSemaphore(NULL, initialCount, maxCount, NULL);
    }

    Semaphore::~Semaphore()
    {
        if (privateData)
        {
            CloseHandle(static_cast<HANDLE>(privateData));
            privateData = 0;
        }
    }

    bool Semaphore::isOk() const
    {
        return privateData != 0;
    }

    SemaphoreError Semaphore::wait()
    {
        return waitTimeout(INFINITE);
    }

    SemaphoreError Semaphore::tryWait()
    {
        SemaphoreError rc = waitTimeout(0);
        if (rc == SEMA_TIMEOUT)
            rc = SEMA_BUSY;

        return rc;
    }

    SemaphoreError Semaphore::waitTimeout(u32 milliseconds)
    {
        DWORD rc = WaitForSingleObject(static_cast<HANDLE>(privateData), milliseconds);

        switch (rc)
        {
        case WAIT_OBJECT_0:
            return SEMA_NO_ERROR;

        case WAIT_TIMEOUT:
            return SEMA_TIMEOUT;
        }

        return SEMA_ERROR;
    }

    SemaphoreError Semaphore::post()
    {
        if (!ReleaseSemaphore(static_cast<HANDLE>(privateData), 1, NULL))
        {
            if (GetLastError() == ERROR_TOO_MANY_POSTS)
            {
                return SEMA_OVERFLOW;
            }
            else
            {
                return SEMA_ERROR;
            }
        }

        return SEMA_NO_ERROR;
    }

#endif

    class ConditionData
    {
    public:
        ConditionData(Mutex& mutex) 
            : numWaiters(0), mutexRef(mutex) {}
    public:    
        int numWaiters;
        CriticalSection csWaiters;
        Mutex& mutexRef;
        Semaphore semaphore;
    };

    Condition::Condition(Mutex& mutex)
    {
        ConditionData* data = new ConditionData(mutex);
        privateData = data;
    }

    Condition::~Condition()
    {
        delete static_cast<ConditionData*>(privateData);
        privateData = 0;
    }

    bool Condition::isOk() const
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);
        return (data && data->mutexRef.isOk() && data->semaphore.isOk());
    }

    ConditionError Condition::wait()
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);

        {
            CriticalSectionLocker lock(data->csWaiters);
            data->numWaiters++;
        }

        data->mutexRef.unlock();
        SemaphoreError err = data->semaphore.wait();
        data->mutexRef.lock();

        if (err == SEMA_NO_ERROR)
            return COND_NO_ERROR;
        else if (err == SEMA_TIMEOUT)
            return COND_TIMEOUT;
        else
            return COND_ERROR;
    }

    ConditionError Condition::waitTimeout(u32 milliseconds)
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);

        {
            CriticalSectionLocker lock(data->csWaiters);
            data->numWaiters++;
        }

        data->mutexRef.unlock();
        SemaphoreError err = data->semaphore.waitTimeout(milliseconds);

        if (err == SEMA_TIMEOUT)
        {
            CriticalSectionLocker lock(data->csWaiters);

            SemaphoreError err2 = data->semaphore.waitTimeout(0);

            if (err2 != SEMA_NO_ERROR)
            {
                data->numWaiters--;
            }
        }

        data->mutexRef.lock();

        return err == SEMA_NO_ERROR ? COND_NO_ERROR
            : err == SEMA_TIMEOUT ? COND_TIMEOUT
            : COND_ERROR;
    }

    ConditionError Condition::signal()
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);

        CriticalSectionLocker lock(data->csWaiters);

        if (data->numWaiters > 0)
        {
            if (data->semaphore.post() != SEMA_NO_ERROR)
                return COND_ERROR;

            data->numWaiters--;
        }

        return COND_NO_ERROR;
    }

    ConditionError Condition::broadcast()
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);

        CriticalSectionLocker lock(data->csWaiters);

        while (data->numWaiters > 0)
        {
            if (data->semaphore.post() != SEMA_NO_ERROR)
                return COND_ERROR;

            data->numWaiters--;
        }

        return COND_NO_ERROR;
    }

}




