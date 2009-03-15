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

namespace hare_core
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

    class CORE_API Mutex
    {
    public:
        Mutex();
       ~Mutex();

        bool isOk() const;

        MutexError lock();

        MutexError tryLock();

        MutexError unlock();

    protected:
        void *privateData;

        HARE_DECLARE_NO_COPY_CLASS(Mutex)
    };


    class CORE_API CriticalSection
    {
    public:
        CriticalSection();
       ~CriticalSection();
        
        void enter();
        void leave();

    private:
        void* privateData;

        HARE_DECLARE_NO_COPY_CLASS(CriticalSection)
    };


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

    class CORE_API Semaphore
    {
    public:
        enum { MAX_INT = 2147483647 };

        Semaphore(int initialCount = 0, int maxCount = MAX_INT);
       ~Semaphore();

        bool isOk() const;

        SemaphoreError wait();
        SemaphoreError tryWait();
        SemaphoreError waitTimeout(u32 milliseconds);
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

    class CORE_API Condition
    {
    public:
        Condition(Mutex& mutex);
       ~Condition();

        bool isOk() const;

        ConditionError wait();

        ConditionError waitTimeout(u32 milliseconds);

        ConditionError signal();

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

    class CORE_API Thread
    {
    public:
        //static Thread* currentThread();
        //static bool isMainThread();
        //static void yield();
        //static void sleep(u32 milliseconds);
        //static int getCPUCount();
        //static int getCurrentId();
        //static bool setConcurrency(int level);

    public:
        typedef void* ExitCode;

        Thread();
        virtual ~Thread();

        ThreadError create(u32 stackSize = 0);
        ThreadError run();

        ExitCode waitExit();

        ThreadError kill();
        ThreadError pause();
        ThreadError resume();

        bool isAlive() const;
        bool isRunning() const;
        bool isPaused() const;
        int getId() const;

        virtual bool testDestroy();

    public:
        virtual void* entry() = 0;
        virtual void onExit()
        {
        }

    private:
        friend class ThreadData;
        ThreadData* privateData;
        CriticalSection cs;
    };
}