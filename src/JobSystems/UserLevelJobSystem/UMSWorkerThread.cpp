#include "stdafx.h"
#include "UMSWorkerThread.h"

#include "UMSScheduler.h"
#include "UMSSchedulerThread.h"
#include "UMSWorkerThreadParameter.h"

#define WORKER_THREAD_STACKSIZE 64 * 1024 //64K

UMSWorkerThread::UMSWorkerThread()
{
}


UMSWorkerThread::~UMSWorkerThread()
{
}

void UMSWorkerThread::Initialize(UMSScheduler* scheduler)
{
	m_scheduler = scheduler;

	SIZE_T size = 0;
	LPPROC_THREAD_ATTRIBUTE_LIST attribList = NULL;

	if (!InitializeProcThreadAttributeList(NULL, 1, 0, &size) && GetLastError() != ERROR_INSUFFICIENT_BUFFER) //Determine buffer size (see MSDN)
	{
		printf("Error: %d", GetLastError()); //ERROR
	}

	attribList = (LPPROC_THREAD_ATTRIBUTE_LIST)malloc(size);

	if (!InitializeProcThreadAttributeList(attribList, 1, 0, &size)) { printf("Error %d", GetLastError()); }

	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms686880(v=vs.85).aspx
	//Create Context and CompletionList
	UMS_CREATE_THREAD_ATTRIBUTES threadAttribs;
	threadAttribs.UmsVersion = UMS_VERSION;
	if (!CreateUmsThreadContext(&m_Context)) { printf("Error: %d", GetLastError()); } //Thread context per worker thread
	threadAttribs.UmsContext = m_Context;
	threadAttribs.UmsCompletionList = m_scheduler->m_CompletionList; //Use completion list of scheduler thread (I  can create new one; every scheduler thread can access every completion list he has a pointer to

	if (!UpdateProcThreadAttribute(attribList, 0, PROC_THREAD_ATTRIBUTE_UMS_THREAD, &threadAttribs, sizeof(threadAttribs), NULL, NULL)) { printf("Error %d", GetLastError()); }

	UMSWorkerThreadParameter workerParam; //gets deleted??? does it matter???
	//workerParam.schedulerThread = schedulerThread;
	workerParam.workerThread = this;

	m_Thread = CreateRemoteThreadEx(GetCurrentProcess(), NULL, WORKER_THREAD_STACKSIZE, UMSWorkerThread::StaticProc, &workerParam, 0, attribList, &m_ThreadId);
}

DWORD UMSWorkerThread::Proc(UMSWorkerThreadParameter * param)
{
	int a = 0;
	//printf("Worker Thread with Id %d is working...", m_ThreadId);

	//Idle()

	Work();
	

	return DWORD();
}

DWORD WINAPI UMSWorkerThread::StaticProc(void * p)
{
	UMSWorkerThreadParameter* workerParam = (UMSWorkerThreadParameter*)p;
	return workerParam->workerThread->Proc(workerParam);
}

void UMSWorkerThread::Work()
{
	m_CurrentTask->m_Callback(m_CurrentTask);
}
