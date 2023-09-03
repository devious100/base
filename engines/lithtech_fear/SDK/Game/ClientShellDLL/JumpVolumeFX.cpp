// ----------------------------------------------------------------------- //
//
// MODULE  : JumpVolumeFX.h
//
// PURPOSE : JumpVolume special fx class - Definition
//
// CREATED : 1/24/02
//
// (c) 2002 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

//
// Includes...
//

	#include "stdafx.h"
	#include "JumpVolumeFX.h"



// ----------------------------------------------------------------------- //
//
//  ROUTINE:	CJumpVolumeFX::CJumpVolumeFX
//
//  PURPOSE:	Constructor...
//
// ----------------------------------------------------------------------- //

CJumpVolumeFX::CJumpVolumeFX()
:	CSpecialFX		(),
	m_vVelocity		( 0.0f, 0.0f, 0.0f )
{
}


// ----------------------------------------------------------------------- //
//
//  ROUTINE:	CJumpVolumeFX::~CJumpVolumeFX
//
//  PURPOSE:	Destructor...
//
// ----------------------------------------------------------------------- //

CJumpVolumeFX::~CJumpVolumeFX()
{
}

// ----------------------------------------------------------------------- //
//
//  ROUTINE:	CJumpVolumeFX::Init
//
//  PURPOSE:	Init the JumpVolume....
//
// ----------------------------------------------------------------------- //

bool CJumpVolumeFX::Init( HLOCALOBJ hServObj, ILTMessage_Read *pMsg )
{
	if( !CSpecialFX::Init( hServObj, pMsg )) return false;
	if( !pMsg ) return false;

	m_vVelocity = pMsg->ReadLTVector();

	g_pCommonLT->SetObjectFlags( m_hServerObject, OFT_Flags, FLAG_TOUCH_NOTIFY, FLAG_TOUCH_NOTIFY );

	return true;
}


// ----------------------------------------------------------------------- //
//
//  ROUTINE:	CJumpVolumeFX::OnServerMessage
//
//  PURPOSE:	Handle a message from the server...
//
// ----------------------------------------------------------------------- //

bool CJumpVolumeFX::OnServerMessage( ILTMessage_Read *pMsg )
{
	if( !pMsg ) return false;

	m_vVelocity = pMsg->ReadLTVector();

	return true;
}