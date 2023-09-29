#include "main.h"

export_t* pExports = NULL;
export_t gExports;

SCREENINFO g_Screen;

void __cdecl HUD_Redraw( float time,int intermission )
{
	gExports.HUD_Redraw( time,intermission );

	cl_entity_t *pLocal = gEngfuncs.GetLocalPlayer();
	g_Local.iIndex = pLocal->index;

	g_Local.bAlive = (pLocal->curstate.solid ? true : false);

	gUtils.ConExec("hideradar 1;cl_bob 0;cl_bobup 0");

	gUtils.GetScreenInfo(&g_Screen);

	gEngfuncs.pfnDrawSetTextColor(0,1.0f,0);
	gEngfuncs.pfnDrawConsoleString(5,5,"[1.6]Lite By Shad0w");

	gESP.HUD_Redraw();

}
void __cdecl HUD_PlayerMove( playermove_s*ppmove,int server )
{
	gExports.HUD_PlayerMove( ppmove,server );

	gEngfuncs.pEventAPI->EV_LocalPlayerViewheight( g_Local.vEye );
	g_Local.vEye = g_Local.vEye + ppmove->origin;

	g_Local.vEye=ppmove->origin+ppmove->view_ofs;
	g_Local.vEye=g_Local.vEye+ppmove->velocity*ppmove->frametime;
	g_Local.iMoveType=ppmove->movetype;
	g_Local.iFlags = ppmove->flags;
}
void __cdecl CL_CreateMove( float frametime,usercmd_s*pCmd,int active )
{
	gExports.CL_CreateMove( frametime,pCmd,active );

	bool bAttacking = pCmd->buttons&IN_ATTACK && ( ( g_Local.m_flNextPrimaryAttack - pCmd->msec / 1000 ) <= 0.0f );

	if( g_Local.bAlive )
	{
		if(pCmd->buttons & IN_ATTACK)
		{
           if (bAttacking) pCmd->buttons |= IN_ATTACK;
           else pCmd->buttons &= ~IN_ATTACK;
		}

		if( pCmd->buttons&IN_JUMP && !(g_Local.iFlags&FL_ONGROUND) && g_Local.iMoveType!=5)
		    pCmd->buttons&=~IN_JUMP;
	}
}
void __cdecl HUD_PostRunCmd(struct local_state_s *from,struct local_state_s *to,struct usercmd_s *cmd,int runfuncs,double time,unsigned int random_seed)
{
	gExports.HUD_PostRunCmd( from,to,cmd,runfuncs,time,random_seed );

	g_Local.iWeaponId = to->client.m_iId;
	g_Local.flFOV = to->client.fov;
	g_Local.iFlags=to->client.flags;
	g_Local.flSpeed=to->client.velocity.Length();

	if( runfuncs )
	{
		g_Local.m_flNextPrimaryAttack = to->weapondata[to->client.m_iId].m_flNextPrimaryAttack;
	}
}
void HookExportFuncs( void )
{
	memcpy(&gExports,pExports,sizeof( export_t ) );
    #define HEF(x) pExports->##x=##x;

	HEF(HUD_Redraw)
	HEF(HUD_PlayerMove)
	HEF(CL_CreateMove)
	HEF(HUD_PostRunCmd)

	oDynamicSound=(DynamicSound_orig)gUtils.Detour((PBYTE)offs.dwSound,(PBYTE)&DynamicSound,8);
	
}