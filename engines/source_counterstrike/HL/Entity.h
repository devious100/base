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
//! \file	Entity.h
//! \brief  Custom implementation of CBaseEntity
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef ENTITY_H
#define ENTITY_H

#include "..\Include.h"
#include "Vector3.h"
#include "SDK.h"

class CBaseCombatWeapon;

class CBaseEntity
{
public:
	CVector3			GetAbsOrigin( );
	CVector3			GetAbsAngles( );
	int					GetIndex( );
	bool				IsDormant( );
	bool				IsBaseCombatCharacter( );
	ClientClass*		GetClientClass( );
	model_t*			GetModel( );
	bool				SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );
	DWORD_PTR			GetClassHash( );
	int					GetHealth( );
	int					GetTeam( );
	char				GetLifeState( );
	int					GetFlags( );
	CBaseCombatWeapon*	GetBaseCombatWeapon( );
	CVector3			GetEyeAngles( );	
	bool				GetModelName( const model_t * mdl, char * out, size_t len );
};

// client class hash names
#define CBASEANIMATING					0x5b26cf98
#define CFUNCRESPAWNROOMVISUALISER		0x738820d2
#define COBJECTDISPENSER				0x4a618798		
#define COBJECTSENTRYGUN				0x12156371
#define COBJECTTELEPORTER				0xdd0f752
#define CTFPLAYER						0xd9d648e3
#define CTFPROJECTILE_ARROW				0xd3a70d1d
#define CTFPROJECTILE_FLARE				0x5b6b4d8e
#define CTFGRENADEPIPEBOMBPROJECTILE	0x1de0da4a
#define CTFPROJECTILE_ROCKET			0x6cd55a8f	
#define CTFSTUNBALL						0x727dff87
#define CDODPLAYER						0x7bf7d905
#define CDODBASEGRENADE					0x24cd39e4
#define CCSPLAYER						0x9a4ec3b8
#define CPLANTEDC4						0xc9bac39d
#define CBASECSGRENADEPROJECTILE		0xfd7e2c06
#define CHEGRENADE						0xaa01bc10
#define CSMOKEGRENADE					0x8176032c
#define CPARTICLESMOKEGRENADE			0x314dffec
#define CAK47							0x8581de05
#define CGLOCK							0xbe3332e6
#define CKNIFE							0x85ad9950
#define CGALIL							0x2a94294b
#define CSCOUT							0xe2a0abd0
#define CAWP							0x118e0119
#define CG3SG1							0x7a7a68aa
#define CUSP							0x6ecdcfb1
#define P228							0x6f0841c6
#define CDeagle							0xd6c7d366
#define CElites							0x2245b4a5
#define CM3								0x660adfe0
#define CXM1014							0xb13c841b
#define CMAC10							0xe3b8558a
#define CMP5							0x2b320309
#define CP90							0xbaa6ef2f
#define CM249							0xec265994
#define CFIVESEVEN						0x29dc18b3
#define CUMP45							0xea832b92
#define CTMP							0xbb4e9a59
#define CFAMAS							0x53e7ed31
#define CM4A1							0xb43820a7
#define CAUG							0xe81e236
#define CSG550							0x2d13f253

#endif