/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/***********/
/* Dvars.h */
/***********/
#pragma once

#define MAX_DVARS		0x500

#define tof(x)		(*((float*)(&x)))
#define tovec(x)	((float*)(x))
#define toi(x)		(*((int*)(&x)))
#define tol(x)		(*((long*)(&x)))
#define tos(x)		(*((char**)(&x)))
#define tobool(x)	((qboolean)(((PBYTE)(x))[0]))

#define GetR(x)		((float)(HIBYTE(HIWORD(x)))/255.0f)
#define GetG(x)		((float)(LOBYTE(HIWORD(x)))/255.0f)
#define GetB(x)		((float)(HIBYTE(LOWORD(x)))/255.0f)
#define GetA(x)		((float)(LOBYTE(LOWORD(x)))/255.0f)

extern dvar_t *			dvarList;

extern dvar_t *			dv_pbss;
extern dvar_t *			dv_wallhack;
extern dvar_t *			dv_nameesp;
extern dvar_t *			dv_distanceesp;
extern dvar_t *			dv_autoshoot;
extern dvar_t *			dv_autosight;
extern dvar_t *			dv_autobash;
extern dvar_t *			dv_autobash_range;
extern dvar_t *			dv_aimbot;
extern dvar_t *			dv_aimvecs;
extern dvar_t *			dv_prediction;
extern dvar_t *			dv_radar;
extern dvar_t *			dv_radar_range;
extern dvar_t *			dv_radar_rect;
extern dvar_t *			dv_radar_prim;
extern dvar_t *			dv_radar_sec;
extern dvar_t *			dv_radar_enemy;
extern dvar_t *			dv_radar_ally;
extern dvar_t *			dv_targetinfo;
extern dvar_t *			dv_norecoil;
extern dvar_t *			dv_ha_fov_y;
extern dvar_t *			dv_ha_fov_x;
extern dvar_t *			dv_ha_div;
extern dvar_t *			dv_ha_freq;
extern dvar_t *			dv_ha_amp;
extern dvar_t *			dv_aim_fov_y;
extern dvar_t *			dv_aim_fov_x;
extern dvar_t *			dv_aim_key;
extern dvar_t *			dv_ha_key;
extern dvar_t *			dv_aim_priority;

void ProcessDvars( );

dvar_t *	Dvar_FindDvar		( const char * name );
qboolean	Dvar_SetFmt			( const char * name, const char * value_fmt, ... );
qboolean	Dvar_SetStr			( const char * name, const char * value );
int			Dvar_GetKeyCode		( dvar_t * dv );

int	WINAPI	_Dvar_GetInt		( dvar_t * dv );