#pragma once

#include "main.h"
#include "utils.h"

class CLocalPlayer
{
public:
	bool bAlive;

	int iIndex,
		iTeam,
		iWeaponId,
		iFlags,
		iMoveType;

	float flFOV,
		  flSpeed,
		  flFrametime;

	int iClip;
	bool bInReload;
	float m_flNextPrimaryAttack;

	vec3_t vEye,
		   vPunchAngle,
	       vAngles;
	
	void CL_CreateMove(float frametime,usercmd_s *cmd);
	void HUD_PlayerMove(struct playermove_s *ppmove);
	void HUD_PostRunCmd( local_state_t *to );
	void HUD_Redraw( void );
	void V_CalcRefdef( ref_params_t *pparams );
};

extern CLocalPlayer g_Local;

struct player_sound_t
{
	vec3_t vOrigin;
	DWORD dwTime;
	bool bValid;
};
struct VectorXYZ_s{vec3_t vOrigin;bool bVisible;};

class CPlayer
{
public:

	int iIndex,
		iTeam;
	bool bAlive,
		 bHasHitbox;
	color3_s clrTeam,
			 clrVisTeam;
	color4_s clrTeam2;

	float fldmFixTime;
	float Distance;
	player_sound_t currentSound, previousSound;
	std::vector<VectorXYZ_s>vHitbox;
	std::vector<VectorXYZ_s>vStand;
	std::vector<VectorXYZ_s>vDuck;
	std::vector<VectorXYZ_s>vJump;

	//Aimbot Pred
	vec3_t vPredVec,
		   vPredOldVec;
	float flPredTime,
		  flPredOldTime;
	bool bValid;
public:
	void TeamInfo( const char *szTeamName );
};

extern CPlayer g_Player[ 33 ];