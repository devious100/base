

#ifndef SBCMD_H
#define SBCMD_H

#include "CmdBase.h"

class CCmdArgs;

typedef void (*pfnCommandFunc)( CCmdArgs* pCmd );

class CSBCmd : public CCmdBase
{
public:
	CSBCmd( const char* szName, const char* szDescription, pfnCommandFunc pFunc );
	CSBCmd( const char* szName, pfnCommandFunc pFunc );

	virtual bool	IsCommand();
	void			Execute( CCmdArgs* pCmd );

protected:
	pfnCommandFunc m_func;
};

#ifndef MAKE_COMMAND
	#define MAKE_COMMAND( name, description ) void FN_##name( CCmdArgs* pCmd );\
		CSBCmd cmd_##name( #name, description, FN_##name );\
		void FN_##name( CCmdArgs* pCmd )
#endif // MAKE_COMMAND

#endif // SBCMD_H