#include "main.h"

engine_studio_api_t* pStudio = NULL;
engine_studio_api_t gStudio;

void __cdecl StudioEntityLight( alight_s*pLight )
{
	gStudio.StudioEntityLight( pLight );

	cl_entity_s*pEnt = gStudio.GetCurrentEntity();

	if( pEnt->origin )
	{
		pLight->ambientlight = 255;
		pLight->color[0] = 255;
		pLight->color[1] = 255;
		pLight->color[2] = 255;
	}
}
void HookStudio( void )
{
	memcpy(&gStudio,pStudio,sizeof(engine_studio_api_t) );
    #define HSF(x) pStudio->##x=##x;

	HSF( StudioEntityLight )
}