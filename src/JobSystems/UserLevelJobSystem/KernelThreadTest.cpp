#include "stdafx.h"
#include "KernelThreadTest.h"

#include "Game.h"

KernelThreadTest::KernelThreadTest()
{
}


KernelThreadTest::~KernelThreadTest()
{
}

void KernelThreadTest::execute()
{
	Game game;
	game.CurrentUserLevelImpl = UserLevelImplementation::KERNELTHREAD;

	game.Initialize();

	game.Start();

	game.Shutdown();
}
