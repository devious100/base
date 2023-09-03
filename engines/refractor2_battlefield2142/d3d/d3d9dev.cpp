/*	Direct3D9 Device */

#include <windows.h>
#include <time.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "timer.h"
#include "main.h"
#include "d3d9.h"
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


// disable annoying warnings
#pragma warning (disable: 4312) //conversion from 'unsigned long' to 'unsigned long *' of greater size
#pragma warning (disable: 4311) //pointer truncation from 'HRESULT
#pragma warning (disable: 4099) //disable debug warning (detours.pdb)
#pragma warning (disable: 4244) //disable conversion from 'DWORD' to 'FLOAT', possible loss of data (crosshair function)
#pragma warning (disable: 4996)

unsigned long RendDx9 = 0; // used to get module base address of RendDx9.dll 
//---------ViewPort Declares---------------------------------------------------------------------------------------
D3DVIEWPORT9 Viewport;


//------some random declares---------------------------------------------------------------------------------------
unsigned int m_Stride; //global variable for model recognition strides
#define D3DHOOK_TEXTURES //for texture hooking

/////////Memory Hack Global Variables//////////////////////////////////////////////////////////////////////////////

	HANDLE pFile; 
	DWORD pid; 
	HWND hwndWindow; 
	DWORD bytes; 
	HANDLE hand = NULL;
	DWORD RendDx9Base;
	DWORD dwBF2142Base;

//Time Viewer
char Timestruct[16] = "hh':'mm':'ss tt";      
char TimeString[25];  

//drawing sprites
LPDIRECT3DTEXTURE9 oTexture = NULL;
LPD3DXSPRITE oSprite = NULL;


//d3d custom crosshair variables
  float ScreenCenterX = 0.0f;//Horizontal Position
  float ScreenCenterY = 0.0f;//Vertical Position
  D3DCOLOR redt = D3DCOLOR_XRGB( 255, 0, 0 );

//D3D Hack Functions
bool Texture;
bool Wireframe;//wireframed world
bool Asuswallhack;//blendable textures
bool NoScope;//removes scope overlay
bool PlayerChams;//player CHAMS
bool VehicleChams;//vehicle CHAMS
bool MiscChams; //all kind of CHAMS
bool Xplosives; //explosives CHAMS
bool WeaponChams;//weapon CHAMS
bool ColoredSky;//colored skydome
bool Time;
bool View;
bool Xhair;
bool Nobreathing;
bool NoFog;
bool NoWater;
bool Minimap;
bool Nametags;
bool MeterTags;
bool bodyESP;
bool classESP;
bool veteranESP;
bool NoRain;
bool Assets;
bool ConsoleUnlock;
bool EMPblur;
bool DirectionIcons;
bool DeployESP;
bool Spam;
bool Votekick;
bool Health;
bool Spoofer;
bool LockXhair;
bool SpinHack;
bool Brightness;

	////Global Variables
	//test hack
	DWORD dwProtect;
	//viewdistance
	DWORD dwPreView;
	float Viewdistanceread = 1.0f;
	float Viewdistance = 2.0f;

	//unlimited commander toys
	BYTE Toys1On[4] = {0xC6,0x40,0x5C,0x00}; //filled up
	BYTE Toys2On[4] = {0xC6,0x46,0x44,0x00};
	BYTE Toys3On[4] = {0xC6,0x46,0x5C,0x00};

	BYTE Toys1Off[4] = {0xC6,0x40,0x5C,0x01};//not filled
	BYTE Toys2Off[4] = {0xC6,0x46,0x44,0x01};
	BYTE Toys3Off[4] = {0xC6,0x46,0x5C,0x01};

	//rain removal
	BYTE RainOn[4] = {0x80,0x7F,0x64,0x01};
	BYTE RainOff[4] = {0x80,0x7F,0x64,0x00};
	
	//minimap hack
	BYTE Minimap1[2] = {0x74,0x09};
	BYTE Minimap2[2] = {0x74,0x06};
	BYTE Minimap3[2] = {0x74,0x09};

	BYTE Minimap1Off[2] = {0x75,0x09};
	BYTE Minimap2Off[2] = {0x75,0x06};
	BYTE Minimap3Off[2] = {0x75,0x09};

	//no breathing 
	BYTE NoBreathing[3] = {0x8B,0x7D,0x00};
	BYTE NoBreathingOff[3] = {0x8B,0x7D,0x10};

	//lock crosshair
	BYTE LockX1[3] = {0xD8,0x4E,0x28};
	BYTE LockX2[3] = {0xD8,0x4E,0x24};
	BYTE LockX3[3] = {0xD8,0x4E,0x08};

	BYTE LockX1Orig[3] = {0xD8,0x46,0x28};
	BYTE LockX2Orig[3] = {0xD8,0x46,0x24};
	BYTE LockX3Orig[3] = {0xD8,0x46,0x08};

	//nametags ESP
	BYTE nametagson[5] = {0xE9,0x5C,0x01,0x00,0x00};
	BYTE nametagsoff[6] = {0x0F,0x84,0x5B,0x01,0x00,0x00};

	//nametags distance indicators ESP
	BYTE tagsdistanceon[2] = {0x75,0x6A};
	BYTE tagsdistanceoff[2] = {0x74,0x6A};	

	//3dicons ESP
	BYTE icons[2] = {0x74,0x10};
	BYTE iconsOff[2] = {0x75,0x10};

	//Deploys ESP
	BYTE deployESP[2] = {0x74,0x13};
	BYTE deployESPoff[2] = {0x75,0x13};

	//Health ESP
	BYTE healthESP[6] = {0x90,0x90,0x90,0x90,0x90,0x90};

	BYTE healthESPOff[6] = {0x0F,0x84,0xF3,0x04,0x00,0x00};
	BYTE healthESP2Off[6] = {0x0F,0x84,0xDF,0x01,0x00,0x00};

	//class ESP
	BYTE classtags1[2] = {0x74,0x17};
	BYTE classtags2[2] = {0x74,0x18};
	BYTE classtags3[6] = {0x0F,0x84,0xA6,0x00,0x00,0x00};
	BYTE classtags4[2] = {0x74,0x10};

	BYTE classtags1Off[2] = {0x75,0x17};
	BYTE classtags2Off[2] = {0x75,0x18};
	BYTE classtags3Off[6] = {0x0F,0x85,0xA6,0x00,0x00,0x00};
	BYTE classtags4Off[2] = {0x75,0x10};

	//body ESP
	BYTE bodyTags[2] = {0x74,0x52};
	BYTE bodyTagsOff[2] = {0x75,0x52};

	//BF2 veteran tags
	BYTE veteranTags[2] = {0x75,0x09};
	BYTE veteranTagsOff[2] = {0x74,0x09};

	//unlock console
	BYTE Console[2] = {0x74,0x1E};
	BYTE ConsoleOff[2] = {0x75,0x1E};

	//no emp blur
	BYTE EMPon[6] = {0x0F,0x85,0x2B,0x01,0x00,0x00};
	BYTE EMPoff[6] = {0x0F,0x8E,0x2B,0x01,0x00,0x00};

	//taunt spam
	BYTE TauntSpam[6] = {0x90,0x90,0x90,0x90,0x90,0x90};
	BYTE TauntSpamOff[6] = {0x89,0x81,0x7C,0x01,0x00,0x00};

	//votekick bypass
	BYTE votekick[3] = {0x90,0x90,0x90};
	BYTE votekickOff[3] = {0xD9,0x46,0x58};

	//spinhack
	BYTE spinhack[5] = {0xA3,0x3C,0x30,0x28,0x43};
	BYTE spinhackOff[3] = {0x23,0x3C,0x30};

	//bright structures
	DWORD dwPreBright;
	float Bright = 2.3f;
	float BrightOff = 1.0f;

//-----------Model recognition list---------------------------------------------------------------------------------

//Sky
#define Sky ((m_Stride == 32) && (NumVertices == 667 && primCount == 1120 && startIndex == 0))

//water
#define Water (NumVertices == 4225 && primCount == 8192)

//Scopes
#define Scope ((NumVertices == 301 && primCount == 450 && startIndex == 24867) || (NumVertices == 154 && primCount == 154 && startIndex == 19005) || (NumVertices == 229 && primCount == 290) || (NumVertices == 2393 && primCount == 1943 && startIndex == 16653) || (NumVertices == 1506 && primCount == 2292 && startIndex == 12675) || (NumVertices == 1488 && primCount == 1447 && startIndex == 15090) || (NumVertices == 1676 && primCount == 1289 && startIndex == 16149) || (NumVertices == 1568 && primCount == 1686 && startIndex == 20016) || (NumVertices == 250 && primCount == 288 && startIndex == 13833) || (NumVertices == 64 && primCount == 48) || (NumVertices == 1014 && primCount == 1064 && startIndex == 22332) || (NumVertices == 1015 && primCount == 813) || (NumVertices == 312 && primCount == 312 && startIndex == 18816) || (NumVertices == 2312 && primCount == 1782 && startIndex == 14787) || (NumVertices == 148 && primCount == 769) || (NumVertices == 359 && primCount == 186 && startIndex == 15885) || (NumVertices == 2723 && primCount == 1906 && startIndex == 16443) || (NumVertices == 3818 && primCount == 2137 && startIndex == 13206) || (NumVertices == 1627 && primCount == 1723 && startIndex == 19617) || (NumVertices == 148 && primCount == 76) || (NumVertices == 12 && primCount == 8) || (NumVertices == 209 && primCount == 150) || (NumVertices == 1007 && primCount == 786) || (NumVertices == 658 && primCount == 896 && startIndex == 19536) || (NumVertices == 181 && primCount == 210) || (NumVertices == 1892 && primCount == 1281 && startIndex == 13131) || (NumVertices == 2216 && primCount == 2039 && startIndex == 16974) || (NumVertices == 244 && primCount == 220) || (NumVertices == 3333 && primCount == 2830 && startIndex == 17436) || (NumVertices == 240 && primCount == 240) || (NumVertices == 54 && primCount == 54 && startIndex == 25302) || (NumVertices == 402 && primCount == 700) || (NumVertices == 312 && primCount == 312 && startIndex == 47025) || (NumVertices == 629 && primCount == 470) || (NumVertices == 2424 && primCount == 3432 && startIndex == 33954) || (NumVertices == 8 && primCount == 4) || (NumVertices == 262 && primCount == 244) || (NumVertices == 1488 && primCount == 1447 && startIndex == 18360) || (NumVertices == 2424 && primCount == 3432 && startIndex == 27807) ||(NumVertices == 1786 && primCount == 1694) || ((NumVertices == 212 && primCount == 106) || (NumVertices == 32 && primCount == 16) || (NumVertices == 2393 && primCount == 1943) || (NumVertices == 574 && primCount == 284) || (NumVertices == 1014 && primCount == 1064) || (NumVertices == 61 && primCount == 90)))

//Tankoverlay(APC too )
#define TankOverlay ((m_Stride == 24) && (NumVertices == 11406 && primCount == 7811) || (NumVertices == 12 && primCount == 4) || (NumVertices == 4793 && primCount == 3811) || (NumVertices == 3056 && primCount == 2235) || (NumVertices == 1658 && primCount == 910) || (NumVertices == 7466 && primCount == 4749) || (NumVertices == 165 && primCount == 188) || (NumVertices == 434 && primCount == 344) || (NumVertices == 3448 && primCount == 2302) || (NumVertices == 5843 && primCount == 4025))

