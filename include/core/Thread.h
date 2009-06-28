//***************************************************************
//  File:    Thread.h
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
#ifndef _THREAD_H_
#define _THREAD_H_

#include "CorePrerequisites.h"
#include <limits.h>     // for INT_MAX

namespace hare
{
    enum MutexError
    {
        MUTEX_NO_ERROR = 0,   // operation completed successfully
        MUTEX_INVALID,        // mutex hasn't been initialized
        MUTEX_DEAD_LOCK,      // mutex is already locked by the calling thread
        MUTEX_BUSY,           // mutex is already locked by another thread
        MUTEX_UNLOCKED,       // attempt to unlock a mutex which is not locked
        MUTEX_ERROR,          // any other error
    };

    /** Mutex class
    */
    class CORE_API Mutex
    {
    public:
        Mutex();
       ~Mutex();
        
        /// Is mutex ok ?
        bool isOk() const;

        /// Lock mutex
        MutexError lock();

        /// Try lock mutex
        MutexError tryLock();

        /// Unlock mutex
        MutexError unlock();

    protected:
        friend class Condition;
        void *privateData;

        HARE_DECLARE_NO_COPY_CLASS(Mutex)
    };

    /** Helper class for Mutex locking and unlocking
    */
    class CORE_API MutexLocker
    {
    public:
        MutexLocker(Mutex& mutexRef)
            : ok(false), mutex(mutexRef)
        {
            ok = (mutex.lock() == MUTEX_NO_ERROR);
        }

       ~MutexLocker()
        {
            if (isOk()) mutex.unlock();
        }

        bool isOk() const { return ok; }

    private:
        bool ok;
        Mutex& mutex;

        HARE_DECLARE_NO_COPY_CLASS(MutexLocker)
    };

    /** CriticalSection class
    */
    class CORE_API CriticalSection
    {
    public:
        CriticalSection();
       ~CriticalSection();
        /// Enter CriticalSection
        void enter();
        /// Leave CriticalSection
        void leave();

    private:
        void* privateData;

        HARE_DECLARE_NO_COPY_CLASS(CriticalSection)
    };

    /** Helper class for CriticalSection locking and unlocking
    */
    class CORE_API CriticalSectionLocker
    {
    public:
        CriticalSectionLocker(CriticalSection& cs)
            : csRef(cs)
        {
            csRef.enter();
        }

       ~CriticalSectionLocker()
        {
            csRef.leave();
        }

    private:
        CriticalSection& csRef;

        HARE_DECLARE_NO_COPY_CLASS(CriticalSectionLocker)
    };

    enum SemaphoreError
    {
        SEMA_NO_ERROR = 0,
        SEMA_INVALID,         // semaphore hasn't been initialized successfully
        SEMA_BUSY,            // returned by tryWait() if wait() would block
        SEMA_TIMEOUT,         // returned by waitTimeout()
        SEMA_OVERFLOW,        // post() would increase counter past the max
        SEMA_ERROR
    };

    /** Semaphore class
    */
    class CORE_API Semaphore
    {
    public:
        Semaphore(int initialCount = 0, int maxCount = INT_MAX);
       ~Semaphore();
        /// Is semaphore ok ?
        bool isOk() const;
    
        /// Wait semaphore
        SemaphoreError wait();
        /// Try wait semaphore
        SemaphoreError tryWait();
        /// Wait semaphore until timeout
        SemaphoreError waitTimeout(uint32 milliseconds);
        /// Release semaphore
        SemaphoreError post();

    private:
        void* privateData;

        HARE_DECLARE_NO_COPY_CLASS(Semaphore)
    };

    enum ConditionError
    {
        COND_NO_ERROR = 0,
        COND_INVALID,
        COND_TIMEOUT,
        COND_ERROR,
    };

    /** Condition class
    */
    class CORE_API Condition
    {
    public:
        Condition(Mutex& mutex);
       ~Condition();

        /// Is condition ok ?
        bool isOk() const;
        /// Wait for condition
        ConditionError wait();
        /// Wait for condition until timeout
        ConditionError waitTimeout(uint32 milliseconds);
        /// Signal the condition
        ConditionError signal();
        /// Signal to all waiters 
        ConditionError broadcast();

    private:
        void *privateData;

        HARE_DECLARE_NO_COPY_CLASS(Condition)
    };

    enum ThreadError
    {
        THREAD_NO_ERROR = 0,
        THREAD_NO_RESOURCE,
        THREAD_RUNNING,
        THREAD_NOT_RUNNING,
        THREAD_ERROR,
    };

    class ThreadData;

    /** Thread class
    */
    class CORE_API Thread
    {
    public:
        //static Thread* currentThread();
        //static bool isMainThread();
        //static void yield();
        //static int getCPUCount();
        //static int getCurrentId();
        //static bool setConcurrency(int level);

        /// Sleep milliseconds
        static void sleep(uint32 milliseconds);

    public:
        typedef void* ExitCode;

        Thread();
        virtual ~Thread();

        /// Create a thread, thread is suspended 
        ThreadError create(uint32 stackSize = 0);
        /// Resume a newly created thread, should be called after create
        ThreadError run();
        
        /// Cancel the thread and wait for thread's exit 
        ExitCode waitExit();

        /// Kill the thread
        ThreadError kill();
        /// Pause the thread
        ThreadError pause();
        /// Resume the thread
        ThreadError resume();

        /// Is thread still alive ? (running or paused)
        bool isAlive() const;
        /// Is thread running ?
        bool isRunning() const;
        /// Is thread paused ?
        bool isPaused() const;
        /// Get thread's id
        int getId() const;

        /** Returns true if the thread was asked to terminate: this function should
            be called <b>by the thread</b> from time to time.
        */
        virtual bool testDestroy();

    public:
        /** Entry point for the thread - called by run() and executes in the context
            of this thread.
        */
        virtual void* entry() = 0;
        
        /** Called when the thread exits - in the context of this thread
            NB: this function will not be called if the thread is kill()ed
        */
        virtual void onExit() {}

    private:
        friend class ThreadData;
        ThreadData* privateData;
        CriticalSection cs;
    };
}

#endif
