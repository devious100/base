

#ifndef VMETHOD_H
#define VMETHOD_H

#include "..\include.h"

class CVMethod
{
public:
	CVMethod( )
	{
		m_size			= 0;
		m_classPtr		= NULL;
		m_oldClassPtr	= NULL;
		m_vmtPtr		= NULL;
		m_newClassPtr	= NULL;
	}

	virtual ~CVMethod( )
	{
		Destroy( );
	}

	BOOL IsReady( )
	{
		return ( m_classPtr && m_oldClassPtr && m_vmtPtr && m_newClassPtr );
	}

	BOOL operator !( )
	{
		return !IsReady( );
	}

	UINT GetTableLength( )
	{
		UINT iIndex = 0;

		for ( ; m_vmtPtr[ iIndex ]; iIndex++ )
		{
			if ( IsBadCodePtr( ( FARPROC )m_vmtPtr[ iIndex ] ) )
			{
				break;
			}
		}
		return iIndex;
	}

	VOID Init( PDWORD_PTR* ppdwClassBase )
	{
		m_classPtr		= ppdwClassBase;
		m_oldClassPtr	= (PDWORD_PTR)m_classPtr;
		m_vmtPtr		= *m_classPtr;

		m_size = GetTableLength( );

		if( m_size <= 0 )
			return;

		m_newClassPtr = ( PDWORD_PTR )malloc( sizeof( DWORD_PTR ) * m_size );

		memcpy( m_newClassPtr, m_vmtPtr, sizeof( DWORD_PTR ) * m_size );

		*m_classPtr = m_newClassPtr;
	}

	PVOID GetInstance( )
	{
		return (PVOID)m_oldClassPtr;
	}

	VOID Destroy( )
	{
		if( m_newClassPtr )
		{
			free( m_newClassPtr );
		}
	}

	template <typename T> T GetMethod( UINT iIndex )
	{
		if( iIndex < 0 || iIndex >= m_size )
			return NULL;

		return ( T )m_vmtPtr[ iIndex ];
	}

	DWORD_PTR HookMethod( UINT iIndex, DWORD_PTR dwHookAddress )
	{
		if( iIndex < 0 || iIndex >= m_size )
			return NULL;

		DWORD_PTR dwOrig = m_newClassPtr[ iIndex ];

		m_newClassPtr[ iIndex ] = dwHookAddress;

		return dwOrig;
	}

private:
	PDWORD_PTR* m_classPtr;
	PDWORD_PTR	m_newClassPtr;
	PDWORD_PTR	m_vmtPtr;
	PDWORD_PTR	m_oldClassPtr;
	UINT		m_size;
};

#endif