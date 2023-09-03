
#include "ogoid++.h"

#include <Psapi.h>

#include "cheats/cheats.h"
#include "d3d/d3dhook.h"

#include "bf2/bf2_general_functions.h"


string ogoid_dir;
string ogoid_dll;
string config_file;

string game_dir;
string bf2_exe;
string pbcl_dll;


int bf2_base = 0;
int bf2_entry = 0;
HANDLE bf2_hProcess = 0;
HMODULE bf2_hModule = 0;

HMODULE ogoid_hModule = 0;

int pbcl_base = 0;
HMODULE pbcl_hModule = 0;
BOOL pbcl_initialized = FALSE;

int renddx9_base = 0;
HMODULE renddx9_hModule = 0;
BOOL renddx9_initialized = FALSE;


HWND bf2_hWnd = 0;
//int bf2_wnd_width = 0;
//int bf2_wnd_height = 0;
//float bf2_wnd_aspect = 0.0f;


DWORD this_frame = 0;
DWORD last_frame = 0;
float frame_time = 0;

D3DHook d3d_hook;


//////////////////////////////////////////////////////////////////////////
int GetModuleBase( HMODULE hModule )
{
	MODULEINFO module;
	BOOL result;
	result = GetModuleInformation(GetCurrentProcess(), hModule, &module, sizeof(module));
	assert(result);

	return (int) module.lpBaseOfDll;
}


//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
#ifdef USE_CONSOLE
	FILE *stream;
#endif

	BOOL result;

	HMODULE dll;

	char filename[MAX_PATH];
	char* base_name;
	HANDLE hFile;

	if(ul_reason_for_call == DLL_THREAD_ATTACH
		|| ul_reason_for_call == DLL_THREAD_DETACH )
		return TRUE;


	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		ZeroMemory(filename, sizeof(filename));

		ogoid_hModule = hModule;

		bf2_hModule = GetModuleHandle(0);
		assert(bf2_hModule);

		result = GetModuleFileName( bf2_hModule, filename, sizeof(filename));
		assert(result);

		base_name = strrchr( filename, '\\' );
		if(!base_name) base_name = filename;
		else base_name++;

#ifdef CHECK_BF2_FILE_NAME
		if (lstrcmpi( base_name, "bf2.exe") != 0) {
			MessageBox(NULL, "This can only be injected on Battlefield 2!", "DEVIOUS-100.COM", MB_OK | MB_ICONASTERISK);
				return FALSE;
		}
#endif

		//MessageBox(NULL, "Loading...", "DEVIOUS-100.COM", MB_OK);

		bf2_hProcess = GetCurrentProcess();
		bf2_base = GetModuleBase(bf2_hModule);


		// checking the file versions through its sizes
		bf2_exe = filename;

		game_dir = bf2_exe;
		game_dir.resize( base_name - filename );
		pbcl_dll = game_dir + "pb\\pbcl.dll";


#ifdef CHECK_BF2_SIZE
		hFile = CreateFile( bf2_exe.c_str(), FILE_READ_ATTRIBUTES,
											 FILE_SHARE_READ, NULL,
											 OPEN_EXISTING,
											 FILE_ATTRIBUTE_NORMAL, NULL );
		if( hFile == INVALID_HANDLE_VALUE)
			MessageBox(NULL, "Warning: can't check Bf2.exe file size!", "DEVIOUS-100.COM", MB_ICONEXCLAMATION + MB_OK);
		else
		{
			if(GetFileSize(hFile, NULL) != BF2_FILE_SIZE)
			{
				MessageBox(NULL, "Bf2.exe file size incorrect! Aborting...", "DEVIOUS-100.COM", MB_ICONASTERISK + MB_OK);
				CloseHandle(hFile);
				return FALSE;
			}
			CloseHandle(hFile);
		}
#endif


#ifdef CHECK_PBCL_SIZE
		hFile = CreateFile( pbcl_dll.c_str(), FILE_READ_ATTRIBUTES,
											  FILE_SHARE_READ, NULL,
											  OPEN_EXISTING,
											  FILE_ATTRIBUTE_NORMAL, NULL );
		if( hFile == INVALID_HANDLE_VALUE)
			MessageBox(NULL, "Warning: can't check PunkBuster pbcl.dll file size!", "DEVIOUS-100.COM", MB_ICONEXCLAMATION + MB_OK);
		else
		{
			if(GetFileSize(hFile, NULL) != PBCL_FILE_SIZE)
			{
				MessageBox(NULL, "pbcl.dll file size incorrect! Aborting...", "DEVIOUS-100.COM", MB_ICONASTERISK + MB_OK);
				CloseHandle(hFile);
				return FALSE;
			}
			CloseHandle(hFile);
		}
#endif

		InitDetours();

		break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}



BOOL GeneralInit( void )
{
	if( !SetMainLoopDetours(TRUE) )
	{
		char* error = "Failed to initialize DEVIOUS-100";
		MessageBox(NULL, error, "DEVIOUS-100.COM", MB_ICONERROR + MB_OK );
		return FALSE;
	}
	else
	{
		UpdateGlobalObjects();
		return TRUE;
	}
}

void InitDetours()
{
	SetGeneralDetours( TRUE );

	renddx9_hModule = GetModuleHandle("RendDx9.dll");
	renddx9_base = GetModuleBase(renddx9_hModule);

	if(GetModuleHandle("RendDx9.dll"))
		SetMainLoopDetours(TRUE);

	if(GetModuleHandle("pbcl.dll"))
		SetPbclDetours(TRUE);
}