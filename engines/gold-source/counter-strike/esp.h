#pragma once

#include "main.h"

class CESP
{
public:

	void DrawPlayerEsp(int iID,vec3_t vOrigin,bool bFarESP);
	vec3_t SmoothenMoveent(vec3_t vOrigin,vec3_t vPrevOrigin,float flCurrentTime,float flTime,float flPrevTime);
	void HUD_Redraw();
	
}extern gESP;