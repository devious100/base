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
//! \file	PETools.cpp
//! \brief  A collection of useful PE functions that avoid using Win32 API
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "PETools.h"
#include "CRC32.h"

////////////////////////////////////////////////////////////////////////////////
// Utils::PE::GetPEB
//! 
//! \return PPEB - 
////////////////////////////////////////////////////////////////////////////////
PPEB Utils::PE::GetPEB( )
{
    __asm
	{
        mov eax, fs:[0x30]
    }
}

////////////////////////////////////////////////////////////////////////////////
// Utils::PE::GetNTHeader
//! 
//! \param module - 
//! \param hdr - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool Utils::PE::GetNTHeader( DWORD_PTR module, PIMAGE_NT_HEADERS* hdr )
{
	PIMAGE_DOS_HEADER		pDosHdr;
    PIMAGE_NT_HEADERS		pNTHdr;

    pDosHdr = (PIMAGE_DOS_HEADER)module;

	if( !pDosHdr )
		return false;
    
    if( pDosHdr->e_magic != IMAGE_DOS_SIGNATURE )
        return false;

    pNTHdr = (PIMAGE_NT_HEADERS)( module + pDosHdr->e_lfanew );
    
    if( pNTHdr->Signature != IMAGE_NT_SIGNATURE )
        return false;

	if( hdr )
		*hdr = pNTHdr;

	return true;
}

HMODULE Utils::PE::GetModuleHandle( DWORD_PTR hash )
{
	PPEB pPEB	= NULL;
	PLDR_MODULE module;
	PLIST_ENTRY entry;
	PLIST_ENTRY InLoadOrderModuleList;

	pPEB = GetPEB( );

	if( pPEB != NULL )
	{
		InLoadOrderModuleList = &pPEB->LoaderData->InLoadOrderModuleList; 

		for (entry = InLoadOrderModuleList->Flink; entry != InLoadOrderModuleList; entry = entry->Flink)
		{ 
			module = CONTAINING_RECORD(entry, LDR_MODULE, InLoadOrderModuleList);

			DWORD_PTR dwHash = CRC32( (void*)module->BaseDllName.Buffer, module->BaseDllName.Length );

			if( hash == dwHash )
			{
				return (HMODULE) module->BaseAddress;
			}
		}
	} 

	return (HMODULE) NULL;
}

////////////////////////////////////////////////////////////////////////////////
// Utils::GetProcAddress
//! 
//! \param module - 
//! \param hash - 
//! \return FARPROC - 
////////////////////////////////////////////////////////////////////////////////
FARPROC Utils::PE::GetProcAddress( HMODULE module, DWORD_PTR hash ) 
{
    WORD i;
    DWORD modulebase;
    
    PIMAGE_NT_HEADERS nthdr;
    PIMAGE_OPTIONAL_HEADER opthdr;
    PIMAGE_DATA_DIRECTORY datadir;
    PIMAGE_EXPORT_DIRECTORY exportdir;
    
    PWORD  ordtable;
    PDWORD addrtable;
    PDWORD nametable;
    
    char *procname;
    
    modulebase = (DWORD) module;

	if( !GetNTHeader( modulebase, &nthdr ) )
		return NULL;
        
    opthdr = &nthdr->OptionalHeader;
    datadir = &opthdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    exportdir = (PIMAGE_EXPORT_DIRECTORY) (modulebase + datadir->VirtualAddress);

    ordtable = (PWORD)  (modulebase + (DWORD) exportdir->AddressOfNameOrdinals);
    addrtable = (PDWORD) (modulebase + (DWORD) exportdir->AddressOfFunctions); 
    nametable = (PDWORD) (modulebase + (DWORD) exportdir->AddressOfNames); 
	
    for( i = 0; i < exportdir->NumberOfNames; i++ )
	{
        procname = (char *) (modulebase + nametable[i]);

		if( !procname )
			continue;

		DWORD_PTR dwHash = CRC32( (void *)procname, strlen( procname ) );

        if( hash == dwHash ) 
		{
            return (FARPROC) (modulebase + addrtable[ (WORD) (modulebase + ordtable[i]) ] );  
		}
    }

    return (FARPROC) NULL;
}