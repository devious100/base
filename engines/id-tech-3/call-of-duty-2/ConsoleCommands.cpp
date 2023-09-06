/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/***********************/
/* ConsoleCommands.cpp */
/***********************/
#include "stdafx.h"

cmdTable_t cmdTable[] =
{
	{ "cu_test",			&fnTest				},
	{ "cu_setdvar",			&fnSetDvar			},
	{ "cu_zoom",			&fnZoom				},
	{ "cu_log",				&fnLog				},
	{ "cu_namesteal",		&fnNameSteal		},
	{ NULL,					NULL				},
};

/* cu_test */
void fnTest( )
{
	//ri->Printf( 0, "tess.shader->name = %s\n", *( char ** )( (DWORD)gfxMI.lpBaseOfDll + 0x009817BC ) );
}

/* cu_setdvar */
void fnSetDvar( )
{
	char szDvar[512] = "";
	char szValue[512]= "";
	if( ri->Cmd_Argc( ) >= 3 )
	{
		strcpy_s<512>( szDvar, ri->Cmd_Argv( 1 ) );

		for( int i = 2; i < ri->Cmd_Argc( ); i++ )
		{
			strcat_s<512>( szValue, ri->Cmd_Argv( i ) );
			if( i != ( ri->Cmd_Argc( ) - 1 ) )
				strcat_s<512>( szValue, " " );
		}

		dvar_t * dv = Dvar_FindDvar( szDvar );

		if( dv )
		{
			ri->Dvar_Get( dv, szValue );

			ri->Printf( 0, "Dvar '^5%s^7' new value '^5%s^7'\n", dv->name, szValue );
		}
		else
			ri->Printf( 0, "Dvar '^5%s^7' not found^1!\n", szDvar );
	}
	else
	{
		ri->Printf( 0, "    ^5cu_setdvar ^9<^7dvar^9> ^9<^7value^9>\n" );
		ri->Printf( 0, "\n^7Example:\n" );
		ri->Printf( 0, "    ^5cu_setdvar ^7cg_thirdperson 1\n\n" );
	}
}

void fnZoom( )
{
	static float currentZoomLevel = 1.0f;

	if( ri->Cmd_Argc( ) == 2 )
	{
		float zoomLevel = atof( ri->Cmd_Argv( 1 ) );

		if( zoomLevel < 1.0f )
			ri->Printf( 0, "^1Invalid zoom level!\n^7Must be greater than or equal to 1.0\n" );
		else
		{
			if( zoomLevel == currentZoomLevel )
			{
				fZoomFactor			= 1.0f;
				currentZoomLevel	= 1.0f;
			}
			else
			{
				fZoomFactor			= 1.0f / zoomLevel;
				currentZoomLevel	= zoomLevel;
			}
		}
	}
	else
	{
		ri->Printf( 0, "    ^5cu_zoom ^9<^7zoom_level^9>\n" );
		ri->Printf( 0, "\n^7Example:\n" );
		ri->Printf( 0, "    ^5cu_zoom ^710\n" );
		ri->Printf( 0, "  = ^510^7x zoom\n\n" );
	}
}

void fnLog( )
{
	if( ri->Cmd_Argc( ) == 2 )
	{
		pLog->Printf( "%s\n", ri->Cmd_Argv( 1 ) );
		ri->Printf( 0, "^7Logged ^9'^5%s^9'^7.\n" );
	}
	else
	{
		ri->Printf( 0, "    ^5cu_log ^9\"^7log_text^9\"\n" );
		ri->Printf( 0, "\n^7Example:\n" );
		ri->Printf( 0, "    ^5cu_log ^9\"^7I GET PRINTED TO A LOG FILE!^9\"\n" );
	}
}

void fnNameSteal( )
{
	int				clientNum;
	clientInfo_t *	ci;

	for( int i = 0; i < 500; i++ )
	{
		srand( timeGetTime( ) );
		clientNum = ( rand( ) % MAX_CLIENTS );

		if( clientNum != cg->clientNum )
		{
			ci = &clientinfo[ clientNum ];

			if( ci->infoValid && strcmp( clientinfo[ cg->clientNum ].name, ci->name ) != 0 )
			{
				Dvar_SetStr( "name", ci->name );
				break;
			}
		}

		Sleep( 1 );
	}
}

void ProcessCommands( )
{
	int i = 0;
	while( cmdTable[i].function && cmdTable[i].name	)
	{
		ri->Cmd_AddCommand( cmdTable[i].name, cmdTable[i].function );

		i++;
	}
}