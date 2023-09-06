/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

#include "StdAfx.h"
#include "LogStream.h"

CLogStream::CLogStream( const char * lpszFileName, const char * lpszMode )
{
	pLogFile = NULL;
	fopen_s( &pLogFile, lpszFileName, lpszMode );
}

CLogStream::CLogStream( const char * lpszFileName )
{
	pLogFile = NULL;
	fopen_s( &pLogFile, lpszFileName, "a+" );
}

CLogStream::~CLogStream(void)
{
	if( pLogFile )
		fclose( pLogFile );
}

void CLogStream::Printf( const char * lpszFormat, ... )
{
	char	szBuffer[1024] = "";
	va_list	ap;

	va_start( ap, lpszFormat );
	_vsnprintf_s<1024>( szBuffer, 1024, lpszFormat, ap );
	va_end( ap );

	if( pLogFile )
	{
		fprintf( pLogFile, "%s", szBuffer );
		fflush( pLogFile );
	}
}

void CLogStream::Print( const char * lpszText )
{
	if( pLogFile )
	{
		fprintf( pLogFile, "%s", lpszText );
		fflush( pLogFile );
	}
}

FILE * CLogStream::GetFile( )
{
	return pLogFile;
}

void CLogStream::LogData( PBYTE pbData, DWORD dwLength )
{
	for( DWORD dwCurrent = 0; dwCurrent < dwLength; dwCurrent += 16 )
	{
		int j = 0;

		Printf( "0x%08lX: ", pbData + dwCurrent );

		for( j = 0; j < 16; j += 4 )
		{
			for( int k = 0; k < 4; k++ )
			{
				if( ( dwCurrent + j + k ) < dwLength )
					Printf( "%02X", pbData[ dwCurrent + j + k ] );
				else
					Print( "  " );
			}
			Print( " " );
		}

		Print( "\t" );

		for( j = 0; j < 16; j++ )
		{
			if( ( dwCurrent + j ) < dwLength )
			{
				if( pbData[dwCurrent + j] < 32 || pbData[dwCurrent + j] > 126 )
					Print( "." );
				else
					Printf( "%c", pbData[dwCurrent + j] );
			}
			else
				Print( " " );
		}

		Print( "\n" );
	}
}
