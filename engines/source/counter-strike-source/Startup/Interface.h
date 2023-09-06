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
//! \file Interface.h
//! \brief 
//! \date 01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef INTERFACE_H
#define INTERFACE_H

#include "..\Include.h"
#include "..\Utils\VMethod.h"

typedef struct Interface_s
{
	void*			pFunc;
	char*			szName;
	Interface_s*	pNext;

} Interface_t;

bool GetInterface( DWORD_PTR dwModule, DWORD_PTR dwHash, CVMethod* pVMethod );

#endif // INTERFACE_H
