/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/***********/
/* cuBot.h */
/***********/

#pragma once

extern CBot	*			pBot;

extern CLogStream *		pLog;
extern HMODULE			hGfxModule;
extern MODULEINFO		gfxMI;

typedef refexport_t *	(*t_GetRefAPI)				( int apiVersion, refimport_t * rimp );
typedef FARPROC			(WINAPI *t_GetProcAddress)	( HMODULE hModule, LPCSTR lpProcName );
//typedef HMODULE		(WINAPI *t_LoadLibraryA)	( LPCSTR lpLibFileName );

extern t_GetRefAPI			o_GetRefAPI;
extern t_GetProcAddress		o_GetProcAddress;
//extern t_LoadLibraryA		o_LoadLibraryA;

refexport_t *			h_GetRefAPI					( int apiVersion, refimport_t * rimp );
FARPROC WINAPI			h_GetProcAddress			( HMODULE hModule, LPCSTR lpProcName );
//HMODULE WINAPI		h_LoadLibraryA				( LPCSTR lpLibFileName );

void					InitializeGameDetours		( );
void					DestroyGameDetours			( );