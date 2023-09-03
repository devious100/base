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
//! \file	CSBCmd.cpp
//! \brief  Command implementation
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "SBCmd.h"

CSBCmd::CSBCmd( const char* szName, const char* szDescription, pfnCommandFunc pFunc )
{
	strncpy( m_name, szName, sizeof( m_name ) );
	strncpy( m_desc, szDescription, sizeof( m_desc ) );
	m_func = pFunc;

	Create( this );
}

CSBCmd::CSBCmd( const char* szName, pfnCommandFunc pFunc )
{
	strncpy( m_name, szName, sizeof( m_name ) );
	m_func = pFunc;

	Create( this );
}

bool CSBCmd::IsCommand()
{
	return true;
}

void CSBCmd::Execute( CCmdArgs* pArgs )
{
	if( !m_func )
		return;

	m_func( pArgs );
}