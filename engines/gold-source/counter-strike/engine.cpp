#include "main.h"

cl_enginefunc_t* pEngfuncs = NULL;
cl_enginefunc_t gEngfuncs;

void HookEngine( void )
{
	memcpy(&gEngfuncs,pEngfuncs,sizeof( cl_enginefunc_t ) );
    #define HEF(x) pEngfuncs->##x=##x;

	HookUserMsgs();
}