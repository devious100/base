/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

#pragma once

class CLogStream
{
public:
	CLogStream( const char * lpszFileName, const char * lpszMode );
	CLogStream( const char * lpszFileName );
	~CLogStream( );

	void	Printf		( const char * lpszFormat, ... );
	void	Print		( const char * lpszText );
	FILE *	GetFile		( );
	void	LogData		( PBYTE pbData, DWORD dwLength );

private:
	FILE * pLogFile;
};
