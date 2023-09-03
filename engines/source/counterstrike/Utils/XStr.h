////////////////////////////////////////////////////////////////////////////////
//! \file \Users\Cal\Documents\Visual Studio 2010\Projects\GUIv2\GUIv2\XStr.h
//! \brief String encryption class
//! \date 11 September 2011
//! \author Cal
//! Copyright Cal (Chod) MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef XSTR_H
#define XSTR_H

#include "..\include.h"

class XStr
{
public:
	XStr( int hashlen, int strlen, int key, ... );
	virtual ~XStr( );

public:
	char *		c( );
	std::string s( );

private:
	char *	m_charList;
	char *	m_retBuf;
	int		m_strLength;
	int		m_hashLength;
	int		m_key;
};

#endif

