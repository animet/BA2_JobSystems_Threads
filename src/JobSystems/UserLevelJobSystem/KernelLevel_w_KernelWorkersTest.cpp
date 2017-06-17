#include "stdafx.h"
#include "KernelLevel_w_KernelWorkersTest.h"
#include "Game.h"

KernelLevel_w_KernelWorkersTest::KernelLevel_w_KernelWorkersTest()
{
}


KernelLevel_w_KernelWorkersTest::~KernelLevel_w_KernelWorkersTest()
{
}

void KernelLevel_w_KernelWorkersTest::execute()
{

	Game game;
	game.CurrentUserLevelImpl = UserLevelImplementation::KERNEL_W_KERNELWORKERS;

	game.Initialize();

	game.Start();

	game.Shutdown();
}
