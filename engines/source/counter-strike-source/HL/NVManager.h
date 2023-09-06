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
//! \file	NVManager.h
//! \brief  NetVar manager supplies a method of getting offsets and hooking props
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef NVMANAGER_H
#define NVMANAGER_H

#include "..\Include.h"
#include "..\HL\SDK.h"

#ifndef MAKE_NETVAR
	#define MAKE_NETVAR( type, offset ) static DWORD_PTR dwOffset = gNVManager.GetOffset( offset );\
										DWORD_PTR dwThis = (DWORD_PTR)this;\
										return *( type* )( dwThis + dwOffset );
#endif

#ifndef MAKE_PNETVAR
	#define MAKE_PNETVAR( type, offset ) static DWORD_PTR dwOffset = gNVManager.GetOffset( offset );\
										 DWORD_PTR dwThis = (DWORD_PTR)this;\
										 return ( type )( dwThis + dwOffset );
#endif

class CNVManager
{
public:
	void		DumpAllClasses( );	
	DWORD_PTR	GetOffset( DWORD_PTR dwHash );
	bool		HookNetVar( DWORD_PTR dwHash, RecvVarProxyFn pHook );

private:
	void		DumpClass( ClientClass *pClass, RecvTable *pTable, int tab );
	bool		HookProp( DWORD_PTR dwHash, ClientClass* pClass, RecvTable* pTable, RecvVarProxyFn pHook );
	DWORD_PTR	FindOffset( DWORD_PTR dwHash, ClientClass* pClass, RecvTable* pTable );
};

extern CNVManager gNVManager;

#endif