//Deploys(centry gun, ammo- ,healthhubs, and some other stuff)
#define Deploys ((m_Stride == 24) && (NumVertices == 3382 && primCount == 2611 && startIndex == 0) || (NumVertices == 682 && primCount == 361 && startIndex == 3) || (NumVertices == 5354 && primCount == 4150 && startIndex == 0) || (NumVertices == 631 && primCount == 328 && startIndex == 3) || (NumVertices == 1084 && primCount == 771 && startIndex == 3) || (NumVertices == 2441 && primCount == 1627 && startIndex == 3) || (NumVertices == 6228 && primCount == 4126 && startIndex == 3) || (NumVertices == 6173 && primCount == 4133) || (NumVertices == 566 && primCount == 299)|| (NumVertices == 4554 && primCount == 3502) || (NumVertices == 682 && primCount == 379) || (NumVertices == 6116 && primCount == 4098) || (NumVertices == 4154 && primCount == 3258) || (NumVertices == 9440 && primCount == 8004) || (NumVertices == 3815 && primCount == 3049) || (NumVertices == 3040 && primCount == 2319) || (NumVertices == 4173 && primCount == 2922))

//Hand Grenades
#define Grenades ((m_Stride == 24) && (NumVertices == 2972 && primCount == 2494))

//APM (Anti Personnel mine)
#define APM ((m_Stride == 24) && (NumVertices == 250 && primCount == 288) || (NumVertices == 2972 && primCount == 2494) || (NumVertices == 122 && primCount == 48) || (NumVertices == 3818 && primCount == 2708))

//RDX DemoPak
#define RDX ((m_Stride == 24) && (NumVertices == 279 && primCount == 144) || (NumVertices == 2558 && primCount == 1980))

//A12 Enforcer Sentry Gun
#define Sentry ((m_Stride == 24) && (NumVertices == 682 && primCount == 361))

//IPS (Infantry Protection Shield)
#define IPS ((m_Stride == 24) && (NumVertices == 6228 && primCount == 4126))

//EMP mine
#define EMP ((m_Stride == 24) && (NumVertices == 331 && primCount == 150))

//EU player
#define Eu ((m_Stride == 52) && ((NumVertices == 524 && primCount == 606 && startIndex == 0) || (NumVertices == 1101 && primCount == 1278 && startIndex == 0) || (NumVertices == 1268 && primCount == 922 && startIndex == 7656) || (NumVertices == 1026 && primCount == 810 && startIndex == 7656) || (NumVertices == 337 && primCount == 452 && startIndex == 10422) || (NumVertices == 1536 && primCount == 1088 && startIndex == 0) || (NumVertices == 351 && primCount == 244 && startIndex == 11778) || (NumVertices == 1440 && primCount == 1064 && startIndex == 4707) || (NumVertices == 1537 && primCount == 1062 && startIndex == 14304) || (NumVertices == 1709 && primCount == 1056 && startIndex == 9459) || (NumVertices == 340 && primCount == 258 && startIndex == 32460) || (NumVertices == 340 && primCount == 258 && startIndex == 29268) || (NumVertices == 586 && primCount == 366 && startIndex == 31152) || (NumVertices == 334 && primCount == 262 && startIndex == 30204) || (NumVertices == 6173 && primCount == 4133) || (NumVertices == 1095 && primCount == 1278) || (NumVertices == 990 && primCount == 1274)|| (NumVertices == 337 && primCount == 452) || (NumVertices == 496 && primCount == 286) || (NumVertices == 496 && primCount == 286) || (NumVertices == 2810 && primCount == 2652) || (NumVertices == 1268 && primCount == 922) || (NumVertices == 414 && primCount == 258) || (NumVertices == 75 && primCount == 78) || (NumVertices == 355 && primCount == 244) || (NumVertices == 450 && primCount == 506)))

//Pac players
#define Pac ((m_Stride == 52) && ((NumVertices == 1231 && primCount == 1278) || (NumVertices == 1726 && primCount == 1083 && startIndex == 7656) || (NumVertices == 1491 && primCount == 1034 && startIndex == 7656) || (NumVertices == 384 && primCount == 344 && startIndex == 10905) || (NumVertices == 1264 && primCount == 1008 && startIndex == 0) || (NumVertices == 312 && primCount == 188 && startIndex == 11937) || (NumVertices == 1234 && primCount == 986 && startIndex == 4482) || (NumVertices == 1209 && primCount == 928 && startIndex == 13332) || (NumVertices == 1466 && primCount == 976 && startIndex == 8832) || (NumVertices == 261 && primCount == 260 && startIndex == 25854) || (NumVertices == 261 && primCount == 260 && startIndex == 22560) || (NumVertices == 551 && primCount == 324 && startIndex == 24690) || (NumVertices == 300 && primCount == 302 && startIndex == 23574) || (NumVertices == 993 && primCount == 1274) || (NumVertices == 1726 && primCount == 1083)|| ( NumVertices == 513 && primCount == 287) || (NumVertices == 3715 && primCount == 2703) || (NumVertices == 404 && primCount == 280) || (NumVertices == 73 && primCount == 64) || NumVertices == 404 && primCount == 280) || (NumVertices == 281 && primCount == 370))

//Commanderassets
#define Commanderassets ((m_Stride == 24) && (NumVertices == 1401 && primCount == 666 && startIndex == 3) || (NumVertices == 2700 && primCount == 1753 && startIndex == 3) || (NumVertices == 1614 && primCount == 1020 && startIndex == 3) || (NumVertices == 1334 && primCount == 841 && startIndex == 3) || (NumVertices == 1303 && primCount == 916) || (NumVertices == 1303 && primCount == 916) || (NumVertices == 250 && primCount == 122) || (NumVertices == 725 && primCount == 336) || (NumVertices == 1401 && primCount == 666) || ((NumVertices == 1334 && primCount == 841) || (NumVertices == 895 && primCount == 446) || (NumVertices == 3096 && primCount == 1936) || (NumVertices == 858 && primCount == 515) || (NumVertices == 429 && primCount == 246))

//Defense(ground and air defense )
#define Defense ((m_Stride == 24) && (NumVertices == 3413 && primCount == 1709 && startIndex == 2859) || (NumVertices == 1531 && primCount == 953 && startIndex == 0) || (NumVertices == 7641 && primCount == 4437 && startIndex == 3984) || (NumVertices == 6845 && primCount == 3553 && startIndex == 2859) || (NumVertices == 5906 && primCount == 2809 && startIndex == 3984) || (NumVertices == 1980 && primCount == 1328 && startIndex == 0) || (NumVertices == 3413 && primCount == 1709) | (NumVertices == 3207 && primCount == 1575) || (NumVertices == 1963 && primCount == 1072) || (NumVertices == 963 && primCount == 428) || (NumVertices == 467 && primCount == 209)))

//Supply crate
#define Supplies ((m_Stride == 24) && (NumVertices == 3096 && primCount == 1936)) 

//UAV
#define Uav ((m_Stride == 24) && (NumVertices == 622 && primCount == 392) || (NumVertices == 622 && primCount == 392) || (NumVertices == 350 && primCount == 199) || (NumVertices == 138 && primCount == 70))

//Silo
#define Silo ((m_Stride == 24) && ((NumVertices == 4213 && primCount == 2398 && startIndex == 9)))

//POD(doesnt work on long range
#define Pod ((m_Stride == 24) && (NumVertices == 1874 && primCount == 1673) || (NumVertices == 1964 && primCount == 1350))

//Titanguns
#define TitanGuns ((m_Stride == 24) && (NumVertices == 218 && primCount == 95) || (NumVertices == 610 && primCount == 322) || (NumVertices == 1073 && primCount == 667))

//Heli's
#define Heli ((m_Stride == 24) && (NumVertices == 509 && primCount == 288) || (NumVertices == 263 && primCount == 91) || (NumVertices == 771 && primCount == 257) || (NumVertices == 7354 && primCount == 3588) || (NumVertices == 4000 && primCount == 1799) || (NumVertices == 1000 && primCount == 434) || (NumVertices == 4441 && primCount == 2966) || (NumVertices == 2923 && primCount == 1535) || (NumVertices == 2531 && primCount == 1445) || (NumVertices == 1612 && primCount == 770) || (NumVertices == 760 && primCount == 314) || (NumVertices == 4315 && primCount == 2792) || (NumVertices == 3430 && primCount == 1349) || (NumVertices == 800 && primCount == 258))

//Cars/buggies
#define Buggy ((m_Stride == 24) && (NumVertices == 4139 && primCount == 2653) || (NumVertices == 2082 && primCount == 1243) || (NumVertices == 510 && primCount == 280) || (NumVertices == 2682 && primCount == 1566) || (NumVertices == 2055 && primCount == 821) || (NumVertices == 775 && primCount == 260))

//Walker
#define Walker ((m_Stride == 24) && (NumVertices == 10218 && primCount == 5034) || (NumVertices == 5974 && primCount == 2773) || (NumVertices == 2134 && primCount == 875) || (NumVertices == 7182 && primCount == 4116) || (NumVertices == 4958 && primCount == 2127) || (NumVertices == 1686 && primCount == 538))

//APC_Tank (apc and tank )
#define Apc_Tank ((m_Stride == 24 || 28) && (NumVertices == 16 && primCount == 8) || (NumVertices == 7089 && primCount == 4290) || (NumVertices == 3458 && primCount == 2062) || (NumVertices == 762 && primCount == 334) || (NumVertices == 884 && primCount == 440) || (NumVertices == 3564 && primCount == 1396) || (NumVertices == 3564 && primCount == 1396) || (NumVertices == 965 && primCount == 335) || (NumVertices == 3560 && primCount == 1256) || (NumVertices == 3560 && primCount == 1256) || (NumVertices == 848 && primCount == 283) || (NumVertices == 5719 && primCount == 3780) || (NumVertices == 2848 && primCount == 1222) || (NumVertices == 1311 && primCount == 556) || (NumVertices == 765 && primCount == 376) || (NumVertices == 280 && primCount == 132) || (NumVertices == 4027 && primCount == 3009))

//Speeder
#define Speeder ((m_Stride == 24) && (NumVertices == 4639 && primCount == 2658) || (NumVertices == 1657 && primCount == 861) || (NumVertices == 470 && primCount == 210))

//Goliath
#define Goliath ((m_Stride == 28) && (NumVertices == 7466 && primCount == 4749) || (NumVertices == 3486 && primCount == 2424) || (NumVertices == 8745 && primCount == 4431) || (NumVertices == 2662 && primCount == 1332) || (NumVertices == 3069 && primCount == 1848) || (NumVertices == 459 && primCount == 205))

