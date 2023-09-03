#ifndef _SDK_H_
#define _SDK_H_

#pragma comment ( lib, "d3d9.lib" )
#pragma comment ( lib, "d3dx9.lib" )

#define PROJECT_FEAR
#define _FINAL
#define INCLUDE_AS_ENUM

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <vector>
#include <fstream>
#include <istream>
#include <string.h>
#include <detours.h>
#include <Tlhelp32.h>

using namespace std;

//FROM SDK
#include "sdk/game/shared/debugnew.h"
//#include "sdk/game/shared/automessage.h"
#include "sdk/engine/sdk/inc/iltmessage.h"
//#include "sdk/engine/sdk/inc/ltbasedefs.h"
//#include "sdk/engine/sdk/inc/ltlink.h"
#include "sdk/game/clientshelldll/gameclientshell.h"
#include "sdk/game/clientshelldll/characterfx.h"
#include "sdk/game/clientshelldll/cmovemgr.h"
//#include "sdk/game/clientshelldll/sfxmgr.h"
#include "sdk/game/clientshelldll/clientweaponmgr.h"
#include "sdk/game/clientshelldll/camerafx.h"
//#include "sdk/game/shared/weapondb.h"
#include "sdk/game/clientshelldll/aimmagnetfx.h"
#include "sdk/game/clientshelldll/pickupitemfx.h"
#include "sdk/game/clientshelldll/playercamera.h"
#include "sdk/game/clientshelldll/targetmgr.h"
#include "sdk/Game/ClientShellDLL/InterfaceMgr.h"
#include "sdk/Game/ClientShellDLL/ClientWeapon.h"
#include "sdk/Game/ClientShellDLL/PlayerStats.h"
#include "sdk/Game/ClientShellDLL/clientinfomgr.h"
#include "sdk/Game/shared/gamemodemgr.h"
#include "sdk/Game/shared/CLTFileToILTInStream.h"
#include "sdk/engine/sdk/inc/ILTInStream.h"
#include "sdk/engine/sdk/inc/ILTfilemgr.h"
#include "sdk/libs/platform/ltlibraryloader.h"
#include "sdk/libs/platform/ltautocriticalsection.h"

//File locations
#define FILELOCATION "I:\\Documents and Settings\\Admin\\My Documents\\Visual Studio Projects\\FearHack\\Release\\Fear_log.txt"
#define SETTINGSLOCATION "I:\\Documents and Settings\\Admin\\My Documents\\Visual Studio Projects\\FearHack\\Release\\settings.ini"

//+windowed 1 : for window mode
//Forwarded Functions from DllMain
void __cdecl add_log(const char * fmt, ...);
DWORD dwWait(LPVOID lpArgs);
bool Patch_WriteMemory(void* dest, void* src, size_t NumBytes);
bool PATCH_ReadMemory (void* Address,void* Data,DWORD Length);

extern DWORD pClient;

//Forwarded from Aimbot.cpp
bool IsVisible ( LTVector pStart, HOBJECT m_hObject );

//Control Function
void StartHackLoop();

extern CGameClientShell*	g_pGameClientShell;
extern CInterfaceMgr*		g_pInterfaceMgr;
extern ILTClient*			g_pLTClient;
extern CSurfaceDB*			g_pSurfaceDB;
extern ModelsDB*			g_pModelsDB;
extern CWeaponDB*			g_pWeaponDB;
extern ILTCSBase*			g_pLTBase;
extern CPlayerMgr*			g_pPlayerMgr;
extern IDatabaseMgr*		g_pLTDatabase;
extern CPlayerStats*		g_pPlayerStats;
extern ILTCommon*			g_pCommonLT;
extern ILTModel*			g_pModelLT;

//Color Definitions
#define RED D3DCOLOR_RGBA(255, 0, 0, 255)
#define BLUE D3DCOLOR_RGBA(0, 0, 255, 255)
#define GREEN D3DCOLOR_RGBA(0, 255, 0, 255)
#define ORANGE D3DCOLOR_RGBA(255, 100, 0, 255)
#define BLACK D3DCOLOR_RGBA(0, 0, 0, 255)
#define WHITE D3DCOLOR_RGBA(255, 255, 255, 255)
#define MENU D3DCOLOR_RGBA(0, 50, 255, 120)
#define MENU2 D3DCOLOR_RGBA(0, 255, 255, 120)

//Bone Definitions
#define BONE_Pelvis "Pelvis" 
#define BONE_Torso "Torso"
#define BONE_Upper_torso "Upper_torso"
#define BONE_Neck "Neck"
#define BONE_Head "Head"
#define BONE_Face_Jaw "Face_Jaw"
#define BONE_Left_shoulder "Left_shoulder"
#define BONE_Left_armu "Left_armu"
#define BONE_Left_arml "Left_arml"
#define BONE_Left_hand "Left_hand"
#define BONE_Right_shoulder "Right_shoulder"
#define BONE_Right_armu "Right_armu"
#define BONE_Right_arml "Right_arml"
#define BONE_Right_hand "Right_hand"
#define BONE_Left_legu "Left_legu"
#define BONE_Left_legl "Left_legl"
#define BONE_Left_foot "Left_foot"
#define BONE_Right_legu "Right_legu"
#define BONE_Right_legl "Right_legl"
#define BONE_Right_foot "Right_foot"

#define DO_ONCE(doit)\
	static bool once = false;\
	if (once == false)\
{\
	once = true;\
	doit }\

//FROM PROJECT
#include "fear.h"
#include "d3ddev.h"
#include "aimbot.h"
#include "esp.h"
#include "menu.h"
#include "draw.h"

#endif