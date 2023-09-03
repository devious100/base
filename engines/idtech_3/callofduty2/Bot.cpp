/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

#include "StdAfx.h"

/* Weapons:

0	None
1
2
3
4
5
6	Colt .45			TT30
7	M1A1 Carbine
8	M1 Garand			SVT40
9	Thompson
10	BAR					PPSh
11	Springfield			Scoped Mosin Nagant
12	Grease Gun			M1897 Trench Gun
13
14
15
16	Luger
17	Kar98k
18	Gewehr 43
19	MP40
20	MP44
21	Scoped Kar98k
*/

typedef enum
{
	WEAPON_NONE,					// 0
	WEAPON_UNKNOWN1,				// 1
	WEAPON_UNKNOWN2,				// 2
	WEAPON_UNKNOWN3,				// 3
	WEAPON_UNKNOWN4,				// 4
	WEAPON_UNKNOWN5,				// 5
	WEAPON_ALLIED_PISTOL,			// 6
	WEAPON_ALLIED_RIFLE,			// 7
	WEAPON_ALLIED_FL,				// 8
	WEAPON_ALLIED_SMG,				// 9
	WEAPON_ALLIED_MG,				// 10
	WEAPON_ALLIED_SNIPER,			// 11
	WEAPON_
} weapon_t;

//#define NUM_TAGS 18
#define NUM_TAGS 16
const char * TagNames[NUM_TAGS] =
{
	"head",
	"helmet",
	"neck",
	//"torso_upper",
	//"torso_lower",
	"back_up",
	"back_mid",
	"back_low",
	"pelvis",
	"tag_butt",
	"right_arm_upper",
	"left_arm_upper",
	"right_arm_lower",
	"left_arm_lower",
	"right_leg_upper",
	"left_leg_upper",
	"right_leg_lower",
	"left_leg_lower",
};

const tag_t TagStructs[NUM_TAGS] =
{
	{ 0, HEAD, 0,  4  },			// head
	{ 0, HEAD, 1,  5  },			// helmet
	{ 0, HEAD, 2,  3  },			// neck
	//{ 0, BODY, 4,  1  },			// torso_upper
	//{ 0, BODY, 3,  0  },			// torso_lower
	{ 0, BODY, 3,  0  },			// back_up
	{ 0, BODY, 4,  1  },			// back_mid
	{ 0, BODY, 5,  2  },			// back_low
	{ 0, BODY, 6,  6  },			// pelvis
	{ 0, BODY, 7,  7  },			// tag_butt
	{ 0, BODY, 8,  8  },			// right_arm_upper
	{ 0, BODY, 9,  9  },			// left_arm_upper
	{ 0, BODY, 10, 10 },			// right_arm_lower
	{ 0, BODY, 11, 11 },			// left_arm_lower
	{ 0, BODY, 12, 12 },			// right_leg_upper
	{ 0, BODY, 13, 13 },			// left_leg_upper
	{ 0, BODY, 14, 14 },			// right_leg_lower
	{ 0, BODY, 15, 15 },			// left_leg_lower
};


CBot::CBot( int iTagPriority )
{
	TagHead = GetTagNumber( "head" );

	tag_t * NewTag;
	for( UINT i = 0; i < NUM_TAGS; i++ )
	{
		NewTag = new tag_t( );

		NewTag->iTagNum			= GetTagNumber( TagNames[i] );
		NewTag->iWhere			= TagStructs[i].iWhere;
		NewTag->iHeadPriority 	= TagStructs[i].iHeadPriority;
		NewTag->iBodyPriority	= TagStructs[i].iBodyPriority;

		TagList.push_back( *NewTag );

		delete NewTag;
	}

	SetTagPriority( iTagPriority );
	
	Stats.Deaths		= 0;
	Stats.Headshots		= 0;
	Stats.TotalKills	= 0;
	Stats.Suicides		= 0;
	Stats.Deaths		= 0;

	//memset( InvalidPlayers, 0, sizeof( InvalidPlayers ) );
}

CBot::~CBot( )
{
	if( !TagList.empty( ) )
		TagList.clear( );

	if( !Targets.empty( ) )
		Targets.clear( );
}

