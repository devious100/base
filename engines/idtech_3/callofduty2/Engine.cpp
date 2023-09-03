/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/**************/
/* Engine.cpp */
/**************/

#include "stdafx.h"

refimport_t *		ri					= (refimport_t *)	NULL;
refexport_t *		re					= (refexport_t *)	NULL;
refimport_t *		o_ri				= (refimport_t *)	NULL;
refexport_t *		o_re				= (refexport_t *)	0x0068A1E8;

cg_t *				cg					= (cg_t *)			0x014EE080;
centity_t *			cg_entities			= (centity_t *)		0x015E2A80;
clientInfo_t *		clientinfo			= (clientInfo_t *)	0x015CF994;
float *				viewangles			= (float *)			0x0098FDEC;
short *				delta_angles		= (short *)			0x0096B6D8;
int *				ping				= (int *)			0x0096B67C;
int *				keycatchers			= (int *)			0x0096B654;

const t_va						va						= (t_va)					0x0044A990;
const t_Com_Milliseconds		Com_Milliseconds		= (t_Com_Milliseconds)		0x00467F30;
const t_Con_Close				Con_Close				= (t_Con_Close)				0x004073B0;
const t_DrawStringExt			DrawStringExt			= (t_DrawStringExt)			0x005322C0;
const t_CG_Trace				CG_Trace				= (t_CG_Trace)				0x004DE690;
const t_CM_BoxTrace				CM_BoxTrace				= (t_CM_BoxTrace)			0x0041D120;
const t_CG_ClipMoveToEntities	CG_ClipMoveToEntities	= (t_CG_ClipMoveToEntities)	0x004DE2A0;
const t_CG_PointContents		CG_PointContents		= (t_CG_PointContents)		0x004DE730;

t_CG_FireRecoil					o_CG_FireRecoil			= (t_CG_FireRecoil)			NULL;
t_CG_Obituary					o_CG_Obituary			= (t_CG_Obituary)			NULL;
t_CG_Player						o_CG_Player				= (t_CG_Player)				NULL;
//t_CG_ResetPlayerEntity			o_CG_ResetPlayerEntity	= (t_CG_ResetPlayerEntity)	NULL;

const xcommand_t				StartAttack				= (xcommand_t)				0x00407A90;
const xcommand_t				StopAttack				= (xcommand_t)				0x00407AB0;
const xcommand_t				ToggleCrouch			= (xcommand_t)				0x00408040;
const xcommand_t				ToggleProne				= (xcommand_t)				0x00408070;

float	fZoomFactor		= 1.0f;