//Guns
#define Guns ((m_Stride == 24) && ((NumVertices == 2346 && primCount == 1694 && startIndex == 0) || (NumVertices == 4615 && primCount == 3585 && startIndex == 0) || (NumVertices == 6893 && primCount == 4797 && startIndex == 0) || (NumVertices == 5261 && primCount == 3926 && startIndex == 90) || (NumVertices == 6788 && primCount == 4402 && startIndex == 0) || (NumVertices == 6573 && primCount == 4256 && startIndex == 0) || (NumVertices == 8297 && primCount == 5520 && startIndex == 0) || (NumVertices == 6384 && primCount == 4361 && startIndex == 0) || (NumVertices == 9440 && primCount == 8004 && startIndex == 0) || (NumVertices == 4545 && primCount == 3461 && startIndex == 0) || (NumVertices == 5667 && primCount == 3972 && startIndex == 0) || (NumVertices == 10661 && primCount == 6998 && startIndex == 0) || (NumVertices == 6981 && primCount == 4943 && startIndex == 0) || (NumVertices == 6217 && primCount == 4950 && startIndex == 756) || (NumVertices == 8022 && primCount == 5195 && startIndex == 0) || (NumVertices == 3815 && primCount == 3049 && startIndex == 0) || (NumVertices == 168 && primCount == 114 && startIndex == 28083) || (NumVertices == 8078 && primCount == 5160 && startIndex == 0) || (NumVertices == 2631 && primCount == 2314 && startIndex == 0)))

///////M_E_N_U//////////
bool bMenuInited;
bool OffsetInit;
bool IniChecker = false;


//Menu functions
void ValueInit();
void InitMenu();
void fillrgba(int x, int y, int w, int h, int r, int g, int b, int a);
void drawopx(int x, int y, int width, int height, int r, int g, int b, int a);
void drawguibox(int x, int y, int w, int h, int r, int g, int b, int a,int rr, int gg, int bb, int aa);
void DrawConString(int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...);
void DoKeyCheck();
void __stdcall CustomPresent(IDirect3DDevice9* pDevice);
void __stdcall CustomInitialize(IDirect3DDevice9* pDevice);

//---------Color Declares-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9    texBlue;  
LPDIRECT3DTEXTURE9    texRed;  
LPDIRECT3DTEXTURE9    texYellow;  
LPDIRECT3DTEXTURE9    texGreen;  
LPDIRECT3DTEXTURE9    texOrange;  
LPDIRECT3DTEXTURE9    texPurple;  
LPDIRECT3DTEXTURE9    texPink;
LPDIRECT3DTEXTURE9    texBrown;
LPDIRECT3DTEXTURE9    texWhite;
LPDIRECT3DTEXTURE9    texBlack;
LPDIRECT3DTEXTURE9    texLightBlue;
LPDIRECT3DTEXTURE9    texSprite;

//--------Menu---------------------------------------------------------------------------------------------------------
ID3DXFont *Font1, *Font2, *Font3, *TimeFont, *ESPfont;

typedef struct
{
	bool isOn;
	char * name;
} menuItem;

menuItem Menu[] = {
	{false, "PlayerChams"},
	{false, "VehicleChams"},
	{false, "MiscChams"},
	{false, "Xplosives"},
	{false, "ColoredSky"},
	{false, "Wireframe"},
	{false, "Asuswallhack"},
	{false, "WeaponChams"},
	{false, "NoScope"},
	{false, "View"},
	{false, "NoFog"},
	{false, "NoWater"},
	{false, "LockXhair"},
	{false, "Nobreathing"},
	{false, "Minimap"},
	{false, "Nametags"},
	{false, "MeterTags"},
	{false, "DeployESP"},
	{false, "classESP"},
	{false, "bodyESP"},
	{false, "veteranESP"},
	{false, "DirectionIcons"},
	{false, "Health"},
	{false, "NoRain"},
	{false, "Assets"},
	{false, "EMPblur"},
	{false, "Time"},
	{false, "Xhair"},
	{false, "Spoofer"},
	{false, "LockXhair"},
	{false, "SpinHack"},
	{false, "Brightness"},


};

int menuSize = 32;
int currentSelected = 1;
int MenuPosX = 23;
int MenuPosY = 2;
int iTextSize = 10;
int iTextSpace = 5;
bool bMenuOn = true;



//----------CHAMS COLORS-------------------------------------------------------------------------------------------

const BYTE blue[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00
};

const BYTE yellow[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00
};

const BYTE green[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00
};

const BYTE ltblue[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00
};

const BYTE purple[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00
};

const BYTE red[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00
};

const BYTE white[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00
};

const BYTE black[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const BYTE orange[58] = 
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0xFF, 0x00
};
//dark yellow
const BYTE tur[60] = 
{ 
     0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
     0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
     0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 
     0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
     0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00, 
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
     0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 
};  

const BYTE pink[58] = 
{
     0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
     0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
     0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 
     0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
     0x80, 0x00, 0xFF, 0x00
};  


//----------END----OF-----GLOBAL-----VARIABLES----------------------------------------------------------------------------------------------------------------------

HRESULT GenerateShader(IDirect3DDevice9 *pD3Ddev, IDirect3DPixelShader9 **pShader, float r, float g, float b )
{
    char szShader[ 256 ];
    ID3DXBuffer *pShaderBuf = NULL;
    sprintf( szShader, "ps.1.4\ndef c0, %f, %f, %f, %f\nmov r0,c0", r, g, b, 1.0f );
    D3DXAssembleShader( szShader, sizeof( szShader ), NULL, NULL, 0, &pShaderBuf, NULL );
    if( FAILED( pD3Ddev->CreatePixelShader((const DWORD*)pShaderBuf->GetBufferPointer(), pShader)) )return E_FAIL;
    return S_OK;
}
struct shaders
{
    IDirect3DPixelShader9 *sBlue, *sRed, *sPurple, *sOrange, *sWhite, *sGreen, *sYellow, *sBlueish;
};shaders sa;

HRESULT CD3DManager::Initialize()
{
	CustomInitialize(m_pD3Ddev); 
	hand = GetCurrentProcess();

	//------------creates font-------------------------------------------------------------------------------------------------------------------
	D3DXCreateFont( m_pD3Ddev, iTextSize, 0, FW_NORMAL, 0, FALSE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &Font1);//Microsoft Sans Serif
	D3DXCreateFont( m_pD3Ddev, iTextSize, 0, FW_BOLD, 0, FALSE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &Font2);
	D3DXCreateFont( m_pD3Ddev, 16, 0, FW_BOLD, 0, FALSE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &Font3);
	D3DXCreateFont( m_pD3Ddev, 18, 0, FW_BOLD, 0, FALSE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &TimeFont);
	D3DXCreateFont( m_pD3Ddev, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &ESPfont );

	//-------------load textures-----------------------------------------------------------------------------------------------------------------
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&blue, sizeof(blue), &texBlue);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&yellow, sizeof(yellow), &texYellow);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&green, sizeof(green), &texGreen);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&ltblue, sizeof(ltblue), &texLightBlue);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&purple, sizeof(purple), &texPurple);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&red, sizeof(red), &texRed);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&white, sizeof(white), &texWhite);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&black, sizeof(black), &texBlack);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&orange, sizeof(orange), &texOrange);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&tur, sizeof(tur), &texBrown);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&pink, sizeof(pink), &texPink);


	//-----------load shaders---------------------------------------------------------------------------------------------------------------------
	GenerateShader( m_pD3Ddev, &sa.sBlue, 0.0f, 0.0f, 1.0f );
    GenerateShader( m_pD3Ddev, &sa.sGreen, 0.0f, 1.0f, 0.0f );
    GenerateShader( m_pD3Ddev, &sa.sOrange, 1.0f, 0.5f, 0.0f );
    GenerateShader( m_pD3Ddev, &sa.sPurple, 1.0f, 0.0f, 1.0f );
    GenerateShader( m_pD3Ddev, &sa.sRed, 1.0f, 0.0f, 0.0f );
    GenerateShader( m_pD3Ddev, &sa.sWhite, 1.0f, 1.0f, 1.0f );
    GenerateShader( m_pD3Ddev, &sa.sYellow, 1.0f, 1.0f, 0.0f );
	GenerateShader( m_pD3Ddev, &sa.sBlueish, 0.0f, 0.3f, 1.0f );


	//----------creates line----------------------------------------------------------------------------------------------------------------------
	ID3DXLine *pLine;
	D3DXCreateLine( m_pD3Ddev , &pLine);
	pLine->SetWidth(1.0);
	pLine->SetAntialias(true);
	pLine->SetGLLines(true);
	return S_OK;

}
void DrawMyText(char pString[], int x, int y, D3DCOLOR col, ID3DXFont *font)
{
	RECT FontRect = { x, y, x+500, y+30 };
	font->DrawText( NULL, pString, -1, &FontRect, DT_LEFT | DT_WORDBREAK, col);
}

HRESULT CD3DManager::PreReset()
{
	return S_OK;  
}

HRESULT CD3DManager::PostReset()
{
	return S_OK;
}

HRESULT CD3DManager::Release()
{
		
	return S_OK;
}

HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid, LPVOID *ppvObj) 
{
	return m_pD3Ddev->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3DDevice9::AddRef() 
{
	m_refCount++;
	return m_pD3Ddev->AddRef();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginScene() 
{
    m_pD3Ddev->GetViewport(&Viewport);

	D3DCOLOR dwhite = D3DCOLOR_XRGB( 255, 255, 255 );
	DrawMyText("BF2142 HaXx v1.40 by buttnugget112", 370, 24, dwhite, Font3);
	return m_pD3Ddev->BeginScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginStateBlock() 
{
	return m_pD3Ddev->BeginStateBlock();
}

HRESULT APIENTRY hkIDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) 
{
	return m_pD3Ddev->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect, D3DCOLOR color) 
{	
	return m_pD3Ddev->ColorFill(pSurface,pRect,color);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **ppSwapChain) 
{
	return m_pD3Ddev->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality,Discard,ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) 
{
	return m_pD3Ddev->CreatePixelShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) 
{
	return m_pD3Ddev->CreateQuery(Type,ppQuery);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateRenderTarget(Width, Height, Format, MultiSample,MultisampleQuality, Lockable, ppSurface,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) 
{
	return m_pD3Ddev->CreateStateBlock(Type, ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) 
{
	HRESULT ret = m_pD3Ddev->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);

#ifdef D3DHOOK_TEXTURES
	if(ret == D3D_OK) { new hkIDirect3DTexture9(ppTexture, this, Width, Height, Format); }
#endif

	return ret;
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pD3Ddev->CreateVertexDeclaration(pVertexElements,ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
{
	return m_pD3Ddev->CreateVertexShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::DeletePatch(UINT Handle) 
{
	return m_pD3Ddev->DeletePatch(Handle);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
//------D-3-D---H-A-C-K-S---------------------------------------------------------------------------------------------------------	
 	
	
	//no fog
	if(NoFog)
	{
		m_pD3Ddev->SetRenderState( D3DRS_FOGENABLE, false );
	}

	//remove water
	if(NoWater)
	{
		if(Water)
		{
			return 0;
		}
	}

	//Colored sky
	if(ColoredSky)//bool
	{
		if(Sky)//model recognition
		{
			return 0;
		}
	}
	
	//Wireframe hack
	if(Wireframe)//bool
	{
		m_pD3Ddev->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );//wireframe mode
		m_pD3Ddev->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount );
		m_pD3Ddev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );//else back to normal
	}

	//Asus wallhack
	if(Asuswallhack)//bool
	{
		m_pD3Ddev->SetRenderState( D3DRS_SRCBLEND, 0x2 );// sets the walls textures transparent
		m_pD3Ddev->SetRenderState( D3DRS_DESTBLEND, 0x2 );
		m_pD3Ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	}

	//weapon chams
	if(WeaponChams)//bool
	{
		if(Guns)//model recognition
		{	
			m_pD3Ddev->SetTexture(0, NULL);
			m_pD3Ddev->SetRenderState(D3DRS_LIGHTING, false);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,false);
			m_pD3Ddev->SetTexture(0,texWhite);
			m_pD3Ddev->SetTexture(1,texWhite);

			m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
				
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,true);
			m_pD3Ddev->SetTexture(0,texWhite);
			m_pD3Ddev->SetTexture(1,texWhite);
		}		
	} 

	//player CHAMS
	if(PlayerChams)//bool
	{
		if(Pac)//model recognition
		{ 
			m_pD3Ddev->SetTexture(0,NULL);
			m_pD3Ddev->SetRenderState(D3DRS_LIGHTING, false);  
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,false);
			m_pD3Ddev->SetPixelShader(sa.sYellow);//pixel shader

			m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
			
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,true);
			m_pD3Ddev->SetPixelShader(sa.sPurple);//pixel shader	
		}
		if(Eu)//model recognition
		{
			m_pD3Ddev->SetTexture(0, NULL);
			m_pD3Ddev->SetRenderState(D3DRS_LIGHTING, false);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,false);
			m_pD3Ddev->SetPixelShader(sa.sBlue);//pixel shader

			m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
				
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,true);
			m_pD3Ddev->SetPixelShader(sa.sRed);//pixel shader	
		}
	}

	//Explosive CHAMS
	if(Xplosives)//bool
	{
		if(Grenades || APM || IPS || RDX || EMP || Sentry)//model recongition
		{
			m_pD3Ddev->SetTexture(0, NULL); //remove original textures
			m_pD3Ddev->SetRenderState(D3DRS_LIGHTING, false);//no lighting
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,false);
			m_pD3Ddev->SetPixelShader(sa.sRed);
			
			m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,true);
			m_pD3Ddev->SetPixelShader(sa.sOrange);
		}
	}

	//Vehicle CHAMS
	if(VehicleChams)//bool
	{
		if(Buggy || Walker || Apc_Tank || Heli)//model recognition
		{
			m_pD3Ddev->SetTexture(0, NULL);
			m_pD3Ddev->SetRenderState(D3DRS_LIGHTING, false);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,false);
			m_pD3Ddev->SetRenderState( D3DRS_SRCBLEND, 0x3 );
			m_pD3Ddev->SetRenderState( D3DRS_DESTBLEND, 0x3 );
			m_pD3Ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
			m_pD3Ddev->SetPixelShader(sa.sBlueish);

			m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,true);
			m_pD3Ddev->SetRenderState( D3DRS_SRCBLEND, 0x3 );
			m_pD3Ddev->SetRenderState( D3DRS_DESTBLEND, 0x3 );
			m_pD3Ddev->SetPixelShader(sa.sGreen);
		}
	}

	// Misc. CHAMS
	if(MiscChams)//bool
	{
		if(Supplies || Commanderassets || TitanGuns || Uav || Defense || Silo || Pod)//model recognition
		{
			m_pD3Ddev->SetRenderState(D3DRS_LIGHTING, false);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,false);//makes model recognition visible even behind walls
			m_pD3Ddev->SetRenderState( D3DRS_SRCBLEND, 0x3 );
			m_pD3Ddev->SetRenderState( D3DRS_DESTBLEND, 0x3 );
			m_pD3Ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
			m_pD3Ddev->SetPixelShader(sa.sYellow);

			m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
			
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,true);
			m_pD3Ddev->SetPixelShader(sa.sOrange);	
		}
	}

	//No Scope Overlay
	if(NoScope)//bool
	{
		if (Scope || TankOverlay)//model recognition
		{
			return 0; // removes scope overlay
		}
	}
	//--------------------------------------------------------------------------------------------------------------

	return m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}	

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{	
	return m_pD3Ddev->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) 
{
	return m_pD3Ddev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{
	return m_pD3Ddev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) 
{
	return m_pD3Ddev->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
{
	return m_pD3Ddev->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::EndScene()
{
//--------M-E-M-M-O-R-Y---H-A-C-K-S---------------------------------------------------------------------------------------------

		HANDLE hand = GetCurrentProcess();
		CustomPresent(m_pD3Ddev);

		//punkbuster screenshot guid spoofer
		if(Spoofer)
		{
			HMODULE hHandle = GetModuleHandle("pbcl.dll");

			BYTE* pGUID = (BYTE*)hHandle + 0xB8D37;//update this offset
			const int ciASCII = (const int)"AHAHHA!You just got PWNED by";
			DWORD dwProtection;
			VirtualProtect(pGUID, 32, PAGE_EXECUTE_READWRITE, &dwProtection);
			memcpy(pGUID, (const PVOID)ciASCII, 32);
			VirtualProtect(pGUID, 32, dwProtection, &dwProtection);
		}
	
		//draw sprites
		D3DXVECTOR3 pos;
		pos.x = Viewport.Width/2-135.0f;
		pos.y = Viewport.Height/16;
		pos.z = 0.0f;

		//draw text
		D3DCOLOR dwhite = D3DCOLOR_XRGB( 255, 255, 255 );
		DrawMyText("BF2142 HaXx v1.40 by buttnugget112", pos.x, 24, dwhite, Font3);

		//draw time
		if(Time)
		{
			D3DCOLOR dTime = D3DCOLOR_XRGB( 255, 000, 000 );                     
			GetTimeFormat(NULL,NULL,NULL,NULL,Timestruct,15);
			sprintf(TimeString,"[ %s ]",Timestruct); 
			DrawMyText(TimeString, 120, 10, dTime, TimeFont);
		}
		
		//custom crosshair
		if(Xhair)
		{
			D3DRECT rec1 = {ScreenCenterX - 12, ScreenCenterY - 1, ScreenCenterX + 12, ScreenCenterY + 1}; // hori line top
			D3DRECT rec2 = {ScreenCenterX - 12, ScreenCenterY, ScreenCenterX + 12, ScreenCenterY + 1}; // hori line bottom
			D3DRECT rec3 = {ScreenCenterX, ScreenCenterY - 12, ScreenCenterX + 1,ScreenCenterY + 12}; // vert line left
			D3DRECT rec4 = {ScreenCenterX - 1, ScreenCenterY - 12, ScreenCenterX + 1,ScreenCenterY + 12}; // vert line right

			m_pD3Ddev->Clear(1, &rec1, D3DCLEAR_TARGET,redt, 0,  0);
			m_pD3Ddev->Clear(1, &rec2, D3DCLEAR_TARGET,redt, 0,  0);
			m_pD3Ddev->Clear(1, &rec3, D3DCLEAR_TARGET,redt, 0,  0);
			m_pD3Ddev->Clear(1, &rec4, D3DCLEAR_TARGET,redt, 0,  0);
		}

		if(LockXhair)
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );
			VirtualProtect((void*)(dwBF2142Base+0x1E0649), sizeof(LockX1), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E0649), (const void*)LockX1, sizeof(LockX1));
			VirtualProtect((void*)(dwBF2142Base+0x1E0649), sizeof(LockX1), dwProtect, 0);
				
			VirtualProtect((void*)(dwBF2142Base+0x1E064C), sizeof(LockX2), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E064C), (const void*)LockX2, sizeof(LockX2));
			VirtualProtect((void*)(dwBF2142Base+0x1E064C), sizeof(LockX2), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x1E064F), sizeof(LockX3), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E064F), (const void*)LockX3, sizeof(LockX3));
			VirtualProtect((void*)(dwBF2142Base+0x1E064F), sizeof(LockX3), dwProtect, 0);
		}
		else
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );
			VirtualProtect((void*)(dwBF2142Base+0x1E0649), sizeof(LockX1Orig), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E0649), (const void*)LockX1Orig, sizeof(LockX1Orig));
			VirtualProtect((void*)(dwBF2142Base+0x1E0649), sizeof(LockX1Orig), dwProtect, 0);
				
			VirtualProtect((void*)(dwBF2142Base+0x1E064C), sizeof(LockX2Orig), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E064C), (const void*)LockX2Orig, sizeof(LockX2Orig));
			VirtualProtect((void*)(dwBF2142Base+0x1E064C), sizeof(LockX2Orig), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x1E064F), sizeof(LockX3Orig), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E064F), (const void*)LockX3Orig, sizeof(LockX3Orig));
			VirtualProtect((void*)(dwBF2142Base+0x1E064F), sizeof(LockX3Orig), dwProtect, 0);
		}

		////viewdistance hack
		if(View)
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );
			ReadProcessMemory(hand, (void*) (RendDx9Base+0x1F7E58), &dwPreView, sizeof(dwPreView), &bytes);

			WriteProcessMemory(hand, (void*)(dwPreView+0x4F0), &Viewdistance, sizeof(Viewdistance), &bytes);
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwPreView+0x4F0), &Viewdistanceread, sizeof(Viewdistanceread), &bytes);
		}
				
		/////////unlimited commander toys
		if (Assets)
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );

			VirtualProtect((void*)(dwBF2142Base+0x3D05CB), sizeof(Toys1On), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x3D05CB), (const void*)Toys1On, sizeof(Toys1On));
			VirtualProtect((void*)(dwBF2142Base+0x3D05CB), sizeof(Toys1On), dwProtect, 0);
				
			VirtualProtect((void*)(dwBF2142Base+0x3AC9E2), sizeof(Toys2On), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x3AC9E2), (const void*)Toys2On, sizeof(Toys2On));
			VirtualProtect((void*)(dwBF2142Base+0x3AC9E2), sizeof(Toys2On), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x3ACA22), sizeof(Toys3Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x3ACA22), (const void*)Toys3On, sizeof(Toys3On));
			VirtualProtect((void*)(dwBF2142Base+0x3ACA22), sizeof(Toys3On), dwProtect, 0);
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x3D05CB), &Toys1On,sizeof(Toys1On), &bytes);   ///unlimited orbital
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x3AC9E2), &Toys2On,sizeof(Toys2On), &bytes);///unlimited supplies
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x3ACA22), &Toys3On,sizeof(Toys3On), &bytes);  ///unlimited emp	
		}	
		else
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );
			VirtualProtect((void*)(dwBF2142Base+0x3D05CB), sizeof(Toys1Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x3D05CB), (const void*)Toys1Off, sizeof(Toys1Off));
			VirtualProtect((void*)(dwBF2142Base+0x3D05CB), sizeof(Toys1Off), dwProtect, 0);
				
			VirtualProtect((void*)(dwBF2142Base+0x3AC9E2), sizeof(Toys2Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x3AC9E2), (const void*)Toys2Off, sizeof(Toys2Off));
			VirtualProtect((void*)(dwBF2142Base+0x3AC9E2), sizeof(Toys2Off), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x3ACA22), sizeof(Toys3Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x3ACA22), (const void*)Toys3Off, sizeof(Toys3Off));
			VirtualProtect((void*)(dwBF2142Base+0x3ACA22), sizeof(Toys3Off), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x3D05CB), &Toys1Off,sizeof(Toys1Off), &bytes);   ///unlimited orbital
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x3AC9E2), &Toys2Off,sizeof(Toys2Off), &bytes);///unlimited supplies
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x3ACA22), &Toys3Off,sizeof(Toys3Off), &bytes);  ///unlimited emp		
		}
		
		//////removals:rain and snow
		if(NoRain)
		{
		RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );
		VirtualProtect((void*)(RendDx9Base+0x144369), sizeof(RainOn), PAGE_READWRITE, &dwProtect);
		memcpy((void*)(RendDx9Base+0x144369), (const void*)RainOn, sizeof(RainOn));
		VirtualProtect((void*)(RendDx9Base+0x144369), sizeof(RainOn), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x144369), &RainOn, 4, &bytes);
		}
		else
		{
		RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );
		VirtualProtect((void*)(RendDx9Base+0x144369), sizeof(RainOff), PAGE_READWRITE, &dwProtect);
		memcpy((void*)(RendDx9Base+0x144369), (const void*)RainOff, sizeof(RainOff));
		VirtualProtect((void*)(RendDx9Base+0x144369), sizeof(RainOff), dwProtect, 0);
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x144369), &RainOff, 4, &bytes);

		}
	
		/////////////minimap hack
		if(Minimap)
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );
			VirtualProtect((void*)(dwBF2142Base+0x383B33), sizeof(Minimap1), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x383B33), (const void*)Minimap1, sizeof(Minimap1));
			VirtualProtect((void*)(dwBF2142Base+0x383B33), sizeof(Minimap1), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x39631F), sizeof(Minimap2), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x39631F), (const void*)Minimap2, sizeof(Minimap2));
			VirtualProtect((void*)(dwBF2142Base+0x39631F), sizeof(Minimap2), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x383C73), sizeof(Minimap3), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x383C73), (const void*)Minimap3, sizeof(Minimap3));
			VirtualProtect((void*)(dwBF2142Base+0x383C73), sizeof(Minimap3), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x383B33), &Minimap1,2, &bytes);
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x39631F), &Minimap2,2, &bytes);
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x383C73), &Minimap3,2, &bytes);
		}
		else
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );
			VirtualProtect((void*)(dwBF2142Base+0x383B33), sizeof(Minimap1Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x383B33), (const void*)Minimap1Off, sizeof(Minimap1Off));
			VirtualProtect((void*)(dwBF2142Base+0x383B33), sizeof(Minimap1Off), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x39631F), sizeof(Minimap2Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x39631F), (const void*)Minimap2Off, sizeof(Minimap2Off));
			VirtualProtect((void*)(dwBF2142Base+0x39631F), sizeof(Minimap2Off), dwProtect, 0);

			VirtualProtect((void*)(dwBF2142Base+0x383C73), sizeof(Minimap3Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x383C73), (const void*)Minimap3Off, sizeof(Minimap3Off));
			VirtualProtect((void*)(dwBF2142Base+0x383C73), sizeof(Minimap3Off), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x383B33), &Minimap1Off,2, &bytes);
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x39631F), &Minimap2Off,2, &bytes);
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x383C73), &Minimap3Off,2, &bytes);
		}

		//////////////no breathing hack   
		if(Nobreathing)
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );

			VirtualProtect((void*)(dwBF2142Base+0x1E242B), sizeof(NoBreathing), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E242B), (const void*)NoBreathing, sizeof(NoBreathing));
			VirtualProtect((void*)(dwBF2142Base+0x1E242B), sizeof(NoBreathing), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x1E242B), &NoBreathing,3, &bytes);
		}
		else
		{
			VirtualProtect((void*)(dwBF2142Base+0x1E242B), sizeof(NoBreathingOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1E242B), (const void*)NoBreathingOff, sizeof(NoBreathingOff));
			VirtualProtect((void*)(dwBF2142Base+0x1E242B), sizeof(NoBreathingOff), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x1E242B), &NoBreathingOff,3, &bytes);
		}

		//////////nametags hack	
		if(Nametags)		
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );
			VirtualProtect((void*)(RendDx9Base+0x118A85), sizeof(nametagson), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118A85), &nametagson, sizeof(nametagson));
			VirtualProtect((void*)(RendDx9Base+0x118A85), sizeof(nametagson), dwProtect, 0);
		}
		else
		{	
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );
			VirtualProtect((void*)(RendDx9Base+0x118A85), sizeof(nametagsoff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118A85), &nametagsoff, sizeof(nametagsoff));
			VirtualProtect((void*)(RendDx9Base+0x118A85), sizeof(nametagsoff), dwProtect, 0);
		}

		//health ESP
		if(Health)		
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x1941C3), sizeof(healthESP), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x1941C3), &healthESP, sizeof(healthESP));
			VirtualProtect((void*)(RendDx9Base+0x1941C3), sizeof(healthESP), dwProtect, 0);

			VirtualProtect((void*)(RendDx9Base+0x1944CD), sizeof(healthESP), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x1944CD), &healthESP, sizeof(healthESP));
			VirtualProtect((void*)(RendDx9Base+0x1944CD), sizeof(healthESP), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x1941C3), &healthESP,6, &bytes);  
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x1944CD), &healthESP,6, &bytes); 
		}
		else
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x1941C3), sizeof(healthESPOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x1941C3), &healthESP, sizeof(healthESPOff));
			VirtualProtect((void*)(RendDx9Base+0x1941C3), sizeof(healthESPOff), dwProtect, 0);

			VirtualProtect((void*)(RendDx9Base+0x1944CD), sizeof(healthESP2Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x1944CD), &healthESP, sizeof(healthESP2Off));
			VirtualProtect((void*)(RendDx9Base+0x1944CD), sizeof(healthESP2Off), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x1941C3), &healthESPOff,6, &bytes); 
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x1944CD), &healthESP2Off,6, &bytes); 
		}

		//////////meter tags hack		
		if(MeterTags)		
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x11A6A1), sizeof(tagsdistanceon), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x11A6A1), &tagsdistanceon, sizeof(tagsdistanceon));
			VirtualProtect((void*)(RendDx9Base+0x11A6A1), sizeof(tagsdistanceon), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x11A6A1), &tagsdistanceon,2, &bytes);  
		}
		else
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x11A6A1), sizeof(tagsdistanceoff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x11A6A1), &tagsdistanceoff, sizeof(tagsdistanceoff));
			VirtualProtect((void*)(RendDx9Base+0x11A6A1), sizeof(tagsdistanceoff), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x11A6A1), &tagsdistanceoff,2, &bytes);
		}

		if(DirectionIcons)//////////direction icons hack	
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(icons), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118382), &icons, sizeof(icons));
			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(icons), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x118382), &icons,2, &bytes);  
		}
		else
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(iconsOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118382), &iconsOff, sizeof(iconsOff));
			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(iconsOff), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x118382), &iconsOff,2, &bytes);
		}

		if(DeployESP)	/////////DeployESP	
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x118E26), sizeof(deployESP), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118E26), &deployESP, sizeof(deployESP));
			VirtualProtect((void*)(RendDx9Base+0x118E26), sizeof(deployESP), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x118E26), &deployESP,2, &bytes);  
		}
		else
		{	
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x118E26), sizeof(deployESPoff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118E26), &deployESPoff, sizeof(deployESP));
			VirtualProtect((void*)(RendDx9Base+0x118E26), sizeof(deployESPoff), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x118E26), &deployESPoff,2, &bytes);
		}

		//remove emp blur
		if(EMPblur)		
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x2E1B9), sizeof(EMPon), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x2E1B9), &EMPon, sizeof(EMPon));
			VirtualProtect((void*)(RendDx9Base+0x2E1B9), sizeof(EMPon), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x2E1B9), &EMPon,6, &bytes);  
		}
		else
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x2E1B9), sizeof(EMPoff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x2E1B9), &EMPoff, sizeof(EMPoff));
			VirtualProtect((void*)(RendDx9Base+0x2E1B9), sizeof(EMPoff), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x2E1B9), &EMPoff,6, &bytes);
		}

		//unlock console
		if(ConsoleUnlock)		
		{		
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );

			VirtualProtect((void*)(dwBF2142Base+0x274D28), sizeof(Console), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x274D28), &Console, sizeof(Console));
			VirtualProtect((void*)(dwBF2142Base+0x274D28), sizeof(Console), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x274D28), &Console,2, &bytes);  
		}
		else
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );

			VirtualProtect((void*)(dwBF2142Base+0x274D28), sizeof(ConsoleOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x274D28), &ConsoleOff, sizeof(ConsoleOff));
			VirtualProtect((void*)(dwBF2142Base+0x274D28), sizeof(ConsoleOff), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x274D28), &ConsoleOff,2, &bytes);
		}

		//unlimited taunt spam
		if(Spam)		
		{	

			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );

			VirtualProtect((void*)(dwBF2142Base+0x1925E6), sizeof(TauntSpam), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1925E6), &TauntSpam, sizeof(TauntSpam));
			VirtualProtect((void*)(dwBF2142Base+0x1925E6), sizeof(TauntSpam), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x1925E6), &TauntSpam,6, &bytes);  
		}
		else
		{
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );

			VirtualProtect((void*)(dwBF2142Base+0x1925E6), sizeof(TauntSpamOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x1925E6), &TauntSpamOff, sizeof(TauntSpamOff));
			VirtualProtect((void*)(dwBF2142Base+0x1925E6), sizeof(TauntSpamOff), dwProtect, 0);
			
			//WriteProcessMemory(hand, (void*)(dwBF2142Base+0x1925E6), &TauntSpamOff,6, &bytes);
		}
		
		//class ESP
		if(classESP)		
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x117de3), sizeof(classtags1), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x117de3), &classtags1, sizeof(classtags1));
			VirtualProtect((void*)(RendDx9Base+0x117de3), sizeof(classtags1), dwProtect, 0);

			VirtualProtect((void*)(RendDx9Base+0x117e0d), sizeof(classtags2), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x117e0d), &classtags2, sizeof(classtags2));
			VirtualProtect((void*)(RendDx9Base+0x117e0d), sizeof(classtags2), dwProtect, 0);
			
			VirtualProtect((void*)(RendDx9Base+0x117e38), sizeof(classtags3), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x117e38), &classtags3, sizeof(classtags3));
			VirtualProtect((void*)(RendDx9Base+0x117e38), sizeof(classtags3), dwProtect, 0);

			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(classtags4), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118382), &classtags4, sizeof(classtags4));
			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(classtags4), dwProtect, 0);
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x117de3), &classtags1, 2, &bytes);//assault
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x117e0d), &classtags2, 2, &bytes);//engineer
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x117e38), &classtags3, 6, &bytes);//support
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x118382), &classtags4, 2, &bytes);//enemy
		}
		else
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x117de3), sizeof(classtags1Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x117de3), &classtags1Off, sizeof(classtags1Off));
			VirtualProtect((void*)(RendDx9Base+0x117de3), sizeof(classtags1Off), dwProtect, 0);

			VirtualProtect((void*)(RendDx9Base+0x117e0d), sizeof(classtags2Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x117e0d), &classtags2Off, sizeof(classtags2Off));
			VirtualProtect((void*)(RendDx9Base+0x117e0d), sizeof(classtags2Off), dwProtect, 0);
			
			VirtualProtect((void*)(RendDx9Base+0x117e38), sizeof(classtags3Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x117e38), &classtags3Off, sizeof(classtags3Off));
			VirtualProtect((void*)(RendDx9Base+0x117e38), sizeof(classtags3Off), dwProtect, 0);

			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(classtags4Off), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118382), &classtags4Off, sizeof(classtags4Off));
			VirtualProtect((void*)(RendDx9Base+0x118382), sizeof(classtags4Off), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x117de3), &classtags1Off, 2, &bytes);//assault
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x117e0d), &classtags2Off, 2, &bytes);//engineer
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x117e38), &classtags3Off, 6, &bytes);//support
			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x118382), &classtags4Off, 2, &bytes);//enemy
		}

		//bodytags 
		if(bodyESP)		
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x191944), sizeof(bodyTags), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x191944), &bodyTags, sizeof(bodyTags));
			VirtualProtect((void*)(RendDx9Base+0x191944), sizeof(bodyTags), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x191944), &bodyTags,2, &bytes);  
		}
		else
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x191944), sizeof(bodyTagsOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x191944), &bodyTagsOff, sizeof(bodyTagsOff));
			VirtualProtect((void*)(RendDx9Base+0x191944), sizeof(bodyTagsOff), dwProtect, 0);

			//WriteProcessMemory(hand, (void*)(RendDx9Base+0x191944), &bodyTagsOff,2, &bytes);
		}

		//veteranTags
		if(veteranESP)		
		{		
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x118303), sizeof(veteranTags), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118303), &veteranTags, sizeof(veteranTags));
			VirtualProtect((void*)(RendDx9Base+0x118303), sizeof(veteranTags), dwProtect, 0);

		}
		else
		{
			RendDx9Base = ( DWORD )GetModuleHandle( "RendDX9.dll" );

			VirtualProtect((void*)(RendDx9Base+0x118303), sizeof(veteranTagsOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(RendDx9Base+0x118303), &veteranTagsOff, sizeof(veteranTagsOff));
			VirtualProtect((void*)(RendDx9Base+0x118303), sizeof(veteranTagsOff), dwProtect, 0);
		}

		//first ever spinhack created for bf
		if(SpinHack)		
		{		
			dwBF2142Base = ( DWORD )GetModuleHandle( "BF2142.exe" );
			VirtualProtect((void*)(dwBF2142Base+0x523932), sizeof(spinhack), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x523932), &spinhack, sizeof(spinhack));
			VirtualProtect((void*)(dwBF2142Base+0x523932), sizeof(spinhack), dwProtect, 0);
		}
		else
		{
			VirtualProtect((void*)(dwBF2142Base+0x523932), sizeof(spinhackOff), PAGE_READWRITE, &dwProtect);
			memcpy((void*)(dwBF2142Base+0x523932), &spinhackOff, sizeof(spinhackOff));
			VirtualProtect((void*)(dwBF2142Base+0x523932), sizeof(spinhackOff), dwProtect, 0);
		}

		//brighter structures
		if(Brightness)
		{
			ReadProcessMemory(hand, (void*)(RendDx9Base+0x203C1C), &dwPreBright, sizeof(dwPreBright), &bytes);

			WriteProcessMemory(hand, (void*)(dwPreBright+0x60),&Bright, sizeof(Bright), &bytes);
			WriteProcessMemory(hand, (void*)(dwPreBright+0x64),&Bright, sizeof(Bright), &bytes);
			WriteProcessMemory(hand, (void*)(dwPreBright+0x68),&Bright, sizeof(Bright), &bytes);
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwPreBright+0x60),&BrightOff, sizeof(BrightOff), &bytes);
			WriteProcessMemory(hand, (void*)(dwPreBright+0x64),&BrightOff, sizeof(BrightOff), &bytes);
			WriteProcessMemory(hand, (void*)(dwPreBright+0x68),&BrightOff, sizeof(BrightOff), &bytes);
		}


