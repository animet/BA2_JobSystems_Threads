// UserLevelJobSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "DebugStatics.h"
#include "KernelLevel_w_KernelWorkersTest.h"
#include "KernelThreadTest.h"
#include "FiberTest.h"
#include "UMSTest.h"

int main()
{
#ifdef USE_STEPS_FOR_MEMORY_ANALYSIS
	printf("To start the execution of the program press any key.\n");
	getch();
#endif // USE_STEPS_FOR_MEMORY_ANALYSIS

#if JOBSYSTEM_DEBUG_LOG
	printf("-- [WARNING] Debug Log enabled. This may invalidate measurements!\n");
#endif
	printf("Main thread id: %d\n", GetCurrentThreadId());
	
	//printf("Executing kernel-level with kernel workers test...\n");
	//KernelLevel_w_KernelWorkersTest kltest;
	//kltest.execute();

	/*printf("Executing KernelThread test...\n");
	KernelThreadTest ktTest;
	ktTest.execute();*/


	printf("Executing fiber test...\n");
	FiberTest ftest;
	ftest.execute();

	/*printf("Executing UMS test...\n");
	UMSTest utest;
	utest.execute();*/

	//For usage:
	getch();

    return 0;
}