centity_t * drawingPlayer = NULL;
void RE_EndFrame( )
{
	if( cg->snap )
	{
		pBot->DrawESP( ( toi( dv_nameesp->value )	  == 1 ), 
					   ( toi( dv_distanceesp->value ) == 1 ) );

		if( toi( dv_radar->value ) == 1 )
		{
			pBot->DrawRadar( 
				tovec( dv_radar_rect->value )[0],
				tovec( dv_radar_rect->value )[1], 
				tovec( dv_radar_rect->value )[2],
				tovec( dv_radar_rect->value )[3],
				tof( dv_radar_range->value ), 
				tovec( dv_radar_prim->value ), 
				tovec( dv_radar_sec->value ), 
				tovec( dv_radar_ally->value ),
				tovec( dv_radar_enemy->value )
			);
		}

		if( clientinfo[ cg->clientNum ].team < TEAM_SPECTATOR && cg->clientNum == cg->snap->ps.clientNum )
		{
			if( toi( dv_aimbot->value ) == 1 )
			{
				int iAimKeyCode = Dvar_GetKeyCode( dv_aim_key );
				if( iAimKeyCode == 0 || ( iAimKeyCode && ( GetAsyncKeyState( iAimKeyCode ) & 0x8000 ) ) )
				{
					if( pBot->Aim( ) )
					{
						if( toi( dv_autoshoot->value ) == 1 )
							pBot->Autoshoot( clientinfo[ cg->clientNum ].curWeapon, true );
						if( toi( dv_autosight->value ) == 1 )
							pBot->Autosight( true );
					}
					else
					{
						if( toi( dv_autoshoot->value ) == 1 )
							pBot->Autoshoot( clientinfo[ cg->clientNum ].curWeapon, false );
						if( toi( dv_autosight->value ) == 1 )
							pBot->Autosight( false );
					}
				}
			}
			else if( toi( dv_aimbot->value ) == 2 )
			{
				int iHAKeyCode = Dvar_GetKeyCode( dv_ha_key );
				if( iHAKeyCode == 0 || ( iHAKeyCode && ( GetAsyncKeyState( iHAKeyCode ) & 0x8000 ) ) )
				{
					int iAimed = pBot->HumanAim( tof( dv_ha_fov_x->value ),
												 tof( dv_ha_fov_y->value ),
												 tof( dv_ha_div->value ),
												 tof( dv_ha_freq->value ),
												 tof( dv_ha_amp->value ) );

					if( iAimed == 2 )
					{
						if( toi( dv_autoshoot->value ) == 1 )
							pBot->Autoshoot( clientinfo[ cg->clientNum ].curWeapon, true );
					}
					else if( iAimed == 1 )
					{
						if( toi( dv_autosight->value ) == 1 )
							pBot->Autosight( true );
					}
					else if( iAimed == 0 )
					{
						if( toi( dv_autoshoot->value ) == 1 )
							pBot->Autoshoot( clientinfo[ cg->clientNum ].curWeapon, false );
						if( toi( dv_autosight->value ) == 1 )
							pBot->Autosight( false );
					}
				}
			}
			else
			{
				clientInfo_t * ci = AimingAtClient( );
					
				if( ci && ci->team < TEAM_SPECTATOR && ( ci->team == TEAM_FREE || ci->team != clientinfo[ cg->clientNum ].team ) )
				{
					pBot->DrawTargetInfo( ci );
					
					if( toi( dv_autoshoot->value ) == 1 )
						pBot->Autoshoot( clientinfo[ cg->clientNum ].curWeapon, true );
					if( toi( dv_autosight->value ) == 1 )
						pBot->Autosight( true );
				}
				else
				{
					if( toi( dv_autoshoot->value ) == 1 )
						pBot->Autoshoot( 0, false );
					if( toi( dv_autosight->value ) == 1 )
						pBot->Autosight( false );
				}
			}
		}

		pBot->ClearTargets( );
		//pBot->DrawStats( );
	}

	re->EndFrame( );
}

void RE_RenderScene( refdef_t * fd )
{
	centity_t *		cent;
	clientInfo_t *	ci;
	UINT			i;

	ScreenW = (float)fd->width;
	ScreenH = (float)fd->height;

	fd->fov_x *= fZoomFactor;
	fd->fov_y *= fZoomFactor;

	for( i = 0; i < MAX_CLIENTS; i++ )
	{
		cent = &cg_entities[ i ];
		ci = &clientinfo[ cent->currentState.clientNum ];

		if(	cent->currentValid == qtrue &&
			cent->currentState.eType == ET_PLAYER && 
			!( cent->currentState.eFlags & 1 ) &&
			//!pBot->InvalidPlayer( cent->currentState.number ) &&
			ci->infoValid == qtrue &&
			ci->team < TEAM_SPECTATOR && 
			ci->clientNum != cg->snap->ps.clientNum  )
		{
			pBot->AddTarget( cent, tof( dv_prediction->value ), toi( dv_aim_priority->value ) );
		}
	}
	
	re->RenderScene( fd );
}

