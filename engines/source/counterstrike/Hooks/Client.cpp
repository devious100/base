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

#include "Hooks.h"
#include "..\HL\HL.h"
#include "..\HL\Local.h"
#include "..\SourceBase\SBVar.h"
#include "..\SourceBase\Console.h"
#include "..\SourceBase\SBCmd.h"
#include "..\SourceBase\SourceBase.h"
#include "..\SourceBase\CmdArgs.h"

CSBVar aim_recoil( "aim_recoil", "Counter the effects of gun recoil when shooting", CValue(true) );

////////////////////////////////////////////////////////////////////////////////
// MAKE_COMMAND
//! Example of a commanf function, in this case it is used to toggle the console on and off
//! 
//! \param toggleconsole - 
//! \param console on and off" - 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
MAKE_COMMAND( toggleconsole, "Toggle the console on and off" )
{
	gConsole.SetActive( !gConsole.GetActive() );
}

////////////////////////////////////////////////////////////////////////////////
// RunClient
//! DESCRIPTION
//! 
//! \param pCmd - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void RunClient( CUserCmd* pCmd )
{
	// *very* simple anti-recoil
	if( aim_recoil.GetValue().m_bool )
		pCmd->viewangles -= CLocalBaseEntity::Get()->GetPunchAngle() * 2.0f;
}

////////////////////////////////////////////////////////////////////////////////
// IN_KeyEvent
//! Hooke IN_KeyEvent function. Called by the engine when a key is pressed
//! 
//! \param eventcode - 
//! \param keynum - 
//! \param *pszCurrentBinding - 
//! \return __stdcall - 
////////////////////////////////////////////////////////////////////////////////
int	__stdcall IN_KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	typedef int  ( __thiscall *tIN_KeyEvent)( void*, int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding );	

	if( eventcode )
	{
		if( keynum == KEY_INSERT )
		{
			CSourceBase::Get()->ProcessCommand( "toggleconsole" );
			return 0;
		}

		if( gConsole.GetActive() )
		{
			gConsole.KeyEvent( keynum );
			return 0;
		}
	}

	return g_pClient.GetMethod< tIN_KeyEvent >( 20 )( g_pClient.GetInstance(), eventcode, keynum, pszCurrentBinding );
}

////////////////////////////////////////////////////////////////////////////////
// CreateMove
//! Hooked CreateMove function
//! 
//! \param flInputSampleTime - 
//! \param *cmd - pointer to the current cmd to modify
//! \return bool __stdcall - 
////////////////////////////////////////////////////////////////////////////////
bool __stdcall CreateMove( float flInputSampleTime, CUserCmd *cmd )
{
	typedef bool ( __thiscall *tCreateMove)( void*, float flInputSampleTime, CUserCmd *cmd );
	bool bResult = HL::GetClientMode()->GetMethod< tCreateMove >( ID_CREATEMOVE )( HL::GetClientMode()->GetInstance(), flInputSampleTime, cmd );

	// here you can manipulate CUserCmd without having to verify CRC
	RunClient( cmd );

	// you must return false if you change the view angles because the engine will call SetViewAngles if you don't
	return false;
}