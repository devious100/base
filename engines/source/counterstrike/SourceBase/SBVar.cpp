

#include "SBVar.h"

CSBVar::CSBVar( const char* szName, const char* szDescription, CValue pValue )
{
	strcpy( m_name, szName );
	strcpy( m_desc, szDescription );

	m_value = pValue;

	Create( this );
}

bool CSBVar::IsCommand()
{
	return false;
}

CValue& CSBVar::GetValue()
{
	return m_value;
}