//-----E-N-D---O-F---M-E-M-O-R-Y---H-A-C-K-S--------------------------------------------------------------------------------------------
	return m_pD3Ddev->EndScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB) 
{
	return m_pD3Ddev->EndStateBlock(ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::EvictManagedResources() 
{
	return m_pD3Ddev->EvictManagedResources();
}

UINT APIENTRY hkIDirect3DDevice9::GetAvailableTextureMem() 
{
	return m_pD3Ddev->GetAvailableTextureMem();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) 
{
	return m_pD3Ddev->GetBackBuffer(iSwapChain,iBackBuffer, Type, ppBackBuffer);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipPlane(DWORD Index, float *pPlane) 
{
	return m_pD3Ddev->GetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9 *pClipStatus) 
{
	return m_pD3Ddev->GetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) 
{
	return m_pD3Ddev->GetCreationParameters(pParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCurrentTexturePalette(UINT *pPaletteNumber)
{
	return m_pD3Ddev->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface) 
{
	return m_pD3Ddev->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDeviceCaps(D3DCAPS9 *pCaps) 
{
	return m_pD3Ddev->GetDeviceCaps(pCaps);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDirect3D(IDirect3D9 **ppD3D9) 
{
	HRESULT hRet = m_pD3Ddev->GetDirect3D(ppD3D9);
	if( SUCCEEDED(hRet) )
		*ppD3D9 = m_pD3Dint;
	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode) 
{
	return m_pD3Ddev->GetDisplayMode(iSwapChain,pMode);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface) 
{
	return m_pD3Ddev->GetFrontBufferData(iSwapChain,pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFVF(DWORD* pFVF) 
{
	return m_pD3Ddev->GetFVF(pFVF);
}

void APIENTRY hkIDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp) 
{
	m_pD3Ddev->GetGammaRamp(iSwapChain,pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData) 
{
	return m_pD3Ddev->GetIndices(ppIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9 *pLight) 
{
	return m_pD3Ddev->GetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLightEnable(DWORD Index, BOOL *pEnable) 
{
	return m_pD3Ddev->GetLightEnable(Index, pEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetMaterial(D3DMATERIAL9 *pMaterial) 
{
	return m_pD3Ddev->GetMaterial(pMaterial);
}

float APIENTRY hkIDirect3DDevice9::GetNPatchMode() 
{
	return m_pD3Ddev->GetNPatchMode();
}

unsigned int APIENTRY hkIDirect3DDevice9::GetNumberOfSwapChains() 
{
	return m_pD3Ddev->GetNumberOfSwapChains();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
{
	return m_pD3Ddev->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader) 
{
	return m_pD3Ddev->GetPixelShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
	return m_pD3Ddev->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pD3Ddev->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) 
{
	return m_pD3Ddev->GetRasterStatus(iSwapChain,pRasterStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) 
{
	return m_pD3Ddev->GetRenderState(State, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) 
{
	return m_pD3Ddev->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) 
{
	return m_pD3Ddev->GetRenderTargetData(pRenderTarget,pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) 
{
	return m_pD3Ddev->GetSamplerState(Sampler,Type,pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetScissorRect(RECT* pRect) 
{
	return m_pD3Ddev->GetScissorRect(pRect);
}

BOOL APIENTRY hkIDirect3DDevice9::GetSoftwareVertexProcessing() 
{
	return m_pD3Ddev->GetSoftwareVertexProcessing();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride) 
{
	return m_pD3Ddev->GetStreamSource(StreamNumber, ppStreamData,OffsetInBytes, pStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* Divider) 
{
	return m_pD3Ddev->GetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	return m_pD3Ddev->GetSwapChain(iSwapChain,pSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9 **ppTexture) 
{
	return m_pD3Ddev->GetTexture(Stage, ppTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue) 
{
	return m_pD3Ddev->GetTextureStageState(Stage, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->GetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pD3Ddev->GetVertexDeclaration(ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader) 
{

	return m_pD3Ddev->GetVertexShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pD3Ddev->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	 
	return m_pD3Ddev->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pD3Ddev->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport) 
{
	return m_pD3Ddev->GetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::LightEnable(DWORD LightIndex, BOOL bEnable) 
{
	return m_pD3Ddev->LightEnable(LightIndex, bEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->MultiplyTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) 
{	
	return m_pD3Ddev->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) 
{
	return m_pD3Ddev->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer,pVertexDecl, Flags);
}

ULONG APIENTRY hkIDirect3DDevice9::Release() 
{
	if( --m_refCount == 0 )
		m_pManager->Release();

	return m_pD3Ddev->Release();
}

HRESULT APIENTRY hkIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) 
{
	m_pManager->PreReset();

	HRESULT hRet = m_pD3Ddev->Reset(pPresentationParameters);

	if( SUCCEEDED(hRet) )
	{
		m_PresentParam = *pPresentationParameters;
		m_pManager->PostReset();
	}

	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipPlane(DWORD Index, CONST float *pPlane) 
{
	return m_pD3Ddev->SetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus) 
{
	return m_pD3Ddev->SetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) 
{
	return m_pD3Ddev->SetCurrentTexturePalette(PaletteNumber);
}

void APIENTRY hkIDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags) 
{
	m_pD3Ddev->SetCursorPosition(X, Y, Flags);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap) 
{
	return m_pD3Ddev->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) 
{
	return m_pD3Ddev->SetDepthStencilSurface(pNewZStencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs) 
{
	return m_pD3Ddev->SetDialogBoxMode(bEnableDialogs);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetFVF(DWORD FVF) 
{
	return m_pD3Ddev->SetFVF(FVF);
}

void APIENTRY hkIDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	m_pD3Ddev->SetGammaRamp(iSwapChain,Flags, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData) 
{
	return m_pD3Ddev->SetIndices(pIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9 *pLight) 
{
	return m_pD3Ddev->SetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9 *pMaterial) 
{	
	return m_pD3Ddev->SetMaterial(pMaterial);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetNPatchMode(float nSegments) 
{	
	return m_pD3Ddev->SetNPatchMode(nSegments);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries) 
{
	return m_pD3Ddev->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader) 
{
	return m_pD3Ddev->SetPixelShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) 
{
	return m_pD3Ddev->SetRenderState(State, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) 
{
	return m_pD3Ddev->SetRenderTarget(RenderTargetIndex,pRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) 
{
	return m_pD3Ddev->SetSamplerState(Sampler,Type,Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetScissorRect(CONST RECT* pRect) 
{
	return m_pD3Ddev->SetScissorRect(pRect);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) 
{
	return m_pD3Ddev->SetSoftwareVertexProcessing(bSoftware);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) 
{
	if(StreamNumber == 0)
	m_Stride = Stride;
	return m_pD3Ddev->SetStreamSource(StreamNumber, pStreamData,OffsetInBytes, Stride);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Divider)
{	
	return m_pD3Ddev->SetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9 *pTexture) 
{
#ifdef D3DHOOK_TEXTURES
	IDirect3DDevice9 *dev = NULL;
	if(pTexture != NULL && ((hkIDirect3DTexture9*)(pTexture))->GetDevice(&dev) == D3D_OK)
	{
		if(dev == this)
			return m_pD3Ddev->SetTexture(Stage, ((hkIDirect3DTexture9*)(pTexture))->m_D3Dtex);
	}
#endif
	
	return m_pD3Ddev->SetTexture(Stage, pTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) 
{
	return m_pD3Ddev->SetTextureStageState(Stage, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->SetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) 
{
	return m_pD3Ddev->SetVertexDeclaration(pDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader) 
{
	return m_pD3Ddev->SetVertexShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9 *pViewport) 
{
	ScreenCenterX = ( float )pViewport->Width / 2;
	ScreenCenterY = ( float )pViewport->Height / 2;
	return m_pD3Ddev->SetViewport(pViewport);
}

BOOL APIENTRY hkIDirect3DDevice9::ShowCursor(BOOL bShow) 
{
	return m_pD3Ddev->ShowCursor(bShow);
}

HRESULT APIENTRY hkIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) 
{
	return m_pD3Ddev->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);
}

HRESULT APIENTRY hkIDirect3DDevice9::TestCooperativeLevel() 
{
	return m_pD3Ddev->TestCooperativeLevel();
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) 
{
	return m_pD3Ddev->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture) 
{
	return m_pD3Ddev->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::ValidateDevice(DWORD *pNumPasses) 
{
	return m_pD3Ddev->ValidateDevice(pNumPasses);
}



//---------- M E N U ------------------------------------------------------------------------------------//

//--------------
ID3DXLine *pLiner;
bool FontCreated = false; //For creating the font once
bool do_once = false;
LPD3DXFONT m_font; //You can make as many fonts as you want using different names

//=======================
bool backupfuncs = false;
//=======================

struct basehookcvar_s
{
	float menu;
};
basehookcvar_s cvar;

struct basehookmenu_s
{
	char title[16];
	float value;
	float min;
	float max;
	float step;
};

int menuIndex = 0;
int menuItems = 34;// When adding more thing's/feat's, always make sure to update this
basehookmenu_s menu[34];//Along w/ this also =P

float menu0;
float menu1;
float menu2;
float menu3;
float menu4;
float menu5;
float menu6;
float menu7;
float menu8;
float menu9;
float menu10;
float menu11;
float menu12;
float menu13;
float menu14;
float menu15;
float menu16;
float menu17;
float menu18;
float menu19;
float menu20;
float menu21;
float menu22;
float menu23;
float menu24;
float menu25;
float menu26;
float menu27;
float menu28;
float menu29;
float menu30;
float menu31;
float menu32;
float menu33;

void ValueInit()
{
	menu0 = 0;
	menu1 = 0;
	menu2 = 0;
	menu3 = 0;
	menu4 = 0;
	menu5 = 0;
	menu6 = 0;
	menu7 = 0;
	menu8 = 0;
	menu9 = 0;
	menu10 = 0;
	menu11 = 0;
	menu12 = 0;
	menu13 = 0;
	menu14 = 0;
	menu15 = 0;
	menu16 = 0;
	menu17 = 0;
	menu18 = 0;
	menu19 = 0;
	menu20 = 0;
	menu21 = 0;
	menu22 = 0;
	menu23 = 0;
	menu24 = 0;
	menu25 = 0;
	menu26 = 0;
	menu27 = 0;
	menu28 = 0;
	menu29 = 0;
	menu30 = 0;
	menu31 = 0;
	menu32 = 0;
	menu33 = 0;
}


void InitMenu()
{

	strcpy( menu[0].title, ">Infantry Set" );
	 menu[0].value = menu0;
	menu[0].min = 0;
	menu[0].max = 1;
	menu[0].step = 1;
	
	strcpy( menu[1].title, ">PlayerCHAMS" );
	 menu[1].value = menu1;
	menu[1].min = 0;
	menu[1].max = 1;
	menu[1].step = 1;

	
	strcpy( menu[2].title, ">VehicleCHAMS" );
	menu[2].value = menu2;
	menu[2].min = 0;
	menu[2].max = 1;
	menu[2].step = 1;

	
	strcpy( menu[3].title, ">MiscCHAMS" );
	menu[3].value = menu3;
	menu[3].min = 0;
	menu[3].max = 1;
	menu[3].step = 1;

	strcpy( menu[4].title, ">ExplosiveCHAMS" );
	menu[4].value = menu4;
	menu[4].min = 0;
	menu[4].max = 1;
	menu[4].step = 1;

	strcpy( menu[5].title, ">WeaponCHAMS" );
	menu[5].value = menu5;
	menu[5].min = 0;
	menu[5].max = 1;
	menu[5].step = 1;

	strcpy( menu[6].title, ">Wireframe" );
	menu[6].value = menu6;
	menu[6].min = 0;
	menu[6].max = 1;
	menu[6].step = 1;

	strcpy( menu[7].title, ">Asuswallhack" );
	menu[7].value = menu7;
	menu[7].min = 0;
	menu[7].max = 1;
	menu[7].step = 1;

	strcpy( menu[8].title, ">No Sky" );
	menu[8].value = menu8;
	menu[8].min = 0;
	menu[8].max = 1;
	menu[8].step = 1;

	strcpy( menu[9].title, ">No Scope" );
	 menu[9].value = menu9;
	menu[9].min = 0;
	menu[9].max = 1;
	menu[9].step = 1;

	strcpy( menu[10].title, ">Viewdistance" );
	menu[10].value = menu10;
	menu[10].min = 0;
	menu[10].max = 1;
	menu[10].step = 1;

	strcpy( menu[11].title, ">No EMB blur" );
	 menu[11].value = menu11;
	menu[11].min = 0;
	menu[11].max = 1;
	menu[11].step = 1;

	strcpy( menu[12].title, ">No Breathing" );
	 menu[12].value = menu12;
	menu[12].min = 0;
	menu[12].max = 1;
	menu[12].step = 1;

	strcpy( menu[13].title, ">No Rain" );
	 menu[13].value = menu13;
	menu[13].min = 0;
	menu[13].max = 1;
	menu[13].step = 1;

	strcpy( menu[14].title, ">No Fog" );
	 menu[14].value = menu14;
	menu[14].min = 0;
	menu[14].max = 1;
	menu[14].step = 1;

	strcpy( menu[15].title, ">No Water" );
	 menu[15].value = menu15;
	menu[15].min = 0;
	menu[15].max = 1;
	menu[15].step = 1;

	strcpy( menu[16].title, ">NameESP" );
	 menu[16].value = menu16;
	menu[16].min = 0;
	menu[16].max = 1;
	menu[16].step = 1;

	strcpy( menu[17].title, ">DistanceESP" );
	 menu[17].value = menu17;
	menu[17].min = 0;
	menu[17].max = 1;
	menu[17].step = 1;

	strcpy( menu[18].title, ">DirectionESP" );
	 menu[18].value = menu18;
	menu[18].min = 0;
	menu[18].max = 1;
	menu[18].step = 1;

	strcpy( menu[19].title, ">DeploysESP" );
	 menu[19].value = menu19;
	menu[19].min = 0;
	menu[19].max = 1;
	menu[19].step = 1;

	strcpy( menu[20].title, ">HealthESP" );
	 menu[20].value = menu20;
	menu[20].min = 0;
	menu[20].max = 1;
	menu[20].step = 1;

	strcpy( menu[21].title, ">ClassESP" );
	 menu[21].value = menu21;
	menu[21].min = 0;
	menu[21].max = 1;
	menu[21].step = 1;

	strcpy( menu[22].title, ">BodyESP" );
	 menu[22].value = menu22;
	menu[22].min = 0;
	menu[22].max = 1;
	menu[22].step = 1;

	strcpy( menu[23].title, ">VeteranESP" );
	 menu[23].value = menu23;
	menu[23].min = 0;
	menu[23].max = 1;
	menu[23].step = 1;

	strcpy( menu[24].title, ">Minimap" );
	 menu[24].value = menu24;
	menu[24].min = 0;
	menu[24].max = 1;
	menu[24].step = 1;

	strcpy( menu[25].title, ">Unlimited Assets" );
	 menu[25].value = menu25;
	menu[25].min = 0;
	menu[25].max = 1;
	menu[25].step = 1;

	strcpy( menu[26].title, ">Custom Xhair" );
	 menu[26].value = menu26;
	menu[26].min = 0;
	menu[26].max = 1;
	menu[26].step = 1;

	strcpy( menu[27].title, ">Unlock Console" );
	 menu[27].value = menu27;
	menu[27].min = 0;
	menu[27].max = 1;
	menu[27].step = 1;
	
	strcpy( menu[28].title, ">Time" );
	 menu[28].value = menu28;
	menu[28].min = 0;
	menu[28].max = 1;
	menu[28].step = 1;

	strcpy( menu[29].title, ">Taunt Spam" );
	menu[29].value = menu29;
	menu[29].min = 0;
	menu[29].max = 1;
	menu[29].step = 1;

	strcpy( menu[30].title, ">SS GUID spoof" );
	menu[30].value = menu30;
	menu[30].min = 0;
	menu[30].max = 1;
	menu[30].step = 1;

	strcpy( menu[31].title, ">Lock Crosshair" );
	menu[31].value = menu31;
	menu[31].min = 0;
	menu[31].max = 1;
	menu[31].step = 1;

	strcpy( menu[32].title, ">SpinHack" );
	menu[32].value = menu32;
	menu[32].min = 0;
	menu[32].max = 1;
	menu[32].step = 1;

	strcpy( menu[33].title, ">Brightness" );
	menu[33].value = menu33;
	menu[33].min = 0;
	menu[33].max = 1;
	menu[33].step = 1;

}

//=============================================
void fillrgba(int x, int y, int w, int h, int r, int g, int b, int a)
{
	D3DXVECTOR2 vLine[2];

	pLiner->SetWidth( w );
	pLiner->SetAntialias( false );
	pLiner->SetGLLines( true );

	vLine[0].x = x + w/2;
	vLine[0].y = y;
	vLine[1].x = x + w/2;
	vLine[1].y = y + h;

	pLiner->Begin( );
	pLiner->Draw( vLine, 2, D3DCOLOR_RGBA( r, g, b, a ) );
	pLiner->End( );
}
//=============================================
void drawopx(int x, int y, int width, int height, int r, int g, int b, int a)
{
	fillrgba(x,y+height,width,1,r,g,b,a);
	fillrgba(x-1,y,1,height,r,g,b,a);
	fillrgba(x,y-1,width,1,r,g,b,a);
	fillrgba(x+width,y,1,height,r,g,b,a);
}
//=============================================

void drawguibox(int x, int y, int w, int h, int r, int g, int b, int a,int rr, int gg, int bb, int aa)
{
	drawopx(x,y,w,h    ,r ,g ,b ,a);        // Body outline
	fillrgba(     x,y,w,h    ,rr,gg,bb,aa);        // Body color
}

//=============================================

void DrawConString(int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...)
{
	LPD3DXFONT Font = m_font;
	D3DCOLOR fontColor = D3DCOLOR_ARGB(Alpha, Red, Green, Blue);  
	RECT rct;
	rct.left=x; rct.top=y; rct.right=rct.left+1000; rct.bottom=rct.top+1000;
	va_list va_alist;
	char logbuf[256] = {0};
	va_start (va_alist, text);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), text, va_alist);
	va_end (va_alist);
	Font->DrawText(NULL, logbuf, -1, &rct, 0, fontColor );   
}

void DoKeyCheck()
{
	if(GetAsyncKeyState(0x2E)&1) //delete
	{
		PlaySound (".\\Sounds\\menu_toggle.wav", NULL, SND_ASYNC);
		bMenuInited = true;
		cvar.menu=!cvar.menu;
	}

	if( cvar.menu)
	{
		if(GetAsyncKeyState(0x26)&1) // up arrow
		{
			PlaySound (".\\Sounds\\menu.wav", NULL, SND_ASYNC);
			if( menuIndex>0 ) menuIndex--;
		}
		if(GetAsyncKeyState(0x28)&1) //downarrow
		{
			PlaySound (".\\Sounds\\menu.wav", NULL, SND_ASYNC);
			if( menuIndex<menuItems-1 ) menuIndex++;
		}

		if(GetAsyncKeyState(0x25)&1) //leftarrow
		{

			if(menuIndex == 0)
			{
				PlayerChams = false;
				VehicleChams = false;
				MiscChams = false;
				Xplosives = false;
				ColoredSky = false;
				Wireframe = false;
				Asuswallhack = false;
				WeaponChams = false;
				NoScope = false; 
				View = false;
				Nobreathing = false;
				NoFog = false;
				NoWater = false;
				Minimap = false;
				Nametags = false;
				Health = false;
				MeterTags = false;
				DirectionIcons = false;
				classESP = false;
				bodyESP = false;
				veteranESP = false;
				NoRain = false;
				Assets = false;
				EMPblur = false;
				Xhair = false;
				Spam = false;
				Time = false;
				Spoofer = false;
				LockXhair = false;
				SpinHack = false;
				Brightness = false;

				menu0 = 0;

			}

			if(menuIndex == 1)
			{
				PlayerChams = false;
				menu1 = 0;
			}

			if(menuIndex == 2)
			{
				VehicleChams = false;
				menu2 = 0;
			}

			if(menuIndex == 3)
			{
				MiscChams = false;
				menu3 = 0;
			}

			if(menuIndex == 4)
			{
				Xplosives = false;
				menu4 = 0;
			}

			if(menuIndex == 5)
			{
				WeaponChams = false;
				menu5 = 0;
			}

			if(menuIndex == 6)
			{
				Wireframe = false;
				menu6 = 0;
			}

			if(menuIndex == 7)
			{
				Asuswallhack = false;
				menu7 = 0;
			}

			if(menuIndex == 8)
			{
				ColoredSky = false;
				menu8 = 0;
			}

			if(menuIndex == 9)
			{
				NoScope = false;
				menu9 = 0;
			}

			if(menuIndex == 10)
			{
				View = false; 
				menu10 = 0;
			}

			if(menuIndex == 11)
			{
				EMPblur = false; 
				menu11 = 0;
			}

			if(menuIndex == 12)
			{
				Nobreathing = false;
				menu12 = 0;
			}

			if(menuIndex == 13) 
			{
				NoRain = false; 
				menu13 = 0;
			} 

			if(menuIndex == 14) 
			{
				NoFog = false; 
				menu14 = 0;
			} 

			if(menuIndex == 15) 
			{
				NoWater = false; 
				menu15 = 0;
			} 

			if(menuIndex == 16) 
			{
				Nametags = false; 
				menu16 = 0;
			} 

			if(menuIndex == 17) 
			{
				MeterTags = false; 
				menu17 = 0;
			} 

			if(menuIndex == 18) 
			{
				DirectionIcons = false; 
				menu18 = 0;
			} 

			if(menuIndex == 19) 
			{
				DeployESP = false; 
				menu19 = 0;
			} 

			if(menuIndex == 20) 
			{
				Health = false; 
				menu20 = 0;
			} 

			if(menuIndex == 21) 
			{
				classESP = false; 
				menu21 = 0;
			}

			if(menuIndex == 22) 
			{
				bodyESP = false; 
				menu22 = 0;
			}

			if(menuIndex == 23) 
			{
				veteranESP = false; 
				menu23 = 0;
			}

			if(menuIndex == 24) 
			{
				Minimap = false; 
				menu24 = 0;
			} 

			if(menuIndex == 25) 
			{
				Assets = false; 
				menu25 = 0;
			} 

			if(menuIndex == 26) 
			{
				Xhair = false; 
				menu26 = 0;
			}

			if(menuIndex == 27) 
			{
				ConsoleUnlock = false; 
				menu27 = 0;
			} 

			if(menuIndex == 28)
			{
				Time = false;
				menu28 = 0;
			}
			
			if(menuIndex == 29)
			{
				Spam = false;
				menu29 = 0;
			}

			if(menuIndex == 30)
			{
				Spoofer = false;
				menu30 = 0;
			}

			if(menuIndex == 31)
			{
				LockXhair = false;
				menu31 = 0;
			}

			if(menuIndex == 32)
			{
				SpinHack = false;
				menu32 = 0;
			}
			if(menuIndex == 33)
			{
				Brightness = false;
				menu33 = 0;
			}
		}

		if(GetAsyncKeyState(0x27)&1) //rightarrow
		{
						
			if(menuIndex == 0)
			{
				PlayerChams = true;
				VehicleChams = true;
				MiscChams = true;
				Xplosives = true;
				ColoredSky = false;
				Wireframe = false;
				Asuswallhack = false;
				WeaponChams = true;
				NoScope = true; 
				View = true;
				Nobreathing = true;
				NoFog = true;
				NoWater = true;
				Minimap = true;
				Nametags = true;
				MeterTags = true;
				classESP = false;
				bodyESP = true;
				veteranESP = false;
				Health = true;
				DirectionIcons = true;
				DeployESP = true;
				NoRain = false;
				Assets = false;
				EMPblur = false;
				ConsoleUnlock = false;
				Time = false;
				Xhair = true;
				Spam = false;
				Spoofer = false;
				LockXhair = false;
				SpinHack = false;
				Brightness = false;

				menu0 = 1; 
			}

			if(menuIndex == 1)
			{
				PlayerChams = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu1 = 1;
			}

			if(menuIndex == 2)
			{
				VehicleChams = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu2 = 1;
			}

			if(menuIndex == 3)
			{
				MiscChams = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu3 = 1;
			}

			if(menuIndex == 4)
			{
				Xplosives = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu4 = 1;
			}

			if(menuIndex == 5)
			{
				WeaponChams = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu5 = 1;
			}

			if(menuIndex == 6)
			{
				Wireframe = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu6 = 1;
			}

			if(menuIndex == 7)
			{
				Asuswallhack = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu7 = 1;
			}

			if(menuIndex == 8)
			{
				ColoredSky = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu8 = 1;
			}

			if(menuIndex == 9)
			{
				NoScope = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu9 = 1;
			}

			if(menuIndex == 10)
			{
				if(View = true) 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu10 = 1;
			}

			if(menuIndex == 11)
			{
				EMPblur = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu11 = 1;
			}

			if(menuIndex == 12)
			{
				Nobreathing = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu12 = 1;
			}

			if(menuIndex == 13) 
			{
				NoRain = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu13 = 1;
			} 

			if(menuIndex == 14) 
			{
				NoFog = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu14 = 1;
			} 

			if(menuIndex == 15) 
			{
				NoWater = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu15 = 1;
			} 

			if(menuIndex == 16) 
			{
				Nametags = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu16 = 1;
			} 

			if(menuIndex == 17) 
			{
				MeterTags = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu17 = 1;
			} 

			if(menuIndex == 18) 
			{
				DirectionIcons = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu18 = 1;
			}

			if(menuIndex == 19) 
			{
				DeployESP = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu19 = 1;
			}


			if(menuIndex == 20) 
			{
				Health = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu20 = 1;
			} 

			if(menuIndex == 21) 
			{
				classESP = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu21 = 1;
			}

			if(menuIndex == 22) 
			{
				bodyESP = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu22 = 1;
			} 

			if(menuIndex == 23) 
			{
				veteranESP = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu23 = 1;
			} 

			if(menuIndex == 24) 
			{
				Minimap = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu24 = 1;
			} 

			if(menuIndex == 25) 
			{
				Assets = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu25 = 1;
			} 

			if(menuIndex == 26) 
			{
				Xhair = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu26 = 1;
			}

			if(menuIndex == 27) 
			{
				ConsoleUnlock = true; 
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu27 = 1;
			} 

			if(menuIndex == 28)
			{
				Time = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu28 = 1;
			}
			
			if(menuIndex == 29)
			{
				Spam = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu29 = 1;
			}

			if(menuIndex == 30)
			{
				Spoofer = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu30 = 1;
			}

			if(menuIndex == 31)
			{
				LockXhair = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu31 = 1;
			}

			if(menuIndex == 32)
			{
				SpinHack = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu32 = 1;
			}
			if(menuIndex == 33)
			{
				Brightness = true;
				PlaySound (".\\Sounds\\activated.wav", NULL, SND_ASYNC);
				menu33 = 1;
			}
		}
	}
}

void __stdcall CustomPresent(IDirect3DDevice9* pDevice)
{
	//Menu
	if( cvar.menu )
	{
		int x = 60;    
		int y = 135;

		InitMenu();

		int r, g, b;
		int mr, mg, mb;
		int menur, menug, menub;
//Color of the Menu Text thats highlighted
		r = 255;
		g = 4;
		b = 4;
//Color of the Menu Text
		mr = 189;
		mg = 238;
		mb = 20;
//Fill Color of the Menu Box
		menur = 68;
		menug = 67;
		menub = 67;

		drawguibox(x - 10, y - 5, 170, (y + 3) + (11.6*menuItems) + 4, 86, 85, 85, 255, menur, menug, menub, 100); //Boundary
		
		drawguibox(x - 7, y - -16, 163, (y - 0) + (365) + 8, 000, 000, 000, 255, menur, menug, menub, 120); //Boundary
		
		DrawConString( x + 25, y, 10, 20, 250, 250, "[ BF2142 HaXx ]" );
		for(int i=0;i<menuItems;i++)
		{
			if( i!=menuIndex )
			{
				DrawConString( x, (y + 15) + (15*i), mr, mg, mb, 255,  menu[i].title );
				DrawConString( x + 135, (y + 15) + (15*i), mr, mg, mb, 255, "%2.0f", menu[i].value );
			}
			else
			{
				DrawConString( x, (y + 15) + (15*i), r, g, b, 255, menu[i].title );
				DrawConString( x + 135, (y + 15) + (15*i), r, g, b, 255, "%2.0f", menu[i].value );
			}
		}
	}
	DoKeyCheck();

}


void __stdcall CustomInitialize(IDirect3DDevice9* pDevice) 
{
	if(!do_once)
	{
		ValueInit();
		D3DXCreateLine(pDevice, &pLiner); 
		if(!FontCreated)//Create the font (only once!)
		{
			FontCreated=true; //Makes it do it only once
			D3DXCreateFont(pDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_font );
		}
		do_once = true;
	}
}


