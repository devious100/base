/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/*
	Credits to dom1n1k for the DataCompare function.

	http://forum.gamedeception.net/
	http://forum.gamedeception.net/showthread.php?t=5484
*/

/*************/
/* cuBot.cpp */
/*************/

#include "stdafx.h"
#pragma warning ( disable : 4312 )
#pragma warning ( disable : 4311 )

//////////////////////////////////////////////////
// Globals
CBot *				pBot				= NULL;

HMODULE				hGfxModule			= NULL;
MODULEINFO			gfxMI;
CLogStream *		pLog				= NULL;

t_GetRefAPI			o_GetRefAPI			= NULL;
t_GetProcAddress	o_GetProcAddress	= NULL;
//t_LoadLibraryA		o_LoadLibraryA		= NULL;
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// DataCompare
BOOL DataCompare( PBYTE pbData, PBYTE pbMask, char * szMask )
{
	for( ; *szMask; ++szMask, ++pbData, ++pbMask )
	{
		if( *szMask == 'x' && *pbData != *pbMask )
			return FALSE;
	}

	return ( *szMask == NULL );
}
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// GetRefAPI hook
refexport_t * h_GetRefAPI( int apiVersion, refimport_t * rimp )
{
	refexport_t * rexp;

	ri = new refimport_t( (*rimp) );

	rexp = o_GetRefAPI( apiVersion, rimp );

	//HookRefExports( rexp );
	re = new refexport_t( (*rexp) );
	rexp->Shutdown				= &RE_Shutdown;
	rexp->BeginRegistration		= &RE_BeginRegistration;
	rexp->AddRefEntityToScene	= &RE_AddRefEntityToScene;
	rexp->EndFrame				= &RE_EndFrame;
	rexp->RenderScene			= &RE_RenderScene;

	return rexp;
}
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// GetProcAddress hook
FARPROC WINAPI h_GetProcAddress( HMODULE hModule, LPCSTR lpProcName )
{
	if( HIWORD( lpProcName ) && _stricmp( lpProcName, "GetRefAPI" ) == 0 )
	{
		o_GetRefAPI = (t_GetRefAPI)o_GetProcAddress( hModule, lpProcName );

		if( o_GetRefAPI )
		{
			hGfxModule = hModule;

			GetModuleInformation( GetCurrentProcess( ), hGfxModule, &gfxMI, sizeof( gfxMI ) );

			o_ri = (refimport_t *)( (DWORD)gfxMI.lpBaseOfDll + 0x001D49D8 );

			return (FARPROC)h_GetRefAPI;
		}
	}

	return o_GetProcAddress( hModule, lpProcName );
}
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Hooking/Unhooking
void InitializeGameDetours( )
{
	// Hook LoadLibraryA
	//o_LoadLibraryA = (t_LoadLibraryA)DetourFunction( (PBYTE)LoadLibraryA, (PBYTE)h_LoadLibraryA );

	// Hook GetProcAddress
	o_GetProcAddress = (t_GetProcAddress)DetourFunction( (PBYTE)GetProcAddress, (PBYTE)h_GetProcAddress );

	// Hook CG_Player
	o_CG_Player = (t_CG_Player)DetourFunction( (PBYTE)0x004DC6D0, (PBYTE)h_CG_Player );

	// Hook CG_Obituary
	o_CG_Obituary = (t_CG_Obituary)DetourFunction( (PBYTE)0x004E03F0, (PBYTE)h_CG_Obituary );

	// Hook CG_FireRecoil
	o_CG_FireRecoil = (t_CG_FireRecoil)DetourFunction( (PBYTE)0x004F5860, (PBYTE)h_CG_FireRecoil );

	// Hook CG_ResetPlayerEntity
	//o_CG_ResetPlayerEntity = (t_CG_ResetPlayerEntity)DetourFunction( (PBYTE)0x004DCA00, (PBYTE)h_CG_ResetPlayerEntity );
}

void DestroyGameDetours( )
{
	// Unhook CG_ResetPlayerEntity
	//DetourRemove( (PBYTE)o_CG_ResetPlayerEntity, (PBYTE)h_CG_ResetPlayerEntity );

	// Unhook CG_FireRecoil
	DetourRemove( (PBYTE)o_CG_FireRecoil, (PBYTE)h_CG_FireRecoil );

	// Unhook CG_Obituary
	DetourRemove( (PBYTE)o_CG_Obituary, (PBYTE)h_CG_Obituary );

	// Unhook CG_Player
	DetourRemove( (PBYTE)o_CG_Player, (PBYTE)h_CG_Player );

	// Unhook GetProcAddress
	DetourRemove( (PBYTE)o_GetProcAddress, (PBYTE)h_GetProcAddress );

	// Unhook LoadLibraryA
	//DetourRemove( (PBYTE)o_LoadLibraryA, (PBYTE)h_LoadLibraryA );
}
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// DllMain
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
		pLog = new CLogStream( "cuBot.log" );
		pLog->Printf( "\n[ Log File Opened ]\n\n" );

		InitializeGameDetours( );
	}
	else if( ul_reason_for_call == DLL_PROCESS_DETACH )
	{
		DestroyGameDetours( );

		pLog->Printf( "\n\n[ Log File Closed ]\n" );
		delete pLog;
	}

    return TRUE;
}
//////////////////////////////////////////////////

#pragma warning ( default : 4312 )
#pragma warning ( default : 4311 )