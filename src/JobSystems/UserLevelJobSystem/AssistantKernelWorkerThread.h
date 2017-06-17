//#pragma once
//
//#include "Fiber.h"
//class FiberScheduler;
//
//class AssistantKernelWorkerThread
//{
//public:
//	AssistantKernelWorkerThread();
//	~AssistantKernelWorkerThread();
//
//	//Kernel-Level
//	HANDLE m_Thread;
//	DWORD_PTR m_ProcessorAffinity;
//	DWORD m_ThreadId;
//	//Fiber
//	void* m_AssistantFiberPointer;
//	//Use this fiber pointer:
//	void* m_concreteAssistantFiberPointer;
//
//	void Initialize(FiberScheduler* scheduler);
//
//	DWORD ThreadProc();
//
//	bool IsReadyForProcessing() const;
//
//	void PushExpensiveFiber(Fiber* fiber);
//
//private:
//
//	static DWORD WINAPI StaticThreadProc(void* p);
//
//	bool PopExpensiveFiber(Fiber** OUT_fiber);
//
//	FiberScheduler* m_scheduler;
//
//	std::queue<Fiber*> m_expensiveFibers;
//	std::mutex m_expensiveFibersMutex;
//	std::mutex m_scheduleMutex;
//	std::condition_variable m_scheduleCondition;
//	//std::mutex m_readyFibersMutex; //May need no mutex because per fiber scheduler thread only one fiber is always executing
//	//DEBUG:
//	bool m_isReadyForProcessing;
//};
//
