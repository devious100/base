#pragma once
#include <windows.h>
#include <psapi.h>
#include "SS_Blocker.h"

#pragma comment(lib,"psapi.lib")

class Route
{
	private:

	public:

		Route()
		{
		}

		virtual ~Route()
		{
		}
		
		void StartHooks()
		{
			DetourFunctionWithTrampoline((PBYTE)fGetWindowPlacement, (PBYTE)Fake_GetWindowPlacemt);

		}

		void FinishHooks()
		{
			DetourRemove((PBYTE)fGetWindowPlacement, (PBYTE)Fake_GetWindowPlacemt);
		}

};