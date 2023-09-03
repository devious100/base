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

#ifndef LOCAL_H
#define LOCAL_H

#include "Entity.h"

class CLocalBaseEntity : public CBaseEntity
{
public:
	// static function to get the local base entity
	static CLocalBaseEntity* Get();

#ifdef HACK_CSGO
	CVector3			GetViewPunchAngle( );
#endif
	CVector3			GetPunchAngle( );	
	CVector3			GetViewOffset( );	
	CVector3			GetLocalEyeAngles( );
	DWORD_PTR			GetLocalPlayerExclusive( );
};

#endif