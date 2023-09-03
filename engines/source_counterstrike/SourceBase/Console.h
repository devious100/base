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
//! \file	Console.h
//! \brief  Input/output console window and command processor
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef CONSOLE_H
#define CONSOLE_H

#include "..\Include.h"

typedef struct Key_s
{
	char m_def[16];
	char m_alt[16];
} Key_t;

class CConsole
{
public:
	CConsole();

	void Render();
	void Echo( const char* szMsg, ... );
	void KeyEvent( int iKey );
	bool GetActive();
	void SetActive( bool bActive );

private:
	std::string					m_input;
	std::vector< std::string >	m_lines;
	bool						m_active;
};

extern CConsole gConsole;

#endif // CONSOLE_H