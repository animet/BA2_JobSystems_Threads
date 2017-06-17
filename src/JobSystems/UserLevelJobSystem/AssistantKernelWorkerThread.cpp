#include "stdafx.h"
//#include "AssistantKernelWorkerThread.h"
//#include "DebugStatics.h"
//#include "FiberScheduler.h"
//
//#define ASSISTANT_KERNELWORKER_THREAD_STACKSIZE 512 * 1024 //512KB
//#define ASSISTANTFIBER_STACKSIZE 512 * 1024 //512KB
//
//AssistantKernelWorkerThread::AssistantKernelWorkerThread()
//	: m_isReadyForProcessing(false)
//{
//}
//
//AssistantKernelWorkerThread::~AssistantKernelWorkerThread()
//{
//}
//
//void AssistantKernelWorkerThread::Initialize(FiberScheduler * scheduler)
//{
//	m_scheduler = scheduler;
//
//	m_Thread = CreateThread(NULL, ASSISTANT_KERNELWORKER_THREAD_STACKSIZE, AssistantKernelWorkerThread::StaticThreadProc, this, 0, &m_ThreadId); //Creates and starts thread. Suspends the thread: (CREATE_SUSPENDED)
//}
//
//DWORD AssistantKernelWorkerThread::ThreadProc()
//{
//	printf("The thread with the id '%d' will be converted to a fiber to schedule other fibers...\n", GetCurrentThreadId());
//
//	//The following fiber cannot be switched to from a fiber created by another thread???!
//	m_AssistantFiberPointer = ConvertThreadToFiber(this); //&m_assistentFiber); //scheduler thread is converted to fiber to schedule other fibers. Members of scheduler fibers are currently not needed
//	if (m_AssistantFiberPointer == NULL) { printf("Error %d", GetLastError()); }
//
//
//#if JOBSYSTEM_DEBUG_LOG
//	char b[512];int n = 0;
//	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- The AssistantKernelWorkerThread (Kernel Thread) with the Id '%d' is converted to a fiber...\n", GetCurrentThreadId()); return b; }());
//#endif
//	m_isReadyForProcessing = true;
//
//	std::unique_lock<std::mutex> guard(m_scheduleMutex);
//	while (true)
//	{
//		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms686919(v=vs.85).aspx
//		//Execute ready fibers....
//		//printf("Fiber scheduler thread with the id '%d' is now working...\n", GetCurrentThreadId());
//
//		while (m_expensiveFibers.size() <= 0) //TODO: lock queue
//		{
//			m_scheduleCondition.wait(guard);
//		}
//
//		Fiber* fiber = nullptr;
//		if (PopExpensiveFiber(&fiber))
//		{
//			fiber->m_AssignedAssistantThread = this;
//			SwitchToFiber(fiber->m_FiberPointer);
//		}
//
//	}
//	
//	return 0;
//}
//
//bool AssistantKernelWorkerThread::IsReadyForProcessing() const
//{
//	return m_isReadyForProcessing;
//}
//
//void AssistantKernelWorkerThread::PushExpensiveFiber(Fiber * fiber)
//{
//	std::unique_lock<std::mutex> guard(m_expensiveFibersMutex);
//	m_expensiveFibers.push(fiber);
//	m_scheduleCondition.notify_one();
//	//SwitchToFiber(fiber->GetFiberSchedulerThread()->m_schedulerFiberPointer); //Must switch in scope of its scheduler thread!
//}
//
//DWORD AssistantKernelWorkerThread::StaticThreadProc(void * p)
//{
//	return ((AssistantKernelWorkerThread*)p)->ThreadProc();
//}
//
//bool AssistantKernelWorkerThread::PopExpensiveFiber(Fiber** OUT_fiber)
//{
//	std::unique_lock<std::mutex> guard(m_expensiveFibersMutex);
//	if (m_expensiveFibers.size() > 0)
//	{
//		Fiber* fiber = m_expensiveFibers.front();
//		m_expensiveFibers.pop();
//		*OUT_fiber = fiber;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
