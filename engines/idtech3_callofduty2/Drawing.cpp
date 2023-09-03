/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/***************/
/* Drawing.cpp */
/***************/
#include "stdafx.h"

vec4_t				colWhite			= { 1.0f, 1.0f, 1.0f, 1.0f };
vec4_t				colBlack			= { 0.0f, 0.0f, 0.0f, 1.0f };
vec4_t				colRed				= { 1.0f, 0.0f, 0.0f, 1.0f };
vec4_t				colGreen			= { 0.0f, 1.0f, 0.0f, 1.0f };
vec4_t				colBlue				= { 0.0f, 0.0f, 1.0f, 1.0f };
vec4_t				colYellow			= { 1.0f, 1.0f, 0.0f, 1.0f };
vec4_t				colCyan				= { 0.0f, 1.0f, 1.0f, 1.0f };
vec4_t				colOrange			= { 1.0f, 0.6f, 0.0f, 1.0f };
vec4_t				colWhite_			= { 1.0f, 1.0f, 1.0f, 0.5f };
vec4_t				colBlack_			= { 1.0f, 1.0f, 1.0f, 0.5f };
vec4_t				colRed_				= { 1.0f, 0.0f, 0.0f, 0.5f };
vec4_t				colGreen_			= { 0.0f, 1.0f, 0.0f, 0.5f };
vec4_t				colBlue_			= { 0.0f, 0.0f, 1.0f, 0.5f };
vec4_t				colYellow_			= { 1.0f, 1.0f, 0.0f, 0.5f };
vec4_t				colCyan_			= { 0.0f, 1.0f, 1.0f, 0.5f };
vec4_t				colOrange_			= { 1.0f, 0.6f, 0.0f, 0.5f };

qhandle_t			sWhite				= NULL;
qhandle_t			sBlack				= NULL;
qhandle_t			sGrenadeIcon		= NULL;
qhandle_t			sFlareA				= NULL;
qhandle_t			sFlareB				= NULL;
qhandle_t			sArrow				= NULL;

fontInfo_t *		consoleFont			= NULL;
fontInfo_t *		normalFont			= NULL;
fontInfo_t *		smallFont			= NULL;
fontInfo_t *		bigFont				= NULL;
fontInfo_t *		extraBigFont		= NULL;
fontInfo_t *		boldFont			= NULL;

void DrawString( float x, float y, float scale, float * colour, fontInfo_t * font, int style, const char * fmt, ... )
{
	char	szBuffer[1024] = "";
	va_list	ap;

	if( !fmt || *fmt == '\0' )
		return;

	if( !font )
		font = normalFont;

	va_start( ap, fmt );
	_vsnprintf_s( szBuffer, 1024, 1023, fmt, ap );
	va_end( ap );

	re->DrawStringExt( szBuffer, 0x7FFFFFFF, font, x, y, scale, scale, colour, 3 );
}

void DrawCenterString( float x, float y, float scale, float * colour, fontInfo_t * font, int style, const char * fmt, ... )
{
	if( !fmt || *fmt == '\0' )
		return;

	if( !font )
		font = normalFont;

	char	szBuffer[1024] = "";
	va_list	ap;

	va_start( ap, fmt );
	_vsnprintf_s( szBuffer, 1024, 1023, fmt, ap );
	va_end( ap );

	float textWidth  = scale * (float)re->StringWidth ( szBuffer, 0, font );
	float textHeight = scale * (float)re->StringHeight( font );

	x -= ( textWidth  * 0.5f );
	y += ( textHeight * 0.5f );

	re->DrawStringExt( szBuffer, 0x7FFFFFFF, font, x, y, scale, scale, colour, style );
}

void DrawStringRect( float x, float y, float scale, float * textColour, float * rectColour, fontInfo_t * font, int style, const char * fmt, ... )
{
	if( !font )
		font = normalFont;

	if( !fmt || *fmt == '\0' )
		return;

	char	szBuffer[1024] = "";
	va_list	ap;

	va_start( ap, fmt );
	_vsnprintf_s( szBuffer, 1024, 1023, fmt, ap );
	va_end( ap );

	float textWidth  = scale * (float)re->StringWidth ( szBuffer, 0, font );
	float textHeight = scale * (float)re->StringHeight( font );

	re->DrawStretchPic( x, y, textWidth + 2.0f, textHeight + 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, rectColour, sWhite );
	re->DrawStringExt( szBuffer, 0x7FFFFFFF, font, x + 1.0f, y + 1.0f + (float)textHeight, scale, scale, textColour, style );
}

void DrawStringRectOutline( float x, float y, float scale, float * textColour, float * rectColour, float * outlineColour, float thickness, fontInfo_t * font, int style, const char * fmt, ... )
{
	if( !font )
		font = normalFont;

	if( !fmt || *fmt == '\0' )
		return;

	char	szBuffer[1024] = "";
	va_list	ap;

	va_start( ap, fmt );
	_vsnprintf_s( szBuffer, 1024, 1023, fmt, ap );
	va_end( ap );

	float textWidth  = scale * (float)re->StringWidth ( szBuffer, 0, font );
	float textHeight = scale * (float)re->StringHeight( font );

	DrawRect( x, y, 
			 textWidth  + 2.0f + ( 2.0f * thickness ), 
			 textHeight + 2.0f + ( 2.0f * thickness ),
			 thickness, 
			 outlineColour );

	x += thickness;
	y += thickness;

	re->DrawStretchPic( x, y, 
						textWidth  + 2.0f,
						textHeight + 2.0f,
						0.0f, 0.0f, 0.0f, 0.0f,
						rectColour, sWhite );

	x += 1.0f;
	y += ( 1.0f + textHeight );

	re->DrawStringExt( szBuffer, 0x7FFFFFFF, font, x, y, scale, scale, textColour, style );
}

void DrawRect( float x, float y, float width, float height, float thickness, float * colour )
{
	re->DrawStretchPic( x, y, width, thickness, 0.0f, 0.0f, 0.0f, 0.0f, colour, sWhite );

	y += thickness;

	re->DrawStretchPic( x, y, thickness, height - ( 2 * thickness ), 0.0f, 0.0f, 0.0f, 0.0f, colour, sWhite );

	y += ( height - ( 2 * thickness ) );

	re->DrawStretchPic( x, y, width, thickness, 0.0f, 0.0f, 0.0f, 0.0f, colour, sWhite );

	y -= ( height - ( 2 * thickness ) );
	x += ( width - thickness );

	re->DrawStretchPic( x, y, thickness, height - ( 2 * thickness ), 0.0f, 0.0f, 0.0f, 0.0f, colour, sWhite );
}

void DrawCircle( float x, float y, float radius, float smoothness, float * colour )
{
	for( float fAngle = 0; fAngle < 90.0f; fAngle += ( 90.0f / smoothness ) )
	{
		re->DrawRotatedPic( x, y, radius * 2, radius * 2, 0.0f, 0.0f, 0.0f, 0.0f, fAngle, colour, sWhite );
	}
}