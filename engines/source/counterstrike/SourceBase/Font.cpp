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
//! \file	Font.cpp
//! \brief  Font class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Font.h"
#include "..\HL\Surface.h"

////////////////////////////////////////////////////////////////////////////////
// CFont::Destroy
//! Called when the font is being destroyed
//! 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CFont::Destroy()
{
	// cleanup
}

////////////////////////////////////////////////////////////////////////////////
// CFont::PrintString
//! Print some text to the screen
//! 
//! \param x - x location in pixels
//! \param y - y location in pixels
//! \param r - red value
//! \param g - green value
//! \param b - blue value
//! \param a - alpha value
//! \param dwAlign - alignment flags
//! \param szText - text to be printed
//! \param ... - variable arguments
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CFont::PrintString( int x, int y, int r, int g, int b, int a, DWORD_PTR dwAlign, const char* szText, ... )
{
	va_list		va_alist;
	char		szBuf[1024];
	int			iLen;
	int			iWidth;
	int			iHeight;	

	va_start( va_alist, szText );
	_vsnprintf( szBuf, sizeof( szBuf ), szText, va_alist );
	va_end( va_alist );

	iLen = strlen( szBuf );

	wchar_t* pszFmt = new wchar_t[ iLen+1 ];
	mbstowcs( pszFmt, szBuf, iLen+1 );

	gSurface.GetTextSize( m_id, pszFmt, iWidth, iHeight );

	if( dwAlign & kAlign_Right )
		x -= iWidth;
	else if( dwAlign & kAlign_CenterH )
		x -= iWidth/2;
	if( dwAlign & kAlign_CenterV )
		y -= iHeight/2;

	gSurface.DrawSetTextColour( r, g, b, a );
	gSurface.DrawSetTextFont( m_id );
	gSurface.DrawSetTextPos( x, y );
	gSurface.DrawPrintText( pszFmt, iLen );

	SAFE_DELETE_ARRAY( pszFmt );
}