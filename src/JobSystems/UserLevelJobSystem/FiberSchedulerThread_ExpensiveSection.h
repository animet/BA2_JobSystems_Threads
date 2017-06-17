//#pragma once
//
//#include "Fiber.h"
//#define MAX_FIBERS_PER_SCHEDULER_THREAD 128
//
//class FiberScheduler;
//
//class FiberSchedulerThread
//{
//public:
//	FiberSchedulerThread();
//	~FiberSchedulerThread();
//
//	//Kernel-Level
//	HANDLE m_Thread;
//	DWORD_PTR m_ProcessorAffinity;
//	DWORD m_ThreadId;
//	//Fiber
//	void* m_schedulerFiberPointer;
//
//	void Initialize(FiberScheduler* scheduler);
//
//	DWORD ThreadProc();
//
//	bool IsReadyForProcessing() const;
//
//	void PushReadyFiber(Fiber* fiber);
//
//	void PushExpensiveFiber(Fiber* fiber);
//
//	void PushFiberWithWorkLeft(Fiber* fiber, void* assistantFiberPointer);
//
//	bool PushJob(Job* Job);
//
//	bool PopJob(Job** OUT_Job);
//
//	size_t JobCount();
//
//	Fiber* GetFiberByPointer(void* fiberPointer);
//
//	Fiber* GetCurrentExecutingFiber();
//
//private:
//
//	static DWORD WINAPI StaticThreadProc(void* p);
//
//	bool PopFiberWithWorkLeft(Fiber** OUT_fiber);
//
//	FiberScheduler* m_scheduler;
//
//	//Fiber m_schedulerFiber;
//	Fiber m_fibers[MAX_FIBERS_PER_SCHEDULER_THREAD];
//	Fiber* m_currentExecutingFiber;
//	std::mutex m_scheduleMutex;
//	std::condition_variable m_scheduleCondition;
//	//std::mutex m_readyFibersMutex; //May need no mutex because per fiber scheduler thread only one fiber is always executing
//	std::queue<Fiber*> m_readyFibers;
//	std::mutex m_fibersWithWorkLeftMutex;
//	std::queue<Fiber*> m_fibersWithWorkLeft;
//	//std::mutex m_expensiveFibersMutex;
//	//std::queue<Fiber*> m_expensiveFibers;
//	std::mutex m_JobsMutex;
//	//std::condition_variable m_JobsCondition; -> would need own mutex "doWorkMutex"
//	std::queue<Job*> m_Jobs;
//	//DEBUG:
//	bool m_isReadyForProcessing;
//};
//
