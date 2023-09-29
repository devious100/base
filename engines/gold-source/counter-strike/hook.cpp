#include "main.h"

DynamicSound_orig oDynamicSound;
sound_hook_t offs;

bool DataCmp(const BYTE *pbData,const BYTE *pbMask,const char *szMask)
{
	for(;*szMask;++szMask,++pbData,++pbMask)
		if(*szMask=='x' && *pbData!=*pbMask)
			return false;
	return (*szMask)==0;
}
DWORD FindPattern(DWORD dwAddr,DWORD dwLen,BYTE *pbMask,char *szMask)
{
	for(DWORD i=0;i<dwLen;i++)
		if(DataCmp((BYTE*)(dwAddr+i),pbMask,szMask))
			return (DWORD)(dwAddr+i);
	return 0;
}
DWORD WINAPI dwHookMethod( LPVOID )
{
	while(!FindWindowA("Valve001",NULL)){ Sleep( 50 ); }
	Sleep( 2000 );

	char *SoundSig="\x83\xEC\x00\xA1\x00\x00\x00\x00\x53\x55\x56\x85\xC0\x57\xC7\x44\x24\x00\x00\x00\x00\x00",
		 *SoundMask="xx?x????xxxxxxxxx????";

	DWORD dwExportPtr = (DWORD)GetModuleHandleA("hw.dll") + 0x182FF;
    
	DWORD dwSound = offs.dwFindPattern((BYTE*)SoundSig,SoundMask,0);

	//pExports = ( export_t* )*(PDWORD)((DWORD)dwExportPtr + 0x116 );
	pExports = (export_t*)*(DWORD*)((DWORD)(pExports) + 0x116);
	while(!pExports->Initialize || !pExports->HUD_GetStudioModelInterface ) { Sleep( 50 ); }
	pEngfuncs = ( cl_enginefunc_t* )*(PDWORD)((DWORD)*pExports->Initialize + 0x1C ); 
	pStudio = ( engine_studio_api_t* )*(PDWORD)((DWORD)*pExports->HUD_GetStudioModelInterface + 0x1A ); 
	while(!pEngfuncs->pfnHookEvent || !pStudio->Mem_Calloc ){ Sleep( 50 ); }
	
	offs.dwSound = dwSound;

	HookExportFuncs();
	HookEngine();
	HookStudio();

	return TRUE;
}