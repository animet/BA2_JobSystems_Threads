#pragma once

#include "FiberWorker.h"
#define MAX_FIBERS_PER_SCHEDULER_THREAD 64

class FiberScheduler;

class FiberSchedulerThread
{
public:
	FiberSchedulerThread();
	~FiberSchedulerThread();

	//Kernel-Level
	HANDLE m_Thread;
	DWORD_PTR m_ProcessorAffinity;
	DWORD m_ThreadId;
	//Fiber
	void* m_SchedulerFiberPointer;

	void Initialize(FiberScheduler* scheduler);

	DWORD ThreadProc();

	bool IsReadyForProcessing() const;

	void Activate();

	void PushReadyFiber(FiberWorker* fiber);

	bool PushJob(Job* job);

	bool PopJob(Job** OUT_Job);

	size_t JobCount();

	size_t JobCount_Unsafe();

	FiberWorker* GetFiberByPointer(void* fiberPointer);

	FiberWorker* GetCurrentExecutingFiber();

private:

	static DWORD WINAPI StaticThreadProc(void* p);

	FiberScheduler* m_scheduler;

	FiberWorker m_fibers[MAX_FIBERS_PER_SCHEDULER_THREAD];
	FiberWorker* m_currentExecutingFiber;
	std::mutex m_scheduleMutex;
	std::condition_variable m_scheduleCondition;
	//std::mutex m_readyFibersMutex; //May need no mutex because per fiber scheduler thread only one fiber is always executing
	std::queue<FiberWorker*> m_readyFibers;
	//std::mutex m_expensiveFibersMutex;
	//std::queue<Fiber*> m_expensiveFibers;
	std::mutex m_jobsMutex;
	std::queue<Job*> m_jobs;
	bool m_isReadyForProcessing;
};

