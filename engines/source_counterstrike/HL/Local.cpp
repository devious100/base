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
//! \file	Local.h
//! \brief  Entity functions specific to the local entity
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Local.h"
#include "NVManager.h"
#include "Engine.h"
#include "EntList.h"

////////////////////////////////////////////////////////////////////////////////
// CLocalBaseEntity::Get
//! static function to get the local base entity
//! 
//! \return CLocalBaseEntity* - 
////////////////////////////////////////////////////////////////////////////////
CLocalBaseEntity* CLocalBaseEntity::Get()
{
	int iLocalIndex = gEngine.GetLocalPlayer();
	return reinterpret_cast< CLocalBaseEntity* >( gEntList.GetClientEntity( iLocalIndex ) );
}

////////////////////////////////////////////////////////////////////////////////
// CLocalBaseEntity::GetPunchAngle
//! 
//! \return Vector3D - 
////////////////////////////////////////////////////////////////////////////////
CVector3 CLocalBaseEntity::GetPunchAngle()
{
	DWORD_PTR dwLocal = GetLocalPlayerExclusive( );
#ifdef HACK_CSGO
	static DWORD_PTR dwPunch = gNVManager.GetOffset( 0xBF25C290 );
#else
	static DWORD_PTR dwPunch = gNVManager.GetOffset( 0x212D0495 );
#endif
	return *( CVector3* )( this + dwLocal + dwPunch );
}


////////////////////////////////////////////////////////////////////////////////
// CLocalBaseEntity::GetViewPunchAngle
//! 
//! \return CVector3 - 
////////////////////////////////////////////////////////////////////////////////
#ifdef HACK_CSGO
CVector3 CLocalBaseEntity::GetViewPunchAngle()
{
	DWORD_PTR dwLocal = GetLocalPlayerExclusive( );
	static DWORD_PTR dwPunch = gNVManager.GetOffset( 0x68F014C0 );

	return *( CVector3* )( this + dwLocal + dwPunch );
}
#endif

////////////////////////////////////////////////////////////////////////////////
// CLocalBaseEntity::GetLocalPlayerExclusive
//! 
//! \return DWORD_PTR - 
////////////////////////////////////////////////////////////////////////////////
DWORD_PTR CLocalBaseEntity::GetLocalPlayerExclusive()
{
	static DWORD_PTR dwOffset = gNVManager.GetOffset( 0x7177BC3E );
	return dwOffset;
}

////////////////////////////////////////////////////////////////////////////////
// CLocalBaseEntity::GetViewOffset
//! 
//! \return Vector3D - 
////////////////////////////////////////////////////////////////////////////////
CVector3 CLocalBaseEntity::GetViewOffset()
{
	MAKE_NETVAR( CVector3, 0xA9F74931 );
}

////////////////////////////////////////////////////////////////////////////////
// CLocalBaseEntity::GetEyeAngles
//! 
//! \return Vector3D - 
////////////////////////////////////////////////////////////////////////////////
CVector3 CLocalBaseEntity::GetLocalEyeAngles()
{
#ifdef HACK_DOD
	MAKE_NETVAR( CVector3, 0xF2AAB150 );
#else
	MAKE_NETVAR( CVector3, 0xA6F17F3A );
#endif
}