// for mixed DLL loading issue, see 
// http://support.microsoft.com/?id=814472 for more info
// 6/20/2003 - Created by Vertigo Software, Inc.
// 11/17/2007 - Modified by Greg Dolley.

#include <windows.h>
//#include <_vcclrit.h> // ***GREGS_VC9_PORT_MOD*** -- This header is obsolete because the mixed DLL loading problem does not apply to this version of MSVC++ and this CLR

// Call this function before you call anything in this DLL.
// It is safe to call from multiple threads; it is not reference
// counted; and is reentrancy safe.
__declspec(dllexport) void __cdecl DllEnsureInit(void)
{
	//__crt_dll_initialize(); // ***GREGS_VC9_PORT_MOD*** -- The mixed DLL loading problem does not apply to this version of MSVC++ and this CLR
}

// Call this function after this whole process is totally done
// calling anything in this DLL. It is safe to call from multiple
// threads; is not reference counted; and is reentrancy safe.
__declspec(dllexport) void __cdecl DllForceTerm(void)
{
	//__crt_dll_terminate(); // ***GREGS_VC9_PORT_MOD*** -- The mixed DLL loading problem does not apply to this version of MSVC++ and this CLR
}

