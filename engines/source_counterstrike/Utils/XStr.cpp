////////////////////////////////////////////////////////////////////////////////
//! \file \Users\Cal\Documents\Visual Studio 2010\Projects\GUIv2\GUIv2\XStr.cpp
//! \brief String encryption class
//! \date 11 September 2011
//! \author Cal
//! Copyright Cal (Chod) MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "XStr.h"

////////////////////////////////////////////////////////////////////////////////
// XStr::XStr
//! Constructor
//! 
//! \param hashlen - length of the hash
//! \param len - string length
//! \param key - hash key
//! \param ... - strings to parse
//! \return - 
////////////////////////////////////////////////////////////////////////////////
__declspec( noinline ) XStr::XStr( int hashlen, int len, int key, ... )
{
	va_list vl;

	va_start( vl, key );

	m_hashLength	= hashlen;
	m_strLength		= len;
	m_key			= key;

	int nBytes = 0;
	int nDecryptedBytes = 0;

	m_charList = new char[ len ];

	for( int i = 0; i < m_hashLength; i++ )
	{
		DWORD_PTR dwHash = va_arg( vl, DWORD_PTR );

		for( int j = 0; j < 4; j++ )
		{
			WORD wWord = HIWORD( dwHash );

			if( j >= 2 ) 
				wWord = LOWORD( dwHash );
			
			switch( j )
			{
			case 0:
				m_charList[ nDecryptedBytes ] = HIBYTE( wWord );
				break;
			case 1:
				m_charList[ nDecryptedBytes ] = LOBYTE( wWord );
				break;
			case 2:
				m_charList[ nDecryptedBytes ] = HIBYTE( wWord );
				break;
			case 3:
				m_charList[ nDecryptedBytes ] = LOBYTE( wWord );
				break;				
			}

			nDecryptedBytes++;

			if( nDecryptedBytes >= len )
				break;
		}			
	}		

	va_end( vl );
}

////////////////////////////////////////////////////////////////////////////////
// XStr::~XStr
//! Destructor - cleans up all strings from memory
//! 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
XStr::~XStr( )
{
	delete [] m_retBuf;

	m_retBuf = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// XStr::c
//! Decrypt the hash and return it as a char array
//! 
//! \return char * - 
////////////////////////////////////////////////////////////////////////////////
__declspec( noinline ) char * XStr::c( )
{
	m_retBuf = new char[ m_strLength + 1 ];

	for( int i = 0; i < m_strLength; i++ )
	{
		m_retBuf[ i ] = (char)m_charList[ i ];

		m_retBuf[ i ] ^= m_key;
		m_key += 1;
		m_key %= 256;
	}

	m_retBuf[ m_strLength ] = '\0';

	delete [] m_charList;

	m_charList = NULL;

	return m_retBuf;
}

////////////////////////////////////////////////////////////////////////////////
// XStr::s
//! Return the decryted characters a string class
//! 
//! \return std::string - 
////////////////////////////////////////////////////////////////////////////////
std::string XStr::s( )
{
	return std::string( c( ) );
}

