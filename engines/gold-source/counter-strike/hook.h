#pragma once

#include "main.h"

extern DWORD WINAPI dwHookMethod( LPVOID );

extern DWORD FindPattern(DWORD dwAddr,DWORD dwLen,BYTE *pbMask,char *szMask);

struct sound_hook_t
{
	DWORD dwSound;

	DWORD dwFindPattern(BYTE *Mask,char *szMask,DWORD Corr)
	{
		return FindPattern((DWORD)GetModuleHandleA("hw.dll"),(DWORD)0x122A0000,Mask,szMask)+Corr;
	}

}extern offs;