int RE_AddRefEntityToScene( refEntity_t * ent, shaderInfo_t * si, centity_t * cent )
{
	if( drawingPlayer )
	{
		clientInfo_t * ci = &clientinfo[ drawingPlayer->currentState.clientNum ];
		if( ci->infoValid == qtrue && drawingPlayer->currentState.clientNum != cg->snap->ps.clientNum  )
		{
			pBot->AddTarget( drawingPlayer, tof( dv_prediction->value ), toi( dv_aim_priority->value ) );

			if( toi( dv_wallhack->value ) == 1 )
				ent->renderfx |= RF_DEPTHHACK | RF_NOSHADOW;
		}
	}

	int iRet = re->AddRefEntityToScene( ent, si, cent );

	return iRet;
}

void RE_BeginRegistration( gfxconfig_t * gfxConfig )
{
	re->BeginRegistration( gfxConfig );

	ProcessDvars( );
	ProcessCommands( );

	/* Register Shaders */
	sWhite			= re->RegisterShader( "white",				3, 7 );
	sBlack			= re->RegisterShader( "black",				3, 7 );
	sArrow			= re->RegisterShader( "hud_grenadepointer",	3, 7 );
	sFlareA			= re->RegisterShader( "gfx_flare_m_b",		3, 7 );
	sFlareB			= re->RegisterShader( "gfx_flare_r_a",		3, 7 );
	sGrenadeIcon	= re->RegisterShader( "hud_grenadeicon",	3, 7 );

	/* Register Fonts */
	consoleFont		= re->RegisterFont( "fonts/consoleFont",	3 );
	normalFont		= re->RegisterFont( "fonts/normalFont",		3 );
	smallFont		= re->RegisterFont( "fonts/smallFont",		3 );
	bigFont			= re->RegisterFont( "fonts/bigFont",		3 );
	extraBigFont	= re->RegisterFont( "fonts/extraBigFont",	3 );
	boldFont		= re->RegisterFont( "fonts/boldFont",		3 );

	pBot			= new CBot( toi( dv_aim_priority->value ) );
}

void RE_Shutdown( )
{
	delete pBot;

	re->Shutdown( );
}

__declspec( naked ) void h_CG_FireRecoil( )
{
	__asm
	{
		push dv_norecoil
		call _Dvar_GetInt //stdcall

		test eax, eax
		jz fire_recoil

		retn

fire_recoil:
		jmp dword ptr [o_CG_FireRecoil]
	}
}

void h_CG_Obituary( )
{
	entityState_t *	ent			= NULL;
	__asm mov ent, eax

	int				iTarget		= ent->otherEntityNum;
	int				iAttacker	= ent->otherEntityNum2;
	int				iMod		= ent->eventParm;
	clientInfo_t *	ciTarget	= NULL;
	clientInfo_t *	ciAttacker	= NULL;

	if( iTarget >= 0 && iTarget < MAX_CLIENTS )
	{
		ciTarget = &clientinfo[ iTarget ];

		// entity is dead
		cg_entities[ iTarget ].currentState.eFlags &= 1;
		//pBot->SetInvalidPlayer( iTarget );
		
		if( iAttacker >= 0 && iAttacker < MAX_CLIENTS )
			ciAttacker = &clientinfo[ iAttacker ];
		else
			iAttacker = ENTITYNUM_WORLD;

		if( ciAttacker && ciAttacker->clientNum == cg->clientNum )
		{
			if( ciTarget->clientNum == cg->clientNum )
				pBot->Stats.Suicides++;
			else
			{
				if( ( iMod & 0xFFFFFF7F ) == 7 )
				{
					pBot->Stats.TotalKills	++;
					pBot->Stats.Bashes		++;
				}
				else if( ( iMod & 0xFFFFFF7F ) == 8 )
				{
					pBot->Stats.TotalKills	++;
					pBot->Stats.Headshots	++;
				}
				else
					pBot->Stats.TotalKills	++;
			}
		}
		else if( ciTarget->clientNum == cg->clientNum )
			pBot->Stats.Deaths++;
	}

	__asm mov eax, ent
	o_CG_Obituary( );
}

void h_CG_Player( )
{
	__asm mov drawingPlayer, eax

	o_CG_Player( );

	__asm xor eax, eax
	__asm mov drawingPlayer, eax
}