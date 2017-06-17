#include "stdafx.h"
#include "FiberTest.h"

#include "Game.h"

//https://msdn.microsoft.com/de-de/library/windows/desktop/ms686919(v=vs.85).aspx

FiberTest::FiberTest()
{
}

void FiberTest::execute()
{

	Game game;
	game.CurrentUserLevelImpl = UserLevelImplementation::FIBER;

	game.Initialize();

	game.Start();

	game.Shutdown();

}
