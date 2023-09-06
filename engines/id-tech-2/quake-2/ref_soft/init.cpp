//rra - LNK4243 problem
// 6/20/2003 - Modified by Vertigo Software, Inc.

#include <windows.h>
#include <_vcclrit.h>

// Call this function before you call anything in this DLL.
// It is safe to call from multiple threads; it is not reference
// counted; and is reentrancy safe.

__declspec(dllexport) void __cdecl DllEnsureInit(void)
{
	// Do nothing else here. If you need extra initialization steps,
	// create static objects with constructors that perform initialization.
	__crt_dll_initialize();
	// Do nothing else here.
}

// Call this function after this whole process is totally done
// calling anything in this DLL. It is safe to call from multiple
// threads; is not reference counted; and is reentrancy safe.
// First call will terminate.

__declspec(dllexport) void __cdecl DllForceTerm(void)
{
	// Do nothing else here. If you need extra terminate steps, 
	// use atexit.
	__crt_dll_terminate();
	// Do nothing else here.
}
