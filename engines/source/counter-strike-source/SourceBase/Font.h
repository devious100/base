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
//! \file	CmdBase.h
//! \brief  Font class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef FONT_H
#define FONT_H

#include "..\Include.h"
#include "ISBObject.h"

enum
{
	kFontFlag_NONE,
	kFontFlag_ITALIC		= 0x001,
	kFontFlag_UNDERLINE		= 0x002,
	kFontFlag_STRIKEOUT		= 0x004,
	kFontFlag_SYMBOL		= 0x008,
	kFontFlag_ANTIALIAS		= 0x010,
	kFontFlag_GAUSSIANBLUR	= 0x020,
	kFontFlag_ROTARY		= 0x040,
	kFontFlag_DROPSHADOW	= 0x080,
	kFontFlag_ADDITIVE		= 0x100,
	kFontFlag_OUTLINE		= 0x200,
	kFontFlag_CUSTOM		= 0x400,
};

enum
{
	kAlign_CenterV	= 0x1,
	kAlign_CenterH	= 0x2,
	kAlign_Left		= 0x4,
	kAlign_Right	= 0x8,
};

class CFont : public ISBObject
{
	friend class CSourceBase;

public:
	// inherited virtual destroyer
	virtual void Destroy();
	
	void PrintString( int x, int y, int r, int g, int b, int a, DWORD_PTR dwAlign, const char* szText, ... );
	int GetTextHeight() { return m_size; }
	
private:
	unsigned long	m_id;
	int				m_size;
};

#endif