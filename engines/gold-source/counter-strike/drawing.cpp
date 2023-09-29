#include "main.h"

CDrawing g_Draw;

void CDrawing::Fill(int x,int y,int w,int h,color4_s clr)
{
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glColor4ub( clr.r, clr.g, clr.b, clr.a );
	glBegin( GL_QUADS );
		glVertex2i( x, y );
		glVertex2i( x + w, y );
		glVertex2i( x + w, y + h );
		glVertex2i( x, y + h );
	glEnd();

	glColor3f( 1.0f, 1.0f, 1.0f );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
}
void CDrawing::Box(int x,int y,int w,int h,int lw,color4_s clr)
{
	Fill(x,y,w,lw,clr);
	Fill(x,y+lw,lw,h-lw,clr);
	Fill(x+ w-lw,y+lw,lw,h-lw,clr);
	Fill(x+ lw,y+h-lw,w-2*lw,lw,clr);
}
void CDrawing::String(bool bCenter,int x,int y,color3_s clrColor,const char *szText,...)
{
	char szBuf[500];
	va_list valist;
	va_start(valist,szText);
	_vsnprintf_s(szBuf,sizeof(szBuf),szText,valist);
	va_end(valist);
	gEngfuncs.pfnDrawSetTextColor(clrColor.r/255.0f,clrColor.g/255.0f,clrColor.b/255.0f);
	gEngfuncs.pfnDrawConsoleString(x-(bCenter?(gUtils.StrWidth("%s",szBuf)/2):0),y,szBuf);
}
void CDrawing::DrawEspText(bool bBG,int x,int y,color3_s clrColor,char *szText,...)
{
	char szBuf[500];
	va_list valist;
	va_start(valist,szText);
	_vsnprintf_s(szBuf,sizeof(szBuf),szText,valist);
	va_end(valist);
	if(bBG)
		g_Draw.Fill(x-gUtils.StrWidth(szBuf)/2-5,y-1,gUtils.StrWidth(szBuf)+10,gUtils.StrHeight()+2,rgba(0,0,0,128));
	g_Draw.String(true,x,y,clrColor,szBuf);
}
void CDrawing::DrawBox( int x, int y, color4_s clr, int radius ) 
{ 
    int radius2 = radius << 1; 
	Fill( x-radius+2, y-radius, radius2-2, 2, clr ); 
	Fill( x-radius, y-radius, 2, radius2,clr ); 
    Fill( x-radius, y+radius, radius2, 2, clr); 
	Fill( x+radius, y-radius, 2, radius2+2, clr ); 
} 