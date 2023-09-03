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
//! \file	Console.cpp
//! \brief  Input/output console window and command processor
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Console.h"

#include "..\HL\SDK.h"
#include "..\HL\Surface.h"
#include "..\HL\Engine.h"

#include "SourceBase.h"
#include "SBCmd.h"
#include "SourceBase.h"
#include "CmdArgs.h"

CConsole gConsole;

static Key_t gKeyMap[MOUSE_COUNT+KEY_COUNT] = {
	{ "", "" },
	{ "0", ")" },			//KEY_0,
	{ "1", "!" },			//KEY_1,
	{ "2", "\"" },			//KEY_2,
	{ "3", "£" },			//KEY_3,
	{ "4", "$" },			//KEY_4,
	{ "5", "%" },			//KEY_5,
	{ "6", "^" },			//KEY_6,
	{ "7", "&" },			//KEY_7,
	{ "8", "*" },			//KEY_8,
	{ "9", "(" },			//KEY_9,
	{ "a", "A" },			//KEY_A,
	{ "b", "B" },			//KEY_B,
	{ "c", "C" },			//KEY_C,
	{ "d", "D" },			//KEY_D,
	{ "e", "E" },			//KEY_E,
	{ "f", "F" },			//KEY_F,
	{ "g", "G" },			//KEY_G,
	{ "h", "H" },			//KEY_H,
	{ "i", "I" },			//KEY_I,
	{ "j", "J" },			//KEY_J,
	{ "k", "K" },			//KEY_K,
	{ "l", "L" },			//KEY_L,
	{ "m", "M" },			//KEY_M,
	{ "n", "N" },			//KEY_N,
	{ "o", "O" },			//KEY_O,
	{ "p", "P" },			//KEY_P,
	{ "q", "Q" },			//KEY_Q,
	{ "r", "R" },			//KEY_R,
	{ "s", "S" },			//KEY_S,
	{ "t", "T" },			//KEY_T,
	{ "u", "U" },			//KEY_U,
	{ "v", "V" },			//KEY_V,
	{ "w", "W" },			//KEY_W,
	{ "x", "X" },			//KEY_X,
	{ "y", "Y" },			//KEY_Y,
	{ "z", "|" },			//KEY_Z,
	{ "0", "0" },			//KEY_PAD_0,
	{ "1", "1" },			//KEY_PAD_1,
	{ "2", "2" },			//KEY_PAD_2,
	{ "3", "3" },			//KEY_PAD_3,
	{ "4", "4" },			//KEY_PAD_4,
	{ "5", "5" },			//KEY_PAD_5,
	{ "6", "6" },			//KEY_PAD_6,
	{ "7", "7" },			//KEY_PAD_7,
	{ "8", "8" },			//KEY_PAD_8,
	{ "9", "9" },			//KEY_PAD_9,
	{ "/", "/" },			//KEY_PAD_DIVIDE,
	{ "*", "*" },			//KEY_PAD_MULTIPLY,
	{ "-", "-" },			//KEY_PAD_MINUS,
	{ "+", "+" },			//KEY_PAD_PLUS,
	{ "enter", "enter" },				//KEY_PAD_ENTER,
	{ ".", ">" },				//KEY_PAD_DECIMAL,
	{ "[", "{" },				//KEY_LBRACKET,
	{ "]", "}" },				//KEY_RBRACKET,
	{ ";", ":" },				//KEY_SEMICOLON,
	{ "'", "@" },				//KEY_APOSTROPHE,
	{ "`", "¬" },				//KEY_BACKQUOTE,
	{ ",", "<" },				//KEY_COMMA,
	{ ".", ">" },				//KEY_PERIOD,
	{ "/", "?" },				//KEY_SLASH,
	{ "\\", "|" },				//KEY_BACKSLASH,
	{ "-", "_" },				//KEY_MINUS,
	{ "=", "+" },				//KEY_EQUAL,
	{ "enter", "enter" },				//KEY_ENTER,
	{ "space", "space" },				//KEY_SPACE,
	{ "backspace", "backspace" },				//KEY_BACKSPACE,
	{ "tab", "tab" },				//KEY_TAB,
	{ "caps", "caps" },				//KEY_CAPSLOCK,
	{ "numlock", "numlock" },				//KEY_NUMLOCK,
	{ "escape", "escape" },				//KEY_ESCAPE,
	{ "scrlock", "scrlock" },				//KEY_SCROLLLOCK,
	{ "insert", "insert" },				//KEY_INSERT,
	{ "delete", "delete" },				//KEY_DELETE,
	{ "home", "home" },				//KEY_HOME,
	{ "end", "end" },				//KEY_END,
	{ "pageup", "pageup" },				//KEY_PAGEUP,
	{ "pagedown", "pagedown" },				//KEY_PAGEDOWN,
	{ "break", "break" },				//KEY_BREAK,
	{ "lshift", "lshift" },				//KEY_LSHIFT,
	{ "rshift", "rshift" },				//KEY_RSHIFT,
	{ "lalt", "lalt" },				//KEY_LALT,
	{ "ralt", "ralt" },				//KEY_RALT,
	{ "lctrl", "lctrl" },				//KEY_LCONTROL,
	{ "rctrl", "rctrl" },				//KEY_RCONTROL,
	{ "lwin", "lwin" },				//KEY_LWIN,
	{ "rwin", "rwin" },				//KEY_RWIN,
	{ "app", "app" },				//KEY_APP,
	{ "up", "up" },				//KEY_UP,
	{ "left", "left" },				//KEY_LEFT,
	{ "down", "down" },				//KEY_DOWN,
	{ "right", "right" },				//KEY_RIGHT,
	{ "f1", "f1" },				//KEY_F1,
	{ "f2", "f2" },				//KEY_F2,
	{ "f3", "f3" },				//KEY_F3,
	{ "f4", "f4" },				//KEY_F4,
	{ "f5", "f5" },				//KEY_F5,
	{ "f6", "f6" },				//KEY_F6,
	{ "f7", "f7" },				//KEY_F7,
	{ "f8", "f8" },				//KEY_F8,
	{ "f9", "f9" },				//KEY_F9,
	{ "f10", "f10" },			//KEY_F10,
	{ "f11", "f11" },			//KEY_F11,
	{ "f12", ")" },				//KEY_F12,
	{ "capstoggle", "capstoggle" },				//KEY_CAPSLOCKTOGGLE,
	{ "numlocktoggle", "numlocktoggle" },				//KEY_NUMLOCKTOGGLE,
	{ "scrlocktoggle", "scrlocktoggle" },				//KEY_SCROLLLOCKTOGGLE,

										//// Mouse
	{ "mouse1", "mouse1" },				//MOUSE_LEFT = MOUSE_FIRST,
	{ "mouse2", "mouse2" },				//MOUSE_RIGHT,
	{ "mouse3", "mouse3" },				//MOUSE_MIDDLE,
	{ "mouse4", "mouse4" },				//MOUSE_4,
	{ "mouse5", "mouse5" },				//MOUSE_5,
	{ "mwheelup", "mwheelup" },				//MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	{ "mwheeldown", "mwheeldown" },				//MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down
};

