////////////////////////////////////////////////////////////////////////////////
//! SourceBase By Chod
//! -------------------------------------------------------------------------
//! SourceBase is free software: you can redistribute it and/or modify 
//! it under the terms of the GNU Lesser General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//! 
//! SourceBase is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//! GNU Lesser General Public License for more details.
//! 
//! You should have received a copy of the GNU Lesser General Public License
//! along with SourceBase. If not, see <http://www.gnu.org/licenses/>.
//! -------------------------------------------------------------------------
//! \file	SourceBase.cpp
//! \brief	The core API for the hack, this includes:
//!			File system, logging, cvar handler etc
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "SourceBase.h"
#include "..\HL\Surface.h"
#include "CmdArgs.h"
#include "SBCmd.h"
#include "SBVar.h"
#include "Console.h"

static const char* g_szDirectories[DIR_MAX] = {
	"",			// DIR_BASE
	"config",	// DIR_CONFIG
	"log",		// DIR_LOG
};

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::CSourceBase
//! Constructor
//! 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
CSourceBase::CSourceBase()
{

}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::~CSourceBase
//! Destructor
//! 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
CSourceBase::~CSourceBase()
{

}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::Get
//! Singleton accessor
//! 
//! \return CSourceBase* - 
////////////////////////////////////////////////////////////////////////////////
CSourceBase* CSourceBase::Get()
{
	static CSourceBase sSourceBase;
	return &sSourceBase;
}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::Init
//! Set up the various directories and log files etc
//! 
//! \param hModule - handle to this module
//! \param szGame - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CSourceBase::Init( HINSTANCE hModule, const char* szGame )
{
	// set up the base directory
	if( GetModuleFileName( hModule, m_baseDirectory, sizeof( m_baseDirectory ) ) )
	{
		for( int i = ( int )strlen( m_baseDirectory ); i > 0; i-- )
		{
			if( m_baseDirectory[ i ] == '\\' )
			{
				m_baseDirectory[ i + 1 ] = 0;
				break;
			}
		}		
	}
	else
	{
		ERR( "Could not resolve base directory" );
		return;
	}

	// copy the game name over
	strcpy( m_gameName, szGame );

	char szGameDir[MAX_PATH];
	sprintf( szGameDir, "%s%s\\", m_baseDirectory, m_gameName );
	if( !DoesFileExist( szGameDir ) )
		CreateDirectory( szGameDir, NULL );

	// create the log file name (based on time)
	time_t	pTime;
	tm*		pInfo;
	time(&pTime);
	pInfo = localtime( &pTime );
	char szLogFile[128];
	sprintf( szLogFile, "%d_%d_%d__%02d_%02d_%d.txt", pInfo->tm_mday, pInfo->tm_mon+1, pInfo->tm_year+1900, pInfo->tm_hour, pInfo->tm_min, pInfo->tm_sec );
	strcpy( m_logFile, GetDirFile( szLogFile, DIR_LOG ) );

	std::ofstream fout;
	fout.open( m_logFile, std::ios::out ); 
	fout.close( );
}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::Shutdown
//! Shutdown and clear resources
//! 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CSourceBase::Shutdown()
{
	// remove any objects and free up memory
	for( unsigned int i = 0; i < m_objectList.size(); ++i )
	{
		ISBObject* pObject = m_objectList.at(i);

		if( pObject )
		{
			pObject->Destroy();
			SAFE_DELETE( pObject );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::GetDirFile
//! Get the full path to a file, including a internal folder if required
//! 
//! \param szFile - name of the file to retrieve
//! \param iDir - directory, see g_szDirectories for folder names
//! \return char* - a pointer to a full path of the file
////////////////////////////////////////////////////////////////////////////////
char* CSourceBase::GetDirFile( char* szFile, int iDir )
{
	static char szOutFile[MAX_PATH];
	if( iDir != DIR_BASE )
	{
		sprintf( szOutFile, "%s%s\\%s\\", m_baseDirectory, m_gameName, g_szDirectories[ iDir ] );

		if( !DoesFileExist( szOutFile ) )
			CreateDirectory( szOutFile, NULL );

		strcat( szOutFile, szFile );
	}
	else
	{
		sprintf( szOutFile, "%s%s\\%s", m_baseDirectory, m_gameName, szFile );
	}

	return szOutFile;
}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::DoesFileExist
//! Determine whether a file exists or not on disk
//! 
//! \param szFile - absolute path to file or folder
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CSourceBase::DoesFileExist( char * szFile )
{
	if( !szFile )
		return false;

	DWORD_PTR dwAttr = GetFileAttributes( szFile );

	if( dwAttr == INVALID_FILE_ATTRIBUTES )
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::Log
//! Write some text to the log file
//! 
//! \param char* szText - 
//! \param ... - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CSourceBase::Log( const char* szText, ... )
{
	va_list		va_alist;
	std::ofstream	fout; 
	char		buf[1024];

	va_start( va_alist, szText );
	_vsnprintf( buf, sizeof( buf ), szText, va_alist );
	va_end( va_alist );
	
	fout.open( m_logFile, std::ios::app ); 

	if( fout.fail( ) ) 
	{
		fout.close();
		return;
	}

	fout << buf << std::endl; 
	fout.close( );
}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::CreateFont
//! Create a font to be used for rendering
//! 
//! \param char* szFace - name of the font face, e.g "Arial"
//! \param iSize - size of the font
//! \param iWeight - weight of the font (bold etc)
//! \param iFlags - udnerline, outline, shadow etc
//! \return CFont* - pointer to the newly created font, or NULL if it failed
////////////////////////////////////////////////////////////////////////////////
CFont* CSourceBase::CreateFont( const char* szFace, int iSize, int iWeight, int iFlags )
{
	CFont* pFont = new CFont;

	if( !pFont )
	{
		ERR( "Failed to allocate new font" );
		return NULL;
	}

	pFont->m_id = gSurface.CreateFont();

	if( !pFont->m_id )
	{
		SAFE_DELETE( pFont );
		ERR( "CreateFont() Bad font ID" );
		return NULL;
	}

	if( !gSurface.SetFontGlyphSet( pFont->m_id, szFace, iSize, iWeight, 0, 0, iFlags ) )
	{
		SAFE_DELETE( pFont );
		ERR( "Font glyph creation failed for font: '%s'", szFace );
		return NULL;
	}

	pFont->m_size = iSize;

	// add it to the list so it can be deleted
	m_objectList.push_back( pFont );
	return pFont;
}

////////////////////////////////////////////////////////////////////////////////
// CSourceBase::ProcessCommand
//! This function is used to interpret simple commands from a string and execute them as
//! predefined commands or cvar changes. 
//! 
//! \param szCommand - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CSourceBase::ProcessCommand( const char* szCommand )
{
	CCmdArgs args;

	BuildArgumentList( szCommand, &args );

	// first check through the cvar list
	CSBVar* pVar = (CSBVar*)FindVar( args.ArgV(0) );

	if( pVar )
	{
		bool bChange = ( args.ArgC() > 1 );

		switch( pVar->GetValue().m_type )
		{
		case kValue_INT:
			if( bChange )
			{
				pVar->GetValue().m_int = atoi( args.ArgV(1) );
				gConsole.Echo( "%s changed to \"%d\"", pVar->GetName(), pVar->GetValue().m_int );
			}
			else
				gConsole.Echo( "%s = \"%d\"", pVar->GetName(), pVar->GetValue().m_int );
			break;

		case kValue_FLOAT:
			if( bChange )
			{
				pVar->GetValue().m_float = (float)atof( args.ArgV(1) );
				gConsole.Echo( "%s changed to \"%f\"", pVar->GetName(), pVar->GetValue().m_float );
			}
			else
				gConsole.Echo( "%s = \"%f\"", pVar->GetName(), pVar->GetValue().m_float );

			break;

		case kValue_BOOL:
			if( bChange )
			{
				pVar->GetValue().m_bool = atoi( args.ArgV(1) ) == 1 ? true : false;
				gConsole.Echo( "%s changed to \"%s\"", pVar->GetName(), pVar->GetValue().m_bool ? "TRUE" : "FALSE" );
			}
			else
				gConsole.Echo( "%s = \"%s\"", pVar->GetName(), pVar->GetValue().m_bool ? "TRUE" : "FALSE" );

			break;
		}

		return;
	}


	// then check all available commands
	CSBCmd* pCommand = FindCommand( args.ArgV(0) );

	if( pCommand )
	{
		pCommand->Execute( &args );
		return;
	}

	gConsole.Echo( "Unknown command: '%s'", args.ArgV(0) );
}

void CSourceBase::BuildArgumentList( std::string sInput, CCmdArgs* pArgs )
{
	size_t nPos;

	while( 1 )
	{
		nPos = sInput.find_first_of( " \n\0" );

		if( nPos != std::string::npos )
		{
			pArgs->m_args.push_back( sInput.substr( 0, nPos ) );
			sInput.erase( 0, nPos+1 );
		}
		else
		{
			// add any left over arguments
			if( sInput.length() > 0 )
			{
				pArgs->m_args.push_back( sInput );
			}
			break;
		}
	}
}

CSBVar* CSourceBase::FindVar( const char* szName )
{
	CSBVar* pVar = (CSBVar*)CCmdBase::sCmdList;

	while( pVar )
	{
		if( !pVar->IsCommand() )
		{
			if( !strcmp( szName, pVar->GetName() ) )
				return pVar;
		}

		pVar = (CSBVar*)pVar->m_next;
	}
	return NULL;
}

CSBCmd* CSourceBase::FindCommand( const char* szCommand )
{
	CSBCmd* pCmd = (CSBCmd*)CCmdBase::sCmdList;

	while( pCmd )
	{
		if( pCmd->IsCommand() )
		{
			if( !strcmp( pCmd->GetName(), szCommand ) )
				return pCmd;
		}

		pCmd = (CSBCmd*)pCmd->m_next;
	}
	return NULL;
}