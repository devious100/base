#include "ogoid++.h"

#include "misc/detour_wrapper.h"

#include "cheats/cheats.h"
#include "d3d/d3dhook.h"

#include "bf2/bf2_general_functions.h"


// in the game main loop there is a function that is called to update the entire
// system (once for each frame), this is what i'm calling the mainloop function.
// the problem is that it is a member of some class (maybe something like CApp), 
// because of this we must declare our detour function also as a member of some class...
class CMainLoop {
public:
	BYTE DetouredMainLoop(void);
};

// the other detours are just plain functions
HMODULE WINAPI	DetouredLoadLibrary( LPCTSTR lpFileName );
BOOL	WINAPI	DetouredFreeLibrary( HMODULE hModule );
int				DetouredPbclCheck( int a, int b, int c );
void			DetouredPbclSShot( int a, int b, int c );

typedef HMODULE	(WINAPI		* LOADLIBRARY)	(LPCTSTR);
typedef BOOL	(WINAPI		* FREELIBRARY)	(HMODULE);
typedef int		(			* PBCLCHECK)	(int,int,int);
typedef void	(			* PBCLSSHOT)	(int,int,int);

typedef BYTE	( CMainLoop::* MAINLOOP)	(void);


Detour DLoadLibrary;
Detour DFreeLibrary;
Detour DPbclCheck;
Detour DPbclSShot;
Detour DMainLoop;


//////////////////////////////////////////////////////////////////////////
BOOL SetGeneralDetours(BOOL enable)
{
	if(!DLoadLibrary.IsInitialized())
		DLoadLibrary.Initialize( (PVOID) LoadLibraryA, (PVOID) DetouredLoadLibrary );
	if(!DFreeLibrary.IsInitialized())
		DFreeLibrary.Initialize( (PVOID) FreeLibrary, (PVOID) DetouredFreeLibrary );

	DLoadLibrary.Attach(enable);
	DFreeLibrary.Attach(enable);

	return TRUE;

}

BOOL SetMainLoopDetours(BOOL enable)
{
	bf2_hModule = GetModuleHandle(0);
	assert(bf2_hModule);
	bf2_base = GetModuleBase(bf2_hModule);
	assert(bf2_base);

	if(!DMainLoop.IsInitialized())
		DMainLoop.Initialize( CAST(PVOID, bf2_base + BF2_MAINLOOP_FNADD), GetMemberPtr(&CMainLoop::DetouredMainLoop));
	DMainLoop.Attach(enable);

	return TRUE;
}

BOOL SetPbclDetours(BOOL enable)
{
	pbcl_hModule = GetModuleHandle("pbcl.dll");
	pbcl_base = GetModuleBase(pbcl_hModule);

	assert(pbcl_base);

	if(!DPbclCheck.IsInitialized())
		DPbclCheck.Initialize( CAST(PVOID, pbcl_base + PBCL_CHECK_FNADD), (PVOID) DetouredPbclCheck );
	if(!DPbclSShot.IsInitialized())
		DPbclSShot.Initialize( CAST(PVOID, pbcl_base + PBCL_SSHOT_FNADD), (PVOID) DetouredPbclSShot );


	DPbclCheck.Attach(enable);
	DPbclSShot.Attach(enable);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
HMODULE WINAPI DetouredLoadLibrary( LPCTSTR lpFileName )
{
	HMODULE hModule = CAST(LOADLIBRARY, DLoadLibrary.GetTarget()) ( lpFileName );

	if(!hModule) 
		return hModule;

	char* base_name = strrchr( (char*) lpFileName, '\\' );
	if(!base_name) base_name = (char*) lpFileName;
	else base_name++;

	if(GetModuleHandle("RendDx9.dll"))
		SetGeneralDetours(TRUE);

	if(GetModuleHandle("pbcl.dll"))
		SetPbclDetours(TRUE);

	if(!lstrcmpi( base_name, "RendDX9.dll"))
		GeneralInit();

	if( !lstrcmpi( base_name, "pbcl.dll"))
		SetPbclDetours(TRUE);

	return hModule;
}

BOOL WINAPI DetouredFreeLibrary (HMODULE hModule)
{
	if(hModule == renddx9_hModule)
	{
		if(d3d_hook.IsHookEnabled())
			d3d_hook.SetHook(FALSE);

		Cheats_AllowMemPointer(FALSE);

		SetMainLoopDetours(FALSE);

		renddx9_hModule = 0;
		renddx9_base = 0;
	}

	else if(hModule == pbcl_hModule)
	{
		SetPbclDetours(FALSE);

		pbcl_hModule = 0;
		pbcl_base = 0;
	}

	return CAST(FREELIBRARY, DFreeLibrary.GetTarget()) (hModule);
}

int DetouredPbclCheck( int a, int b, int c)
{
 	int result;

	// truly speaking the real routine is declared as
	// int pbclcheck(int,char*,int), being the second parameter
	// a pointer to a code that determine what action
	// will be done. the memory check only run when this
	// code is 0x6d (other codes do other things), but
	// since we won't need this i declared everything
	// as 'int' (the previous pbcl.dll version had the code
	// as the third parameter, not the second, so i'm thinking
	// that it is better to avoid to mess with it).

	//uncomment the next instruction to see in the console when this routine is called
	//LOGMSG( "pb check, a: " << hex << a << ", b: " << b << " (ch: " << (int) *((char*) b) << ") c: " << c << );

 
	// we have to disable everything...
	BOOL d3dHook_was_set = d3d_hook.IsHookEnabled();
	if(d3dHook_was_set)
		d3d_hook.SetHook(FALSE);

	//Cheats_AllowMemPointer(FALSE);
	Cheats_AllowMemPointer(FALSE);

	SetPbclDetours(FALSE);
	SetMainLoopDetours(FALSE);
	SetGeneralDetours(FALSE);
 
	// call the original function
 	result = CAST( PBCLCHECK, DPbclCheck.GetTarget() ) (a, b, c);

	// just to enable them again...
	SetGeneralDetours(TRUE);
	SetMainLoopDetours(TRUE);
	SetPbclDetours(TRUE);

	// only reenable the cheats if we aren't getting a new clean screenshot
	if(!screenshot.WaitingToFetchScreen())
		Cheats_AllowMemPointer(TRUE);

	if(d3dHook_was_set)
		d3d_hook.SetHook(TRUE);
 
 	return result;
}



void DetouredPbclSShot(int a, int b, int c)
{
	screenshot.WriteBufferToScreen();
	CAST( PBCLSSHOT, DPbclSShot.GetTarget() ) (a, b, c);
}

//////////////////////////////////////////////////////////////////////////
BYTE CMainLoop::DetouredMainLoop (void)
{
	last_frame = this_frame;
	this_frame = GetTickCount();
	frame_time = (this_frame - last_frame) / 1000.0f;

	InitDetours();

	if(!d3d_hook.IsHookEnabled())
		d3d_hook.SetHook(TRUE);

	if(!got_all_global_objects)
		UpdateGlobalObjects();

	 if(got_all_global_objects && player_manager->LocalPlayer)
	 {
		 local_player = player_manager->LocalPlayer;

		// hotkeys_enabled = GetAsyncKeyState(VK_MENU)  & ~1;

		// for_each(cheat_list.begin(), cheat_list.end(), CheatRefresh);
			aimbot.Refresh();
			screenshot.Refresh();
	 }

	 return CALL_MEMBER_FN(*this, SetMemberPtr<MAINLOOP>(DMainLoop.GetTarget())) ();;
}
