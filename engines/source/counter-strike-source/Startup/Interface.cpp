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
//! \file	Interface.cpp
//! \brief 
//! \date 01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Interface.h"
#include "..\Utils\VMethod.h"
#include "..\Utils\PETools.h"
#include "..\Utils\CRC32.h"

////////////////////////////////////////////////////////////////////////////////
// FindListAddress
//! Find the address of the linked list pointer in an address range
//! 
//! \param start - 
//! \param out - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool FindListAddress( DWORD_PTR start, PDWORD_PTR out )
{
	BYTE* pbThis = ( BYTE* )start;
	DWORD_PTR dwJumpAddress = NULL, dwRealStart = NULL, dwPointer;

	/* First byte is usually a jump */
	/* Must be CSS ? */
	if( pbThis[ 0 ] == 0xE9 )
	{
		dwRealStart = start - 0x70;

		if( out )
		{
			memcpy( ( void* )&dwPointer, ( void* )( dwRealStart + 3 ), 4 );

			*out = dwPointer;
		}

		return true;		
	}
	else // CSGO ?
	{
		if( pbThis[ 4 ] == 0xE9 )
		{
			dwRealStart = start - 0x70;

			if( out )
			{
				memcpy( ( void* )&dwPointer, ( void* )( dwRealStart + 6 ), 4 );

				*out = dwPointer;
			}
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
// GetPointerByHash
//! Get an interface pointer from the engine
//! 
//! \param module - module to search in
//! \param hash - hash of the interface
//! \param out - pointer to store it in
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool GetPointerByHash( HMODULE module, DWORD_PTR hash, void** out )
{
	FARPROC pProc			= NULL;
	DWORD_PTR dwListAddress = NULL;

	if( !out )
		return false;

	pProc = Utils::PE::GetProcAddress( module, 0x8e46b8b4 );

	if( !pProc )
		return false;

	if( !FindListAddress( (DWORD_PTR)pProc, &dwListAddress ) )
		return false;

	Interface_t * pIFaceList = *( Interface_t** )( dwListAddress );

	if ( pIFaceList )
	{
		while( pIFaceList )
		{
			DWORD_PTR dwHash = CRC32( pIFaceList->szName, strlen( pIFaceList->szName ) );

			if( dwHash == hash )
			{
				if( out )
				{
					*out = (void*)((int (*)(void))pIFaceList->pFunc)( );
				}
				return true;
			}

			pIFaceList = pIFaceList->pNext;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// GetInterface
//! Get an interface and set up a vmethod
//! 
//! \param dwModule - base address of the module to search in
//! \param dwHash - 
//! \param pVMethod - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool GetInterface( DWORD_PTR dwModule, DWORD_PTR dwHash, CVMethod* pVMethod )
{
	void* pIFace = NULL;

	if( !GetPointerByHash( (HMODULE)dwModule, dwHash, &pIFace ) )
		return false;

	pVMethod->Init( (PDWORD_PTR*)pIFace );

	return true;
}