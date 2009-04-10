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
#   include <windows.h>
#   include <process.h>
#   define THREAD_CALLCONV __stdcall
typedef unsigned THREAD_RETVAL;
#elif HARE_PLATFORM == HARE_PLATFORM_LINUX
#   include <errno.h>
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

    //---------------------------------------------------------------------
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

    //---------------------------------------------------------------------
    enum ThreadState
    {
        STATE_NEW,          // didn't start execution yet (=> RUNNING)
        STATE_RUNNING,      // thread is running (=> PAUSED, CANCELED)
        STATE_PAUSED,       // thread is temporarily suspended (=> RUNNING)
        STATE_CANCELED,     // thread should terminate a.s.a.p. (=> EXITED)
        STATE_EXITED        // thread is terminating
    };

    class ThreadData
    {
    public:
        ThreadData() : handle(0), state(STATE_NEW), id(0)
        {
        }

        static THREAD_RETVAL THREAD_CALLCONV threadEntry(void* param)
        {
            Thread* thread = (Thread*)param;

            DWORD exitCode = (DWORD)thread->entry();

            thread->onExit();

            CriticalSectionLocker lock(thread->cs);
            thread->privateData->state = STATE_EXITED;

            return exitCode;
        }

    public:
        HANDLE handle;
        ThreadState state;
        u32 id;
    };

    Thread::Thread()
    {
        privateData = new ThreadData();
    }

    Thread::~Thread()
    {
        delete privateData;
        privateData = 0;
    }

    /* static */
    void Thread::sleep(u32 milliseconds)
    {
        ::Sleep(milliseconds);
    }

    ThreadError Thread::create(u32 stackSize)
    {
        CriticalSectionLocker lock(cs);

        privateData->handle = (HANDLE)_beginthreadex(NULL, stackSize, ThreadData::threadEntry, this,
            CREATE_SUSPENDED, (u32*)&privateData->id);

        if (!privateData->handle)
            return THREAD_NO_RESOURCE;

        return THREAD_NO_ERROR;
    }

    ThreadError Thread::run()
    {
        CriticalSectionLocker lock(cs);

        if (privateData->state != STATE_NEW)
        {
            return THREAD_RUNNING;
        }

        return resume();
    }

    ThreadError Thread::kill()
    {
        if (!isRunning())
            return THREAD_NOT_RUNNING;

        if (!TerminateThread(privateData->handle, (DWORD)-1))
        {
            return THREAD_ERROR;
        }

        CloseHandle(privateData->handle);
        privateData->handle = 0;
        privateData->state = STATE_EXITED;

        return THREAD_NO_ERROR;
    }

    ThreadError Thread::pause()
    {
        CriticalSectionLocker lock(cs);

        DWORD num = SuspendThread(privateData->handle);
        if (num == (DWORD)-1)
        {
            return THREAD_ERROR;
        }

        privateData->state = STATE_PAUSED;

        return THREAD_NO_ERROR;
    }

    ThreadError Thread::resume()
    {
        CriticalSectionLocker lock(cs);

        DWORD num = ResumeThread(privateData->handle);
        if (num == (DWORD)-1)
        {
            return THREAD_ERROR;
        }

        if (privateData->state != STATE_EXITED)
        {
            privateData->state = STATE_RUNNING;
        }

        return THREAD_NO_ERROR;
    }

    bool Thread::isAlive() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->state == STATE_RUNNING || privateData->state == STATE_PAUSED;
    }

    bool Thread::isRunning() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->state == STATE_RUNNING;
    }

    bool Thread::isPaused() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->state == STATE_PAUSED;
    }

    int Thread::getId() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->id;
    }

    Thread::ExitCode Thread::waitExit()
    {
        ExitCode exitCode = (ExitCode)-1;

        if (privateData->state == STATE_PAUSED || privateData->state == STATE_NEW)
        {
            resume();
        }

        privateData->state = STATE_CANCELED;

        WaitForSingleObject(privateData->handle, INFINITE);

        for (;;)
        {
            if (!GetExitCodeThread(privateData->handle, (LPDWORD)&exitCode))
            {
                exitCode = (ExitCode)-1;
                break;
            }

            if ((DWORD)exitCode != STILL_ACTIVE)
                break;

            Sleep(1);
        }

        return exitCode;
    }

    bool Thread::testDestroy()
    {
        CriticalSectionLocker lock(cs);

        return privateData->state == STATE_CANCELED;
    }

