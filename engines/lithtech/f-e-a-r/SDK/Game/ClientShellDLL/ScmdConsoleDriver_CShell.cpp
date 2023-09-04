// ----------------------------------------------------------------------- //
//
// MODULE  : ScmdConsoleDriver_CShell.cpp
//
// PURPOSE : GameClient.dll driver for scmdclient
//
// CREATED : 10/24/02
//
// (c) 1999-2002 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#include "stdafx.h"
#include "ScmdConsoleDriver_CShell.h"
#include "HUDMessageQueue.h"

// ----------------------------------------------------------------------- //
//
//	ROUTINE:	ScmdConsoleDriver_CShell::SendToServer
//
//	PURPOSE:	Sends formed message to server.  
//
//	RETURN:		false on error.
//
// ----------------------------------------------------------------------- //

bool ScmdConsoleDriver_CShell::SendToServer( ILTMessage_Read& msg )
{
	g_pLTClient->SendToServer( &msg, MESSAGE_GUARANTEED );
	return true;
}


// ----------------------------------------------------------------------- //
//
//	ROUTINE:	ScmdConsoleDriver_CShell::WriteMessage
//
//	PURPOSE:	Writes message to clients output window.
//
//	RETURN:		false on error.
//
// ----------------------------------------------------------------------- //

bool ScmdConsoleDriver_CShell::WriteMessage( wchar_t const* pszMessage )
{
	// Check inputs.
	if( !pszMessage || !pszMessage[0] )
	{
		ASSERT( !"ScmdConsole_Impl::WriteMessage: Invalid inputs." );
		return false;
	}
	g_pChatMsgs->AddMessage( pszMessage, kMsgScmd );
	
	return true;
}

// ----------------------------------------------------------------------- //
//
//	ROUTINE:	ScmdConsoleDriver_CShell::LoadStringResId
//
//	PURPOSE:	Loads string resource.
//
//	RETURN:		false on error.
//
// ----------------------------------------------------------------------- //

wchar_t const* ScmdConsoleDriver_CShell::LoadStringResId( const char* szResId )
{
	return LoadString( szResId );
}
