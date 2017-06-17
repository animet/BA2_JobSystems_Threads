#include "stdafx.h"
#include "ThreadingUtility.h"

u_int ThreadingUtility::GetNumberOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}