/*bool CBot::InvalidPlayer( int entNum )
{
	if( entNum < MAX_CLIENTS )
		return ( InvalidPlayers[ entNum ] == 1 );
	else
		return true;
}

void CBot::SetInvalidPlayer( int entNum )
{
	if( entNum < MAX_CLIENTS )
		InvalidPlayers[ entNum ] = 1;
}

void CBot::SetValidPlayer( int entNum )
{
	if( entNum < MAX_CLIENTS )
		InvalidPlayers[ entNum ] = 0;
}*/

bool CBot::TagSortHead( tag_t lhs, tag_t rhs )
{
	return ( lhs.iHeadPriority < rhs.iHeadPriority );
}

bool CBot::TagSortBody( tag_t lhs, tag_t rhs )
{
	return ( lhs.iBodyPriority < rhs.iBodyPriority );
}

void CBot::SetTagPriority( int priority )
{
	if( priority == HEAD || priority == HEAD_PRIORITY )
		std::sort( TagList.begin( ), TagList.end( ), TagSortHead );
	else if( priority == BODY || priority == BODY_PRIORITY )
		std::sort( TagList.begin( ), TagList.end( ), TagSortBody );

	TagPriority = priority;
}

bool CBot::AddTarget( centity_t * cent, float prediction, int priority )
{
	BotTarget_t		NewTarget;
	UINT			i;
	orientation_t	or;

	if( !cent )
		return false;

	if( !Targets.empty( ) )
	{
		for( i = 0; i < Targets.size( ); i++ )
		{
			if( cent == Targets[i].cent )
				return false;
		}
	}

	if( TagPriority != priority )
		SetTagPriority( priority );

	NewTarget.cent		= cent;
	NewTarget.ci		= &clientinfo[ cent->currentState.clientNum ];
	NewTarget.enemy		= ( NewTarget.ci->team == TEAM_FREE || NewTarget.ci->team != clientinfo[ cg->clientNum ].team );
	NewTarget.distance	= GetDistance( cent->lerpOrigin, cg->refdef.vieworg );

	GetTagOrientation( cent, TagHead, &or );
	CalcAimVectors( &or, tovec( dv_aimvecs->value ), NewTarget.esppoint );

	if( prediction )
		VectorMA( NewTarget.esppoint, prediction, cent->currentState.pos.trDelta, NewTarget.aimpoint );
	else
		VectorCopy( NewTarget.esppoint, NewTarget.aimpoint );

	NewTarget.visible = IsEntityVisible( NewTarget.aimpoint, cent->currentState.number );//IsPointVisible( NewTarget.aimpoint );

	if( !NewTarget.visible || ( priority != HEAD && priority != HEAD_PRIORITY ) )
	{
		for( i = 0; i < TagList.size( ); i++ )
		{
			if( !( priority == HEAD && TagList[i].iWhere != HEAD ) && 
				!( priority == BODY && TagList[i].iWhere != BODY ) )
			{
				GetTagOrientation( cent, TagList[i].iTagNum, &or );

				if( prediction )
					VectorMA( or.origin, prediction, cent->currentState.pos.trDelta, NewTarget.aimpoint );
				else
					VectorCopy( or.origin, NewTarget.aimpoint );

				if( /*IsPointVisible( NewTarget.aimpoint )*/IsEntityVisible( NewTarget.aimpoint, cent->currentState.number ) )
				{
					NewTarget.visible = true;
					break;
				}
			}
		}
	}
	/*if( prediction )
		VectorMA( NewTarget.esppoint, prediction, cent->currentState.pos.trDelta, NewTarget.aimpoint );
	else
		VectorCopy( NewTarget.esppoint, NewTarget.aimpoint );

	NewTarget.visible	= ( IsPointVisible( NewTarget.aimpoint ) == qtrue );

	if( !NewTarget.visible )
	{
		for( i = 0; i < TagList.size( ); i++ )
		{
			GetTagOrientation( cent, TagList[i], &or );

			if( prediction )
				VectorMA( or.origin, prediction, cent->currentState.pos.trDelta, NewTarget.aimpoint );
			else
				VectorCopy( or.origin, NewTarget.aimpoint );

			if( IsPointVisible( NewTarget.aimpoint ) == qtrue )
			{
				NewTarget.visible = qtrue;
				break;
			}
		}
	}*/

	NewTarget.GotAngles = false;
	NewTarget.aimangles[0] = 0.0f;
	NewTarget.aimangles[1] = 0.0f;
	NewTarget.aimangles[2] = 0.0f;

	Targets.push_back( NewTarget );

	return true;
}

