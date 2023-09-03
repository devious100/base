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

#include "Hooks.h"
#include "..\HL\Hl.h"

////////////////////////////////////////////////////////////////////////////////
// Hook
//! Do the hooking!
//! 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void Hook()
{
	g_pPanel.HookMethod( ID_PAINTTRAVERSE, (DWORD_PTR)PaintTraverse );

	g_pClient.HookMethod( 20, (DWORD_PTR)IN_KeyEvent );

	HL::GetClientMode()->HookMethod( ID_CREATEMOVE, (DWORD_PTR)CreateMove );
}