////////////////////////////////////////////////////////////////////////////////
//! 
//! \param echo - 
//! \param text to the console" - 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
MAKE_COMMAND( echo, "Echo some text to the console" )
{
	// pCmd is passes as an argument to command functions
	int iArgs = pCmd->ArgC();

	std::string sText;

	for( int i = 1; i < iArgs; ++i )
	{
		sText.append( pCmd->ArgV(i) );
		sText.append( " " );
	}

	gConsole.Echo( sText.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
// CConsole::CConsole
//! Constructor
//! 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
CConsole::CConsole()
{
	m_active = false;
}

////////////////////////////////////////////////////////////////////////////////
// CConsole::GetActive
//! Get the console active state
//! 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CConsole::GetActive()
{
	return m_active;
}

////////////////////////////////////////////////////////////////////////////////
// CConsole::SetActive
//! Set the console active state
//! 
//! \param bActive - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CConsole::SetActive( bool bActive )
{
	m_active = bActive;
}

////////////////////////////////////////////////////////////////////////////////
// CConsole::KeyEvent
//! Called when a key is pressed
//! 
//! \param iKey - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CConsole::KeyEvent( int iKey )
{
	Key_t pKey = gKeyMap[iKey];

	switch( iKey )
	{
	case KEY_LSHIFT:
	case KEY_RSHIFT:
		return;

	case KEY_ENTER:
		CSourceBase::Get()->ProcessCommand( m_input.c_str() );
		m_input.clear();
		return;

	case KEY_SPACE:
		m_input.append( " " );
		return;

	case KEY_BACKSPACE:
		m_input.erase( m_input.length()-1 );
		return;
	}

	if( iKey >= KEY_0 && iKey <= KEY_EQUAL )
	{
		if( GetAsyncKeyState( VK_LSHIFT ) || GetAsyncKeyState( VK_RSHIFT ) )
			m_input.append( pKey.m_alt );
		else
			m_input.append( pKey.m_def );
	}
}

////////////////////////////////////////////////////////////////////////////////
// CConsole::Render
//! Render the console
//! 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CConsole::Render()
{
	if( !m_active )
		return;

	int iWidth, iHeight;
	gEngine.GetScreenSize( iWidth, iHeight );

	iHeight /= 2;

	gSurface.DrawSetColour( 10, 10, 10, 128 );
	gSurface.DrawFilledRect( 0, 0, iWidth, iHeight );

	int th = g_pMyFont->GetTextHeight();

	g_pMyFont->PrintString( 5, iHeight-th, 255, 255, 255, 255, kAlign_Left, "> %s", m_input.c_str() );

	for( unsigned int i = 0; i < m_lines.size(); ++i )
	{
		unsigned int idx = (m_lines.size()-i)+1;
		g_pMyFont->PrintString( 5, iHeight-(th*(idx)), 255, 255, 255, 255, kAlign_Left, (char*)m_lines[i].c_str() );
	}
}

////////////////////////////////////////////////////////////////////////////////
// CConsole::Echo
//! Echo some text to the console
//! 
//! \param char* szMsg - 
//! \param ... - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CConsole::Echo( const char* szMsg, ... )
{
	va_list		va_alist;
	char		szBuf[2048];

	va_start( va_alist, szMsg );
	_vsnprintf( szBuf, sizeof( szBuf ), szMsg, va_alist );
	va_end( va_alist );

	m_lines.push_back( szBuf );
}