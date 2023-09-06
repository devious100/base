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
//! \file	Engine.h
//! \brief  Replication IEngineClient class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_H
#define ENGINE_H

#include "..\Include.h"
#include "Vector3.h"
#include "SDK.h"

class CEngineClient : public CVMethod
{
public:
	void		GetViewAngles( CVector3& vAngles );
	void		SetViewAngles( CVector3& vAngles );
	bool		GetPlayerInfo( int iEntNum, player_info_t *pInfo );
	int			GetLocalPlayer();
	VMatrix&	WorldToScreenMatrix();
	void		GetScreenSize( int& iWidth, int& iHeight );
	bool		WorldToScreen( CVector3& vIn, CVector3& vOut );
};

extern CEngineClient gEngine;

#endif
	