#elif HARE_PLATFORM == HARE_PLATFORM_LINUX
    //---------------------------------------------------------------------
    CriticalSection::CriticalSection()
    {
        privateData = new Mutex();
    }

    CriticalSection::~CriticalSection()
    {
        delete static_cast<Mutex*>(privateData);
        privateData = 0;
    }

    void CriticalSection::enter()
    {
        static_cast<Mutex*>(privateData)->lock();
    }

    void CriticalSection::leave()
    {
        static_cast<Mutex*>(privateData)->unlock();
    }

    //---------------------------------------------------------------------
    Mutex::Mutex() : privateData(0)
    {
        pthread_mutex_t* mutex = new pthread_mutex_t;
        if (pthread_mutex_init(mutex, NULL))
        {
            delete mutex;
        }
        else
        {
            privateData = mutex;
        }
    }

    Mutex::~Mutex()
    {
        if (privateData)
        {
            pthread_mutex_t* mutex = (pthread_mutex_t*)privateData;
            privateData = 0;
            pthread_mutex_destroy(mutex);
            delete mutex;
        }
    }

    bool Mutex::isOk() const
    {
        return privateData != 0;
    }

    MutexError Mutex::lock()
    {
        pthread_mutex_t* mutex = (pthread_mutex_t*)privateData;
        int err = pthread_mutex_lock(mutex);
        switch (err)
        {
        case EDEADLK:
            return MUTEX_DEAD_LOCK;

        case EINVAL:
            break;

        case 0:
            return MUTEX_NO_ERROR;

        default:
            assert(false && "pthread_mutex_lock()");
        }
        return MUTEX_ERROR;
    }

    MutexError Mutex::tryLock()
    {
        pthread_mutex_t* mutex = (pthread_mutex_t*)privateData;
        int err = pthread_mutex_trylock(mutex);
        switch (err)
        {
        case EBUSY:
            return MUTEX_BUSY;

        case EINVAL:
            break;

        case 0:
            return MUTEX_NO_ERROR;

        default:
            assert(false && "pthread_mutex_trylock()");
        }
        return MUTEX_ERROR;
    }

    MutexError Mutex::unlock()
    {
        pthread_mutex_t* mutex = (pthread_mutex_t*)privateData;
        int err = pthread_mutex_unlock(mutex);
        switch (err)
        {
        case EPERM:
            return MUTEX_UNLOCKED;

        case EINVAL:
            break;

        case 0:
            return MUTEX_NO_ERROR;

        default:
            assert(false && "pthread_mutex_unlock()");
        }
        return MUTEX_ERROR;
    }

    //---------------------------------------------------------------------
    static u32 _getTimeMilliseconds()
    {
        return clock() / CLOCKS_PER_SEC * 1000;
    }

    class SemaphoreInternal
    {
    public:
        SemaphoreInternal(int initialCount, int maxCount)
            : condition(mutex)
        {
            if ((initialCount < 0 || maxCount < 0) ||
                ((maxCount > 0) && (initialCount > maxCount)))
            {
                assert(false && "Semaphore: invalid initial or maximal count");
                ok = false;
            }
            else
            {
                maxcount = (size_t)maxCount;
                count = (size_t)initialCount;
            }

            ok = mutex.isOk() && condition.isOk();
        }

        bool isOk() const { return ok; }

        SemaphoreError wait()
        {
            MutexLocker locker(mutex);

            while (count == 0)
            {
                if (condition.wait() != COND_NO_ERROR)
                    return SEMA_ERROR;
            }

            count--;

            return SEMA_NO_ERROR;
        }

        SemaphoreError tryWait()
        {
            MutexLocker locker(mutex);

            if (count == 0)
                return SEMA_BUSY;

            count--;

            return SEMA_NO_ERROR;
        }

        SemaphoreError waitTimeout(u32 milliseconds)
        {
            MutexLocker locker(mutex);

            u32 startTime = _getTimeMilliseconds();

            while (count == 0)
            {
                u32 elapsed = _getTimeMilliseconds() - startTime;
                u32 remainingTime = milliseconds - elapsed;

                if (remainingTime <= 0)
                {
                    return SEMA_TIMEOUT;
                }

                switch (condition.waitTimeout(remainingTime))
                {
                case COND_TIMEOUT:
                    return SEMA_TIMEOUT;

                default:
                    return SEMA_ERROR;

                case COND_NO_ERROR:
                    ;
                }
            }

            count--;

            return SEMA_NO_ERROR;
        }

        SemaphoreError post()
        {
            MutexLocker locker(mutex);

            if (maxcount > 0 && count == maxcount)
            {
                return SEMA_OVERFLOW;
            }

            count++;

            return condition.signal() == COND_NO_ERROR ? SEMA_NO_ERROR : SEMA_ERROR;
        }

    private:
        Mutex mutex;
        Condition condition;

        size_t count;
        size_t maxcount;

        bool ok;
    };

    Semaphore::Semaphore(int initialCount, int maxCount)
    {
        SemaphoreInternal* sema = new SemaphoreInternal(initialCount, maxCount);
        if (!sema->isOk())
        {
            delete sema;
            privateData = 0;
        }
        else
            privateData = sema;
    }

    Semaphore::~Semaphore()
    {
        if (privateData)
        {
            SemaphoreInternal* sema = (SemaphoreInternal*)privateData;
            privateData = 0;
            delete sema;
        }
    }

    bool Semaphore::isOk() const
    {
        return privateData != 0;
    }

    SemaphoreError Semaphore::wait()
    {
        return static_cast<SemaphoreInternal*>(privateData)->wait();
    }

    SemaphoreError Semaphore::tryWait()
    {
        return static_cast<SemaphoreInternal*>(privateData)->tryWait();
    }

    SemaphoreError Semaphore::waitTimeout(u32 milliseconds)
    {
        return static_cast<SemaphoreInternal*>(privateData)->waitTimeout(milliseconds);
    }

    SemaphoreError Semaphore::post()
    {
        return static_cast<SemaphoreInternal*>(privateData)->post();
    }

    //---------------------------------------------------------------------
    class ConditionData
    {
    public:
        ConditionData(Mutex& mutex) : mutexRef(mutex)
        {
            ok = pthread_cond_init(&condition, NULL) == 0;
        }
       ~ConditionData()
        {
            if (ok)
                pthread_cond_destroy(&condition);
        }
        bool isOk() const
        {
            return ok;
        }
    public:
        Mutex& mutexRef;
        pthread_cond_t condition;
        bool ok;
    };

    Condition::Condition(Mutex& mutex) : privateData(0)
    {
        ConditionData* data = new ConditionData(mutex);
        if (!data->isOk())
        {
            delete data;
        }
        else
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
        return (data && data->isOk());
    }

    ConditionError Condition::wait()
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);

        int err = pthread_cond_wait(&data->condition, (pthread_mutex_t*)data->mutexRef.privateData);
        if (err != 0)
        {
            return COND_ERROR;
        }

        return COND_NO_ERROR;
    }

    ConditionError Condition::waitTimeout(u32 milliseconds)
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);
        u32 time = _getTimeMilliseconds() + milliseconds;

        timespec tspec;

        tspec.tv_sec = time / 1000;
        tspec.tv_nsec = (time % 1000) * 1000000;

        int err = pthread_cond_timedwait(&data->condition,
            (pthread_mutex_t*)data->mutexRef.privateData, &tspec);

        switch (err)
        {
        case ETIMEDOUT:
            return COND_TIMEOUT;

        case 0:
            return COND_NO_ERROR;
        }

        return COND_ERROR;
    }

    ConditionError Condition::signal()
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);
        int err = pthread_cond_signal(&data->condition);
        if (err != 0)
        {
            return COND_ERROR;
        }

        return COND_NO_ERROR;
    }

    ConditionError Condition::broadcast()
    {
        ConditionData* data = static_cast<ConditionData*>(privateData);
        int err = pthread_cond_broadcast(&data->condition);
        if (err != 0)
        {
            return COND_ERROR;
        }

        return COND_NO_ERROR;
    }

    //---------------------------------------------------------------------
    static const Thread::ExitCode EXITCODE_CANCELLED = (Thread::ExitCode)-1;

    enum ThreadState
    {
        STATE_NEW,          // didn't start execution yet (=> RUNNING)
        STATE_RUNNING,      // running (=> PAUSED or EXITED)
        STATE_PAUSED,       // suspended (=> RUNNING or EXITED)
        STATE_EXITED        // thread doesn't exist any more
    };

    class ThreadData
    {
    public:
        static void *PthreadStart(Thread* thread)
        {
            ThreadData* data = thread->privateData;

            data->semRun.wait();

            bool dontRunAtAll = true;

            {
                CriticalSectionLocker lock(thread->cs);
                dontRunAtAll = (data->state == STATE_NEW && data->cancelled);
            }

            if (!dontRunAtAll)
            {
                data->exitcode = thread->entry();

                {
                    CriticalSectionLocker lock(thread->cs);
                    data->state = STATE_EXITED;
                }
            }

            if (dontRunAtAll)
            {
                delete thread;
                return EXITCODE_CANCELLED;
            }
            else
            {
                thread->onExit();

                {
                    CriticalSectionLocker lock(thread->cs);
                    data->state = STATE_EXITED;
                }

                pthread_exit(data->exitcode);

                return NULL;
            }
        }

        ThreadError run()
        {
            assert(state == STATE_NEW);

            state = STATE_RUNNING;

            signalRun();

            return THREAD_NO_ERROR;
        }

        void signalRun() { semRun.post(); }

        void waitExit()
        {
            CriticalSectionLocker lock(csJoin);

            if (pthread_join(threadId, &exitcode) != 0)
            {
                assert(false);
            }
        }

        void pause()
        {
            assert(state == STATE_PAUSED);

            semSuspend.wait();
        }

        void resume()
        {
            assert(state == STATE_PAUSED);

            if (paused)
            {
                semSuspend.post();
                paused = false;
            }

            state = STATE_RUNNING;
        }

    public:
        pthread_t threadId;
        ThreadState state;
        bool cancelled;
        bool paused;
        Thread::ExitCode exitcode;
        CriticalSection csJoin;
        Semaphore semRun;
        Semaphore semSuspend;

    public:
        ThreadData()
        {
            state = STATE_NEW;
            cancelled = false;
            threadId = 0;
            exitcode = 0;
            paused = false;
        }
    };

    //---------------------------------------------------------------------
    static void *PthreadStart(void* ptr)
    {
        return ThreadData::PthreadStart((Thread*)ptr);
    }

    Thread::Thread()
    {
        privateData = new ThreadData();
    }

    Thread::~Thread()
    {
        delete privateData;
        privateData = 0;
    }

    /* static */
    void Thread::sleep(u32 milliseconds)
    {
        timespec tmReq;
        tmReq.tv_sec = (time_t)(milliseconds / 1000);
        tmReq.tv_nsec = (milliseconds % 1000) * 1000000;
        (void)nanosleep(&tmReq, (timespec*)NULL);
    }

    ThreadError Thread::create(u32 stackSize)
    {
        if (privateData->state != STATE_NEW)
        {
            return THREAD_RUNNING;
        }

        // set up the thread attribute: right now, we only set thread priority
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        if (stackSize)
            pthread_attr_setstacksize(&attr, stackSize);

        // this will make the threads created by this process really concurrent
        if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM) != 0)
        {
            assert(false && "pthread_attr_setscope(PTHREAD_SCOPE_SYSTEM) failed");
        }

        // create the new OS thread object
        int rc = pthread_create(&privateData->threadId,
                &attr, PthreadStart, (void*)this);

        if (pthread_attr_destroy(&attr) != 0)
        {
            assert(false && "pthread_attr_destroy() failed");
        }

        if (rc != 0)
        {
            privateData->state = STATE_EXITED;
            return THREAD_NO_RESOURCE;
        }

        return THREAD_NO_ERROR;
    }

    ThreadError Thread::run()
    {
        CriticalSectionLocker lock(cs);

        return privateData->run();
    }

    ThreadError Thread::kill()
    {
        switch (privateData->state)
        {
        case STATE_NEW:
        case STATE_EXITED:
            return THREAD_NOT_RUNNING;

        case STATE_PAUSED:
            resume();
        default:
            if (pthread_cancel(privateData->threadId) != 0)
            {
                return THREAD_ERROR;
            }

            privateData->exitcode = EXITCODE_CANCELLED;

            return THREAD_NO_ERROR;
        }
    }

    ThreadError Thread::pause()
    {
        CriticalSectionLocker lock(cs);

        if (privateData->state != STATE_RUNNING)
        {
            return THREAD_NOT_RUNNING;
        }

        privateData->state = STATE_PAUSED;

        return THREAD_NO_ERROR;
    }

    ThreadError Thread::resume()
    {
        CriticalSectionLocker lock(cs);

        switch (privateData->state)
        {
        case STATE_PAUSED:
            privateData->resume();
            return THREAD_NO_ERROR;

        case STATE_EXITED:
            return THREAD_NO_ERROR;

        default:
            return THREAD_ERROR;
        }
    }

    bool Thread::isAlive() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->state == STATE_RUNNING || privateData->state == STATE_PAUSED;
    }

    bool Thread::isRunning() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->state == STATE_RUNNING;
    }

    bool Thread::isPaused() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->state == STATE_PAUSED;
    }

    int Thread::getId() const
    {
        CriticalSectionLocker lock((CriticalSection&)cs);

        return privateData->threadId;
    }

    Thread::ExitCode Thread::waitExit()
    {
        privateData->waitExit();

        return privateData->exitcode;
    }

    bool Thread::testDestroy()
    {
        cs.enter();

        if (privateData->state == STATE_PAUSED)
        {
            privateData->paused = true;

            cs.leave();

            privateData->pause();
        }
        else
        {
            cs.leave();
        }

        return privateData->cancelled;
    }

#endif
}
