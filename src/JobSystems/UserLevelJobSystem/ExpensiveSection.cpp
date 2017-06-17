#include "stdafx.h"
//#include "ExpensiveSection.h"
//#include "FiberScheduler.h"
//#include "AssistantKernelWorkerThread.h"
//
//ExpensiveSection::ExpensiveSection()
//{
//}
//
//
//ExpensiveSection::~ExpensiveSection()
//{
//}
//
//void ExpensiveSection::Enter()
//{
//	//TODO: GetCurrentExecuntingFiber pointer
//	DWORD tid = GetCurrentThreadId();
//	void* fiberPointer = GetCurrentFiber();
//	Fiber* fiber = FiberScheduler::GetInstance()->GetFiberByPointer(fiberPointer);
//	assert(fiber != nullptr);
//	fiber->GetFiberSchedulerThread()->PushExpensiveFiber(fiber);
//
//	SwitchToFiber(fiber->GetFiberSchedulerThread()->m_schedulerFiberPointer); //Must switch in scope of its scheduler thread!
//	DWORD tid_2 = GetCurrentThreadId();
//
//	//AVOID this call: see MSDN
//	/*void* fiberPointer = GetCurrentFiber();
//	Fiber* fiber = FiberScheduler::GetInstance()->GetFiberByPointer(fiberPointer);
//	FiberScheduler::GetInstance()->PushExpensiveJobFiber(fiber);*/
//}
//
//void ExpensiveSection::Leave()
//{
//	DWORD tid = GetCurrentThreadId();
//	
//	void* fiberPointer = GetCurrentFiber();
//	Fiber* fiber = FiberScheduler::GetInstance()->GetFiberByPointer(fiberPointer);
//	assert(fiber->m_AssignedAssistantThread != nullptr);
//	AssistantKernelWorkerThread* assitantThread = fiber->m_AssignedAssistantThread;
//	assert(assitantThread->m_ThreadId == tid);
//	fiber->m_AssignedAssistantThread = nullptr;
//	fiber->GetFiberSchedulerThread()->PushFiberWithWorkLeft(fiber, assitantThread->m_AssistantFiberPointer);
//
//	SwitchToFiber(assitantThread->m_AssistantFiberPointer); //Must switch in scope of its assistant thread!
//	DWORD tid_2 = GetCurrentThreadId();
//}
