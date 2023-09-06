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
//! \file	HL.cpp
//! \brief  Some useful engine functions
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////


#include "HL.h"
#include "..\Utils\FindPattern.h"
#include "..\Utils\XStr.h"

////////////////////////////////////////////////////////////////////////////////
// HL::GetClientMode
//! Get the CLientModeShared pointer
//! 
//! \return CVMethod* - 
////////////////////////////////////////////////////////////////////////////////
CVMethod* HL::GetClientMode( )
{	
#ifdef HACK_CSGO
	static DWORD_PTR dwClientMode = FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, true, (BYTE*)"\xC7\x5\x00\x00\x00\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x10\x8B\xC8\x8B\x2\x68\x00\x00\x00\x00\xFF\xD0\xC3\xCC", "xx????????x????xxxxxxx????xxx" );
#else
	static DWORD_PTR dwClientMode = FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, true, (BYTE*)"\x8B\xD\x00\x00\x00\x00\x8B\x11\x8B\x42\x00\xFF\xD0\x50\x8B\xCF", XStr( /*xx????xxxx?xxxx*/ 0x04, 0x0F, 0xF2, 0x8A8BCBCA, 0xC9C88081, 0x8283C385, 0x86877800 ).c() );
#endif

	static CVMethod pClientMode;

	if( !pClientMode.IsReady() )
	{
		pClientMode.Init( (PDWORD_PTR*)dwClientMode );
	}

	return &pClientMode;
}

////////////////////////////////////////////////////////////////////////////////
// GetClobalVars
//! Get the CGlobalVarsBase pointer
//! 
//! \return CGlobalVarsBase* - 
////////////////////////////////////////////////////////////////////////////////
CGlobalVarsBase* GetClobalVars()
{
#ifdef HACK_CSS
	static PDWORD_PTR pdwGlobalVars = ( PDWORD_PTR )FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, false, (BYTE*)"\x8B\xD\x00\x00\x00\x00\xD9\x41\x00\x8B\xCF\xD9\x1C\x24\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\x8B\xCF\xE8\x00\x00\x00\x00\x8B\xF8", "xx????xx?xxxxxx????x????x?x????xxx????x" );
#elif HACK_DOD
	static PDWORD_PTR pdwGlobalVars = ( PDWORD_PTR )FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, false, (BYTE*)"\x8B\xD\x00\x00\x00\x00\xD9\x41\x00\x8B\xCF\xD9\x1C\x24\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\x8B\xCF\xE8\x00\x00\x00\x00\x8B\xF8", "xx????xx?xxxxxx????x????x?x????xxx????x" );
#elif HACK_CSGO
	static PDWORD_PTR pdwGlobalVars = ( PDWORD_PTR )FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, false, (BYTE*)"\x8B\xD\x00\x00\x00\x00\x8B\x96\x00\x00\x00\x00\xD9\x41\x00\xD9\x5A\x00\x8B\x96\x00\x00\x00\x00\x33\xDB\x83\xC4\x00\x33\xFF", "xx????xx????xx?xx?xx????xxxx?x" );
#else
	static PDWORD_PTR pdwGlobalVars = ( PDWORD_PTR )FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x1, false, (BYTE*)"\xA1\x00\x00\x00\x00\xD9\x40\x00\x5F\xD9\x9E\x00\x00\x00\x00\x5E\x5B\x8B\xE5\x5D", "x????xx?xxx????xxxx" );
#endif

	if( !pdwGlobalVars )
		return NULL;

	memcpy( (void*)&pdwGlobalVars, (void*)pdwGlobalVars, 4 );

	return reinterpret_cast< CGlobalVarsBase* >( *pdwGlobalVars );
}