#pragma once

#include "UMSSchedulerThread.h"
#include "UMSWorkerThread.h"

#define MAX_UMS_SCHEDULER_THREADS 32
#define MAX_UMS_WORKER_THREADS 256

//https://msdn.microsoft.com/en-us/library/windows/desktop/dd627187(v=vs.85).aspx#ums_best_practices

struct UMSSchedulerThreadParameter;

class UMSScheduler
{
public:
	~UMSScheduler();

	//UMS
	PUMS_COMPLETION_LIST m_CompletionList; //One or more
	HANDLE m_CompletionEvent; //One or more

	void Initialize();

	static UMSScheduler* GetInstance();

	static void WINAPI StaticProc(UMS_SCHEDULER_REASON reason, ULONG_PTR activationPayload, PVOID schedulerParam);

	void ProcessStartup(UMSSchedulerThread* uThread);

	void ProcessYield();

	void ProcessBlock();

	void QueryAndQueueReadyWorkerThreads();

	bool PushTask(Job* task);

	bool PopTask(Job** OUT_task);

private:

	UMSScheduler();

	UMSSchedulerThread* getMinSizedSchedulerThreadWorkerQueue();

	static UMSScheduler* m_instance;

	u_int m_numOfProcessors;
	u_int m_numOfSchedulerThreads;
	UMSSchedulerThread m_schedulerThreads[MAX_UMS_SCHEDULER_THREADS];
	UMSWorkerThread m_workerThreads[MAX_UMS_WORKER_THREADS];
	std::map<PVOID, UMSWorkerThread*> m_workerThradsByContext;
	std::mutex m_tasksMutex;
	std::queue<Job*> m_tasks;
};

