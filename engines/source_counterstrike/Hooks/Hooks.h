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
//! \file	Hooks.h
//! \brief  Hook the engine!
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef HOOKS_H
#define HOOKS_H

#include "..\Include.h"
#include "..\HL\SDK.h"

#ifdef HACK_CSGO
	#define ID_CREATEMOVE 24
	#define ID_SETCURSORPOS 7
	#define ID_GETCURSORPOS 8
	#define ID_PAINTTRAVERSE 41
	#define ID_GETVIEWANGLES 18
	#define ID_SETVIEWANGLES 19
	#define ID_WORLDTOSCREENMATRIX 37
	#define ID_FINDCVAR 16
	#define ID_TRACERAY 5
	#define ID_DRAWSETCOLOUR 14
	#define ID_DRAWLINE 18
	#define ID_DRAWFILLEDRECT 15
	#define ID_CREATEFONT 70
	#define ID_SETFONTGLYPHSET 71
	#define ID_GETTEXTSIZE 78
	#define ID_DRAWSETTEXTCOLOUR 24
	#define ID_DRAWSETTEXTFONT 22
	#define ID_DRAWSETTEXTPOS 25
	#define ID_DRAWPRINTTEXT 27
	#define ID_GETMODELMATERIALCOUNT 16
#else
	#define ID_CREATEMOVE 21
	#define ID_SETCURSORPOS 6
	#define ID_GETCURSORPOS 7
	#define ID_PAINTTRAVERSE 40
	#define ID_GETVIEWANGLES 19
	#define ID_SETVIEWANGLES 20
	#define ID_WORLDTOSCREENMATRIX 36
	#define ID_FINDCVAR 12
	#define ID_TRACERAY 4
	#define ID_DRAWSETCOLOUR 11
	#define ID_DRAWLINE 15
	#define ID_DRAWFILLEDRECT 12
	#define ID_CREATEFONT 65
	#define ID_SETFONTGLYPHSET 66
	#define ID_GETTEXTSIZE 73
	#define ID_DRAWSETTEXTCOLOUR 19
	#define ID_DRAWSETTEXTFONT 17
	#define ID_DRAWSETTEXTPOS 20
	#define ID_DRAWPRINTTEXT 22
	#define ID_GETMODELMATERIALCOUNT 15
#endif

void Hook();

// function prototypes
void __stdcall PaintTraverse( unsigned int vguiPanel, bool forceRepaint, bool allowForce );
int	 __stdcall IN_KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding );
bool __stdcall CreateMove( float flInputSampleTime, CUserCmd *cmd );

#endif // HOOKS_H