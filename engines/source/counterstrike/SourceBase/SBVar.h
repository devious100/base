

#ifndef SBVAR_H
#define SBVAR_H

#include "CmdBase.h"

enum kValueType
{
	kValue_NONE,
	kValue_INT,
	kValue_FLOAT,
	kValue_BOOL
};

class CValue
{
public:
	CValue()
	{
		memset( (void*)&(*this), 0, sizeof( CValue ) );
		m_type = kValue_NONE;
	}

	CValue( int iValue )
	{
		memset( (void*)&(*this), 0, sizeof( CValue ) );
		m_int = iValue;
		m_type = kValue_INT;
	}
	CValue( float flValue )
	{
		memset( (void*)&(*this), 0, sizeof( CValue ) );
		m_float = flValue;
		m_type = kValue_FLOAT;
	}
	CValue( bool bValue )
	{
		memset( (void*)&(*this), 0, sizeof( CValue ) );
		m_bool = bValue;
		m_type = kValue_BOOL;
	}

	union
	{
		float	m_float;
		int		m_int;
		bool	m_bool;
	};

	kValueType m_type;
};

class CSBVar : public CCmdBase
{
public:
	CSBVar( const char* szName, const char* szDescription, CValue pValue );

	virtual bool		IsCommand();

	CValue& GetValue();
	
private:
	CValue m_value;
};

#endif // SBVAR_H