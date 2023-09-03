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
//! \file	Surface.h
//! \brief  Drawing tools
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef SURFACE_H
#define SURFACE_H

#include "..\Include.h"

class CSurface : public CVMethod
{
public:
	void			DrawSetColour( int r, int g, int b, int a );
	void			DrawLine( int x0, int y0, int x1, int y1 );
	void			DrawFilledRect( int x, int y, int w, int h );
	unsigned long	CreateFont( );
	bool			SetFontGlyphSet( unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags );
	void			GetTextSize( unsigned long font, const wchar_t *text, int &wide, int &tall );
	void			DrawSetTextColour( int r, int g, int b, int a );
	void			DrawSetTextFont( unsigned long font );
	void			DrawSetTextPos( int x, int y );
	void			DrawPrintText( const wchar_t *text, int textLen );
};

extern CSurface gSurface;

#endif // SURFACE_H