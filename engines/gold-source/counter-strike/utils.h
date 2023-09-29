#pragma once

#include "main.h"

struct color3_s{int r,g,b;};
struct color5_s{float r,g,b;};
struct color4_s{int r,g,b,a;};

color3_s rgb(int r,int g,int b);
color4_s rgba(int r,int g,int b,int a);

class CUtils
{
public:
	int iCenterX,iCenterY;
public:

	void GetScreenInfo(SCREENINFO *screeninfo);
	void ConEcho(char *szText,...);
	void ConExec(char *szCommand,...);
	void VectorAngles(const float *flForward,float *flAngles);
	void *Detour(BYTE *pbFunc,BYTE *pbNewFunc,int iLen);
	int StrWidth(char *szText,...);
	int StrHeight();
	bool CalcScreen(float *flOrigin,float *flVecScreen);
	bool isValidEnt(int iIndex);

}extern gUtils;