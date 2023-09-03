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
//! \file	PETools.h
//! \brief  A collection of useful PE functions that avoid using Win32 API
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef PETOOLS_H
#define PETOOLS_H

#include "..\Include.h"
#include "peb.h"

namespace Utils
{
	namespace PE
	{	
		PPEB	GetPEB( );
		FARPROC GetProcAddress( HMODULE module, DWORD_PTR hash );
		HMODULE GetModuleHandle( DWORD_PTR hash );
		bool	GetNTHeader( DWORD_PTR module, PIMAGE_NT_HEADERS* hdr );
	};
};

#endif // PETOOLS_H