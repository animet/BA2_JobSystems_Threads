#include "stdafx.h"
#include "ExpensiveSystemX.h"

#include "JobManager.h"
//#include "UMSScheduler.h"
#include "FiberScheduler.h"
#include "Scheduler.h"
#include "KernelThreadScheduler.h"
#include "Game.h"

#include "ExpensiveSection.h"

ExpensiveSystemX::ExpensiveSystemX()
{
}


ExpensiveSystemX::~ExpensiveSystemX()
{
}



int ExpensiveSystemX::Update()
{
	Job* job = JobManager::GetInstance()->CreateJob();
	job->m_Callback = [](Job* job) {

		++ExpensiveSystemX::m_JobExecCount;
		auto start_time = std::chrono::high_resolution_clock::now();
		
		int fh, bytesRead;
		char buffer[1024];
		/*ExpensiveSection::Enter();*/
		fh = _open("C:\\Temp\\JobSystemImpl.exe", _O_RDONLY);
		if (fh == -1)
		{
		}
		//perror("Open failed on input file");
		else
		{
			
			//printf("Open succeeded on input file\n");

			bytesRead = _read(fh, &buffer, 256);

			if (bytesRead <= 0)
			{
				//Could not read
			}

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

std::atomic_uint ExpensiveSystemX::m_JobExecCount = 0;