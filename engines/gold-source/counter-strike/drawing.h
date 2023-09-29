#pragma once

#include "main.h"

class CDrawing
{
public:
	void Fill( int x, int y, int w, int h, color4_s clr );
	void Box( int x, int y, int w, int h, int lw, color4_s clr );
	void String(bool bCenter,int x,int y,color3_s clrColor,const char *szText,...);
	void DrawEspText(bool bBG,int x,int y,color3_s clrColor,char *szText,...);
	void DrawBox( int x, int y, color4_s clr, int radius );
};

extern CDrawing g_Draw;