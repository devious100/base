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
//! \file	EngineCvar.cpp
//! \brief  Replication IEngineCVar class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "EngineCvar.h"
#include "..\Hooks\Hooks.h"

CEngineCVar gCvar;

////////////////////////////////////////////////////////////////////////////////
// CEngineCVar::FindVar
//! 
//! \param szVar - 
//! \return ConVar* - 
////////////////////////////////////////////////////////////////////////////////
ConVar* CEngineCVar::FindVar( const char* szVar )
{
	typedef ConVar* ( __thiscall *tFindVar )( void*, const char* szVar );
	return GetMethod< tFindVar >( ID_FINDCVAR )( GetInstance(), szVar );
}