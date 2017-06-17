//#pragma once
//
//#include "FiberSchedulerThread.h"
//#include "AssistantKernelWorkerThread.h"
//
//#define MAX_FIBER_SCHEDULER_THREADS 32
//
//class FiberScheduler
//{
//public:
//	~FiberScheduler();
//
//	static FiberScheduler* GetInstance();
//
//	void Initialize();
//
//	bool PushJob(Job* Job);
//
//	bool PushExpensiveJobFiber(Fiber* fiber); //TMP
//
//	bool AreAllSchedulerThreadsReadyForProcessing() const;
//
//	Fiber* GetFiberByPointer(void* fiberPointer);
//
//	Fiber* GetCurrentExecutingFiberBySchedulerThread(FiberSchedulerThread* schedulerThread);
//
//private:
//
//	FiberScheduler();
//
//	FiberSchedulerThread* getMinSizedJobQueue();
//
//	static FiberScheduler* m_instance;
//	static std::mutex m_initInstanceMutex;
//
//	u_int m_numOfProcessors;
//	u_int m_numOfSchedulerThreads;
//	FiberSchedulerThread m_schedulerThreads[MAX_FIBER_SCHEDULER_THREADS];
//	AssistantKernelWorkerThread m_assistentWorkerThread; //Maybe array? For system calls, or something like printf, ...
//};
//