void CBot::ClearTargets( )
{
	if( !Targets.empty( ) )
		Targets.clear( );
}

bool CBot::GetTagOrigin( centity_t * cent, short tag, vec3_t origin )
{
	int handle = GetHandle( cent );
	
	if( handle )
	{
		orientation_t or;
		if( LerpTag( cent, &or, tag, handle ) )
		{
			VectorCopy( or.origin, origin );
			return true;
		}
		//if( LerpTag( cent, origin, tag, handle ) )
		//	return true;
	}

	return false;
}

bool CBot::GetTagOrientation( centity_t * cent, short tag, orientation_t * or )
{
	int handle = GetHandle( cent );

	if( handle )
	{
		if( LerpTag( cent, or, tag, handle ) )
			return true;
	}

	return false;
}

void CBot::DrawTargetInfo( clientInfo_t * ci )
{
	if( ci && toi( dv_targetinfo->value ) == 1 )
		DrawCenterString( ScreenW / 2.0f, 100.0f, 1.0f, colWhite, bigFont, 3, "^9Target ^1[^7%s^1]", ci->name );
}

bool CBot::Aim( )
{
	if( !Targets.empty( ) )
	{
		SortTargets( false );

		for( UINT i = 0; i < Targets.size( ); i++ )
		{
			if( Targets[i].enemy && Targets[i].visible )
			{
				/*vec3_t angles;
				GetAngleToOrigin( Targets[i].aimpoint, angles );
				viewangles[PITCH]	+= angles[PITCH];
				viewangles[YAW]		+= angles[YAW];*/
				GetAngleToOrigin( Targets[i].aimpoint, Targets[i].aimangles );
				viewangles[PITCH]	+= Targets[i].aimangles[PITCH];
				viewangles[YAW]		+= Targets[i].aimangles[YAW];

				DrawTargetInfo( Targets[i].ci );

				return true;
			}
		}
	}

	return false;
}

void CBot::UpdateTargetAngles( )
{
	for( UINT i = 0; i < Targets.size( ); i++ )
	{
		if( !Targets[i].GotAngles && Targets[i].visible && Targets[i].enemy )
		{
			GetAngleToOrigin( Targets[i].aimpoint, Targets[i].aimangles );
			Targets[i].GotAngles = true;
		}
	}
}

