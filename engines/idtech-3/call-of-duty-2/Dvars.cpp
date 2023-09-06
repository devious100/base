/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/*************/
/* Dvars.cpp */
/*************/
#include "stdafx.h"

dvar_t * dvarList		= (dvar_t*)0x000C5C9D0;

enum_type dv_keycodes_e[] =
{ 
	"None", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
	"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	"mouse1", "mouse2", "mouse3", 
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"num0", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9", 
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", 
	NULL
};

dvar_t *	dv_pbss;
enum_type	dv_pbss_e[]				= { "Clean", "Dirty", NULL };
dvar_t *	dv_wallhack;
dvar_t *	dv_nameesp;
dvar_t *	dv_distanceesp;
dvar_t *	dv_autoshoot;
dvar_t *	dv_autosight;
dvar_t *	dv_autobash;
dvar_t *	dv_autobash_range;
dvar_t *	dv_radar;
dvar_t *	dv_radar_range;
dvar_t *	dv_radar_rect;
dvar_t *	dv_radar_prim;
dvar_t *	dv_radar_sec;
dvar_t *	dv_radar_enemy;
dvar_t *	dv_radar_ally;
dvar_t *	dv_targetinfo;
dvar_t *	dv_norecoil;
/** Aiming **/
dvar_t *	dv_aimbot;
enum_type	dv_aimbot_e[]			= { "Off", "Instant", "Human", NULL };
dvar_t *	dv_aim_priority;
enum_type	dv_aim_priority_e[]		= { "Head Only", "Body Only", "Head > Body", "Body > Head", NULL };
dvar_t *	dv_aimvecs;
dvar_t *	dv_prediction;
dvar_t *	dv_aim_fov_y;
dvar_t *	dv_aim_fov_x;
dvar_t *	dv_aim_key;
/** Human Aiming **/
dvar_t *	dv_ha_fov_y;
dvar_t *	dv_ha_fov_x;
dvar_t *	dv_ha_div;
dvar_t *	dv_ha_freq;
dvar_t *	dv_ha_amp;
dvar_t *	dv_ha_key;
/******************/

