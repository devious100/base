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
//! \file	Weapon.cpp
//! \brief  Weapon definition
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Weapon.h"
#include "..\Utils\FindPattern.h"
#include "NVManager.h"

////////////////////////////////////////////////////////////////////////////////
// CBaseCombatWeapon::GetWeaponID
//! 
//! \return int - 
////////////////////////////////////////////////////////////////////////////////
int CBaseCombatWeapon::GetWeaponID( )
{
	int iID = 0;

#ifdef HACK_CSS

	static DWORD_PTR dwGetActiveWeapon = FindPattern( gModules[MODULE_CLIENT].dwValue, 0xFFFFFFFF, 0x2, false, (BYTE*)"\x8B\x90\x00\x00\x00\x00\x8B\xCF\xFF\xD2\x83\xF8\x00\x75\x00\x8B\x45\x00\xF7\xD8", "xx????xxxxxx?x?xx?x" );
	static DWORD_PTR dwPTR = NULL;

	if( !dwPTR )
	{
		memcpy( &dwPTR, (void*)dwGetActiveWeapon, 4 );
	}

	__asm
	{
		MOV ECX, this
		MOV EAX,DWORD PTR DS:[ECX]
		MOV EDX,DWORD PTR DS:[dwPTR]
		MOV EDX,DWORD PTR DS:[EAX+EDX]
		CALL EDX
		MOV iID, EAX
	}
#elif HACK_TF2
	__asm
	{
		MOV ECX, this
		MOV EAX,DWORD PTR DS:[ECX]
		MOV EDX,DWORD PTR DS:[EAX+0x5B4]
		CALL EDX
		MOV iID, EAX
	}
#elif HACK_DOD // again, too lazy to fix. dods hasn't updated in years so it's fine
	iID = GetClientClass()->m_ClassID;

#elif HACK_CSGO

	__asm
	{
		MOV ECX, this
		MOV EAX,DWORD PTR DS:[ECX]
		MOV EDX,DWORD PTR DS:[EAX+0x5F0] // make your own pattern i'm too lazy
		CALL EDX
		MOV iID, EAX
	}

#endif

	return iID;
}

////////////////////////////////////////////////////////////////////////////////
// CBaseCombatWeapon::GetLastFireTime
//! 
//! \return float - 
////////////////////////////////////////////////////////////////////////////////
float CBaseCombatWeapon::GetLastFireTime( )
{
	MAKE_NETVAR( float, 0x1FF61ED4 );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseCombatWeapon::GetNextPrimaryAttack
//! 
//! \return float - 
////////////////////////////////////////////////////////////////////////////////
float CBaseCombatWeapon::GetNextPrimaryAttack( )
{
	MAKE_NETVAR( float, 0xDB7B106E );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseCombatWeapon::GetNextSecondaryAttack
//! 
//! \return float - 
////////////////////////////////////////////////////////////////////////////////
float CBaseCombatWeapon::GetNextSecondaryAttack( )
{
	MAKE_NETVAR( float, 0xFEDAF53 );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseCombatWeapon::GetClip1
//! 
//! \return int - 
////////////////////////////////////////////////////////////////////////////////
int CBaseCombatWeapon::GetClip1( )
{
	MAKE_NETVAR( int, 0x1CD99C69 );
}