int CBot::HumanAim( float fov_x, float fov_y, float fDivisions, float fFrequency, float fAmplitude )
{
	static bool		bAiming			= false;
	static int		iAimed			= 0;
	static float	fTotalPitch		= 0.0f;
	static float	fTotalYaw		= 0.0f;
	static float	fResultant		= 0.0f;
	static float	fRoll			= 0.0f;
	static float	fCurrent		= 0.0f;
	static float	fCurrentPitch	= 0.0f;
	static float	fCurrentYaw		= 0.0f;
	static float	fLastPitch		= 0.0f;
	static float	fLastYaw		= 0.0f;

	if( bAiming )
	{
		fCurrent		+= ( fResultant   / fDivisions );
		fCurrentPitch	+= ( fTotalPitch  / fDivisions );
		fCurrentYaw		+= ( fTotalYaw    / fDivisions );
		
		//fAmplitude *= ( fResultant / 100.0f );
		fAmplitude = ( ( fAmplitude - 0.05f ) * ( fResultant / 100.0f ) ) + 0.05f;
		float fSin		= sin( ( fCurrent / fResultant ) * ( fFrequency * M_PI ) ) * fAmplitude;
		float fNewPitch	= fCurrentPitch + ( fSin * cos( fRoll ) );
		float fNewYaw	= fCurrentYaw   + ( fSin * sin( fRoll ) );

		viewangles[PITCH] += ( fNewPitch - fLastPitch );
		viewangles[YAW]   += ( fNewYaw   - fLastYaw   );

		fLastPitch = fNewPitch;
		fLastYaw   = fNewYaw;

		iAimed++;

		if( iAimed >= fDivisions )
		{
			bAiming = false;
			return 2; // Autoshoot
		}

		return 1; // Autosight
	}
	else if( !Targets.empty( ) )
	{
		UpdateTargetAngles( ); //cheap
		SortTargets( true );

		for( UINT i = 0; i < Targets.size( ); i++ )
		{
			if( Targets[i].GotAngles && Targets[i].enemy && Targets[i].visible )
			{
				if( Targets[i].aimangles[0] < -( fov_y / 2.0f ) ||
					Targets[i].aimangles[0] >  ( fov_y / 2.0f ) ||
					Targets[i].aimangles[1] < -( fov_x / 2.0f ) ||
					Targets[i].aimangles[1] >  ( fov_x / 2.0f ) )
					continue;

				fTotalPitch		= Targets[i].aimangles[PITCH];
				fTotalYaw		= Targets[i].aimangles[YAW];
				fResultant		= sqrt( ( fTotalPitch * fTotalPitch ) + ( fTotalYaw * fTotalYaw ) );
				fRoll			= atan( fTotalPitch / fTotalYaw );
				fCurrent		= 0.0f;
				fCurrentPitch	= 0.0f;
				fCurrentYaw		= 0.0f;
				fLastPitch		= 0.0f;
				fLastYaw		= 0.0f;
				bAiming			= true;
				iAimed			= 0;

				//Start aiming this frame if you don't have a fear of recursive functions
				//return HumanAim( fov_x, fov_y, fDivisions, fFrequency, fAmplitude );

				return 1; // Autosight
			}
		}
	}

	return 0;
}

void CBot::Autobash( )
{
	
}

void CBot::Autoshoot( int weapon, bool toggle )
{
	if( toggle )
	{
		StartAttack( );
		StopAttack( );
	}
		//ri->Cmd_ExecuteText( EXEC_INSERT, "+attack; wait 1; -attack" );
}

void CBot::Autosight( bool toggle )
{
	static bool bSighted = false;

	if( toggle )
	{
		ri->Cmd_ExecuteText( EXEC_INSERT, "+speed" );
		bSighted = true;
	}
	else if( bSighted )
	{
		ri->Cmd_ExecuteText( EXEC_INSERT, "-speed" );
		bSighted = false;
	}
}

void CBot::DrawRadar( float x, float y, float w, float h, float range, float * BackColour, float * LineColour, float * FriendlyColour, float * EnemyColour )
{
	re->DrawStretchPic( x, y, w, h, 0.0f, 0.0f, 0.0f, 0.0f, BackColour, sWhite );
	DrawRect( x, y, w, h, 1.0f, LineColour );

	for( UINT dH = 0; dH < 2; dH++ )
	{
		for( UINT dW = 0; dW < 2; dW++ )
		{
			DrawRect( x + ( dW * ( w / 2.0f ) ),
					  y + ( dH * ( h / 2.0f ) ),
					  ( w / 2.0f ), ( h / 2.0f ),
					  1.0f, LineColour );
		}
	}

	if( !Targets.empty( ) )
	{
		for( UINT i = 0; i < Targets.size( ); i++ )
		{
			float *	TargetOrigin	= Targets[i].cent->lerpOrigin;

			vec4_t	res;
			VectorSubtract( TargetOrigin, cg->refdef.vieworg, res );

			float	dx				= DotProduct( res, cg->refdef.viewaxis[1] );
			float	dy				= DotProduct( res, cg->refdef.viewaxis[0] );

			float	Xpos			= ( x + ( w / 2.0f ) ) - ( dx * ( ( w / 2 ) / range ) );
			float	Ypos			= ( y + ( h / 2.0f ) ) - ( dy * ( ( h / 2 ) / range ) );

			if( Xpos >= ( x + w - 1.0f ) || Xpos <= ( x + 1.0f ) || Ypos >= ( y + h - 1.0f ) || Ypos <= ( y + 1.0f ) )
				continue;

			if( Targets[i].enemy )
				re->DrawRotatedPic( Xpos - 1.0f, Ypos - 1.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 45.0f, EnemyColour, sWhite );
			else
				re->DrawRotatedPic( Xpos - 1.0f, Ypos - 1.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 45.0f, FriendlyColour, sWhite );
		}
	}
}