void ProcessDvars( )
{
	dv_pbss				= ri->Dvar_Registere			( "cu_pbss",			dv_pbss_e, 0, 1 );
	dv_wallhack			= ri->Dvar_Registerir			( "cu_wallhack",		0, 0, 1, 1 );
	dv_nameesp			= ri->Dvar_Registerir			( "cu_nameesp",			0, 0, 1, 1 );
	dv_distanceesp		= ri->Dvar_Registerir			( "cu_distanceesp",		0, 0, 1, 1 );
	dv_autoshoot		= ri->Dvar_Registerir			( "cu_autoshoot",		0, 0, 1, 1 );
	dv_autosight		= ri->Dvar_Registerir			( "cu_autosight",		0, 0, 1, 1 );
	dv_autobash			= ri->Dvar_Registerir			( "cu_autobash",		0, 0, 1, 1 );
	dv_autobash_range	= ri->Dvar_Registerf			( "cu_autobash_range",	200.0f, 0.0f, 999999.0f, 1 );	
	dv_radar			= ri->Dvar_Registerir			( "cu_radar",			0, 0, 1, 1 );
	dv_radar_range		= ri->Dvar_Registerf			( "cu_radar_range",		2000.0f, 0.0f, 999999.0f, 1 );
	dv_radar_rect		= ri->Dvar_Register4fr			( "cu_radar_rect",		20.0f, 300.0f, 120.0f, 120.0f, 10.0f, 3000.0f, 1 );
	dv_radar_prim		= ri->Dvar_Register4fr			( "cu_radar_prim",		0.0f, 0.4f, 0.2f, 0.3f, 0.0f, 1.0f, 1 );
	dv_radar_sec		= ri->Dvar_Register4fr			( "cu_radar_sec",		0.0f, 0.9f, 0.5f, 0.5f, 0.0f, 1.0f, 1 );
	dv_radar_enemy		= ri->Dvar_Register4fr			( "cu_radar_enemy",		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1 );
	dv_radar_ally		= ri->Dvar_Register4fr			( "cu_radar_ally",		0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1 );
	dv_targetinfo		= ri->Dvar_Registerir			( "cu_targetinfo",		0, 0, 1, 1 );
	dv_norecoil			= ri->Dvar_Registerir			( "cu_norecoil",		0, 0, 1, 1 );
	/** Aiming **/
	dv_aimbot			= ri->Dvar_Registere			( "cu_aimbot",			dv_aimbot_e, 0, 1 );
	dv_aim_priority		= ri->Dvar_Registere			( "cu_aim_priority",	dv_aim_priority_e, 2, 1 );
	dv_aimvecs			= ri->Dvar_Register3fr			( "cu_aimvecs",			0.0f, 0.0f, 6.0f, -999.0f, 999.0f, 1 );
	dv_prediction		= ri->Dvar_Registerf			( "cu_prediction",		1.0f, 0.0f, 999.0f, 1 );
	dv_aim_fov_y		= ri->Dvar_Registerf			( "cu_aim_fov_y",		360.0f, 0.001f, 360.0f, 1 );
	dv_aim_fov_x		= ri->Dvar_Registerf			( "cu_aim_fov_x",		360.0f, 0.001f, 360.0f, 1 );
	dv_aim_key			= ri->Dvar_Registere			( "cu_aim_key",			dv_keycodes_e, 0, 1 );
	/** Human Aiming **/
	dv_ha_fov_y			= ri->Dvar_Registerf			( "cu_ha_fov_y",		35.0f, 0.001f, 360.0f, 1 );
	dv_ha_fov_x			= ri->Dvar_Registerf			( "cu_ha_fov_x",		65.0f, 0.001f, 360.0f, 1 );
	dv_ha_div			= ri->Dvar_Registerf			( "cu_ha_div",			15.0f, 1.0f, 99999.0f, 1 );
	dv_ha_freq			= ri->Dvar_Registerf			( "cu_ha_freq",			1.0f, 0.001f, 9999.0f, 1 );
	dv_ha_amp			= ri->Dvar_Registerf			( "cu_ha_amp",			5.0f, 0.001f, 9999.0f, 1 );
	dv_ha_key			= ri->Dvar_Registere			( "cu_ha_key",			dv_keycodes_e, 0, 1 );
}

dvar_t * Dvar_FindDvar( const char * name )
{
	for( int i = 0; i < MAX_DVARS; i++ )
	{
		if( dvarList[i].name )
		{
			if( _stricmp( dvarList[i].name, name ) == 0 )
				return &(dvarList[i]);
		}
	}

	return NULL;
}

//e.g.
// Dvar_SetFmt( "cg_thirdperson", "%i", dv_thirdperson );
// Dvar_SetFmt( "somecolour", "%g %g %g %g", r, g, b, a );
qboolean Dvar_SetFmt( const char * name, const char * value_fmt, ... )
{
	if( !name || !value_fmt )
		return qfalse;

	dvar_t * dv = Dvar_FindDvar( name );

	if( dv == NULL )
		return qfalse;

	char	szBuffer[512] = "";
	va_list	ap;

	va_start( ap, value_fmt );
	_vsnprintf_s( szBuffer, 512, 511, value_fmt, ap );
	va_end( ap );

	ri->Dvar_Get( dv, szBuffer );

	return qtrue;
}

qboolean Dvar_SetStr( const char * name, const char * value )
{
	if( !name || !value )
		return qfalse;

	dvar_t * dv = Dvar_FindDvar( name );

	if( dv == NULL )
		return qfalse;
	
	ri->Dvar_Get( dv, value );

	return qtrue;
}

int iKeyCodes[] =
{
	0,
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,
	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, 
};
int Dvar_GetKeyCode( dvar_t * dv )
{
	return iKeyCodes[ toi( dv->value ) ];
}

int	WINAPI _Dvar_GetInt( dvar_t * dv )
{
	return toi( dv->value );
}