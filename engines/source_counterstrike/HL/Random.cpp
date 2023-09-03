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
//! \file	Random.cpp
//! \brief  Random functions used by various engine shits
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Random.h"
#include "..\Utils\FindPattern.h"

CRandom gRandom;

////////////////////////////////////////////////////////////////////////////////
// CRandom::CRandom
//! Constructor
//! 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
CRandom::CRandom()
{
	m_randomSeed = NULL;
	m_randomFloat = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// CRandom::MD5_PseudoRandom
//! 
//! \param iSeed - 
//! \return size_t - 
////////////////////////////////////////////////////////////////////////////////
unsigned int CRandom::MD5_PseudoRandom( unsigned int iSeed )
{
	static DWORD_PTR dwFunction = FindPattern( gModules[MODULE_CLIENT].dwValue, 0xFFFFFFFF, 0x0, false, (BYTE*)"\x55\x8B\xEC\x83\xEC\x00\x6A\x00\x8D\x45\x00\x6A\x00\x50\xE8\x00\x00\x00\x00\x6A\x00\x8D\x4D\x00\x51\x8D\x55\x00\x52", "xxxxx?x?xx?x?xx????x?xx?xxx?" );

	__asm
	{
		PUSH iSeed
		CALL dwFunction
		ADD  ESP, 0x4
	}
}

////////////////////////////////////////////////////////////////////////////////
// CRandom::SetupOffsets
//! Set up the offsets needed for RandomFloat/RandomSeed
//! It's done like this incase the wrong function gets called first
//! 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CRandom::SetupOffsets()
{
	if( m_randomSeed )
		return;

#ifdef HACK_CSGO
	m_randomSeed = FindPattern( gModules[MODULE_VSTDLIB].dwValue, 0x00FFFFFF, 0x0, false, (BYTE*)"\x55\x8B\xEC\x8B\xD\x00\x00\x00\x00\x8B\x1\x8B\x55\x00\x8B\x0\x52\xFF\xD0\x5D\xC3\xCC", "xxxxx????xxxx?xxxxxxx" );
#else
	m_randomSeed = FindPattern( gModules[MODULE_VSTDLIB].dwValue, 0x00FFFFFF, 0x0, false, (BYTE*)"\x55\x8B\xEC\x8B\xD\x00\x00\x00\x00\x8B\x1\x8B\x55\x00\x8B\x0\x52\xFF\xD0\x5D\xC3", "xxxxx????xxxx?xxxxxx" );
#endif

	// random float is always + 0x20
	m_randomFloat = m_randomSeed + 0x20;
}

////////////////////////////////////////////////////////////////////////////////
// CRandom::RandomSeed
//! 
//! \param iSeed - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CRandom::RandomSeed( unsigned int iSeed )
{
	if( !m_randomSeed )
		SetupOffsets();

	__asm
	{
		PUSH [iSeed]
		MOV  EAX, [m_randomSeed]
		CALL EAX
		ADD  ESP, 0x4
	}
}

////////////////////////////////////////////////////////////////////////////////
// CRandom::RandomFloat
//! 
//! \param flMin - 
//! \param flMax - 
//! \return float - 
////////////////////////////////////////////////////////////////////////////////
float CRandom::RandomFloat( float flMin, float flMax )
{
	if( !m_randomSeed )
		SetupOffsets();

	float flRandom = 0.0f;
	__asm
	{
		FLD  flMax
		SUB  ESP, 0x8
		FSTP DWORD PTR SS:[ESP+0x4]
		FLD  flMin
		FSTP DWORD PTR SS:[ESP]
		MOV  EAX, [m_randomFloat]
		CALL EAX
		FSTP flRandom
		ADD  ESP,0x8
	}

	return flRandom;
}