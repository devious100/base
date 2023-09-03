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
//! \file	Engine.cpp
//! \brief  Replication IEngineClient class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Engine.h"
#include "..\Hooks\Hooks.h"

CEngineClient gEngine;

////////////////////////////////////////////////////////////////////////////////
// CEngineClient::GetViewAngles
//! 
//! \param vAngles - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CEngineClient::GetViewAngles( CVector3& vAngles )
{
	typedef void ( __thiscall *tGetViewAngles )( void*, CVector3& );
	GetMethod< tGetViewAngles >( ID_GETVIEWANGLES )( GetInstance(), vAngles );
}

////////////////////////////////////////////////////////////////////////////////
// CEngineClient::SetViewAngles
//! 
//! \param vAngles - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CEngineClient::SetViewAngles( CVector3& vAngles )
{
	typedef void ( __thiscall *tSetViewAngles )( void*, CVector3& );
	GetMethod< tSetViewAngles >( ID_SETVIEWANGLES )( GetInstance(), vAngles );
}

////////////////////////////////////////////////////////////////////////////////
// CEngineClient::GetPlayerInfo
//! 
//! \param iEntNum - 
//! \param *pInfo - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CEngineClient::GetPlayerInfo( int iEntNum, player_info_t *pInfo )
{
	typedef bool ( __thiscall *tGetPlayerInfo )( void*, int, player_info_t* );
	return GetMethod< tGetPlayerInfo >( 8 )( GetInstance(), iEntNum, pInfo );
}

////////////////////////////////////////////////////////////////////////////////
// CEngineClient::GetLocalPlayer
//! 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
int	CEngineClient::GetLocalPlayer()
{
	typedef int ( __thiscall *tGetLocalPlayer )( void* );
	return GetMethod< tGetLocalPlayer >( 12 )( GetInstance() );
}

////////////////////////////////////////////////////////////////////////////////
// CEngineClient::WorldToScreenMatrix
//! 
//! \return VMatrix& - 
////////////////////////////////////////////////////////////////////////////////
VMatrix& CEngineClient::WorldToScreenMatrix()
{
	typedef VMatrix& ( __thiscall *tWorldToScreenMatrix )( void* );
	return GetMethod< tWorldToScreenMatrix >( ID_WORLDTOSCREENMATRIX )( GetInstance() );
}

////////////////////////////////////////////////////////////////////////////////
// CEngineClient::GetScreenSize
//! 
//! \param iWidth - 
//! \param iHeight - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CEngineClient::GetScreenSize( int& iWidth, int& iHeight )
{
	typedef void ( __thiscall *tGetScreenSize )( void*, int& width, int& height );
	GetMethod< tGetScreenSize >( 5 )( GetInstance(), iWidth, iHeight );
}

////////////////////////////////////////////////////////////////////////////////
// CEngineClient::WorldToScreen
//! Transform a 3D world space point into a 2d screen space point
//! 
//! \param vIn - 
//! \param vOut - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CEngineClient::WorldToScreen( CVector3& vIn, CVector3& vOut )
{
	float w;
	bool bBehind = false;

	VMatrix worldToScreen = WorldToScreenMatrix( );

	vOut.x = worldToScreen[0][0] * vIn[0] + worldToScreen[0][1] * vIn[1] + worldToScreen[0][2] * vIn[2] + worldToScreen[0][3];
	vOut.y = worldToScreen[1][0] * vIn[0] + worldToScreen[1][1] * vIn[1] + worldToScreen[1][2] * vIn[2] + worldToScreen[1][3];
	w	   = worldToScreen[3][0] * vIn[0] + worldToScreen[3][1] * vIn[1] + worldToScreen[3][2] * vIn[2] + worldToScreen[3][3];
	vOut.z = 0.0f;	

	if( w < 0.001f )
	{
		bBehind = true;
		vOut.x *= 100000;
		vOut.y *= 100000;
	}
	else
	{
		bBehind = false;
		float invw = 1.0f / w;
		vOut.x *= invw;
		vOut.y *= invw;
	}

	if( !bBehind )
	{
		int sw, sh;
		GetScreenSize( sw, sh );

		float x = (float)sw / 2;
		float y = (float)sh / 2;

		x += 0.5f * vOut.x * sw + 0.5f;
		y -= 0.5f * vOut.y * sh + 0.5f;

		vOut.x = x;
		vOut.y = y;

		return true;
	}

	return false;
}