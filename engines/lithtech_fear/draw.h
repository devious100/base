#ifndef DRAW_H
#define DRAW_H

#include "sdk.h"


class Draw
{
public:
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	void FillRGBA(int x, int y, int w, int h, D3DCOLOR Color);
	void DrawMyText( int X, int Y, D3DCOLOR Color, char szString[] );
	void DrawMenuText( int X, int Y, D3DCOLOR Color, char szString[] );
	void DrawText(int X, int Y, D3DCOLOR Color, const char* pszString, ...);
	void DrawText(int X, int Y, D3DCOLOR Color, ID3DXFont* Font, const char* pszString, ...);
	void DrawLine(int startX, int startY, int endX, int endY, D3DCOLOR Color);

	D3DVIEWPORT9 oViewport;
	ID3DXLine *pLine;
	ID3DXFont *g_pFont;
	ID3DXFont *MenuFont;
};

#endif