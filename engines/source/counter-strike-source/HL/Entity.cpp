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
//! \file	Entity.cpp
//! \brief  Custom implementation of CBaseEntity
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Entity.h"
#include "NVmanager.h"
#include "..\Utils\CRC32.h"
#include "..\Utils\XStr.h"
#include "..\Utils\FindPattern.h"
#include "ModelInfo.h"

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetAbsOrigin
//! 
//! \return CVector3 - 
////////////////////////////////////////////////////////////////////////////////
CVector3 CBaseEntity::GetAbsOrigin( )
{
#ifdef HACK_CSGO
	__asm
	{
		MOV ECX, this
		MOV EAX, DWORD PTR DS:[ECX]
		CALL DWORD PTR DS:[EAX+0x28]
	}
#else
	__asm
	{
		MOV ECX, this
		MOV EAX, DWORD PTR DS:[ECX]
		CALL DWORD PTR DS:[EAX+0x24]
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetAbsAngles
//! 
//! \return CVector3 - 
////////////////////////////////////////////////////////////////////////////////
CVector3 CBaseEntity::GetAbsAngles( )
{
#ifdef HACK_CSGO
	__asm
	{
		MOV ECX, this
		MOV EAX, DWORD PTR DS:[ECX]
		CALL DWORD PTR DS:[EAX+0x2C]
	}
#else
	__asm
	{
		MOV ECX, this
		MOV EAX, DWORD PTR DS:[ECX]
		CALL DWORD PTR DS:[EAX+0x28]
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetIndex
//! 
//! \return int - 
////////////////////////////////////////////////////////////////////////////////
int CBaseEntity::GetIndex( )
{
#ifdef HACK_CSGO
	__asm
	{
		MOV EDI, this
		LEA ECX, DWORD PTR DS:[EDI+0x8]
		MOV EDX, DWORD PTR DS:[ECX]
		CALL DWORD PTR DS:[EDX+0x28]
	}
#else
	__asm
	{
		MOV EDI, this
		LEA ECX, DWORD PTR DS:[EDI+0x8]
		MOV EDX, DWORD PTR DS:[ECX]
		CALL DWORD PTR DS:[EDX+0x24]
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::IsDormant
//! 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CBaseEntity::IsDormant( )
{
#ifdef HACK_CSGO
	__asm
	{
		MOV EDI, this
		LEA ECX, [EDI + 0x8]
		MOV EDX, DWORD PTR DS:[ecx]
		CALL [EDX + 0x24]
	}
#else
	__asm
	{
		MOV EDI, this
		LEA ECX, [EDI + 0x8]
		MOV EDX, DWORD PTR DS:[ECX]
		CALL [EDX + 0x20]
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::IsBaseCombatCharacter
//! 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CBaseEntity::IsBaseCombatCharacter( )
{
#ifdef HACK_CSS
	return GetClassHash() == CCSPLAYER;
#elif HACK_CSGO
	return GetClassHash() == CCSPLAYER;
#elif HACK_TF2
	return GetClassHash() == CTFPLAYER;
#elif HACK_DOD
	return GetClassHash() == CDODPLAYER;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetClassHash
//! 
//! \return DWORD_PTR - 
////////////////////////////////////////////////////////////////////////////////
DWORD_PTR CBaseEntity::GetClassHash( )
{
	ClientClass* pClass = GetClientClass( );

	if( !pClass )
		return 0;

	const char* pszName = pClass->m_pNetworkName;

	if( !pszName )
		return 0;

	return CRC32( (void*)pszName, strlen( pszName ) );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetClientClass
//! 
//! \return ClientClass* - 
////////////////////////////////////////////////////////////////////////////////
ClientClass* CBaseEntity::GetClientClass( )
{
	__asm
	{
		MOV EDI, this
		LEA ECX, [EDI + 0x8]
		MOV EDX, DWORD PTR DS:[ECX]
		CALL [EDX + 0x8]
	}
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetModel
//! 
//! \return model_t* - 
////////////////////////////////////////////////////////////////////////////////
model_t* CBaseEntity::GetModel( )
{
#ifdef HACK_CSGO
	__asm
	{
		MOV EDI, this
		LEA ECX, [EDI + 0x4]
		MOV EDX, DWORD PTR DS:[ECX]
		CALL [EDX + 0x20]
	}
#else
	__asm
	{
		MOV EDI, this
		LEA ECX, [EDI + 0x4]
		MOV EDX, DWORD PTR DS:[ECX]
		CALL [EDX + 0x24]
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::SetupBones
//! 
//! \param matrix - 
//! \param maxbones - 
//! \param mask - 
//! \param time - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CBaseEntity::SetupBones( matrix3x4_t* matrix, int maxbones, int mask, float time )
{
#ifdef HACK_CSGO
	__asm
	{
		MOV EDI, this
		LEA ECX, [EDI + 0x4]
		MOV EDX, DWORD PTR DS:[ecx]
		PUSH time
		PUSH mask
		PUSH maxbones
		PUSH matrix
		CALL [EDX + 0x34]
	}
#else
	__asm
	{
		MOV EDI, this
		LEA ECX, [EDI + 0x4]
		MOV EDX, DWORD PTR DS:[ecx]
		PUSH time
		PUSH mask
		PUSH maxbones
		PUSH matrix
		CALL [EDX + 0x40]
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetHealth
//! 
//! \return int - 
////////////////////////////////////////////////////////////////////////////////
int CBaseEntity::GetHealth( )
{
	MAKE_NETVAR( int, 0xA93054E3 );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetTeam
//! 
//! \return int - 
////////////////////////////////////////////////////////////////////////////////
int CBaseEntity::GetTeam( )
{
	MAKE_NETVAR( int, 0xC08B6C6E );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetLifeState
//! DESCRIPTION
//! 
//! \return char - 
////////////////////////////////////////////////////////////////////////////////
char CBaseEntity::GetLifeState( )
{
	MAKE_NETVAR( char, 0xD795CCFC );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetFlags
//! 
//! \return int - 
////////////////////////////////////////////////////////////////////////////////
int CBaseEntity::GetFlags( )
{
	MAKE_NETVAR( int, 0xE456D580 );
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetEyeAngles
//! 
//! \return Vector3D - 
////////////////////////////////////////////////////////////////////////////////
CVector3 CBaseEntity::GetEyeAngles( )
{
#ifdef HACK_TF2
	MAKE_NETVAR( CVector3, 0xB2528FC3 );
#elif HACK_CSS
	MAKE_NETVAR( CVector3, 0xA6F17F3A );
#elif HACK_CSGO
	MAKE_NETVAR( CVector3, 0xA6F17F3A );
#elif HACK_DOD
	MAKE_NETVAR( CVector3, 0xF2AAB150 );
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetBaseCombatWeapon
//! 
//! \return CBaseCombatWeapon* - 
////////////////////////////////////////////////////////////////////////////////
CBaseCombatWeapon* CBaseEntity::GetBaseCombatWeapon( )
{
#ifdef HACK_CSS
	static PDWORD_PTR pdwOffset = ( PDWORD_PTR )FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, false, (BYTE*)"\x8B\x90\x00\x00\x00\x00\x53\x8B\xCF\xFF\xD2\x8B\xD8\x8B\x6\x8B\x50\x00\x8B\xCE\xFF\xD2", "xx????xxxxxxxxxxx?xxx" );
#elif HACK_TF2
	static PDWORD_PTR pdwOffset = ( PDWORD_PTR )FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, false, (BYTE*)"\x8B\x82\x00\x00\x00\x00\x83\xC4\x00\x8B\xCE\xFF\xD0\x3B\xC7\x74\x00\x8B\x10\x89\x45\x00", "xx????xx?xxxxxxx?xxxx" );
#elif HACK_DOD
	static PDWORD_PTR pdwOffset = ( PDWORD_PTR )FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x2, false, (BYTE*)"\x8B\x82\x00\x00\x00\x00\x83\xC4\x00\x8B\xCE\xFF\xD0\x3B\xC7\x74\x00\x8B\x10\x89\x45\x00", "xx????xx?xxxxxxx?xxxx" );
#endif

	static DWORD_PTR dwOffsetPtr = NULL;

	if( !dwOffsetPtr )
	{
		memcpy( &dwOffsetPtr, (void*)pdwOffset, 4 );
	}

	__asm
	{
		MOV EAX, this
		MOV EDX, DWORD PTR DS:[EAX]
		MOV ECX, EAX
		MOV EAX, DWORD PTR DS:[dwOffsetPtr]
		MOV EAX, [EDX+EAX]
		CALL EAX
	}
}

////////////////////////////////////////////////////////////////////////////////
// CBaseEntity::GetModelName
//! Helper function to get the mode name
//! 
//! \param model_t * mdl - 
//! \param out - 
//! \param len - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CBaseEntity::GetModelName( const model_t * mdl, char * out, size_t len )
{
	if( !out )
		return false;

	if( !mdl )
		return false;

	std::string sName = gModelInfo.GetModelName( mdl );

	size_t nPos = sName.find_last_of( "/" );

	if( !nPos != std::string::npos )
	{
		nPos += 1;

		if( sName.find( "w_" ) != std::string::npos || sName.find( "c_" ) != std::string::npos )
			nPos += 2;

		sName = sName.substr( nPos );
	}	

	sName = sName.erase( sName.length( ) - 4, 4 );

	sName[ 0 ] = toupper( sName[ 0 ] );

	while( ( nPos = sName.find( "_" ) ) != std::string::npos )
	{
		sName.replace( nPos, 1, " " );
		sName[ nPos+1 ] = toupper( sName[ nPos+1 ] );
	}

	strcpy_s( out, len, sName.c_str( ) );

	return true;
}