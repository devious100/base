/*
	This is a hack for the game fear that I worked for about three weeks on and off.
	Got sick of the game so I decided to release it. Have fun with it, but if you decide
	to use it don't forget to credit me. People I would like to give credit to are
	DrUnKeN ChEeTaH for getting me involved in the fear hacking scene. Chod, for the
	info that he posted on GD. Finally K@N@VEL, for his help and ideas on the project.
																						*/

//#define _NO_ADD

#include "sdk.h"

DWORD pClient = NULL;

bool PATCH_ReadMemory (void* Address,void* Data,DWORD Length)
{
	DWORD OldProtect;
	if(!VirtualProtect(Address,Length,PAGE_READWRITE,&OldProtect))return false;
	memcpy(Data,Address,Length);
	return (BOOL)VirtualProtect(Address,Length,OldProtect,NULL);
}
bool Patch_WriteMemory(void* dest, void* src, size_t NumBytes)
{
	DWORD Protect;
	if(!VirtualProtect( dest, NumBytes, PAGE_EXECUTE_READWRITE, &Protect))return false;
	memcpy( dest, src, NumBytes);
	return (BOOL)VirtualProtect( dest, NumBytes, Protect, NULL);
}
void Hook()
{
	if(pClient)
	{
		
		if (g_pGameClientShell == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1A4FE8),&g_pGameClientShell,4);
			add_log("g_pGameClientShell = 0x%.8x", (DWORD)g_pGameClientShell);
		}
		if(g_pInterfaceMgr == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1ABEB0),&g_pInterfaceMgr,4);
			add_log("g_pInterfaceMgr = 0x%.8x", (DWORD)g_pInterfaceMgr);
		}
		if(g_pLTClient == NULL)
		{  
			PATCH_ReadMemory( (void*)(pClient+0x1A5418),&g_pLTClient,4);
			add_log("g_pLTClient = 0x%.8x", (DWORD)g_pLTClient);
		}
		if(g_pLTBase == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1AFA40),&g_pLTBase,4);
			add_log("g_pLTBase = 0x%.8x", (DWORD)g_pLTBase);
		}
		if(g_pPlayerMgr == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1ACAC4),&g_pPlayerMgr,4);
			add_log("g_pPlayerMgr = 0x%.8x", (DWORD)g_pPlayerMgr);
		}
		if(g_pSurfaceDB == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1AFAAC),&g_pSurfaceDB,4);
			add_log("g_pSurfaceDB = 0x%.8x", (DWORD)g_pSurfaceDB);
		}
		if(g_pWeaponDB == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1AFA48),&g_pWeaponDB,4);
			add_log("g_pWeaponDB = 0x%.8x", (DWORD)g_pWeaponDB);
		}
		if(g_pModelsDB == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1AFA90),&g_pModelsDB,4);
			add_log("g_pModelsDB = 0x%.8x", (DWORD)g_pModelsDB);
		}
		if(g_pLTDatabase == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1AFA30),&g_pLTDatabase,4);
			add_log("g_pLTDatabase = 0x%.8x", (DWORD)g_pLTDatabase);
		}
		if(g_pPlayerStats == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1ACF28),&g_pPlayerStats,4);
			add_log("g_pPlayerStats = 0x%.8x", (DWORD)g_pPlayerStats);
		}	
		if(g_pCommonLT == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1AFA44),&g_pCommonLT,4);
			add_log("g_pCommonLT = 0x%.8x", (DWORD)g_pCommonLT);
		}
		if(g_pModelLT == NULL)
		{
			PATCH_ReadMemory( (void*)(pClient+0x1AFA38),&g_pModelLT,4);
			add_log("g_pModelLT = 0x%.8x", (DWORD)g_pModelLT);
		}
		add_log("pClinet = 0x%.8x", pClient);
	}
	
}



DWORD dwWait(LPVOID lpArgs)
{
	
	while (pClient == NULL)
	{
		HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,NULL);
		if (SnapShot)
		{
			MODULEENTRY32 ME;
			ME.dwSize = sizeof(MODULEENTRY32);
			BOOL r = Module32First (SnapShot,&ME);
			while(r)
			{
				if(strstr((char*)&ME.szModule,"Gam") && strstr((char*)&ME.szModule,".tmp"))
				{
					pClient = (DWORD)ME.modBaseAddr;
					break;
				}
				r = Module32Next (SnapShot,&ME);
			}
			CloseHandle(SnapShot);
		}
		Sleep(100);
	} 
	
	Hook();
	
	return 0;
}

BYTE MousePatch[3] = {0xC2, 0x08, 0x00};
void SCP_Patch()
{
	HMODULE hMod = NULL;
	hMod = GetModuleHandle("USER32.dll");
	if(hMod)
	{
		DWORD dwSCP = NULL;
		dwSCP =	(DWORD)GetProcAddress(hMod, "SetCursorPos");
		if(dwSCP)
		{
			DWORD dwProtect;
			VirtualProtect((void*)dwSCP, 3, PAGE_EXECUTE_READWRITE, &dwProtect);
			memcpy((void*)dwSCP, &MousePatch, 3);
			VirtualProtect((void*)dwSCP, 3, dwProtect, &dwProtect);
			add_log("mouse patched");
		}
	}

}

bool WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{		
		DeleteFile(FILELOCATION);
		FEAR.m_pD3D->StartHooks();
		SCP_Patch();
		return true;		
	}
	
	return false;
}

void __cdecl add_log(const char * fmt, ...)
{
#ifndef _NO_ADD
	va_list va_alist;
	char logbuf[256];
	FILE * fp;
	struct tm * current_tm;
	time_t current_time;

	time (&current_time);
	current_tm = localtime (&current_time);

	sprintf (logbuf, "[%02d:%02d:%02d] ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);

	va_start (va_alist, fmt);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	if ( (fp = fopen ( FILELOCATION , "a")) != NULL )
	{
		fprintf ( fp, "%s\n", logbuf );
		fclose (fp);
	}
#endif _NO_ADD
}