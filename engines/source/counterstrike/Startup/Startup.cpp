////////////////////////////////////////////////////////////////////////////////
//! SourceBase By Chod
//! -------------------------------------------------------------------------
//! SourceBase is free software: you can redistribute it and/or modify 
//! it under the terms of the GNU Lesser General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//! 
//! SourceBase is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//! GNU Lesser General Public License for more details.
//! 
//! You should have received a copy of the GNU Lesser General Public License
//! along with SourceBase. If not, see <http://www.gnu.org/licenses/>.
//! -------------------------------------------------------------------------
//! \file	Startup.cpp
//! \brief  Main startup routine
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "..\Include.h"
#include "Interface.h"
#include "..\Utils\PETools.h"
#include "..\Utils\FindPattern.h"
#include "..\Hooks\Hooks.h"
#include "..\SourceBase\SourceBase.h"

#include "..\HL\Engine.h"
#include "..\HL\EntList.h"
#include "..\HL\EngineCvar.h"
#include "..\HL\Trace.h"
#include "..\HL\Surface.h"
#include "..\HL\ModelInfo.h"
#include "..\HL\NVManager.h"

#define MAKE_MODULE( hash ) { NULL, hash },

Module_t gModules[MODULE_MAX] = {
	MAKE_MODULE( 0x61ee3ed1 ) // client.dll
	MAKE_MODULE( 0x220245d7 ) // engine.dll
	MAKE_MODULE( 0x7fc54971 ) // vstdlib.dll
	MAKE_MODULE( 0x783aff85 ) // matsurface.dll
	MAKE_MODULE( 0xe8483ba )  // launcher.dll
	MAKE_MODULE( 0xe8483ba )  // shaderapi_dx9.dll
	MAKE_MODULE( 0x2e3c2275 ) // tier0.dll
	MAKE_MODULE( 0x14a71afe ) // vgui.dll
	MAKE_MODULE( 0x8db91414 ) // physics.dll
	MAKE_MODULE( 0x309dad83 ) // materialsystem.dll
	MAKE_MODULE( 0xd76795b9 ) // overlay...whatever.dll	
};

// A list of virtual interfaces we may need
CVMethod g_pClient;
CVMethod g_pVGUI;
CVMethod g_pPrediction;
CVMethod g_pMovement;
CVMethod g_pInput;
CVMethod g_pSurface;
CVMethod g_pPanel;
CVMethod g_pSound;
CVMethod g_pModelRender;
CVMethod g_pCmdInput;
CVMethod g_pMaterial;
CVMethod g_pRenderView;
CVMethod g_pPlatTime;

CFont* g_pMyFont = NULL;

////////////////////////////////////////////////////////////////////////////////
// FindModules
//! Find all the required HL modules
//! 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool FindModules()
{
	bool bFailed = false;

	// iterate through all the modules so that we can try to find every module
	// if we are using threads it's safe to keep searching, otherwise the game would lock up!
#if USE_THREADS
	while( 1 )
	{
#endif
		for( unsigned int i = 0; i < MODULE_MAX; ++i )
		{
			gModules[i].dwValue = reinterpret_cast< DWORD_PTR >( Utils::PE::GetModuleHandle( gModules[i].dwHash ) );

			if( !gModules[i].dwValue )
				bFailed = true;
		}

#if USE_THREADS
		// if using threads and all modules were found, we can just exit here.
		// otherwise keep on searching
		// TODO: maybe add an exit condition here such as keeping the number of loops to 10, for example
		if( !bFailed )
			break;
	}
#endif

	// make an error notification if one or more modules were not found
	if( bFailed )
	{		
		ERR( "Coult not find one or more required modules" );
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Startup
//! This is the main startup routine of the hack.
//! 
//! \return BOOL - 
////////////////////////////////////////////////////////////////////////////////
BOOL Startup()
{
	// try and find all the modules
	if( !FindModules() )
		return FALSE;

	// now get all the needed interfaces by hash value
	if( !GetInterface( gModules[MODULE_CLIENT].dwValue, 0x9e1f0acc, &g_pClient ) )			return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0xb8b3de9c, &g_pVGUI ) )			return FALSE;
	if( !GetInterface( gModules[MODULE_CLIENT].dwValue, 0xe87be103, &gEntList ) )			return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0x83ab6d25, &gEngine ) )			return FALSE;
	if( !GetInterface( gModules[MODULE_CLIENT].dwValue, 0x8d050ba8, &g_pPrediction ) )		return FALSE;
	if( !GetInterface( gModules[MODULE_CLIENT].dwValue, 0x92a33900, &g_pMovement ) )		return FALSE;
	if( !GetInterface( gModules[MODULE_PHYSICS].dwValue, 0x3706599f, &g_pSurface ) )		return FALSE;
	if( !GetInterface( gModules[MODULE_VGUI].dwValue, 0xbac58f71, &g_pInput ) )				return FALSE;
	if( !GetInterface( gModules[MODULE_VGUI].dwValue, 0xea5e12aa, &g_pPanel ) )				return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0x26c23014, &g_pSound ) )			return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0xb59bc006, &g_pModelRender ) )		return FALSE;
	if( !GetInterface( gModules[MODULE_MATERIAL].dwValue, 0xac5425d6, &g_pMaterial ) )		return FALSE;	

#ifdef HACK_CSGO
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0x6a8e23ca, &gModelInfo ) )			return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0x510a076, &gTrace ) )				return FALSE;
	if( !GetInterface( gModules[MODULE_VSTDLIB].dwValue, 0xb968fd14, &gCvar ) )				return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0xadf811cd, &g_pRenderView ) )		return FALSE;
#else
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0x6c4c6845, &gModelInfo ) )			return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0x9b7435d5, &gTrace ) )				return FALSE;
	if( !GetInterface( gModules[MODULE_VSTDLIB].dwValue, 0x2061acae, &gCvar ) )				return FALSE;
	if( !GetInterface( gModules[MODULE_ENGINE].dwValue, 0x339c846e, &g_pRenderView ) )		return FALSE;
#endif

#ifdef HACK_CSGO
	CreateInterfaceFn appSystemFactory = (CreateInterfaceFn)FindPattern( gModules[MODULE_ENGINE].dwValue, 0x00634000, 0x0, false, (BYTE*)"\x55\x8B\xEC\x8B\x45\x00\x8B\xD\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\x8B\x4D\x00\x85\xC9\x74\x00", "xxxxx?xx????xx????xx?xxx" );
#else
	CreateInterfaceFn appSystemFactory = (CreateInterfaceFn)FindPattern( gModules[MODULE_ENGINE].dwValue, 0x00634000, 0x1, true, (BYTE*)"\xA1\x00\x00\x00\x00\x8B\xD\x00\x00\x00\x00\x8B\x11\x83\xC4\x00\x68\x00\x00\x00\x00", "x????xx????xxxx?x???" );
#endif

	// for some reason getting surface pointer by hash always results in a b0rked address so have to use appSystemFactory to get it :/
	void* lpSurface = NULL;
#ifdef HACK_CSGO
	lpSurface = ( void* )appSystemFactory( "VGUI_Surface031", 0 );
#else
	lpSurface = ( void* )appSystemFactory( "VGUI_Surface030", 0 );
#endif
	gSurface.Init( (PDWORD_PTR*)lpSurface );

	// create a font
	g_pMyFont = CSourceBase::Get()->CreateFont( "Helvitica", 14, 300, kFontFlag_DROPSHADOW );

	// now do the hooking
	Hook();

	// dump all the netvars (you can remove this)
#ifdef SB_DEBUG
	gNVManager.DumpAllClasses();
#endif
	
	return TRUE;
}