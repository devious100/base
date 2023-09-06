
#include "detour_wrapper.h"
#include "detours.h"


void Detour::Initialize(PVOID target, PVOID detour)
{
	assert(!pTarget && !pDetour);

	pTarget = target;
	pDetour = detour;
}

BOOL Detour::Attach(BOOL enable)
{
	LONG result;

	if(enable == enabled ) return TRUE;

	assert(pTarget && pDetour);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if(enable)
		result = DetourAttach(&(PVOID&) pTarget, pDetour);
	else
		result = DetourDetach(&(PVOID&) pTarget, pDetour);

	assert( result == NO_ERROR);

	DetourTransactionCommit();

	enabled = enable;

	return TRUE;
}


BOOL VTableDetour::Initialize( PVOID object, unsigned int member_offset, PVOID detour_function )
{

	assert( !initialized);
	assert( object &&  detour_function );

	vtable = *(DWORD**) object;

	member_address = vtable + member_offset;

	original_proc = *member_address;
	detour_proc = (DWORD) detour_function;

	initialized = TRUE;
	enabled = FALSE;

	return TRUE;
}
BOOL VTableDetour::Deinitialize(void)
{
	if(initialized && enabled)
		Attach(FALSE);

	initialized = enabled = FALSE;

	return TRUE;
}
BOOL VTableDetour::Attach( BOOL enable )
{
	assert(initialized);

	if(enable == enabled)
		return TRUE;

	assert(member_address);

	if(enable) *member_address = detour_proc;
	else	   *member_address = original_proc;

	enabled = enable;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
PVOID GetMemberFunctionFromVtable( PVOID object, unsigned int function_index )
{
	assert(object);

	DWORD* vtable = *((DWORD**) object);

	assert(vtable);

	return (PVOID) *(vtable + function_index);
}



