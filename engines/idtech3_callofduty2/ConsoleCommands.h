/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/*********************/
/* ConsoleCommands.h */
/*********************/
#pragma once

typedef struct
{
	const char *	name;
	xcommand_t		function;
} cmdTable_t;

extern cmdTable_t cmdTable[];

void ProcessCommands		( );

void fnTest					( );
void fnSetDvar				( );
void fnZoom					( );
void fnLog					( );
void fnNameSteal			( );