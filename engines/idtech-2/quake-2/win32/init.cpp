// for mixed DLL loading issue, see 
// http://support.microsoft.com/?id=814472 for more info
// 6/20/2003 - Created by Vertigo Software, Inc.

#include <windows.h>

#pragma warning(disable : 4100)		// unreferenced formal parameter

typedef void (__stdcall *pfnEnsureInit)(void);
typedef void (__stdcall *pfnForceTerm)(void);

void InitDll(char* dllName)
{
	#if _MANAGED
		HMODULE hDll = GetModuleHandle(dllName);
		if (!hDll)
			return;
			
		pfnEnsureInit pfnDll = (pfnEnsureInit)GetProcAddress(hDll, "DllEnsureInit");
		if (!pfnDll)
			return;

		pfnDll();
	#endif
}


void TermDll(HMODULE hDll)
{
	#if _MANAGED
		if (!hDll)
			return;

		pfnForceTerm pfnDll = (pfnForceTerm)GetProcAddress(hDll, "DllForceTerm");
		if (!pfnDll)
			return;
		
		pfnDll();
	#endif
}
