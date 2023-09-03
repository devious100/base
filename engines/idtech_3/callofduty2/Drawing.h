/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/*************/
/* Drawing.h */
/*************/
#pragma once

extern vec4_t			colWhite;
extern vec4_t			colBlack;
extern vec4_t			colRed;
extern vec4_t			colGreen;
extern vec4_t			colBlue;
extern vec4_t			colYellow;
extern vec4_t			colCyan;
extern vec4_t			colOrange;
extern vec4_t			colWhite_;
extern vec4_t			colBlack_;
extern vec4_t			colRed_;
extern vec4_t			colGreen_;
extern vec4_t			colBlue_;
extern vec4_t			colYellow_;
extern vec4_t			colCyan_;
extern vec4_t			colOrange_;

extern vec4_t			colRadarGreen;

extern qhandle_t		sWhite;
extern qhandle_t		sBlack;
extern qhandle_t		sGrenadeIcon;
extern qhandle_t		sFlareA;
extern qhandle_t		sFlareB;
extern qhandle_t		sArrow;

extern fontInfo_t *		consoleFont;
extern fontInfo_t *		normalFont;
extern fontInfo_t *		smallFont;
extern fontInfo_t *		bigFont;
extern fontInfo_t *		extraBigFont;
extern fontInfo_t *		boldFont;

void	DrawString				( float x, float y, float scale, float * colour, fontInfo_t * font, int style, const char * fmt, ... );
void	DrawCenterString		( float x, float y, float scale, float * colour, fontInfo_t * font, int style, const char * fmt, ... );
void	DrawStringRect			( float x, float y, float scale, float * textColour, float * rectColour, fontInfo_t * font, int style, const char * fmt, ... );
void	DrawStringRectOutline	( float x, float y, float scale, float * textColour, float * rectColour, float * outlineColour, float thickness, fontInfo_t * font, int style, const char * fmt, ... );
void	DrawRect				( float x, float y, float width, float height, float thickness, float * colour );
void	DrawCircle				( float x, float y, float radius, float smoothness, float * colour );