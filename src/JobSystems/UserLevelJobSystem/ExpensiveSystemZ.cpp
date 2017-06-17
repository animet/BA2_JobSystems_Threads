#include "stdafx.h"
#include "ExpensiveSystemZ.h"

#include "JobManager.h"
//#include "UMSScheduler.h"
#include "FiberScheduler.h"
#include "Scheduler.h"
#include "KernelThreadScheduler.h"
#include "Game.h"

#include "ExpensiveSection.h"

ExpensiveSystemZ::ExpensiveSystemZ()
{
}


ExpensiveSystemZ::~ExpensiveSystemZ()
{
}


int ExpensiveSystemZ::Update()
{
	Job* job = JobManager::GetInstance()->CreateJob();
	job->m_Callback = [](Job* job) {

		++ExpensiveSystemZ::m_JobExecCount;
		auto start_time = std::chrono::high_resolution_clock::now();
		
		/*ExpensiveSection::Enter();*/
		int fh;
		fh = _open("C:\\Temp\\JobSystemImpl.exe", _O_RDONLY);
		if (fh == -1)
		{}
			//perror("Open failed on input file");
		else
		{
			//printf("Open succeeded on input file\n");
			_close(fh);
		}
		/*ExpensiveSection::Leave();*/

		auto end_time = std::chrono::high_resolution_clock::now();
		auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	};

	switch (m_Game->CurrentUserLevelImpl)
	{
	case UserLevelImplementation::FIBER:
		FiberScheduler::GetInstance()->PushJob(job);
		break;
	case UserLevelImplementation::KERNEL_W_KERNELWORKERS:
		Scheduler::GetInstance()->PushJob(job);
		break;
	case UserLevelImplementation::KERNELTHREAD:
		KernelThreadScheduler::GetInstance()->PushJob(job);
		break;
	default:
		assert(false);
		break;
	}

	return 1;
}

std::atomic_uint ExpensiveSystemZ::m_JobExecCount = 0;