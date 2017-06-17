#pragma once

class UMSSchedulerThread;
class UMSWorkerThread;

struct UMSWorkerThreadParameter
{
	UMSSchedulerThread* schedulerThread;
	UMSWorkerThread* workerThread;
};