void CBot::DrawESP( bool bName, bool bDistance )
{
	if( !bName && !bDistance )
		return;

	if( !Targets.empty( ) )
	{
		for( UINT i = 0; i < Targets.size( ); i++ )
		{
			float x, y;

			Targets[i].esppoint[2] += 12.0f;
			if( WorldToScreen( Targets[i].esppoint, &x, &y ) )
			{
				float * TextColour = ( Targets[i].visible ) ? colWhite : colWhite_;

				if( bName )
				{
					if( Targets[i].enemy )
						DrawCenterString( x, y, 0.8f, TextColour, normalFont, 3, "^1[ ^7%s ^1]", Targets[i].ci->name );
					else
						DrawCenterString( x, y, 0.8f, TextColour, normalFont, 3, "^2[ ^7%s ^2]", Targets[i].ci->name );
				}
				if( bDistance )
				{
					float height = (float)re->StringHeight( normalFont ) * 0.9f;
					DrawCenterString( x, y + height, 0.65f, TextColour, normalFont, 3, "%.1f", Targets[i].distance / 40.0f );
				}
			}
		}
	}
}

bool CBot::SortPredicateA( BotTarget_t lhs, BotTarget_t rhs )
{
	if( !lhs.visible || !lhs.enemy )
		return false;

	// sort by distance
	return ( lhs.distance < rhs.distance );
}

bool CBot::SortPredicateB( BotTarget_t lhs, BotTarget_t rhs )
{
	if( !lhs.visible || !lhs.enemy || !lhs.GotAngles || !rhs.GotAngles )
		return false;

	// sort by angle
	float fLHS = ( lhs.aimangles[0] * lhs.aimangles[0] ) + ( lhs.aimangles[1] * lhs.aimangles[1] );
	float fRHS = ( rhs.aimangles[0] * rhs.aimangles[0] ) + ( rhs.aimangles[1] * rhs.aimangles[1] );

	return ( fLHS < fRHS );
}

void CBot::SortTargets( bool bHuman )
{
	if( bHuman )
		std::sort( Targets.begin( ), Targets.end( ), SortPredicateB );
	else
		std::sort( Targets.begin( ), Targets.end( ), SortPredicateA );
}

DWORD	o_R_GetHandle	= 0x004356B0;
DWORD	o_R_LerpTag		= 0x004CCD40;
int CBot::LerpTag( centity_t * cent, orientation_t * _or, short tag, int handle )
{
	__asm
	{
		push	ecx

		mov		ecx, handle
		movzx	eax, tag
		push	_or
		push	cent
		call	dword ptr [o_R_LerpTag]
		add		esp, 8

		pop		ecx
	}
}
/*DWORD	o_R_LerpTag		= 0x004CCE70;
int CBot::LerpTag( centity_t * cent, vec3_t origin, short tag, int handle )
{
	__asm
	{
		push ecx

		mov		ecx, handle
		movzx	eax, tag
		push	origin
		push	cent
		call	dword ptr [o_R_LerpTag]
		add		esp, 8

		pop ecx
	}
}*/

int CBot::GetHandle( centity_t * cent )
{
	__asm
	{
		push ebx
		push ecx

		mov ebx, cent
		mov eax, [ebx+220h]
		mov ecx, [ebx+0F0h]
		call dword ptr [o_R_GetHandle]

		pop ecx
		pop ebx
	}
}

short CBot::GetTagNumber( const char * tag_name )
{
	return ( ( short (*)( const char *, int, size_t, int ) )0x00477500 )( tag_name, 1, strlen( tag_name ) + 1, 6 );
}