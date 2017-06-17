#include "stdafx.h"
#include "UMSTest.h"

#include "UMSScheduler.h"
#include "Job.h"
#include "Game.h"

//https://msdn.microsoft.com/en-us/library/windows/desktop/dd627187(v=vs.85).aspx#ums_best_practices

UMSTest::UMSTest()
{
}


UMSTest::~UMSTest()
{
}

void UMSTest::execute()
{

	Game game;
	game.CurrentUserLevelImpl = UserLevelImplementation::UMS;

	game.Initialize();

	game.Start();

	game.Shutdown();

}
