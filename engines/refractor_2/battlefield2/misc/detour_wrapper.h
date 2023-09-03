#ifndef detour_wrapper_h
#define detour_wrapper_h

#include "../default_includes.h"

#include "detours.h"


// a simple class used to wrap the microsoft detours express lib.
class Detour
{
private:
	PVOID pTarget;
	PVOID pDetour;

	BOOL enabled;

public:
	Detour( ): pTarget(0), pDetour(0), enabled(FALSE) {}

	void Initialize(PVOID target, PVOID detour );

	BOOL Attach(BOOL enable);

	BOOL IsInitialized(void) { return (BOOL) pTarget && pDetour; }
	BOOL IsEnabled(void) { return enabled; }

	PVOID GetTarget(void) { return pTarget; }

	~Detour() { if(enabled) Attach(FALSE); }
};


// i wrote this some time ago but abandoned it, don't ever remember if it is working
// or not. my plan was to use this to detour virtual member functions (eg.,
// CPlayer::GetCommaner() etc.)
class VTableDetour
{
	DWORD* vtable;

	DWORD* member_address;

	DWORD original_proc;
	DWORD detour_proc;

	BOOL initialized;
	BOOL enabled;

public:
	VTableDetour(): initialized(FALSE), enabled(FALSE) {}
	~VTableDetour() { Deinitialize(); }

	BOOL Initialize( PVOID object, unsigned int member_offset, PVOID detour_function );
	BOOL Deinitialize(void);
	BOOL Attach( BOOL enable );

	DWORD GetOriginal(void) { assert(initialized); return original_proc; }

	BOOL IsInitialized(void) { return initialized; }
	BOOL IsEnabled(void) { return enabled; }
};


//////////////////////////////////////////////////////////////////////////
PVOID GetMemberFunctionFromVtable( PVOID object, unsigned int function_index );



// C++ don't allow to convert a pointer to a member into a simple pointer
// eg., "LPVOID gen_pointer = &SomeClass::SomeMember" don't work, my solution
// is to use some assembly...
template<class T> inline PVOID GetMemberPtr(T member_ptr)
{
	PVOID gen_ptr;
	__asm mov eax, member_ptr;
	__asm mov gen_ptr, eax;

	return gen_ptr;
}

// the reverse also isn't valid
template<class T> inline T SetMemberPtr(PVOID gen_ptr)
{
	T member_ptr;
	__asm mov eax, gen_ptr;
	__asm mov member_ptr, eax;
	return member_ptr;
}

#endif //detour_wrapper_h