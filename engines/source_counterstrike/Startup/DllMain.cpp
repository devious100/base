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
//! \file	DllMain.cpp
//! \brief	Dll entry point
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "..\Include.h"
#include "..\SourceBase\SourceBase.h"

////////////////////////////////////////////////////////////////////////////////
// SetupSourceBaseAPI
//! 
//! \param hModule - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void SetupSourceBaseAPI( HINSTANCE hModule )
{
#ifdef HACK_CSS
	CSourceBase::Get()->Init( hModule, "CSS" );
#elif HACK_TF2
	CSourceBase::Get()->Init( hModule, "TF2" );
#elif HACK_DOD
	CSourceBase::Get()->Init( hModule, "DODS" );
#elif HACK_CSGO
	CSourceBase::Get()->Init( hModule, "CSGO" );
#endif
}

////////////////////////////////////////////////////////////////////////////////
// DllMain
//! Dll Entry point
//! 
//! \param hinstDLL - handle to this module
//! \param fdwReason - attach reason
//! \param lpvReserved - 
//! \return BOOL WINAPI - return TRUE if successful, or FALSE if failed
////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	BOOL Startup();

	// remove DLL_THREAD_ATTACH and DLL_THREAD_DETACH calls as we don't care about them
	DisableThreadLibraryCalls( hinstDLL );

	// set up the SourceBase API
	SetupSourceBaseAPI( hinstDLL );

	if( fdwReason == DLL_PROCESS_ATTACH )
	{
#if USE_THREADS
		// create the startup thread
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Startup, NULL, 0, NULL );
#else
		return Startup();
#endif
	}
	else if( fdwReason == DLL_PROCESS_DETACH )
	{
		// make sure all resources get cleaned up
		CSourceBase::Get()->Shutdown();
	}
	return TRUE;
}