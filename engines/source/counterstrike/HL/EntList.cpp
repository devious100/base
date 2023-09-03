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
//! \file	EntList.cpp
//! \brief  Replication IEntityList class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "EntList.h"

CEntList gEntList;

////////////////////////////////////////////////////////////////////////////////
// CEntList::GetClientEntity
//! 
//! \param iIndex - 
//! \return CBaseEntity* - 
////////////////////////////////////////////////////////////////////////////////
CBaseEntity* CEntList::GetClientEntity( int iIndex )
{
	typedef CBaseEntity* ( __thiscall *tGetClientEntity )( void*, int entnum );
	return GetMethod< tGetClientEntity >( 3 )( GetInstance(), iIndex );
}

////////////////////////////////////////////////////////////////////////////////
// CEntList::GetHighestEntityIndex
//! 
//! \return int - 
////////////////////////////////////////////////////////////////////////////////
int CEntList::GetHighestEntityIndex()
{
	typedef int ( __thiscall *tGetHighestEntityIndex )( void* );
	return GetMethod< tGetHighestEntityIndex >( 6 )( GetInstance() );
}