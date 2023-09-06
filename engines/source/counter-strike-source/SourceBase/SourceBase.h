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
//! \file	SourceBase.h
//! \brief	The core API for the hack, this includes:
//!			File system, logging, cvar handler etc
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef SOURCEBASE_H
#define SOURCEBASE_H

#include "..\Include.h"
#include "Font.h"

enum
{
	DIR_BASE,		// The base directory
	DIR_CONFIG,		// Directory that will store all configs
	DIR_LOG,		// Directory that will store all logs

	// add any directories you want here, but you must also add them to g_szDirectories

	DIR_MAX
};

class CCmdArgs;
class CSBVar;
class CSBCmd;

class CSourceBase
{
private:
	CSourceBase();

public:
	virtual ~CSourceBase();

	static CSourceBase*	Get();                                          //!< Singleton accessor
	void				Init( HINSTANCE hModule, const char* szGame );  //!< Set up the SourceBase API
	void				Shutdown();                                     //!< Shutdown the API and clean up any resources
	char*				GetDirFile( char* szFile, int iDir=DIR_BASE );  //!< Get a file from the hack directory tree
	bool				DoesFileExist( char * szFile );                 //!< Determine if a directory exists or not using an absolute path
	void				Log( const char* szText, ... );

	CFont*				CreateFont( const char* szFace, 
									int iSize, 
									int iWeight,
									int iFlags );

	void				ProcessCommand( const char* szCommand );
	CSBVar*				FindVar( const char* szName );
	CSBCmd*				FindCommand( const char* szCommand );

private:
	void				BuildArgumentList( std::string sInput, CCmdArgs* pArgs );

private:
	char m_baseDirectory[MAX_PATH];
	char m_gameName[64];
	char m_logFile[MAX_PATH];

	std::vector< ISBObject* > m_objectList;
};

#ifndef LOG
	#if SB_DEBUG
		#define LOG CSourceBase::Get()->Log
	#else
		#define LOG // don't do any logging in release mode
	#endif // SB_DEBUG
#endif // LOG

#endif // SOURCEBASE_H