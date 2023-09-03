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
//! \file	NVManager.cpp
//! \brief  NetVar manager supplies a method of getting offsets and hooking props
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "NVmanager.h"
#include "..\Utils\CRC32.h"
#include "..\SourceBase\SourceBase.h"

CNVManager gNVManager;

////////////////////////////////////////////////////////////////////////////////
// CNVManager::DumpAllClasses
//! dump all classes
//! 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CNVManager::DumpAllClasses( )
{
	typedef ClientClass* (__stdcall *tGetAllClasses)( void );
	ClientClass* pClass = g_pClient.GetMethod< tGetAllClasses >( 8 )( );

	if( !pClass )
		return;

	for( ; pClass; pClass = pClass->m_pNext )
		DumpClass( pClass, pClass->m_pRecvTable, 0 );
}

////////////////////////////////////////////////////////////////////////////////
// CNVManager::DumpClass
//! 
//! \param *pClass - 
//! \param *pTable - 
//! \param tab - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CNVManager::DumpClass( ClientClass *pClass, RecvTable *pTable, int tab )
{
	RecvProp *pProp = NULL;

	if ( pTable->m_nProps < 0 )
		return;

	char pad[1024] = "";

	for( int j = 0; j < tab; j++ )
		strcat( pad, "\t" );

	LOG( "%s%s", pad, pTable->m_pNetTableName );

	for( int i = 0; i < pTable->m_nProps; i++ )
	{		
		pProp = &pTable->m_pProps[i];

		if( !pProp )
			continue;		

		char szHashBuf[256];
		sprintf( szHashBuf, "%s%s", pTable->m_pNetTableName, pProp->m_pVarName );

		DWORD_PTR dwHash = CRC32( ( void* )szHashBuf, strlen ( szHashBuf ) );

		LOG( "%s%s 0x%X [0x%X]", pad, pProp->m_pVarName, pProp->m_Offset, dwHash );

		if ( pProp->m_pVarName[0] == 0x30 )
		{
			if ( pProp->m_pDataTable )
				DumpClass( pClass, pProp->m_pDataTable, tab+1 );
			else
				continue;
		}

		if ( pProp->m_pDataTable )
			DumpClass( pClass, pProp->m_pDataTable, tab+1 );
		else
			continue;
	}
}

////////////////////////////////////////////////////////////////////////////////
// CNVManager::GetOffset
//! 
//! \param dwHash - 
//! \return DWORD_PTR - 
////////////////////////////////////////////////////////////////////////////////
DWORD_PTR CNVManager::GetOffset( DWORD_PTR dwHash )
{
	typedef ClientClass* (__stdcall *tGetAllClasses)( void );
	ClientClass* pClass = g_pClient.GetMethod< tGetAllClasses >( 8 )( );

	if( !pClass )
		return NULL;

	for( ; pClass; pClass = pClass->m_pNext )
	{
		DWORD_PTR dwOffset = FindOffset( dwHash, pClass, pClass->m_pRecvTable );

		if( dwOffset != NULL )
			return dwOffset;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
// CNVManager::HookNetVar
//! Hook a proxy function
//! 
//! \param dwHash - 
//! \param pHook - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool CNVManager::HookNetVar( DWORD_PTR dwHash, RecvVarProxyFn pHook )
{
	typedef ClientClass* (__stdcall *tGetAllClasses)( void );
	ClientClass* pClass = g_pClient.GetMethod< tGetAllClasses >( 8 )( );

	if( !pClass )
		return NULL;

	for( ; pClass; pClass = pClass->m_pNext )
	{
		bool bResult = HookProp( dwHash, pClass, pClass->m_pRecvTable, pHook );

		if( bResult )
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// CNVManager::FindNetVar
//! Iterate the table to point to our own table
//! 
//! \param dwHash - 
//! \param pClass - 
//! \param pTable - 
//! \return RecvProp* - 
////////////////////////////////////////////////////////////////////////////////
bool CNVManager::HookProp( DWORD_PTR dwHash, ClientClass* pClass, RecvTable* pTable, RecvVarProxyFn pHook )
{
	RecvProp *pProp = NULL;
	bool bResult = false;

	if ( pTable->m_nProps < 0 )
		return false;

	for( int i = 0; i < pTable->m_nProps; i++ )
	{		
		pProp = &pTable->m_pProps[i];

		if( !pProp )
			continue;		

		char szHashBuf[256];
		sprintf( szHashBuf, "%s%s", pTable->m_pNetTableName, pProp->m_pVarName );

		DWORD_PTR dwThisHash = CRC32( ( void* )szHashBuf, strlen ( szHashBuf ) );

		if( dwHash == dwThisHash )
		{
			pProp->m_ProxyFn = pHook;
			return true;
		}

		if ( pProp->m_pVarName[0] == 0x30 )
		{
			if ( pProp->m_pDataTable )
			{
				bResult = HookProp( dwHash, pClass, pProp->m_pDataTable, pHook );

				if( bResult )
					return true;
			}
			else
				continue;
		}

		if ( pProp->m_pDataTable )
		{
			bResult = HookProp( dwHash, pClass, pProp->m_pDataTable, pHook );

			if( bResult )
				return true;
		}
		else
			continue;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
// CNVManager::FindOffset
//! 
//! \param *pClass - 
//! \param *pTable - 
//! \return DWORD_PTR - 
////////////////////////////////////////////////////////////////////////////////
DWORD_PTR CNVManager::FindOffset( DWORD_PTR dwHash, ClientClass* pClass, RecvTable* pTable )
{
	RecvProp *pProp = NULL;

	DWORD_PTR dwRet = NULL;

	if ( pTable->m_nProps < 0 )
		return NULL;

	for( int i = 0; i < pTable->m_nProps; i++ )
	{		
		pProp = &pTable->m_pProps[i];

		if( !pProp )
			continue;		

		char szHashBuf[256];
		sprintf( szHashBuf, "%s%s", pTable->m_pNetTableName, pProp->m_pVarName );

		DWORD_PTR dwThisHash = CRC32( ( void* )szHashBuf, strlen ( szHashBuf ) );

		if( dwHash == dwThisHash )
		{
			#if SB_DEBUG
				LOG( "Found netvar [%s::%s] Offset [0x%x]", pTable->m_pNetTableName, pProp->m_pVarName, pProp->m_Offset );
			#endif

			return pProp->m_Offset;
		}

		if ( pProp->m_pVarName[0] == 0x30 )
		{
			if ( pProp->m_pDataTable )
			{
				dwRet = FindOffset( dwHash, pClass, pProp->m_pDataTable );

				if( dwRet != NULL )
					return dwRet;
			}
			else
				continue;
		}

		if ( pProp->m_pDataTable )
		{
			dwRet = FindOffset( dwHash, pClass, pProp->m_pDataTable );

			if( dwRet != NULL )
				return dwRet;
		}
		else
			continue;
	}

	return dwRet;
}