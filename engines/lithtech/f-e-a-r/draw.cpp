#include "sdk.h"


void Draw::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{

	D3DXVECTOR2 vLine[2];

	pLine->SetWidth( w );
	pLine->SetAntialias( false );
	pLine->SetGLLines( true );

	vLine[0].x = x + w/2;	vLine[0].y = y;
	vLine[1].x = x + w/2;
	vLine[1].y = y + h;

	pLine->Begin( );
	pLine->Draw( vLine, 2, D3DCOLOR_RGBA( r, g, b, a ) );
	pLine->End( ); 

}
void Draw::FillRGBA(int x, int y, int w, int h, D3DCOLOR Color)
{
	D3DXVECTOR2 vLine[2];

	pLine->SetWidth( w );
	pLine->SetAntialias( false );
	pLine->SetGLLines( true );

	vLine[0].x = x + w/2;
	vLine[0].y = y;
	vLine[1].x = x + w/2;
	vLine[1].y = y + h;

	pLine->Begin( );
	pLine->Draw( vLine, 2, Color);
	pLine->End( ); 

}

void Draw::DrawLine(int startX, int startY, int endX, int endY, D3DCOLOR Color)
{
	D3DXVECTOR2 vLine[2];

	pLine->SetWidth(1);
	pLine->SetAntialias( false );
	pLine->SetGLLines( true );

	vLine[0].x = startX;
	vLine[0].y = startY;
	vLine[1].x = endX;
	vLine[1].y = endY;

	pLine->Begin( );
	pLine->Draw( vLine, 2, Color);
	pLine->End( ); 
}

void Draw::DrawMyText(int X, int Y, D3DCOLOR Color, char szString[] )
{
	RECT FontRect = { X, Y, X + 120, Y + 16 };
	g_pFont->DrawText( NULL, szString, -1, &FontRect, DT_NOCLIP , Color );
}

void Draw::DrawMenuText(int X, int Y, D3DCOLOR Color, char szString[] )
{
	RECT FontRect = { X, Y, X + 120, Y + 16 };
	MenuFont->DrawText( NULL, szString, -1, &FontRect, DT_NOCLIP , Color );
}
void Draw::DrawText(int X, int Y, D3DCOLOR Color, const char* pszString, ...)
{
	RECT FontRect = { X, Y, X + 120, Y + 16 };
	va_list va_alist;
	char logbuf[256] = { '\0' };
	va_start (va_alist, pszString);
	vsprintf (logbuf, pszString, va_alist);
	va_end (va_alist);
	MenuFont->DrawText( NULL, logbuf, -1, &FontRect, DT_NOCLIP , Color );
}
void Draw::DrawText(int X, int Y, D3DCOLOR Color, ID3DXFont* Font, const char* pszString, ...)
{
	RECT FontRect = { X, Y, X + 120, Y + 16 };
	va_list va_alist;
	char logbuf[256] = { '\0' };
	va_start (va_alist, pszString);
	vsprintf (logbuf, pszString, va_alist);
	va_end (va_alist);
	Font->DrawText( NULL, logbuf, -1, &FontRect, DT_NOCLIP , Color );
}

