#include "main.h"

CESP gESP;

#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
float fCurrentFOV = 90.0f;

static inline float GetDistanceFrom(const float* pos)
{	
	register double a = pos[0] - g_Local.vEye[0];
	register double b = pos[1] - g_Local.vEye[1];
	register double c = pos[2] - g_Local.vEye[2];
	return (float)sqrt(a*a + b*b + c*c);
}
void CESP::DrawPlayerEsp(int iID,vec3_t vOrigin,bool bFarESP)
{
	vec3_t vVecScreen;

	if(bFarESP)
	{
		if(g_Player[iID].fldmFixTime+2.5f<=gEngfuncs.GetClientTime())
			g_Player[iID].bAlive=true;
		if(!g_Player[iID].bAlive)
			return;
	}
	if(!bFarESP && g_Player[iID].bAlive)
		SoundUpdate(g_Player[iID].currentSound,g_Player[iID].previousSound,vOrigin)

		g_Player[iID].Distance  = GetDistanceFrom (vOrigin); if(g_Player[iID].Distance<1) g_Player[iID].Distance=1; // avoid division by zero
	
	if(gUtils.CalcScreen(vOrigin,vVecScreen))
	{
		cl_entity_s *ent=gEngfuncs.GetEntityByIndex(iID);
		color3_s clrTeam=g_Player[iID].clrTeam;
		int x=(int)vVecScreen[0],
			y=(int)vVecScreen[1];
        #define ystep y+=gUtils.StrHeight()

		float distance = g_Player[iID].Distance/22.0f;
	    int   boxradius = (350.0*90.0) / (distance*fCurrentFOV);	 
	    BOUND_VALUE(boxradius,1,350);

		g_Draw.DrawBox(x,y,g_Player[iID].clrTeam2,boxradius);
	}
}
vec3_t SmoothenMovement(vec3_t vOrigin,vec3_t vPrevOrigin,float flCurrentTime,float flTime,float flPrevTime)
{
	vec3_t vPos=vOrigin-vPrevOrigin;
	float flInterp=1.0f;
	if(flTime-flPrevTime+flTime>flCurrentTime)
		flInterp-=((flTime-flPrevTime+flTime-flCurrentTime)/(flTime-flPrevTime));
	return(vOrigin+vPos*flInterp);
}
void CESP::HUD_Redraw()
{
	for(int iID=1;iID<=gEngfuncs.GetMaxClients();++iID)
	{
		if(gUtils.isValidEnt(iID))
			DrawPlayerEsp(iID,gEngfuncs.GetEntityByIndex(iID)->origin,false);
		else if(iID!=g_Local.iIndex)
		{
			if(g_Player[iID].currentSound.dwTime+1000<=GetTickCount())
				g_Player[iID].currentSound.bValid=false;
			else
				DrawPlayerEsp(iID,(1.0f?(SmoothenMovement(g_Player[iID].currentSound.vOrigin,g_Player[iID].previousSound.vOrigin,(float)GetTickCount(),(float)g_Player[iID].currentSound.dwTime,(float)g_Player[iID].previousSound.dwTime)):g_Player[iID].currentSound.vOrigin),true);
		}
	}
}