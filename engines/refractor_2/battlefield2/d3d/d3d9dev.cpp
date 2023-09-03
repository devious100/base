/*	Direct3D9 Device */

#include "../ogoid++.h"
#include "../cheats/cheats.h"
#include "d3d9dev.h"
#include "d3dhook.h"
#include <time.h>
#include <D3dx9tex.h>

//#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx9d.lib")



/*LPDIRECT3DTEXTURE9 chamsTextures[2];

const BYTE red_texture[60] = { 0x42, 0x4D, 0x3C, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 
0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
0x01,0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x0B, 
0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 
0x00, 0x01 };

const BYTE yellow_texture[60] = { 0x42, 0x4D, 0x3C, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 
0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x0B, 
0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 
0x00, 0x01 };


// those variables i used to log vertex and primitive count when developing
// the chams
int num_vertex=0;
BOOL VK_HOME_state=FALSE;
BOOL VK_END_state=FALSE;
int range_vextex = 0;
BOOL VK_INSERT_state=FALSE;
BOOL VK_DELETE_state=FALSE;*/

//---------Memory Hacks Variables----------------------------------
BOOL bbNameTags, bbBars, bbIcons, bbNoFog, bbDistanceTags, bbMinimap, bbMap, bbWeaponUnlocks, bbEnemyMines, bbBridgeIcons, bbFlash, bbCommands, bbShellShock, bbNoTVGuidedStatic, bbCommander, bbSquadleader, bbPanel, bbViewDistance;
BOOL bBNameTags, bBBars, bBIcons, bBNoFog, bBDistanceTags, bBMinimap, bBMap, bBWeaponUnlocks, bBEnemyMines, bBBridgeIcons, bBFlash, bBCommands, bBShellShock, bBNoTVGuidedStatic, bBCommander, bBSquadleader, bBPanel, bBViewDistance;

bool bbNoSky = false; 
bool bbNoWater = false; 
bool bbNoFogo = false; 
bool bbNoScope = false;
bool bbNoTankOverlay = false;
bool bbChams = false; 
bool bbWireframe = false; 
bool bbVehicleChams = false; 
bool bbGroundWeaponChams = false;
bool bbMobileObjects = false; 
bool bbAssetChams = false;
bool bbWeaponChams = false; 
bool bbMineChams = false; 
bool bbAsusWallhack = false;
bool bbWhitewalls = false; 
bool bbTime = false; 
bool bbFPS = false;

BOOL ScrIniter = FALSE;


bool OffsetInit;

HANDLE hand;

BYTE original_team = 0;
BYTE original_commander = 0;
DWORD RendDx9Base;
DWORD BF2Base;
bool IniChecker = false;

BOOL CheatsAllower = FALSE;

bool bMenuInited = false;
const BYTE HLDistanceDefault[4] = {0x00, 0x00, 0xF0, 0x42};
const BYTE HLDistance400[4] = {0x00, 0x00, 0xC8, 0x43};
const BYTE HLDistance800[4] = {0x00, 0x00, 0x48, 0x44};
const BYTE HLDistance1200[4] = {0x00, 0x00, 0x96, 0x44};
const BYTE HLDistance10000[4] = {0x00, 0x40, 0x1C, 0x46};
const BYTE HLDistance50000[4] = {0x00, 0x50, 0x43, 0x47};

const BYTE NameTags1[6] = {0x0F, 0x85, 0x18, 0x01, 0x00, 0x00};		
BYTE NameTags1Orig[6] = {0};

const BYTE NameTag[6] = {0x0F, 0x84, 0xF3, 0x00, 0x00, 0x00};
BYTE NameTagOrig[6] = {0};

const BYTE Distance[2] = {0x75, 0x0C};
BYTE DistanceOrig[2] ={0};

const BYTE MedicIcon[2] = {0x74, 0x34};				
BYTE MedicIconOrig[2] = {0};

const BYTE SupportIcon[2] = {0x74,0x0C};				
BYTE SupportIconOrig[2] = {0};

const BYTE EngineerIcon[2] = {0x74,0x0C};				
BYTE EngineerIconOrig[2] = {0};

const BYTE NoFog[3] = {0xD9, 0x46, 0x37};	
BYTE NoFogOrig[3] = {0};

const BYTE Nop6Bytes[6] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
const BYTE Nop4Bytes[4] = {0x90, 0x90, 0x90, 0x90};
const BYTE Nop2Bytes[2] = {0x90, 0x90};

BYTE Health1Orig[6] = {0};
BYTE Health2Orig[6] = {0};
BYTE Health3Orig[6] = {0};

BYTE Minimap1[2] = {0};
BYTE Minimap2[2] = {0};
BYTE Minimap3[6] = {0};
BYTE Minimap4[2] = {0};
BYTE Minimap5[2] = {0};
BYTE Minimap6[2] = {0};
BYTE Minimap7[2] = {0};

BYTE WeaponsUnlocker1[2] = {0};
BYTE WeaponsUnlocker2[2] = {0};

BYTE MinimapKits1[2] = {0};
BYTE EnemyMines1[2] = {0};
BYTE Shellshock1[2] = {0};
BYTE FlashBang1[6] = {0};
BYTE RadarIcons1[2] = {0};
BYTE dMap1[2] = {0};
BYTE UnauthorizedCommands1[2] = {0};
BYTE UnauthorizedCommands2[2] = {0};
BYTE NoTVStatic1[6] = {0};

BYTE dwSuppliesOrig;
BYTE dwArtilleryOrig;
BYTE dwVehiclesOrig;

BYTE dwPanelFiller = 0x00;

DWORD dwHealth1; // RendDx9Base + 0x1B21CD;
DWORD dwHealth2; // RendDx9Base + 0x1B22FE;
DWORD dwHealth3; // RendDx9Base + 0x1B234C;
DWORD dwDistance; // RendDx9Base + 0x12EDF3;
DWORD dwNameTags; // RendDx9Base + 0x12D85D;
DWORD dwNameTags2; // RendDx9Base + 0x12D873;
DWORD dwMedicIcon; // RendDx9Base + 0x12CD1A;
DWORD dwSupportIcon; // RendDx9Base + 0x12CD80;
DWORD dwEngineerIcon; // RendDx9Base + 0x12CD61;
DWORD dwMinimap1; // BF2Base + 0x364F99;
DWORD dwMinimap2; // BF2Base + 0x364FBF;
DWORD dwMinimap3; // BF2Base + 0x364FE5;
DWORD dwMinimap4; // BF2Base + 0x365075;
DWORD dwMinimap5; // BF2Base + 0x36509B;
DWORD dwMinimap6; // BF2Base + 0x3650BE;
DWORD dwMinimap7; // BF2Base + 0x36FFC9;
DWORD dwWeaponsUnlocker1; // BF2Base + 0x5B96E;
DWORD dwWeaponsUnlocker2; // BF2Base + 0xEABE5;
DWORD dwMinimapKits; // RendDx9Base + 0x363A50;
DWORD dwEnemyMines; // RendDx9Base + 0x12D521;
DWORD dwFlashBang; // BF2Base + 0x25E6F4;
DWORD dwShellshock; // RendDx9Base + 0x4EC65;
DWORD dwRadarIcons; // RendDx9Base + 0x12F489;
DWORD dwThdMap; // RendDx9Base + 0x0012D1CD;
DWORD dwUnauthorizedCommands1; // BF2Base + 0x28E160;
DWORD dwUnauthorizedCommands2; // BF2Base + 0x28E178;
DWORD dwNoTVGuidedStatic; // BF2Base + 0x377B6E;
//-----------------------------------------------------------------
void fCommander(BOOL enable);
void fTeam();
void fPanel(BOOL enable);
void fPanelFullMaker();
void fSquadLeader(BOOL enable);
void fSquad0();
void fSquad1();
void fSquad2();
void fSquad3();
void fSquad4();
void fSquad5();
void fSquad6();
void fSquad7();
void fSquad8();
void fSquad9();
void fNameTags(BOOL enable);
void fBars(BOOL enable);
void fNameTagsDistance(int iDistance);
void fIcons(BOOL enable);
void fCCommander();
void fNoFog(BOOL enable);
void fDistanceTags(BOOL enable);
void fMinimap(BOOL enable);
void fMinimapKits(BOOL enable);
void fWeaponUnlocks(BOOL enable);
void fEnemyMines(BOOL enable);
void fRadarBridgeIcons(BOOL enable);
void fFlash(BOOL enable);
void fLockXhair(BOOL enable);
void fViewDistance(BOOL enable);
void fZoom(BOOL enable);
void fUnauthorizedCommands(BOOL enable);
void fShellShock(BOOL enable);
void fNoTVGuidedStatic(BOOL enable);


void ValueInit();
void InitMenu();
void fillrgba(int x, int y, int w, int h, int r, int g, int b, int a);
void drawopx(int x, int y, int width, int height, int r, int g, int b, int a);
void drawguibox(int x, int y, int w, int h, int r, int g, int b, int a,int rr, int gg, int bb, int aa);
void DrawConString(int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...);
void DoKeyCheck();
void __stdcall CustomPresent(IDirect3DDevice9* pDevice);
void __stdcall CustomInitialize(IDirect3DDevice9* pDevice);

//---------Additional Tweaks---------------------------------------
//FPS Viewer
float fFps = 0.0f;
float fLastTick = 0.0f;
float fCurrentTick = clock() * 0.001f;
char  chFrames[50] = {0};
bool FPS;

//Time Viewer
char Timestruct[16] = "hh':'mm':'ss tt";      
char TimeString[25];     
bool Time;
//-----------------------------------------------------------------

//---------Class Pointers------------------------------------------
//MemoryCl		*pMem	= new MemoryCl();
bool Checker = false;
//-----------------------------------------------------------------

//---------Declares----------------------------------------------------
float original_viewdist;
float patched_viewdist;
//---------------------------------------------------------------------

//---------Declares------------------------------------------------
unsigned int m_Stride; 
bool color2 = false;
//-----------------------------------------------------------------

//---------API Hooking---------------------------------------------
bool bDetourRemoved = false;
extern FARPROC fpDirect3D;
//-----------------------------------------------------------------

//---------ViewPort Declares---------------------------------------
D3DVIEWPORT9 Viewport;
//-----------------------------------------------------------------

//----------Crosshair----------------------------------------------
D3DXVECTOR2 CrosshairX[2], CrosshairY[2];
ID3DXLine *pLine;
D3DXVECTOR2 XhairUp       [1];
D3DXVECTOR2 XhairDwn      [1];
D3DXVECTOR2 XhairLft      [1];
D3DXVECTOR2 XhairRgt      [1];
bool XHairIni = false;
//-----------------------------------------------------------------

//----------Color Declares-----------------------------------------
LPDIRECT3DTEXTURE9    Blue;  
LPDIRECT3DTEXTURE9    Red;  
LPDIRECT3DTEXTURE9    Yellow;  
LPDIRECT3DTEXTURE9    Green;  
LPDIRECT3DTEXTURE9    Orange;  
LPDIRECT3DTEXTURE9    Purple;  
LPDIRECT3DTEXTURE9    Pink;
LPDIRECT3DTEXTURE9    Brown;
LPDIRECT3DTEXTURE9    White;
D3DCOLOR dWhite = D3DCOLOR_XRGB( 255, 255, 255 );
D3DCOLOR dGreen = D3DCOLOR_XRGB( 0, 255, 0 );
D3DCOLOR dOrange = D3DCOLOR_XRGB( 255, 127, 0 );
D3DCOLOR dRed = D3DCOLOR_XRGB(255, 0, 0);
//-----------------------------------------------------------------

//---------Menu----------------------------------------------------
ID3DXFont *Font1, *Font2, *TimeFont;

typedef struct
{
	bool isOn;
	char * name;
} menuItem;

menuItem Menu[] = {
	{false, "[ IK Platinum ]"},
	{false, "Name Tags"},
	{false, "Distance Tags"},
	{false, "Health Bars"},
	{false, "Kit Icons"},
	{false, "Mini Map"},
	{false, "3D Map"},
	{false, "Weapon Unlocks"},
	{false, "Enemy Mines"},
	{false, "Radar & Bridge Icons"},
	{false, "ShellShock Removal"},
	{false, "Flash Removal"},
	{false, "TV Guided Static Removal"},
	{false, "Weapon Chams"},
	{false, "ASUS Wallhack"},
	{false, "WhiteWalls"},
	{false, "Unauthorized Commands"},
	{false, "XQZ Wallhack"},
	{false, "Wireframe"},
	{false, "Custom Xhair"},
	{false, "Player Chams"},
	{false, "Vehicle Chams"},
	{false, "View Distance"},
	{false, "Mobile Object Chams"},
	{false, "Commander Asset Chams"},
	{false, "Mine Chams"},
	{false, "No Fog"},
	{false, "No Water"},
	{false, "No Sky"},
	{false, "No Scope"},
	{false, "No Tank Overlay"},
	{false, "Force Commander"},
	{false, "Force Squad Leader"},
	{false, "Team Switch"},
	{false, "Unlimited Assets"},
	{false, "Squad Jumping"},
	{false, "AimBot"},
	{false, "Display Time"},
	{false, "Display FPS"},
};
int menuSize = 36;
int currentSelected = 1;
int MenuPosX = 35;
int MenuPosY = 20;
int iTextSize = 11;
int iTextSpace = 1;
bool bMenuOn = false;


//-----------------------------------------------------------------

//---------Hack Functions------------------------------------------
bool Chams;
bool Wallhack;
bool Wireframe;
bool VehicleChams;
bool AssetChams;
bool MobileObjects;
bool GroundWeaponChams;
bool MineChams;
bool SquadJumping;
bool UnlimitedAssets;
bool NoFogo;
bool NoWater;
bool NoSky;
bool NoScope;
bool NoTankOverlay;
bool NoGuidedMissileStatic;
bool bAimbot;
bool WeaponChams;
bool bbbCommander;
bool Xhair;
bool bbLeader;
bool Zoom;
bool AsusWallhack;
bool Whitewalls;

BOOL bChams;
BOOL bWallhack;
BOOL bWireframe;
BOOL bVehicleChams;
BOOL bAssetChams;
BOOL bMobileObjects;
BOOL bGroundWeaponChams;
BOOL bMineChams;
BOOL bSquadJumping;
BOOL bUnlimitedAssets;
BOOL bNoFog;
BOOL bNoWater;
BOOL bNoSky;
BOOL bNoScope;
BOOL bNoTankOverlay;
BOOL bNoGuidedMissileStatic;
BOOL bbAimbot;
BOOL bWeaponChams;
BOOL bCommander;
BOOL bXhair;
BOOL bbbLeader;
BOOL bZoom;
BOOL bAsusWallhack;
BOOL bWhitewalls;


#define MEC_PROC_VEHICLES_ONE (m_Stride == 28 && (NumVertices == 5026 && primCount == 1599) || (NumVertices == 10502 && primCount == 3849) || (NumVertices == 5172 && primCount == 1738) || (NumVertices == 636 && primCount == 200) || (NumVertices == 622 && primCount == 200) || (NumVertices == 7670 && primCount == 2786 && startIndex == 5727))
#define MEC_PROC_VEHICLES_TWO (m_Stride == 24 && (NumVertices == 3796 && primCount == 2179) || (NumVertices == 2691 && primCount == 1484) || (NumVertices == 493 && primCount == 191) || (NumVertices == 381 && primCount == 216) || (NumVertices == 593 && primCount == 370) || (NumVertices == 4674 && primCount == 2643) || (NumVertices == 2522 && primCount == 1304) || (NumVertices == 546 && primCount == 311) || (NumVertices == 151 && primCount == 128) || (NumVertices == 4799 && primCount == 2468) || (NumVertices == 2350 && primCount == 1125) || (NumVertices == 554 && primCount == 298) || (NumVertices == 68 && primCount == 39) || (NumVertices == 6225 && primCount == 3893) || (NumVertices == 3280 && primCount == 1633) || (NumVertices == 448 && primCount == 246) || (NumVertices == 5374 && primCount == 2845) || (NumVertices == 382 && primCount == 158 && startIndex == 29337))
#define MEC_PROC_VEHICLES_THREE (m_Stride == 24 && (NumVertices == 51 && primCount == 38) || (NumVertices == 2813 && primCount == 1471) || (NumVertices == 508 && primCount == 288) || (NumVertices == 4162 && primCount == 2357) || (NumVertices == 2608 && primCount == 1096) || (NumVertices == 494 && primCount == 188) || (NumVertices == 9221 && primCount == 2922) || (NumVertices == 4322 && primCount == 1383) || (NumVertices == 1077 && primCount == 336) || (NumVertices == 4561 && primCount == 2574) || (NumVertices == 2604 && primCount == 1351) || (NumVertices == 531 && primCount == 262))
#define MEC_PROC_VEHICLES_FOUR (m_Stride == 28 && (NumVertices == 9861 && primCount == 3136) || (NumVertices == 4677 && primCount == 1493) || (NumVertices == 483 && primCount == 151))
#define MEC_PROC_VEHICLES_FIVE (m_Stride == 24 && (NumVertices == 3669 && primCount == 2245) || (NumVertices == 2036 && primCount == 1116) || (NumVertices == 387 && primCount == 215) || (NumVertices == 593 && primCount == 370) || (NumVertices == 5164 && primCount == 3035) || (NumVertices == 2814 && primCount == 1541) || (NumVertices == 601 && primCount == 273) || (NumVertices == 5441 && primCount == 2634) || (NumVertices == 2864 && primCount == 1104) || (NumVertices == 852 && primCount == 453) || (NumVertices == 5 && primCount == 3) || (NumVertices == 3746 && primCount == 2352) || (NumVertices == 1962 && primCount == 1138) || (NumVertices == 531 && primCount == 270) || (NumVertices == 4708 && primCount == 2624) || (NumVertices == 2241 && primCount == 1245) || (NumVertices == 569 && primCount == 237) || (NumVertices == 2609 && primCount == 1096) || (NumVertices == 4163 && primCount == 2357) || (NumVertices == 5078 && primCount == 2592) || (NumVertices == 8 && primCount == 4 && startIndex == 25332))
#define US_VEHICLES_ONE (m_Stride == 28 && (NumVertices == 8761 && primCount == 2964) || (NumVertices == 4023 && primCount == 1352) || (NumVertices == 1059 && primCount == 333) || (NumVertices == 6364 && primCount == 2333) || (NumVertices == 3878 && primCount == 1251) || (NumVertices == 1083 && primCount == 350))
#define US_VEHICLES_TWO (m_Stride == 24 && (NumVertices == 4835 && primCount == 2786) || (NumVertices == 2945 && primCount == 1553) || (NumVertices == 157 && primCount == 94) || (NumVertices == 501 && primCount == 402) || (NumVertices == 5770 && primCount == 3315) || (NumVertices == 2360 && primCount == 1227) || (NumVertices == 353 && primCount == 188) || (NumVertices == 4096 && primCount == 2361) || (NumVertices == 3092 && primCount == 1663) || (NumVertices == 582 && primCount == 263) || (NumVertices == 291 && primCount == 184) || (NumVertices == 4387 && primCount == 2632) || (NumVertices == 2532 && primCount == 1337) || (NumVertices == 456 && primCount == 230) || (NumVertices == 3748 && primCount == 2177) || (NumVertices == 2708 && primCount == 1371) || (NumVertices == 695 && primCount == 358) || (NumVertices == 291 && primCount == 184) || (NumVertices == 347 && primCount == 298) || (NumVertices == 913 && primCount == 590 && startIndex == 12309) || (NumVertices == 1644 && primCount == 964 && startIndex == 14079) || (NumVertices == 774 && primCount == 440 && startIndex == 16971) || (NumVertices == 542 && primCount == 295 && startIndex == 18291) || (NumVertices == 114 && primCount == 68 && startIndex == 19176) || (NumVertices == 2997 && primCount == 1856 && startIndex == 6741))
#define US_VEHICLES_THREE (m_Stride == 24 && (NumVertices == 5675 && primCount == 3347) || (NumVertices == 2654 && primCount == 1375) || (NumVertices == 319 && primCount == 150) || (NumVertices == 2602 && primCount == 1467) || (NumVertices == 461 && primCount == 251) || (NumVertices == 4504 && primCount == 2682) || (NumVertices == 3884 && primCount == 1658) || (NumVertices == 294 && primCount == 166) || (NumVertices == 2340 && primCount == 1439) || (NumVertices == 1191 && primCount == 658) || (NumVertices == 371 && primCount == 183) || (NumVertices == 48 && primCount == 24 && startIndex == 9474) || (NumVertices == 1323 && primCount == 828 && startIndex == 13926) || (NumVertices == 678 && primCount == 396 && startIndex == 19299) || (NumVertices == 2997 && primCount == 1856 && startIndex == 8358) || (NumVertices == 1637 && primCount == 963 && startIndex == 16410) || (NumVertices == 108 && primCount == 68 && startIndex == 21420) || (NumVertices == 613 && primCount == 311 && startIndex == 20487))
#define LEAVE_OUT_OBJECTS (m_Stride == 52 && (NumVertices == 1322 || NumVertices == 1112 || NumVertices == 1045 || NumVertices == 1578 || (NumVertices == 1073 && primCount == 1384) || (NumVertices == 1409 && primCount == 1581) || (NumVertices == 341 && primCount == 200) || (NumVertices == 1321 && primCount == 1439) || (NumVertices == 1407 && primCount == 1568)))

//--------Recognition Strides For USMC--------------------------------------
#define U1 (m_Stride == 24 && (NumVertices == 4835 && primCount == 2786) || (NumVertices == 2945 && primCount == 1553) || (NumVertices == 157 && primCount == 94) || (NumVertices == 501 && primCount == 402))
#define U2 (m_Stride == 24 && (NumVertices == 5770 && primCount == 3315) || (NumVertices == 2360 && primCount == 1227) || (NumVertices == 353 && primCount == 188) || (NumVertices == 9728 && primCount == 5869) || (NumVertices == 141 && primCount == 80) || (NumVertices == 489 && primCount == 250))
#define U3 (m_Stride == 24 && (NumVertices == 4096 && primCount == 2361) || (NumVertices == 3092 && primCount == 1663) || (NumVertices == 582 && primCount == 263) || (NumVertices == 291 && primCount == 184) || (NumVertices == 4387 && primCount == 2632) || (NumVertices == 2532 && primCount == 1337) || (NumVertices == 456 && primCount == 230) || (NumVertices == 3748 && primCount == 2177) || (NumVertices == 2708 && primCount == 1371) || (NumVertices == 695 && primCount == 358) || (NumVertices == 291 && primCount == 184) || (NumVertices == 347 && primCount == 298))
#define U4 (m_Stride == 24 && (NumVertices == 5675 && primCount == 3347) || (NumVertices == 2654 && primCount == 1375) || (NumVertices == 319 && primCount == 150))
#define U5 (m_Stride == 24 && (NumVertices == 4504 && primCount == 2682) || (NumVertices == 3884 && primCount == 1658) || (NumVertices == 294 && primCount == 166) || (NumVertices == 493 && primCount == 255) || (NumVertices == 678 && primCount == 366))
#define U6 (m_Stride == 24 && (NumVertices == 2602 && primCount == 1467) || (NumVertices == 461 && primCount == 251) || (NumVertices == 48 && primCount == 24 && startIndex== 9474))
#define U7 (m_Stride == 28 && (NumVertices == 8761 && primCount == 2964) || (NumVertices == 4023 && primCount == 1352) || (NumVertices == 1059 && primCount == 333))
#define U8 ((m_Stride == 28 && (NumVertices == 6364 && primCount == 2333) || (NumVertices == 3878 && primCount == 1251) || (NumVertices == 1083 && primCount == 350))) || (m_Stride == 24 && (NumVertices == 493 && primCount == 255) || (NumVertices == 678 && primCount == 366))
#define U9 (m_Stride == 24 && (NumVertices == 2340 && primCount == 1439) || (NumVertices == 1191 && primCount == 658) || (NumVertices == 371 && primCount == 183))
#define U10 (m_Stride == 24 && (NumVertices == 1323 && primCount == 828 && startIndex== 13926) || (NumVertices == 678 && primCount == 396 && startIndex== 19299) || (NumVertices == 2997 && primCount == 1856 && startIndex== 8358) || (NumVertices == 1637 && primCount == 963 && startIndex== 16410) || (NumVertices == 108 && primCount == 68 && startIndex== 21420) || (NumVertices == 613 && primCount == 311 && startIndex== 20487) || (NumVertices == 913 && primCount == 590 && startIndex== 12309) || (NumVertices == 1644 && primCount == 964 && startIndex== 14079) || (NumVertices == 774 && primCount == 440 && startIndex== 16971) || (NumVertices == 542 && primCount == 295 && startIndex== 18291) || (NumVertices == 114 && primCount == 68 && startIndex== 19176) || (NumVertices == 2997 && primCount == 1856 && startIndex== 6741))
//#define USMC_Artillery (m_Stride == 24 && (NumVertices == 3852 && primCount == 2208) || (NumVertices == 1786 && primCount == 854) || (NumVertices == 707 && primCount == 269))


#define All_USMC_Items ((m_Stride == 24 && (NumVertices == 4835 && primCount == 2786) || (NumVertices == 2945 && primCount == 1553) || (NumVertices == 157 && primCount == 94) || (NumVertices == 501 && primCount == 402)) || (m_Stride == 24 && (NumVertices == 5770 && primCount == 3315) || (NumVertices == 2360 && primCount == 1227) || (NumVertices == 353 && primCount == 188) || (NumVertices == 9728 && primCount == 5869) || (NumVertices == 141 && primCount == 80) || (NumVertices == 489 && primCount == 250)) || (m_Stride == 24 && (NumVertices == 4096 && primCount == 2361) || (NumVertices == 3092 && primCount == 1663) || (NumVertices == 582 && primCount == 263) || (NumVertices == 291 && primCount == 184) || (NumVertices == 4387 && primCount == 2632) || (NumVertices == 2532 && primCount == 1337) || (NumVertices == 456 && primCount == 230) || (NumVertices == 3748 && primCount == 2177) || (NumVertices == 2708 && primCount == 1371) || (NumVertices == 695 && primCount == 358) || (NumVertices == 291 && primCount == 184) || (NumVertices == 347 && primCount == 298)) || (m_Stride == 24 && (NumVertices == 5675 && primCount == 3347) || (NumVertices == 2654 && primCount == 1375) || (NumVertices == 319 && primCount == 150)) || (m_Stride == 24 && (NumVertices == 4504 && primCount == 2682) || (NumVertices == 3884 && primCount == 1658) || (NumVertices == 294 && primCount == 166) || (NumVertices == 493 && primCount == 255) || (NumVertices == 678 && primCount == 366)) || (m_Stride == 24 && (NumVertices == 2602 && primCount == 1467) || (NumVertices == 461 && primCount == 251) || (NumVertices == 48 && primCount == 24 && startIndex== 9474)) || (m_Stride == 28 && (NumVertices == 8761 && primCount == 2964) || (NumVertices == 4023 && primCount == 1352) || (NumVertices == 1059 && primCount == 333)) || ((m_Stride == 28 && (NumVertices == 6364 && primCount == 2333) || (NumVertices == 3878 && primCount == 1251) || (NumVertices == 1083 && primCount == 350))) || (m_Stride == 24 && (NumVertices == 493 && primCount == 255) || (NumVertices == 678 && primCount == 366)) || (m_Stride == 24 && (NumVertices == 2340 && primCount == 1439) || (NumVertices == 1191 && primCount == 658) || (NumVertices == 371 && primCount == 183)) || (m_Stride == 24 && (NumVertices == 1323 && primCount == 828 && startIndex== 13926) || (NumVertices == 678 && primCount == 396 && startIndex== 19299) || (NumVertices == 2997 && primCount == 1856 && startIndex== 8358) || (NumVertices == 1637 && primCount == 963 && startIndex== 16410) || (NumVertices == 108 && primCount == 68 && startIndex== 21420) || (NumVertices == 613 && primCount == 311 && startIndex== 20487) || (NumVertices == 913 && primCount == 590 && startIndex== 12309) || (NumVertices == 1644 && primCount == 964 && startIndex== 14079) || (NumVertices == 774 && primCount == 440 && startIndex== 16971) || (NumVertices == 542 && primCount == 295 && startIndex== 18291) || (NumVertices == 114 && primCount == 68 && startIndex== 19176) || (NumVertices == 2997 && primCount == 1856 && startIndex== 6741)))
//--------------------------------------------------------------------------

//--------Recognition Strides For MEC---------------------------------------
#define M1 (m_Stride == 24 && (NumVertices == 4799 && primCount == 2468) || (NumVertices == 2350 && primCount == 1125) || (NumVertices == 554 && primCount == 298) || (NumVertices == 68 && primCount == 39))
#define M2 (m_Stride == 24 && (NumVertices == 6225 && primCount == 3893) || (NumVertices == 3280 && primCount == 1633) || (NumVertices == 448 && primCount == 246) || (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768))
#define M3 (m_Stride == 24 && (NumVertices == 3796 && primCount == 2179) || (NumVertices == 2691 && primCount == 1484) || (NumVertices == 493 && primCount == 191) || (NumVertices == 381 && primCount == 216 ) || (NumVertices == 593 && primCount == 370) || (NumVertices == 827 && primCount == 562) || (NumVertices == 4674 && primCount == 2643) || (NumVertices == 2522 && primCount == 1304) || (NumVertices == 546 && primCount == 311) || (NumVertices == 593 && primCount == 370) || (NumVertices == 151 && primCount == 128))
#define M4 (m_Stride == 24 && (NumVertices == 4163 && primCount == 2357) || (NumVertices == 4162 && primCount == 2357) || (NumVertices == 2608 && primCount == 1096) || (NumVertices == 2609 && primCount == 1096) || (NumVertices == 494 && primCount == 188) || (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768) || (NumVertices == 25 && primCount == 15)) 
#define M5 (m_Stride == 24 && (NumVertices == 5078 && primCount == 2592) || (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768) || (NumVertices == 8 && primCount == 4 && startIndex == 25332) || (NumVertices == 382 && primCount == 158 && startIndex == 29337))
#define M6 (m_Stride == 24 && (NumVertices == 5374 && primCount == 2845) || (NumVertices == 51 && primCount == 38) || (NumVertices == 2813 && primCount == 1471) || (NumVertices == 508 && primCount == 288))
#define M7 ((m_Stride == 24 && (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768)) || (m_Stride == 28 && (NumVertices == 5026 && primCount == 1599) || (NumVertices == 622 && primCount == 200) || (NumVertices == 7670 && primCount == 2786 && startIndex == 5727)))
#define M8 (m_Stride == 28 && (NumVertices == 9221 && primCount == 2922) || (NumVertices == 4322 && primCount == 1383) || (NumVertices == 1077 && primCount == 336) || (NumVertices == 10502 && primCount == 3849) || (NumVertices == 5172 && primCount == 1738) || (NumVertices == 636 && primCount == 200))
//#define MEC_Artillery (m_Stride == 24 && (NumVertices == 2771 && primCount == 1725) || (NumVertices == 1833 && primCount == 793) || (NumVertices == 474 && primCount == 183))


#define All_MEC_Items ((m_Stride == 24 && (NumVertices == 4799 && primCount == 2468) || (NumVertices == 2350 && primCount == 1125) || (NumVertices == 554 && primCount == 298) || (NumVertices == 68 && primCount == 39)) /**/ || /**/ (m_Stride == 24 && (NumVertices == 6225 && primCount == 3893) || (NumVertices == 3280 && primCount == 1633) || (NumVertices == 448 && primCount == 246) || (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768)) /**/ || /**/ (m_Stride == 24 && (NumVertices == 3796 && primCount == 2179) || (NumVertices == 2691 && primCount == 1484) || (NumVertices == 493 && primCount == 191) || (NumVertices == 381 && primCount == 216 ) || (NumVertices == 593 && primCount == 370) || (NumVertices == 827 && primCount == 562) || (NumVertices == 4674 && primCount == 2643) || (NumVertices == 2522 && primCount == 1304) || (NumVertices == 546 && primCount == 311) || (NumVertices == 593 && primCount == 370) || (NumVertices == 151 && primCount == 128)) /**/ || /**/(m_Stride == 24 && (NumVertices == 4163 && primCount == 2357) || (NumVertices == 4162 && primCount == 2357) || (NumVertices == 2608 && primCount == 1096) || (NumVertices == 2609 && primCount == 1096) || (NumVertices == 494 && primCount == 188) || (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768) || (NumVertices == 25 && primCount == 15))  /**/ || /**/(m_Stride == 24 && (NumVertices == 5078 && primCount == 2592) || (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768) || (NumVertices == 8 && primCount == 4 && startIndex == 25332) || (NumVertices == 382 && primCount == 158 && startIndex == 29337)) /**/ || /**/(m_Stride == 24 && (NumVertices == 5374 && primCount == 2845) || (NumVertices == 51 && primCount == 38) || (NumVertices == 2813 && primCount == 1471) || (NumVertices == 508 && primCount == 288)) /**/ || /**/((m_Stride == 24 && (NumVertices == 562 && primCount == 277) || (NumVertices == 1293 && primCount == 768)) || (m_Stride == 28 && (NumVertices == 5026 && primCount == 1599) || (NumVertices == 622 && primCount == 200) || (NumVertices == 7670 && primCount == 2786 && startIndex == 5727))) /**/ || /**/ (m_Stride == 28 && (NumVertices == 9221 && primCount == 2922) || (NumVertices == 4322 && primCount == 1383) || (NumVertices == 1077 && primCount == 336) || (NumVertices == 10502 && primCount == 3849) || (NumVertices == 5172 && primCount == 1738) || (NumVertices == 636 && primCount == 200)))
//--------------------------------------------------------------------------

//--------Recognition Strides For China-------------------------------------
#define C1 (m_Stride == 24 && (NumVertices == 5441 && primCount == 2634) || (NumVertices == 2864 && primCount == 1104) || (NumVertices == 852 && primCount == 453) || (NumVertices == 5 && primCount == 3))
#define C2 (m_Stride == 24 && (NumVertices == 5164 && primCount == 3035) || (NumVertices == 2814 && primCount == 1541) || (NumVertices == 601 && primCount == 273) || (NumVertices == 551 && primCount == 270))
#define C3 (m_Stride == 24 && (NumVertices == 4561 && primCount == 2574) || (NumVertices == 2604 && primCount == 1351) || (NumVertices == 531 && primCount == 262) || (NumVertices == 3669 && primCount == 2245) || (NumVertices == 2036 && primCount == 1116) || (NumVertices == 387 && primCount == 215) || (NumVertices == 593 && primCount == 370) || (NumVertices == 151 && primCount == 128))
#define C4 (m_Stride == 24 && (NumVertices == 4708 && primCount == 2624) || (NumVertices == 2241 && primCount == 1245) || (NumVertices == 569 && primCount == 237) || (NumVertices == 551 && primCount == 270) || (NumVertices == 1293 && primCount == 768))
#define C5 ((m_Stride == 24 && (NumVertices == 551 && primCount == 270) || (NumVertices == 1293 && primCount == 768)) || (m_Stride == 28 && (NumVertices == 9861 && primCount == 3136) || (NumVertices == 4677 && primCount == 1493) || (NumVertices == 483 && primCount == 151)))
#define C6 (m_Stride == 24 && (NumVertices == 3746 && primCount == 2352) || (NumVertices == 1962 && primCount == 1138) || (NumVertices == 531 && primCount == 270))
//#define China_Artillery (m_Stride == 24 && (NumVertices == 2771 && primCount == 1725) || (NumVertices == 1833 && primCount == 793) || (NumVertices == 474 && primCount == 183))


#define All_Chinese_Items ((m_Stride == 24 && (NumVertices == 5441 && primCount == 2634) || (NumVertices == 2864 && primCount == 1104) || (NumVertices == 852 && primCount == 453) || (NumVertices == 5 && primCount == 3)) || (m_Stride == 24 && (NumVertices == 5164 && primCount == 3035) || (NumVertices == 2814 && primCount == 1541) || (NumVertices == 601 && primCount == 273) || (NumVertices == 551 && primCount == 270)) || (m_Stride == 24 && (NumVertices == 4561 && primCount == 2574) || (NumVertices == 2604 && primCount == 1351) || (NumVertices == 531 && primCount == 262) || (NumVertices == 3669 && primCount == 2245) || (NumVertices == 2036 && primCount == 1116) || (NumVertices == 387 && primCount == 215) || (NumVertices == 593 && primCount == 370) || (NumVertices == 151 && primCount == 128)) || (m_Stride == 24 && (NumVertices == 4708 && primCount == 2624) || (NumVertices == 2241 && primCount == 1245) || (NumVertices == 569 && primCount == 237) || (NumVertices == 551 && primCount == 270) || (NumVertices == 1293 && primCount == 768)) || ((m_Stride == 24 && (NumVertices == 551 && primCount == 270) || (NumVertices == 1293 && primCount == 768)) || (m_Stride == 28 && (NumVertices == 9861 && primCount == 3136) || (NumVertices == 4677 && primCount == 1493) || (NumVertices == 483 && primCount == 151))) || (m_Stride == 24 && (NumVertices == 3746 && primCount == 2352) || (NumVertices == 1962 && primCount == 1138) || (NumVertices == 531 && primCount == 270)))
//--------------------------------------------------------------------------

#define LeaveOutObjects (m_Stride == 52 && (NumVertices == 1322 || NumVertices == 1112 || NumVertices == 1045 || NumVertices == 1578 || (NumVertices == 1073 && primCount == 1384) || (NumVertices == 1409 && primCount == 1581) || (NumVertices == 341 && primCount == 200) || (NumVertices == 1321 && primCount == 1439) || (NumVertices == 1407 && primCount == 1568)))
//--------Recognition Strides For Player Items------------------------------
#define Mines (m_Stride == 24 && NumVertices == 2073 && primCount == 2412)
#define Sniper_Mines (m_Stride == 24 && NumVertices == 2055 && primCount == 1500)
#define C4_Mines (m_Stride == 24 && (NumVertices == 288 && primCount == 242 && startIndex == 5358) || (NumVertices == 1920 && primCount == 1786))
#define Kit_Bags (m_Stride == 52 && (NumVertices == 251 && primCount == 248) || (NumVertices == 248 && primCount == 248))
//--------------------------------------------------------------------------

//--------Recognition Strides For Ground Weapons----------------------------
#define US_Stinger (m_Stride == 24 && (NumVertices == 1643 && primCount == 861) || (NumVertices == 3313 && primCount == 2065) || (NumVertices == 306 && primCount == 128) || (NumVertices == 1314 && primCount == 1176))
#define US_Rocket_Launcher (m_Stride == 24 && (NumVertices == 403 && primCount == 153) || (NumVertices == 2910 && primCount == 1696) || (NumVertices == 1095 && primCount == 631) || (NumVertices == 1283 && primCount == 1176))
#define US_WALL_Mounted_MG (m_Stride == 24 && (NumVertices == 6613 && primCount == 4987) || (NumVertices == 953 && primCount == 912))
#define MEC_Hongjiang (m_Stride == 24 && (NumVertices == 2624 && primCount == 1682) || (NumVertices == 1402 && primCount == 721) || (NumVertices == 422 && primCount == 160))
#define MEC_Stinger (m_Stride == 24 && (NumVertices == 3480 && primCount == 1810) || (NumVertices == 1880 && primCount == 824) || (NumVertices == 563 && primCount == 213))
#define MEC_Mounted_MG (m_Stride == 24 && (NumVertices == 6745 && primCount == 4595))
//--------------------------------------------------------------------------

//--------Recognition Strides For Misc Stuff--------------------------------
#define Sniper_Scope_Overlay (m_Stride == 24 && (NumVertices == 831 && primCount == 1166) || (NumVertices == 763 && primCount == 1166) || (NumVertices == 394 && primCount == 388) || (NumVertices == 980 && primCount == 1238))
#define Sea (m_Stride == 28 && NumVertices == 4225 && primCount == 8192)
#define Sky (m_Stride == 32 && NumVertices == 667 && primCount == 1120)
#define Grenades (m_Stride == 24 && (NumVertices == 217 && primCount == 153 && startIndex == 3909) || (NumVertices == 1396 && primCount == 1303 && startIndex == 0))
#define Tank_Overlays_Barrels (m_Stride == 28 && (NumVertices == 901 && primCount == 810 && startIndex == 2490) || (NumVertices == 2424 && primCount == 782 && startIndex == 0) || (NumVertices == 1761 && primCount == 569 && startIndex == 0) || (NumVertices == 155 && primCount == 56 && startIndex == 1734) || (NumVertices == 3441 && primCount == 1109 && startIndex == 0) || (NumVertices == 813 && primCount == 728 && startIndex == 3327) || (NumVertices == 48 && primCount == 48) || (NumVertices == 8 && primCount == 8))
#define Guided_Missile_Static (m_Stride == 24 && NumVertices == 4 && primCount == 2 && startIndex == 0)
//--------------------------------------------------------------------------

//--------Recognition Strides For Commander Stuff---------------------------
#define Radars (m_Stride == 24 && NumVertices == 4058 && primCount == 2515 && startIndex == 3)
#define USMC_Command_Post (m_Stride == 24 && (NumVertices == 3221 && primCount == 2072 && startIndex == 3) || (NumVertices == 2251 && primCount == 1148 && startIndex == 6699) || (NumVertices == 761 && primCount == 339 && startIndex == 10623))
#define MEC_Command_Post (m_Stride == 24 && (NumVertices == 2251 && primCount == 1148 && startIndex == 3) || (NumVertices == 761 && primCount == 339 && startIndex == 3927))
//--------------------------------------------------------------------------


#define HANDS ((NumVertices == 1407 && primCount == 1568 && startIndex == 0) || (NumVertices == 1045 && primCount == 1395 && startIndex == 0) || (NumVertices == 1578 && primCount == 1584 && startIndex == 0) || (NumVertices == 1073 && primCount == 1384 && startIndex == 0) || (NumVertices == 1046 && primCount == 1352 && startIndex == 0) || (NumVertices == 1082 && primCount == 1384 && startIndex == 0) || (NumVertices == 1045 && primCount == 1395 && startIndex == 0) || (NumVertices == 1409 && primCount == 1581 && startIndex == 0))
#define USA ((NumVertices == 480 && primCount == 574 && startIndex == 11553) || (NumVertices == 549 && primCount == 518 && startIndex == 11727) || (NumVertices == 782 && primCount == 686 && startIndex == 9495) || (NumVertices == 662 && primCount == 692 && startIndex == 9651) || (NumVertices == 398 && primCount == 280 && startIndex == 13281) || (NumVertices == 361 && primCount == 280 && startIndex == 13275) || (NumVertices == 2303 && primCount == 1607 && startIndex == 4557) || (NumVertices == 1270 && primCount == 1050 && startIndex == 0) || (NumVertices == 1518 && primCount == 1291 && startIndex == 10776) || (NumVertices == 2675 && primCount == 1867 && startIndex == 21903) || (NumVertices == 2414 && primCount == 1631 && startIndex == 29052) || (NumVertices == 2148 && primCount == 1486 && startIndex == 16086) || (NumVertices == 2240 && primCount == 1421 && startIndex == 35463) || (NumVertices == 456 && primCount == 272 && startIndex == 14121) || (NumVertices == 495 && primCount == 274 && startIndex == 14115))
#define MEC ((NumVertices == 551 && primCount == 648 && startIndex == 11805) || (NumVertices == 526 && primCount == 628 && startIndex == 10260) || (NumVertices == 661 && primCount == 604 && startIndex == 9153) || (NumVertices == 571 && primCount == 622 && startIndex == 8394) || (NumVertices == 349 && primCount == 280 && startIndex == 10965) || (NumVertices == 394 && primCount == 280 && startIndex == 12144) || (NumVertices == 1972 && primCount == 1531 && startIndex == 0) || (NumVertices == 2080 && primCount == 1546 && startIndex == 26316) || (NumVertices == 1311 && primCount == 1377 && startIndex == 15345) || (NumVertices == 1665 && primCount == 1533 && startIndex == 32274) || (NumVertices == 3229 && primCount == 2549 && startIndex == 5865) ||  (NumVertices == 1745 && primCount == 1450 && startIndex == 20760) || (NumVertices == 2381 && primCount == 1607 && startIndex == 38085) || (NumVertices == 407 && primCount == 264 && startIndex == 13230) || (NumVertices == 463 && primCount == 256 && startIndex == 13995))
#define China ((NumVertices == 532 && primCount == 574 && startIndex == 11115) || (NumVertices == 486 && primCount == 574 && startIndex == 11331) || (NumVertices == 617 && primCount == 642 && startIndex == 8349) || (NumVertices == 643 && primCount == 674 && startIndex == 8469) || (NumVertices == 406 && primCount == 280 && startIndex == 10275) || (NumVertices == 396 && primCount == 280 && startIndex == 10491) || (NumVertices == 1867 && primCount == 1486 && startIndex == 22305) || (NumVertices == 1423 && primCount == 1434 && startIndex == 10851) || (NumVertices == 1975 && primCount == 1486 && startIndex == 4839) || (NumVertices == 2318 && primCount == 1575 && startIndex == 34329) || (NumVertices == 1854 && primCount == 1500 && startIndex == 16575) || (NumVertices == 1503 && primCount == 1233 && startIndex == 0) || (NumVertices == 1803 && primCount == 1484 && startIndex == 28293) || (NumVertices == 422 && primCount == 268 && startIndex == 13311) || (NumVertices == 425 && primCount == 274 && startIndex == 13095))
///
#define Insurgent ((NumVertices == 586 && primCount == 650 && startIndex == 10332) || (NumVertices == 646 && primCount == 646 && startIndex == 8316) || (NumVertices == 714 && primCount == 672 && startIndex == 8316) || (NumVertices == 371 && primCount == 272 && startIndex == 13242) || (NumVertices == 443 && primCount == 284 && startIndex == 14166) || (NumVertices == 620 && primCount == 628 && startIndex == 10254) || (NumVertices == 623 && primCount == 628 && startIndex == 12282) || (NumVertices == 441 && primCount == 286 && startIndex == 12138) || (NumVertices == 1807 && primCount == 1406 && startIndex == 20229) || (NumVertices == 663 && primCount == 691 && startIndex == 37632) || (NumVertices == 1297 && primCount == 1401 && startIndex == 14598) || (NumVertices == 1557 && primCount == 1164 && startIndex == 25629) || (NumVertices == 2044 && primCount == 1756 && startIndex == 30192) || (NumVertices == 1461 && primCount == 1191 && startIndex == 0) || (NumVertices == 2836 && primCount == 2489 && startIndex == 5025))
#define	SAS ((NumVertices == 622 && primCount == 672 && startIndex == 9495) || (NumVertices == 598 && primCount == 694 && startIndex == 11211) || (NumVertices == 405 && primCount == 278 && startIndex == 14415) || (NumVertices == 384 && primCount == 274 && startIndex == 14151) || (NumVertices == 536 && primCount == 588 && startIndex == 11511) || (NumVertices == 437 && primCount == 572 && startIndex == 9495) || (NumVertices == 441 && primCount == 286 && startIndex == 13275) || (NumVertices == 498 && primCount == 286 && startIndex == 13293) || (NumVertices == 1182 && primCount == 884 && startIndex == 2973) || (NumVertices == 918 && primCount == 732 && startIndex == 0) || (NumVertices == 800 && primCount == 548 && startIndex == 36480) || (NumVertices == 1946 && primCount == 1725 && startIndex == 21318) || (NumVertices == 1777 && primCount == 1552 && startIndex == 29283) || (NumVertices == 1622 && primCount == 1457 && startIndex == 14463) || (NumVertices == 2023 && primCount == 1807 && startIndex == 6234))
#define NavySEAL ((NumVertices == 615 && primCount == 658 && startIndex == 12057) || (NumVertices == 555 && primCount == 648 && startIndex == 9495) ||(NumVertices == 385 && primCount == 268 && startIndex == 14031) || (NumVertices == 376 && primCount == 274 && startIndex == 14013) || (NumVertices == 465 && primCount == 568 && startIndex == 10353) || (NumVertices == 437 && primCount == 572 && startIndex == 12297) || (NumVertices == 433 && primCount == 286 && startIndex == 9495) || (NumVertices == 502 && primCount == 286 && startIndex == 11439) || (NumVertices == 1601 && primCount == 1322 && startIndex == 5043) || (NumVertices == 1294 && primCount == 1126 && startIndex == 0) || (NumVertices == 1168 && primCount == 942 && startIndex == 31392) || (NumVertices == 1569 && primCount == 1360 && startIndex == 19968) || (NumVertices == 1461 && primCount == 1210 && startIndex == 25917) || (NumVertices == 1480 && primCount == 1236 && startIndex == 14817) || (NumVertices == 1258 && primCount == 1065 && startIndex == 10452))
#define MECSF ((NumVertices == 1555 && primCount == 1164 && startIndex == 21318) || (NumVertices == 567 && primCount == 640 && startIndex == 9495) || (NumVertices == 686 && primCount == 836 && startIndex == 9495) || (NumVertices == 460 && primCount == 376 && startIndex == 14415) || (NumVertices == 342 && primCount == 268 && startIndex == 14691) || (NumVertices == 430 && primCount == 622 && startIndex == 11415) || (NumVertices == 528 && primCount == 610 && startIndex == 12003) || (NumVertices == 445 && primCount == 286 && startIndex == 13281) || (NumVertices == 474 && primCount == 286 && startIndex == 13833) || (NumVertices == 1117 && primCount == 1155 && startIndex == 10647) || (NumVertices == 1117 && primCount == 1155 && startIndex == 10647) || (NumVertices == 953 && primCount == 937 && startIndex == 30237) || (NumVertices == 2250 && primCount == 1805 && startIndex == 3825) || (NumVertices == 1311 && primCount == 1019 && startIndex == 0) || (NumVertices == 1131 && primCount == 1123 && startIndex == 25902) || (NumVertices == 1649 && primCount == 1322 && startIndex == 15042))
#define Spetsnaz ((NumVertices == 613 && primCount == 644 && startIndex == 9495) || (NumVertices == 640 && primCount == 684 && startIndex == 9495) || (NumVertices == 392 && primCount == 286 && startIndex == 13605) || (NumVertices == 442 && primCount == 320 && startIndex == 14133) || (NumVertices == 303 && primCount == 436 && startIndex == 12297) || (NumVertices == 418 && primCount == 470 && startIndex == 11547) || (NumVertices == 448 && primCount == 290 && startIndex == 11427) || (NumVertices == 490 && primCount == 302 && startIndex == 12957) || (NumVertices == 1892 && primCount == 1588 && startIndex == 3840) || (NumVertices == 1038 && primCount == 1008 && startIndex == 0) || (NumVertices == 453 && primCount == 422 && startIndex == 36813) || (NumVertices == 1473 && primCount == 1264 && startIndex == 24843) || (NumVertices == 1832 && primCount == 1619 && startIndex == 30084) || (NumVertices == 1612 && primCount == 1498 && startIndex == 10134) || (NumVertices == 2700 && primCount == 2176 && startIndex == 15972))
#define Rebels ((NumVertices == 666 && primCount == 686 && startIndex == 9495) || (NumVertices == 664 && primCount == 692 && startIndex == 9495) || (NumVertices == 486 && primCount == 402 && startIndex == 14457) || (NumVertices == 445 && primCount == 272 && startIndex == 14193) || (NumVertices == 574 && primCount == 670 && startIndex == 11553) || (NumVertices == 457 && primCount == 588 && startIndex == 11571) || (NumVertices == 472 && primCount == 298 && startIndex == 13563) || (NumVertices == 444 && primCount == 286 && startIndex == 13335) || (NumVertices == 1519 && primCount == 1164 && startIndex == 25578) || (NumVertices == 1601 && primCount == 1665 && startIndex == 12453) || (NumVertices == 1338 && primCount == 1405 && startIndex == 35967) || (NumVertices == 2389 && primCount == 2075 && startIndex == 4425) || (NumVertices == 1171 && primCount == 1079 && startIndex == 0) || (NumVertices == 1787 && primCount == 1439 && startIndex == 30162) || (NumVertices == 1861 && primCount == 1592 && startIndex == 19056))
///
#define EuroForce ((NumVertices == 623 && primCount == 702 && startIndex == 11265) || (NumVertices == 594 && primCount == 702 && startIndex == 9495) || (NumVertices == 415 && primCount == 278 && startIndex == 14505) || (NumVertices == 413 && primCount == 278 && startIndex == 14571) || (NumVertices == 486 && primCount == 590 && startIndex == 9495) || (NumVertices == 485 && primCount == 612 && startIndex == 12459) || (NumVertices == 441 && primCount == 286 && startIndex == 13371) || (NumVertices == 444 && primCount == 286 && startIndex == 11601) || (NumVertices == 2311 && primCount == 1826 && startIndex == 7044) || (NumVertices == 1827 && primCount == 1512 && startIndex == 0) || (NumVertices == 2240 && primCount == 1421 && startIndex == 44820) || (NumVertices == 2592 && primCount == 1998 && startIndex == 28254) || (NumVertices == 2133 && primCount == 1662 && startIndex == 37188) || (NumVertices == 2053 && primCount == 1661 && startIndex == 20586) || (NumVertices == 1518 && primCount == 1291 && startIndex == 15276))
///
#define Kits ((NumVertices == 251 && primCount == 248) || (NumVertices == 248 && primCount && 248))
///
#define Artillery ((NumVertices == 3852 && primCount == 2208) || (NumVertices == 1786 && primCount == 854) || (NumVertices == 707 && primCount == 269) || (NumVertices == 2771 && primCount == 1725) || (NumVertices == 1833 && primCount == 793) || (NumVertices == 474 && primCount == 183))
#define Radar (NumVertices == 4058 && primCount == 2515 && startIndex == 3)
#define Commandpost1 ((NumVertices == 3221 && primCount == 2072 && startIndex == 3) || (NumVertices == 2251 && primCount == 1148 && startIndex == 6699) || (NumVertices == 761 && primCount == 339 && startIndex == 10623))
#define Commandpost2 ((NumVertices == 2251 && primCount == 1148 && startIndex == 3) || (NumVertices == 761 && primCount == 339 && startIndex == 3927))
///
#define Weapons ((NumVertices == 5157 && primCount == 3570) || (NumVertices == 1135 && primCount == 1082) || (NumVertices == 5358 && primCount == 3710) || (NumVertices == 1442 && primCount == 1008) || (NumVertices == 3182 && primCount == 2126) || (NumVertices == 6256 && primCount == 4221) || (NumVertices == 1920 && primCount == 1786) || (NumVertices == 977 && primCount == 896) || (NumVertices == 5095 && primCount == 3448) || (NumVertices == 2055 && primCount == 1500) || (NumVertices == 4806 && primCount == 3109) || (NumVertices == 1079 && primCount == 686) || (NumVertices == 296 && primCount == 240) || (NumVertices == 4463 && primCount == 3573) || (NumVertices == 2073 && primCount == 2412) || (NumVertices == 1360 && primCount == 1120) || (NumVertices == 3872 && primCount == 2726) || (NumVertices == 1720 && primCount == 1137) || (NumVertices == 375 && primCount == 244) || (NumVertices == 1159 && primCount == 1016) || (NumVertices == 4651 && primCount == 3394) || (NumVertices == 2927 && primCount == 2182) || (NumVertices == 4042 && primCount == 2978) || (NumVertices == 2176 && primCount == 1669) || (NumVertices == 5073 && primCount == 3499) || (NumVertices == 4973 && primCount == 3499) || (NumVertices == 4680 && primCount == 3486) || (NumVertices == 40 && primCount == 50) || (NumVertices == 5631 && primCount == 3278) || (NumVertices == 2739 && primCount == 2280) || (NumVertices == 4029 && primCount == 2836) || (NumVertices == 2991 && primCount == 1995) || (NumVertices == 4842 && primCount == 3420) || (NumVertices == 186 && primCount == 168) || (NumVertices == 3488 && primCount == 2110) || (NumVertices == 5241 && primCount == 3574) || (NumVertices == 5161 && primCount == 3643) || (NumVertices == 5160 && primCount == 3643) || (NumVertices == 5019 && primCount == 3375) || (NumVertices == 4868 && primCount == 3489) || (NumVertices == 4049 && primCount == 2887) || (NumVertices == 5009 && primCount == 3343) || (NumVertices == 3696 && primCount == 3184))
#define Weapons2 ((NumVertices == 453 && primCount == 250) || (NumVertices == 533 && primCount == 274) || (NumVertices == 484 && primCount == 269) || (NumVertices == 1594 && primCount == 985) || (NumVertices == 550 && primCount == 291) || (NumVertices == 474 && primCount == 273) || (NumVertices == 339 && primCount == 180) || (NumVertices == 390 && primCount == 256) || (NumVertices == 242 && primCount == 174) || (NumVertices == 550 && primCount == 294) || (NumVertices == 500 && primCount == 250) || (NumVertices == 593 && primCount == 290) || (NumVertices == 517 && primCount == 254) || (NumVertices == 396 && primCount == 208) || (NumVertices == 586 && primCount == 293) || (NumVertices == 587 && primCount == 280) || (NumVertices == 521 && primCount == 270) || (NumVertices == 524 && primCount == 260) || (NumVertices == 449 && primCount == 210) || (NumVertices == 503 && primCount == 253) || (NumVertices == 266 && primCount == 149) || (NumVertices == 360 && primCount == 192) || (NumVertices == 401 && primCount == 238) || (NumVertices == 483 && primCount == 250) || (NumVertices == 481 && primCount == 242) || (NumVertices == 473 && primCount == 243) || (NumVertices == 384 && primCount == 196) || (NumVertices == 409 && primCount == 242) || (NumVertices == 390 && primCount == 264))
#define unlockweapons ((NumVertices == 5719 && primCount == 3493) || (NumVertices == 5510 && primCount == 3787) || (NumVertices == 4970 && primCount == 3444) || (NumVertices == 4583 && primCount == 3015) || (NumVertices == 4523 && primCount == 3802) || (NumVertices == 4172 && primCount == 3436) || (NumVertices == 5055 && primCount == 3547) || (NumVertices == 4661 && primCount == 3506) || (NumVertices == 1392 && primCount == 1323) || (NumVertices == 4560 && primCount == 2812) || (NumVertices == 1340 && primCount == 1019) || (NumVertices == 3964 && primCount == 2442) || (NumVertices == 8008 && primCount == 5562) || (NumVertices == 3796 && primCount == 2951) || (NumVertices == 4807 && primCount == 3406) || (NumVertices == 3723 && primCount == 2713) || (NumVertices == 4937 && primCount == 3425) || (NumVertices == 5434 && primCount == 3319) || (NumVertices == 5133 && primCount == 3601) || (NumVertices == 3607 && primCount == 2908) || (NumVertices == 3226 && primCount == 2568))
#define unlockweapons2 ((NumVertices == 523 && primCount == 250) || (NumVertices == 555 && primCount == 283) || (NumVertices == 471 && primCount == 269) || (NumVertices == 564 && primCount == 268) || (NumVertices == 509 && primCount == 270) || (NumVertices == 374 && primCount == 220) || (NumVertices == 2820 && primCount == 1861) || (NumVertices == 1007 && primCount == 511) || (NumVertices == 604 && primCount == 294) || (NumVertices == 538 && primCount == 302) || (NumVertices == 629 && primCount == 297) || (NumVertices == 463 && primCount == 243) || (NumVertices == 1840 && primCount == 1087) || (NumVertices == 406 && primCount == 222) || (NumVertices == 447 && primCount == 229) || (NumVertices == 470 && primCount == 226) || (NumVertices == 373 && primCount == 213) || (NumVertices == 451 && primCount == 249) || (NumVertices == 497 && primCount == 272))
///
#define Explosives ((NumVertices == 2073 && primCount == 2412) || (NumVertices == 2055 && primCount == 1500) || (NumVertices == 288 && primCount == 242 && startIndex == 5358) || (NumVertices == 1920 && primCount == 1786))
//#define Grenades ((NumVertices == 217 && primCount == 153 && startIndex == 3909) || (NumVertices == 1396 && primCount == 1303 && startIndex == 0))

LPDIRECT3DTEXTURE9 chamsTextures[9];

const BYTE blue[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00
};

const BYTE yellow[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00
};

const BYTE green[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00
};

const BYTE ltblue[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00
};

const BYTE purple[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00
};

const BYTE red[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00
};

const BYTE white[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00
};

const BYTE black[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const BYTE orange[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0xFF, 0x00
};

//-----------------------------------------------------------------------------
HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if( FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)) )
		return E_FAIL;  

	WORD colour16 =    ((WORD)((colour32>>28)&0xF)<<12)
		|(WORD)(((colour32>>20)&0xF)<<8)
		|(WORD)(((colour32>>12)&0xF)<<4)
		|(WORD)(((colour32>>4)&0xF)<<0);  

	D3DLOCKED_RECT d3dlr;      
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);  
	WORD *pDst16 = (WORD*)d3dlr.pBits;  

	for(int xy=0; xy < 8*8; xy++)
		*pDst16++ = colour16;  

	(*ppD3Dtex)->UnlockRect(0);  

	return S_OK;  
}

//-----------------------------------------------------------------------------
IDirect3DDevice9hook::IDirect3DDevice9hook(IDirect3DDevice9 *pOriginalDeviceInterface)
{
	m_pD3Ddev = pOriginalDeviceInterface; 
	m_pD3Ddev->GetViewport(&Viewport);

	hand = GetCurrentProcess();

	GenerateTexture(m_pD3Ddev,&Red,       D3DCOLOR_ARGB(255,255,0,0      ));  
	GenerateTexture(m_pD3Ddev,&Yellow,    D3DCOLOR_ARGB(255,255,255,0    ));  
	GenerateTexture(m_pD3Ddev,&Blue,      D3DCOLOR_ARGB(255,0,0,255      ));  
	GenerateTexture(m_pD3Ddev,&Green,     D3DCOLOR_ARGB(255,0,255,0      ));  
	GenerateTexture(m_pD3Ddev,&Orange,    D3DCOLOR_ARGB(255,255,165,0    ));  
	GenerateTexture(m_pD3Ddev,&Purple,    D3DCOLOR_ARGB(255,160,32,240   ));  
	GenerateTexture(m_pD3Ddev,&Pink  ,    D3DCOLOR_ARGB(255,255,20,147   ));  
	GenerateTexture(m_pD3Ddev,&Brown ,    D3DCOLOR_ARGB(255,166,42,42    )); 
	GenerateTexture(m_pD3Ddev,&White ,    D3DCOLOR_ARGB(255,255,255,255  )); 

	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&blue, sizeof(blue), &chamsTextures[0]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&yellow, sizeof(yellow), &chamsTextures[1]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&green, sizeof(green), &chamsTextures[2]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&ltblue, sizeof(ltblue), &chamsTextures[3]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&purple, sizeof(purple), &chamsTextures[4]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&red, sizeof(red), &chamsTextures[5]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&white, sizeof(white), &chamsTextures[6]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&black, sizeof(black), &chamsTextures[7]);
	D3DXCreateTextureFromFileInMemory(m_pD3Ddev, (LPCVOID)&orange, sizeof(orange), &chamsTextures[8]);

	D3DXCreateFont( m_pD3Ddev, iTextSize, 0, FW_NORMAL, 0, FALSE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &Font1);//Microsoft Sans Serif
	D3DXCreateFont( m_pD3Ddev, iTextSize, 0, FW_BOLD, 0, FALSE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &Font2);
	D3DXCreateFont( m_pD3Ddev, 16, 0, FW_BOLD, 1, TRUE,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &TimeFont);

	D3DXCreateLine( m_pD3Ddev , &pLine);
	pLine->SetWidth(1.0);
	pLine->SetAntialias(true);
	pLine->SetGLLines(true);

	CustomInitialize(m_pD3Ddev); 

}

void DrawMyText(char pString[], int x, int y, D3DCOLOR col, ID3DXFont *font)
{
	RECT FontRect = { x, y, x+200, y+16 };
	font->DrawText( NULL, pString, -1, &FontRect, DT_LEFT | DT_WORDBREAK, col);
}

IDirect3DDevice9hook::~IDirect3DDevice9hook(void)
{
	if(m_pD3Ddev)
	{
		m_pD3Ddev->Release();
		m_pD3Ddev = 0;
	}

	chamsTextures[0]->Release();
	chamsTextures[1]->Release();
	chamsTextures[2]->Release();
	chamsTextures[3]->Release();
	chamsTextures[4]->Release();
	chamsTextures[5]->Release();
	chamsTextures[6]->Release();
	chamsTextures[7]->Release();
	chamsTextures[8]->Release();
}

HRESULT APIENTRY IDirect3DDevice9hook::QueryInterface(REFIID riid, LPVOID *ppvObj) 
{
	return m_pD3Ddev->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY IDirect3DDevice9hook::AddRef() 
{
	return m_pD3Ddev->AddRef();
}

HRESULT APIENTRY IDirect3DDevice9hook::BeginScene() 
{
	m_pD3Ddev->GetViewport(&Viewport);
	float ScreenCenterX = (float)Viewport.Width / 2 + Viewport.X;
	float ScreenCenterY = (float)Viewport.Height / 2 + Viewport.Y;
	CrosshairX[0].x = ScreenCenterX-1-7;
	CrosshairX[0].y = ScreenCenterY;
	CrosshairX[1].x = ScreenCenterX-1+8;
	CrosshairX[1].y = ScreenCenterY;
	CrosshairY[0].x = ScreenCenterX-1;
	CrosshairY[0].y = ScreenCenterY-7;
	CrosshairY[1].x = ScreenCenterX-1;
	CrosshairY[1].y = ScreenCenterY+8;
	m_pD3Ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 255, 255, 255 ), 1.0f, 0 );
	return m_pD3Ddev->BeginScene();
}

HRESULT APIENTRY IDirect3DDevice9hook::BeginStateBlock() 
{
	return m_pD3Ddev->BeginStateBlock();
}

HRESULT APIENTRY IDirect3DDevice9hook::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) 
{
	return m_pD3Ddev->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT APIENTRY IDirect3DDevice9hook::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect, D3DCOLOR color) 
{	
	return m_pD3Ddev->ColorFill(pSurface,pRect,color);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **ppSwapChain) 
{
	return m_pD3Ddev->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture,pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality,Discard,ppSurface, pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer,pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) 
{
	return m_pD3Ddev->CreatePixelShader(pFunction, ppShader);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) 
{
	return m_pD3Ddev->CreateQuery(Type,ppQuery);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateRenderTarget(Width, Height, Format, MultiSample,MultisampleQuality, Lockable, ppSurface,pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) 
{
	return m_pD3Ddev->CreateStateBlock(Type, ppSB);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer,pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pD3Ddev->CreateVertexDeclaration(pVertexElements,ppDecl);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
{
	return m_pD3Ddev->CreateVertexShader(pFunction, ppShader);
}

HRESULT APIENTRY IDirect3DDevice9hook::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture,pSharedHandle);
}

HRESULT APIENTRY IDirect3DDevice9hook::DeletePatch(UINT Handle) 
{
	return m_pD3Ddev->DeletePatch(Handle);
}


HRESULT APIENTRY IDirect3DDevice9hook::DrawIndexedPrimitive(D3DPRIMITIVETYPE type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	//---------------No Sky Function---------------------------------------------------------------
	if(NoSky)
		if(NumVertices == 667 && primCount == 1120)
			return D3D_OK;
	//---------------------------------------------------------------------------------------------

	//---------------No Water Function-------------------------------------------------------------
	if(NoWater)
		if(NumVertices == 4225 && primCount == 8192)
			return D3D_OK;
	//---------------------------------------------------------------------------------------------

	//---------------No Fog Function---------------------------------------------------------------
	if(NoFogo)
		m_pD3Ddev->SetRenderState( D3DRS_FOGENABLE , false );
	//---------------------------------------------------------------------------------------------

	//---------------No Sniper Scope Overlay Function---------------------------------------------------------------
	if (NoScope && m_Stride == 24 && primCount > NumVertices)
		return D3D_OK;
	//---------------------------------------------------------------------------------------------


	if(NoTankOverlay && Tank_Overlays_Barrels || (NumVertices == 831 && primCount == 1166 || NumVertices == 763 && primCount == 1166 || NumVertices == 394 && primCount == 388 || NumVertices == 980 && primCount == 1238 && startIndex == 10275))
		return D3D_OK;

	if (Chams)
	{

		LPDIRECT3DTEXTURE9 texCham = 0, texWHCham = 0;
		if (USA)				{ texCham = chamsTextures[0]; texWHCham = chamsTextures[2]; }
		else { if (MEC)			{ texCham = chamsTextures[5]; texWHCham = chamsTextures[1]; }
		if (China)				{ texCham = chamsTextures[4]; texWHCham = chamsTextures[3]; }
		if (Insurgent)			{ texCham = chamsTextures[5]; texWHCham = chamsTextures[1]; }
		if (SAS)				{ texCham = chamsTextures[0]; texWHCham = chamsTextures[2]; }
		if (NavySEAL)			{ texCham = chamsTextures[2]; texWHCham = chamsTextures[6]; }
		if (MECSF)				{ texCham = chamsTextures[4]; texWHCham = chamsTextures[1]; }
		if (Spetsnaz)			{ texCham = chamsTextures[1]; texWHCham = chamsTextures[0]; }
		if (Rebels)				{ texCham = chamsTextures[3]; texWHCham = chamsTextures[5]; }
		if (EuroForce)			{ texCham = chamsTextures[4]; texWHCham = chamsTextures[3]; }
		if (HANDS)				{ texCham = chamsTextures[6]; texWHCham = chamsTextures[2]; } }

		if (texCham)			
		{
			if (Wallhack)
			{
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, false);
				if (Chams)
					m_pD3Ddev->SetTexture(0, texWHCham);
				m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, true);
			}

			if (Chams)
			{
				m_pD3Ddev->SetTexture(0, texCham);
				//m_pD3Ddev->SetRenderState( D3DRS_SRCBLEND, 0x5 );
				//m_pD3Ddev->SetRenderState( D3DRS_DESTBLEND, 0x2 );
				//m_pD3Ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

				return m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			}

		}
	}

	if(Wireframe)
	{
		m_pD3Ddev->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		m_pD3Ddev->DrawIndexedPrimitive( type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount );
		m_pD3Ddev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );		
	}


	if(VehicleChams)
	{
		if(m_Stride == 24 || m_Stride == 28 && primCount < NumVertices && BaseVertexIndex != 0)
		{
			if(Wallhack)
			{
				m_pD3Ddev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

				m_pD3Ddev->DrawIndexedPrimitive( type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount );

				m_pD3Ddev->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
			}

			if(VehicleChams)
			{
				DWORD dwOldZEnable;
				m_pD3Ddev->SetTexture(0,NULL);
				m_pD3Ddev->SetTexture(0, Blue);
				m_pD3Ddev->GetRenderState(D3DRS_ZENABLE,&dwOldZEnable);
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
				m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_NEVER);

				m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,dwOldZEnable);
				m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
				m_pD3Ddev->SetTexture(0,Green);
			}
		}
	}

	if(GroundWeaponChams)
	{
		if(US_Stinger || US_Rocket_Launcher || US_WALL_Mounted_MG || MEC_Hongjiang || MEC_Stinger || MEC_Mounted_MG)
		{
			if(!LeaveOutObjects)
			{
				DWORD dwOldZEnable;
				m_pD3Ddev->SetTexture(0,NULL);
				m_pD3Ddev->SetTexture(0, chamsTextures[2]);
				m_pD3Ddev->GetRenderState(D3DRS_ZENABLE,&dwOldZEnable);
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
				m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_NEVER);

				m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,dwOldZEnable);
				m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
				m_pD3Ddev->SetTexture(0, chamsTextures[8]);
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if (MobileObjects)
	{

		LPDIRECT3DTEXTURE9 texCham = 0, texWHCham = 0;
		if (Explosives)			{ texCham = chamsTextures[4]; texWHCham = chamsTextures[3]; } 
		else { 
			if (Grenades)	{ texCham = chamsTextures[4]; texWHCham = chamsTextures[3]; }
			if (Kits)		{ texCham = chamsTextures[0]; texWHCham = chamsTextures[3]; } }

		if (texCham) 
		{
			if (Wallhack)
			{
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, false);
				if (MobileObjects)
					m_pD3Ddev->SetTexture(0, texWHCham);
				m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, true);
			}

			if (MobileObjects)
			{
				m_pD3Ddev->SetTexture(0, texCham);
				return m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			}
		}
	}

	if (AssetChams)
	{

		LPDIRECT3DTEXTURE9 texCham = 0, texWHCham = 0;
		if (Artillery)				{ texCham = chamsTextures[2]; texWHCham = chamsTextures[0]; }
		else { if (Radar)			{ texCham = chamsTextures[2]; texWHCham = chamsTextures[0]; }
		if (Commandpost1)	{ texCham = chamsTextures[2]; texWHCham = chamsTextures[0]; }
		if (Commandpost2)	{ texCham = chamsTextures[2]; texWHCham = chamsTextures[0]; } }
		if (texCham) 
		{
			if (Wallhack)
			{
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, false);
				if (AssetChams)
					m_pD3Ddev->SetTexture(0, texWHCham);
				m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, true);
			}
			if (AssetChams)
			{
				m_pD3Ddev->SetTexture(0, texCham);
				return m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			}
		}
	}

	if (WeaponChams)
	{

		LPDIRECT3DTEXTURE9 texCham = 0, texWHCham = 0;
		if (Weapons)				{ texCham = chamsTextures[3]; texWHCham = chamsTextures[1]; } 
		else { if (Weapons2)		{ texCham = chamsTextures[3]; texWHCham = chamsTextures[1]; }
		if (unlockweapons)	{ texCham = chamsTextures[3]; texWHCham = chamsTextures[1]; }
		if (unlockweapons2)	{ texCham = chamsTextures[3]; texWHCham = chamsTextures[1]; } }
		if (texCham) 
		{
			if (Wallhack)
			{
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, false);
				if (WeaponChams)
					m_pD3Ddev->SetTexture(0, texWHCham);
				m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, true);
			}
			if (WeaponChams)
			{
				m_pD3Ddev->SetTexture(0, texCham);
				return m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			}
		}
	}

	if(MineChams)
	{
		if(Mines)
		{
			DWORD dwOldZEnable;
			m_pD3Ddev->SetTexture(0,NULL);
			m_pD3Ddev->SetTexture(0, Red);
			m_pD3Ddev->GetRenderState(D3DRS_ZENABLE,&dwOldZEnable);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
			m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_NEVER);

			m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,dwOldZEnable);
			m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			m_pD3Ddev->SetTexture(0, Orange);
		}

		if(Sniper_Mines)
		{
			DWORD dwOldZEnable;
			m_pD3Ddev->SetTexture(0,NULL);
			m_pD3Ddev->SetTexture(0, Red);
			m_pD3Ddev->GetRenderState(D3DRS_ZENABLE,&dwOldZEnable);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
			m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_NEVER);

			m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,dwOldZEnable);
			m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			m_pD3Ddev->SetTexture(0, Orange);
		}

		if(C4_Mines)
		{
			DWORD dwOldZEnable;
			m_pD3Ddev->SetTexture(0,NULL);
			m_pD3Ddev->SetTexture(0, Red);
			m_pD3Ddev->GetRenderState(D3DRS_ZENABLE,&dwOldZEnable);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
			m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_NEVER);

			m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE,dwOldZEnable);
			m_pD3Ddev->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			m_pD3Ddev->SetTexture(0, Orange);
		}
	}

	if(SquadJumping)
	{
		if(GetAsyncKeyState(VK_NUMPAD1))
		{
			fSquad1();
		}
		if(GetAsyncKeyState(VK_NUMPAD2))
		{
			fSquad2();
		}
		if(GetAsyncKeyState(VK_NUMPAD3))
		{
			fSquad3();
		}
		if(GetAsyncKeyState(VK_NUMPAD4))
		{
			fSquad4();
		}
		if(GetAsyncKeyState(VK_NUMPAD5))
		{
			fSquad5();
		}
		if(GetAsyncKeyState(VK_NUMPAD6))
		{
			fSquad6();
		}
		if(GetAsyncKeyState(VK_NUMPAD7))
		{
			fSquad7();
		}
		if(GetAsyncKeyState(VK_NUMPAD8))
		{
			fSquad8();
		}
		if(GetAsyncKeyState(VK_NUMPAD9))
		{
			fSquad9();
		}
		if(GetAsyncKeyState(VK_NUMPAD0))
		{
			fSquad0();
		}
	}

	if(UnlimitedAssets)
	{
		fPanelFullMaker();
	}

	if(bbbCommander || bbLeader)
	{
		fCCommander();
	}

	if(AsusWallhack)
	{
		m_pD3Ddev->SetRenderState( D3DRS_SRCBLEND, 0x5 );
		m_pD3Ddev->SetRenderState( D3DRS_DESTBLEND, 0x2 );
		m_pD3Ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

		return m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

	if(Whitewalls)
	{
		for(int i=0; i<10; i++)
			m_pD3Ddev->SetTexture( i, chamsTextures[6]);
	}

	return m_pD3Ddev->DrawIndexedPrimitive(type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{
	return m_pD3Ddev->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY IDirect3DDevice9hook::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) 
{
	return m_pD3Ddev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{
	return m_pD3Ddev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY IDirect3DDevice9hook::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) 
{
	return m_pD3Ddev->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY IDirect3DDevice9hook::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
{
	return m_pD3Ddev->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT APIENTRY IDirect3DDevice9hook::EndScene()
{
	hand = GetCurrentProcess();
	MainInit();

	if(!color2)
	{
		SetMainLoopDetours(TRUE);
		InitializeMem();
		color2 = true;
	}

	if(Xhair)
	{
		pLine->Begin();
		pLine->Draw(CrosshairX,  2, D3DCOLOR_RGBA(255, 0, 0, 255)); 
		pLine->End();
		pLine->Begin();
		pLine->Draw(CrosshairY,  2, D3DCOLOR_RGBA(255, 0, 0, 255)); 
		pLine->End();
	}

	CustomPresent(m_pD3Ddev);

	if(Time)
	{
		D3DCOLOR dTime = D3DCOLOR_XRGB( 255, 255, 255 );                     
		GetTimeFormat(NULL,NULL,NULL,NULL,Timestruct,15);
		sprintf(TimeString,"[ %s ]",Timestruct); 
		DrawMyText(TimeString, 340, 10, dTime, TimeFont);
	}

	if(FPS)
	{
		D3DCOLOR dTime = D3DCOLOR_XRGB( 255, 255, 255 ); 
		fCurrentTick = clock() * 0.001f;
		++fFps;
		if(fCurrentTick - fLastTick > 1.0f)
		{
			fLastTick = fCurrentTick;
			sprintf(chFrames, "[ FPS: %d ]", int(fFps));
			fFps = 0;
		}
		DrawMyText(chFrames, 440, 10, dTime, TimeFont);
	}

	if(bMenuInited)
	{
		D3DCOLOR dTime = D3DCOLOR_XRGB( 255, 0, 0 ); 
		DrawMyText("www.Devious-100.com", 335, 575, dTime, TimeFont);
	}

	return m_pD3Ddev->EndScene();
}

HRESULT APIENTRY IDirect3DDevice9hook::EndStateBlock(IDirect3DStateBlock9** ppSB) 
{
	return m_pD3Ddev->EndStateBlock(ppSB);
}

HRESULT APIENTRY IDirect3DDevice9hook::EvictManagedResources() 
{
	return m_pD3Ddev->EvictManagedResources();
}

UINT APIENTRY IDirect3DDevice9hook::GetAvailableTextureMem() 
{
	return m_pD3Ddev->GetAvailableTextureMem();
}

HRESULT APIENTRY IDirect3DDevice9hook::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) 
{
	return m_pD3Ddev->GetBackBuffer(iSwapChain,iBackBuffer, Type, ppBackBuffer);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetClipPlane(DWORD Index, float *pPlane) 
{
	return m_pD3Ddev->GetClipPlane(Index, pPlane);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetClipStatus(D3DCLIPSTATUS9 *pClipStatus) 
{
	return m_pD3Ddev->GetClipStatus(pClipStatus);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) 
{
	return m_pD3Ddev->GetCreationParameters(pParameters);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetCurrentTexturePalette(UINT *pPaletteNumber)
{
	return m_pD3Ddev->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface) 
{
	return m_pD3Ddev->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetDeviceCaps(D3DCAPS9 *pCaps) 
{
	return m_pD3Ddev->GetDeviceCaps(pCaps);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetDirect3D(IDirect3D9 **ppD3D9) 
{
	return m_pD3Ddev->GetDirect3D(ppD3D9);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode) 
{
	return m_pD3Ddev->GetDisplayMode(iSwapChain,pMode);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface) 
{
	return m_pD3Ddev->GetFrontBufferData(iSwapChain,pDestSurface);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetFVF(DWORD* pFVF) 
{
	return m_pD3Ddev->GetFVF(pFVF);
}

void APIENTRY IDirect3DDevice9hook::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp) 
{
	m_pD3Ddev->GetGammaRamp(iSwapChain,pRamp);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetIndices(IDirect3DIndexBuffer9** ppIndexData) 
{
	return m_pD3Ddev->GetIndices(ppIndexData);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetLight(DWORD Index, D3DLIGHT9 *pLight) 
{
	return m_pD3Ddev->GetLight(Index, pLight);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetLightEnable(DWORD Index, BOOL *pEnable) 
{
	return m_pD3Ddev->GetLightEnable(Index, pEnable);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetMaterial(D3DMATERIAL9 *pMaterial) 
{
	return m_pD3Ddev->GetMaterial(pMaterial);
}

float APIENTRY IDirect3DDevice9hook::GetNPatchMode() 
{
	return m_pD3Ddev->GetNPatchMode();
}

unsigned int APIENTRY IDirect3DDevice9hook::GetNumberOfSwapChains() 
{
	return m_pD3Ddev->GetNumberOfSwapChains();
}

HRESULT APIENTRY IDirect3DDevice9hook::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
{
	return m_pD3Ddev->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetPixelShader(IDirect3DPixelShader9** ppShader) 
{
	return m_pD3Ddev->GetPixelShader(ppShader);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
	return m_pD3Ddev->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pD3Ddev->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) 
{
	return m_pD3Ddev->GetRasterStatus(iSwapChain,pRasterStatus);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) 
{
	return m_pD3Ddev->GetRenderState(State, pValue);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) 
{
	return m_pD3Ddev->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) 
{
	return m_pD3Ddev->GetRenderTargetData(pRenderTarget,pDestSurface);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) 
{
	return m_pD3Ddev->GetSamplerState(Sampler,Type,pValue);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetScissorRect(RECT* pRect) 
{
	return m_pD3Ddev->GetScissorRect(pRect);
}

BOOL APIENTRY IDirect3DDevice9hook::GetSoftwareVertexProcessing() 
{
	return m_pD3Ddev->GetSoftwareVertexProcessing();
}

HRESULT APIENTRY IDirect3DDevice9hook::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride) 
{
	return m_pD3Ddev->GetStreamSource(StreamNumber, ppStreamData,OffsetInBytes, pStride);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetStreamSourceFreq(UINT StreamNumber,UINT* Divider) 
{
	return m_pD3Ddev->GetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	return m_pD3Ddev->GetSwapChain(iSwapChain,pSwapChain);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetTexture(DWORD Stage, IDirect3DBaseTexture9 **ppTexture) 
{
	return m_pD3Ddev->GetTexture(Stage, ppTexture);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue) 
{
	return m_pD3Ddev->GetTextureStageState(Stage, Type, pValue);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->GetTransform(State, pMatrix);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pD3Ddev->GetVertexDeclaration(ppDecl);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetVertexShader(IDirect3DVertexShader9** ppShader) 
{
	return m_pD3Ddev->GetVertexShader(ppShader);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pD3Ddev->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pD3Ddev->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::GetViewport(D3DVIEWPORT9 *pViewport) 
{
	return m_pD3Ddev->GetViewport(pViewport);
}

HRESULT APIENTRY IDirect3DDevice9hook::LightEnable(DWORD LightIndex, BOOL bEnable) 
{
	return m_pD3Ddev->LightEnable(LightIndex, bEnable);
}

HRESULT APIENTRY IDirect3DDevice9hook::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->MultiplyTransform(State, pMatrix);
}

HRESULT APIENTRY IDirect3DDevice9hook::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) 
{	
	return m_pD3Ddev->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY IDirect3DDevice9hook::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) 
{
	return m_pD3Ddev->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer,pVertexDecl, Flags);
}

ULONG APIENTRY IDirect3DDevice9hook::Release() 
{
	return m_pD3Ddev->Release();
}

HRESULT APIENTRY IDirect3DDevice9hook::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) 
{
	return m_pD3Ddev->Reset(pPresentationParameters);
	/*
	HRESULT hRet = m_pD3Ddev->Reset(pPresentationParameters);

	if( SUCCEEDED(hRet) )
		m_PresentParam = *pPresentationParameters;

	return hRet;
	*/
}

HRESULT APIENTRY IDirect3DDevice9hook::SetClipPlane(DWORD Index, CONST float *pPlane) 
{
	return m_pD3Ddev->SetClipPlane(Index, pPlane);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus) 
{
	return m_pD3Ddev->SetClipStatus(pClipStatus);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetCurrentTexturePalette(UINT PaletteNumber) 
{
	return m_pD3Ddev->SetCurrentTexturePalette(PaletteNumber);
}

void APIENTRY IDirect3DDevice9hook::SetCursorPosition(int X, int Y, DWORD Flags) 
{
	m_pD3Ddev->SetCursorPosition(X, Y, Flags);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap) 
{
	return m_pD3Ddev->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) 
{
	return m_pD3Ddev->SetDepthStencilSurface(pNewZStencil);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetDialogBoxMode(BOOL bEnableDialogs) 
{
	return m_pD3Ddev->SetDialogBoxMode(bEnableDialogs);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetFVF(DWORD FVF) 
{
	return m_pD3Ddev->SetFVF(FVF);
}

void APIENTRY IDirect3DDevice9hook::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	m_pD3Ddev->SetGammaRamp(iSwapChain,Flags, pRamp);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetIndices(IDirect3DIndexBuffer9* pIndexData) 
{
	return m_pD3Ddev->SetIndices(pIndexData);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetLight(DWORD Index, CONST D3DLIGHT9 *pLight) 
{
	return m_pD3Ddev->SetLight(Index, pLight);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetMaterial(CONST D3DMATERIAL9 *pMaterial) 
{	
	return m_pD3Ddev->SetMaterial(pMaterial);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetNPatchMode(float nSegments) 
{	
	return m_pD3Ddev->SetNPatchMode(nSegments);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries) 
{
	return m_pD3Ddev->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetPixelShader(IDirect3DPixelShader9* pShader) 
{
	return m_pD3Ddev->SetPixelShader(pShader);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) 
{
	return m_pD3Ddev->SetRenderState(State, Value);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) 
{
	return m_pD3Ddev->SetRenderTarget(RenderTargetIndex,pRenderTarget);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) 
{
	return m_pD3Ddev->SetSamplerState(Sampler,Type,Value);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetScissorRect(CONST RECT* pRect) 
{
	return m_pD3Ddev->SetScissorRect(pRect);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetSoftwareVertexProcessing(BOOL bSoftware) 
{
	return m_pD3Ddev->SetSoftwareVertexProcessing(bSoftware);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) 
{
	return m_pD3Ddev->SetStreamSource(StreamNumber, pStreamData,OffsetInBytes, Stride);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetStreamSourceFreq(UINT StreamNumber,UINT Divider)
{	
	return m_pD3Ddev->SetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetTexture(DWORD Stage, IDirect3DBaseTexture9 *pTexture) 
{
	return m_pD3Ddev->SetTexture(Stage, pTexture);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) 
{
	return m_pD3Ddev->SetTextureStageState(Stage, Type, Value);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->SetTransform(State, pMatrix);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) 
{
	return m_pD3Ddev->SetVertexDeclaration(pDecl);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetVertexShader(IDirect3DVertexShader9* pShader) 
{
	return m_pD3Ddev->SetVertexShader(pShader);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY IDirect3DDevice9hook::SetViewport(CONST D3DVIEWPORT9 *pViewport) 
{
	return m_pD3Ddev->SetViewport(pViewport);
}

BOOL APIENTRY IDirect3DDevice9hook::ShowCursor(BOOL bShow) 
{
	return m_pD3Ddev->ShowCursor(bShow);
}

HRESULT APIENTRY IDirect3DDevice9hook::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) 
{
	return m_pD3Ddev->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);
}

HRESULT APIENTRY IDirect3DDevice9hook::TestCooperativeLevel() 
{
	return m_pD3Ddev->TestCooperativeLevel();
}

HRESULT APIENTRY IDirect3DDevice9hook::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) 
{
	return m_pD3Ddev->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);
}

HRESULT APIENTRY IDirect3DDevice9hook::UpdateTexture(IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture) 
{
	return m_pD3Ddev->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT APIENTRY IDirect3DDevice9hook::ValidateDevice(DWORD *pNumPasses) 
{
	return m_pD3Ddev->ValidateDevice(pNumPasses);
}
















void MainInit()
{
	RendDx9Base = (DWORD)GetModuleHandle("RendDX9.dll");
	BF2Base = (DWORD)GetModuleHandle("BF2.exe");

	renddx9_hModule = GetModuleHandle("RendDx9.dll");
	renddx9_base = GetModuleBase(renddx9_hModule);
}

void InitializeMem( void )
{
	if(!OffsetInit)
	{
		dwHealth1 = RendDx9Base + 0x1B21CD;
		dwHealth2 = RendDx9Base + 0x1B22FE;
		dwHealth3 = RendDx9Base + 0x1B234C;
		dwDistance = RendDx9Base + 0x12EDF3;
		dwNameTags = RendDx9Base + 0x12D882; //0x12D85D;
		//dwNameTags2 = RendDx9Base + 0x12D873;
		dwMedicIcon = RendDx9Base + 0x12CD1A;
		dwSupportIcon = RendDx9Base + 0x12CD80;
		dwEngineerIcon = RendDx9Base + 0x12CD61;
		dwMinimap1 = BF2Base + 0x364F99;
		dwMinimap2 = BF2Base + 0x364FBF;
		dwMinimap3 = BF2Base + 0x364FE5;
		dwMinimap4 = BF2Base + 0x365075;
		dwMinimap5 = BF2Base + 0x36509B;
		dwMinimap6 = BF2Base + 0x3650BE;
		dwMinimap7 = BF2Base + 0x36FFC9;
		dwWeaponsUnlocker1 = BF2Base + 0x5B96E;
		dwWeaponsUnlocker2 = BF2Base + 0xEABE5;
		dwMinimapKits = RendDx9Base + 0x363A50;
		dwEnemyMines = RendDx9Base + 0x12D521;
		dwFlashBang = BF2Base + 0x25E6F4;
		dwShellshock = RendDx9Base + 0x4EC65;
		dwRadarIcons = RendDx9Base + 0x12F489;
		dwThdMap = RendDx9Base + 0x0012D1CD;
		dwUnauthorizedCommands1 = BF2Base + 0x28E160;
		dwUnauthorizedCommands2 = BF2Base + 0x28E178;
		dwNoTVGuidedStatic = BF2Base + 0x377B6E;

		original_viewdist = -1.0f;
		patched_viewdist = 2.0f;

		ReadProcessMemory(hand, (void*)dwHealth1, &Health1Orig, sizeof(Health1Orig), 0);
		ReadProcessMemory(hand, (void*)dwHealth2, &Health2Orig, sizeof(Health2Orig), 0);
		ReadProcessMemory(hand, (void*)dwHealth3, &Health3Orig, sizeof(Health3Orig), 0);
		ReadProcessMemory(hand, (void*)dwDistance, &DistanceOrig, sizeof(DistanceOrig), 0);
		ReadProcessMemory(hand, (void*)dwNameTags, &NameTagOrig, sizeof(NameTagOrig), 0);
		//ReadProcessMemory(hand, (void*)dwNameTags2, &NameTags1Orig, sizeof(NameTags1Orig), 0);
		ReadProcessMemory(hand, (void*)dwMedicIcon, &MedicIconOrig, sizeof(MedicIconOrig), 0);
		ReadProcessMemory(hand, (void*)dwSupportIcon, &SupportIconOrig, sizeof(SupportIconOrig), 0);
		ReadProcessMemory(hand, (void*)dwEngineerIcon, &EngineerIconOrig, sizeof(EngineerIconOrig), 0);
		ReadProcessMemory(hand, (void*)dwMinimap1, &Minimap1, sizeof(Minimap1), 0);
		ReadProcessMemory(hand, (void*)dwMinimap2, &Minimap2, sizeof(Minimap2), 0);
		ReadProcessMemory(hand, (void*)dwMinimap3, &Minimap3, sizeof(Minimap3), 0);
		ReadProcessMemory(hand, (void*)dwMinimap4, &Minimap4, sizeof(Minimap4), 0);
		ReadProcessMemory(hand, (void*)dwMinimap5, &Minimap5, sizeof(Minimap5), 0);
		ReadProcessMemory(hand, (void*)dwMinimap6, &Minimap6, sizeof(Minimap6), 0);
		ReadProcessMemory(hand, (void*)dwMinimap7, &Minimap7, sizeof(Minimap7), 0);
		ReadProcessMemory(hand, (void*)dwWeaponsUnlocker1, &WeaponsUnlocker1, sizeof(WeaponsUnlocker1), 0);
		ReadProcessMemory(hand, (void*)dwWeaponsUnlocker2, &WeaponsUnlocker2, sizeof(WeaponsUnlocker2), 0);
		ReadProcessMemory(hand, (void*)dwMinimapKits, &MinimapKits1, sizeof(MinimapKits1), 0);
		ReadProcessMemory(hand, (void*)dwEnemyMines, &EnemyMines1, sizeof(EnemyMines1), 0);
		ReadProcessMemory(hand, (void*)dwFlashBang, &FlashBang1, sizeof(FlashBang1), 0);
		ReadProcessMemory(hand, (void*)dwShellshock, &Shellshock1, sizeof(Shellshock1), 0);
		ReadProcessMemory(hand, (void*)dwRadarIcons, &RadarIcons1, sizeof(RadarIcons1), 0);
		ReadProcessMemory(hand, (void*)dwThdMap, &dMap1, sizeof(dMap1), 0);
		ReadProcessMemory(hand, (void*)dwUnauthorizedCommands1, &UnauthorizedCommands1, sizeof(UnauthorizedCommands1), 0);
		ReadProcessMemory(hand, (void*)dwUnauthorizedCommands2, &UnauthorizedCommands2, sizeof(UnauthorizedCommands2), 0);
		ReadProcessMemory(hand, (void*)dwNoTVGuidedStatic, &NoTVStatic1, sizeof(NoTVStatic1), 0);
		ReadProcessMemory(hand, (void*)(BF2Base + 0x39EAC4), &dwSuppliesOrig, 1, 0);
		ReadProcessMemory(hand, (void*)(BF2Base + 0x37EBAB), &dwArtilleryOrig, 1, 0);
		ReadProcessMemory(hand, (void*)(BF2Base + 0x39E8F1), &dwVehiclesOrig, 1, 0);
		OffsetInit = true;
	}
}


void fCommander(BOOL enable)
{
	if(enable)
	{
		original_commander = local_player->commander;
		local_player->commander = 1;
		bBCommander = TRUE;
	}

	else 
	{
		local_player->commander = original_commander;
		bBCommander = FALSE;
	}
}

void fTeam()
{
	original_team = local_player->player_team;

	if(original_team == 1)
		local_player->player_team = 2;
	else
		local_player->player_team = 1;
}


void fPanel(BOOL enable)
{


	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)(BF2Base + 0x39EAC4), &dwPanelFiller, 1, 0);
		WriteProcessMemory(hand, (void*)(BF2Base + 0x37EBAB), &dwPanelFiller, 1, 0);
		WriteProcessMemory(hand, (void*)(BF2Base + 0x39E8F1), &dwPanelFiller, 1, 0);
		bBPanel = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)(BF2Base + 0x39EAC4), &dwSuppliesOrig, 1, 0);
		WriteProcessMemory(hand, (void*)(BF2Base + 0x37EBAB), &dwArtilleryOrig, 1, 0);
		WriteProcessMemory(hand, (void*)(BF2Base + 0x39E8F1), &dwVehiclesOrig, 1, 0);
		bBPanel = FALSE;
	}

}

void fPanelFullMaker()
{
	//CCommanderHolder* holder = *((CCommanderHolder**) (BF2Base + BF2_COMMANDER_HOLDER));
	if(!hud_manager) return;
	hud_manager->assets->ArtilleryLoading = hud_manager->assets->SuppliesLoading = 0;
	hud_manager->CarsHealth = 1.0f;
	hud_manager->assets->CarsLoadTime = 0.0f;
}

void fCCommander(void)
{

	if(bBCommander == TRUE)
	{
		if(!local_player) return;
		local_player->commander = 1;
	}

	if(bBSquadleader == TRUE)
	{

		BYTE bart = 1;
		DWORD dwteambase;
		DWORD prebase;

		ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
		ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);

		WriteProcessMemory(hand, (void*)(dwteambase+0x111), &bart, 1, 0);
	}
}

void fSquadLeader(BOOL enable)
{

	BYTE bart;
	DWORD dwteambase;
	DWORD prebase;

	if(enable==TRUE)
	{
		bart = 1;

		ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
		ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
		WriteProcessMemory(hand, (void*)(dwteambase+0x111), &bart, 1, 0);
		bBSquadleader = TRUE;
	}
	else
	{
		bart = 0;

		ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
		ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
		WriteProcessMemory(hand, (void*)(dwteambase+0x111), &bart, 1, 0);
		bBSquadleader = FALSE;
	}

}

void fSquad0(void)
{

	BYTE bart = 000000000;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad1(void)
{

	BYTE bart = 000000001;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad2(void)
{

	BYTE bart = 000000002;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad3(void)
{

	BYTE bart = 000000003;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad4(void)
{

	BYTE bart = 000000004;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad5(void)
{

	BYTE bart = 000000005;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad6(void)
{

	BYTE bart = 000000006;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad7(void)
{

	BYTE bart = 000000007;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad8(void)
{

	BYTE bart = 8;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}

void fSquad9(void)
{

	BYTE bart = 9;
	DWORD dwteambase;
	DWORD prebase;

	ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
	ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
	WriteProcessMemory(hand, (void*)(dwteambase+0x10C), &bart, 1, 0);
}





//-----------------------------------------------------------------------------
//----------------Memory Parts-------------------------------------------------
//-----------------------------------------------------------------------------








void fNameTags(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwNameTags, &NameTag,6, 0);
		//WriteProcessMemory(hand, (void*)dwNameTags2, &NameTags1,6, 0);
		bBNameTags = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwNameTags, &NameTagOrig,6, 0);
		//WriteProcessMemory(hand, (void*)dwNameTags2, &NameTags1Orig,6, 0);
		bBNameTags = FALSE;
	}
}

void fDistanceTags(BOOL enable)
{
	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwDistance, &Distance,2, 0);
		bBDistanceTags = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwDistance, &DistanceOrig,2, 0);
		bBDistanceTags = FALSE;
	}
}

void fBars(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwHealth1, &Nop6Bytes, sizeof(Nop6Bytes), 0);
		WriteProcessMemory(hand, (void*)dwHealth2, &Nop6Bytes, sizeof(Nop6Bytes), 0);
		WriteProcessMemory(hand, (void*)dwHealth3, &Nop6Bytes, sizeof(Nop6Bytes), 0);
		bBBars = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwHealth1, &Health1Orig, sizeof(Health1Orig), 0);
		WriteProcessMemory(hand, (void*)dwHealth2, &Health2Orig, sizeof(Health1Orig), 0);
		WriteProcessMemory(hand, (void*)dwHealth3, &Health3Orig, sizeof(Health1Orig), 0);
		bBBars = FALSE;
	}
}

void fNameTagsDistance(int iDistance)
{

	if(iDistance == 120)
	{
		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x237A3C), &HLDistanceDefault, 4, 0);
	}
	else if(iDistance == 400)
	{
		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x237A3C), &HLDistance400, 4, 0);
	}
	else if(iDistance == 800)
	{
		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x237A3C), &HLDistance800, 4, 0);
	}
	else if(iDistance == 1200)
	{
		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x237A3C), &HLDistance1200, 4, 0);
	}
	else if(iDistance == 20000)
	{
		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x237A3C), &HLDistance10000, 4, 0);
	}
	else if(iDistance == 50000)
	{
		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x237A3C), &HLDistance50000, 4, 0);
	}
}

void fIcons(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwMedicIcon, &MedicIcon,2, 0);
		WriteProcessMemory(hand, (void*)dwSupportIcon, &SupportIcon,2, 0);
		WriteProcessMemory(hand, (void*)dwEngineerIcon, &EngineerIcon,2, 0);
		bBIcons = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwMedicIcon, &MedicIconOrig,2, 0);
		WriteProcessMemory(hand, (void*)dwSupportIcon, &SupportIconOrig,2, 0);
		WriteProcessMemory(hand, (void*)dwEngineerIcon, &EngineerIconOrig,2, 0);
		bBIcons = TRUE;
	}
}

void fNoFog(BOOL enable)
{

	if(enable == TRUE) 
	{  
		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x4FA29), &NoFog,3, 0);
		bBNoFog = TRUE; 
	}
	else
	{

		WriteProcessMemory(hand, (void*)(RendDx9Base + 0x4FA29), &NoFogOrig,3, 0); 
		bBNoFog = FALSE;
	}
}

void fViewDistance(BOOL enable)
{
	//int holder_ptrvd = renddx9_base + RENDDX9_VIEWDIST_HOLDER;
	//CViewDistance_holder* holder = *((CViewDistance_holder**) holder_ptrvd);

	//assert(holder);

	float* viewdist_add = &(renderer->maxViewDistance);
	//assert(viewdist_add);

	if(enable)
	{
		if(original_viewdist < 0.0f)
			original_viewdist = renderer->maxViewDistance;

		renderer->maxViewDistance = patched_viewdist;
		bBViewDistance = TRUE;
	}
	else 
		if(original_viewdist > 0.0f)
		{
			renderer->maxViewDistance = original_viewdist;
			bBViewDistance = FALSE;
		}
}


void fMinimap(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwMinimap1, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		WriteProcessMemory(hand, (void*)dwMinimap2, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		WriteProcessMemory(hand, (void*)dwMinimap3, &Nop6Bytes, sizeof(Nop6Bytes), 0);
		WriteProcessMemory(hand, (void*)dwMinimap4, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		WriteProcessMemory(hand, (void*)dwMinimap5, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		WriteProcessMemory(hand, (void*)dwMinimap6, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		WriteProcessMemory(hand, (void*)dwMinimap7, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		bBMinimap = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwMinimap1, &Minimap1, sizeof(Minimap1), 0);
		WriteProcessMemory(hand, (void*)dwMinimap2, &Minimap2, sizeof(Minimap2), 0);
		WriteProcessMemory(hand, (void*)dwMinimap3, &Minimap3, sizeof(Minimap3), 0);
		WriteProcessMemory(hand, (void*)dwMinimap4, &Minimap4, sizeof(Minimap4), 0);
		WriteProcessMemory(hand, (void*)dwMinimap5, &Minimap5, sizeof(Minimap5), 0);
		WriteProcessMemory(hand, (void*)dwMinimap6, &Minimap6, sizeof(Minimap6), 0);
		WriteProcessMemory(hand, (void*)dwMinimap7, &Minimap7, sizeof(Minimap7), 0);
		bBMinimap = FALSE;
	}
}

void fMinimapKits(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwThdMap, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		bBMap = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwThdMap, &dMap1, sizeof(dMap1), 0);
		bBMap = FALSE;
	}
}

void fWeaponUnlocks(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwWeaponsUnlocker1, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		WriteProcessMemory(hand, (void*)dwWeaponsUnlocker2, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		bBWeaponUnlocks = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwWeaponsUnlocker1, &WeaponsUnlocker1, sizeof(WeaponsUnlocker1), 0);
		WriteProcessMemory(hand, (void*)dwWeaponsUnlocker2, &WeaponsUnlocker2, sizeof(WeaponsUnlocker2), 0);
		bBWeaponUnlocks = FALSE;
	}
}

void fEnemyMines(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwEnemyMines, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		bBEnemyMines = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwEnemyMines, &EnemyMines1, sizeof(EnemyMines1), 0);
		bBEnemyMines = FALSE;
	}
}

void fRadarBridgeIcons(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwRadarIcons, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		bBBridgeIcons = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwRadarIcons, &RadarIcons1, sizeof(RadarIcons1), 0);
		bBBridgeIcons = FALSE;
	}
}

void fFlash(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwFlashBang, &Nop6Bytes, sizeof(Nop6Bytes), 0);
		bBFlash = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwFlashBang, &FlashBang1, sizeof(FlashBang1), 0);
		bBFlash = FALSE;
	}
}

void fLockXhair(BOOL enable)
{
	if(enable == TRUE)
	{
	}
	else
	{
	}
}

void fZoom(BOOL enable)
{

	if(enable == TRUE)
	{
		float zoomer = 0.19f;
		float holder = 0.0f;
		DWORD dwteambase;
		DWORD prebase;

		ReadProcessMemory(hand, (void*)(0x9D903C), &prebase, sizeof(prebase), 0);
		ReadProcessMemory(hand, (void*)(prebase+0x60), &dwteambase, sizeof(dwteambase), 0);
		WriteProcessMemory(hand, (void*)(dwteambase+0xC8), &holder, 1, 0);
		WriteProcessMemory(hand, (void*)(dwteambase+0xC4), &zoomer, 1, 0);
	}
}

void fUnauthorizedCommands(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwUnauthorizedCommands1, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		WriteProcessMemory(hand, (void*)dwUnauthorizedCommands2, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		bBCommands = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwUnauthorizedCommands1, &UnauthorizedCommands1, sizeof(UnauthorizedCommands1), 0);
		WriteProcessMemory(hand, (void*)dwUnauthorizedCommands2, &UnauthorizedCommands2, sizeof(UnauthorizedCommands2), 0);
		bBCommands = FALSE;
	}
}

void fShellShock(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwShellshock, &Nop2Bytes, sizeof(Nop2Bytes), 0);
		bBShellShock = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwShellshock, &Shellshock1, sizeof(Shellshock1), 0);
		bBShellShock = FALSE;
	}
}

void fNoTVGuidedStatic(BOOL enable)
{

	if(enable == TRUE)
	{
		WriteProcessMemory(hand, (void*)dwNoTVGuidedStatic, &Nop6Bytes, sizeof(Nop6Bytes), 0);
		bBNoTVGuidedStatic = TRUE;
	}
	else
	{
		WriteProcessMemory(hand, (void*)dwNoTVGuidedStatic, &NoTVStatic1, sizeof(NoTVStatic1), 0);
		bBNoTVGuidedStatic = FALSE;
	}
}

void Cheats_AllowMemPointer(BOOL enableMemHacks)
{
	if(enableMemHacks == FALSE)
	{
		//add_log("In disabling");
		if(bBNameTags == TRUE)
			bbNameTags = TRUE;
		else
			bbNameTags = FALSE;

		if(bBBars == TRUE)
			bbBars = TRUE;
		else
			bbBars = FALSE;

		if(bBIcons == TRUE)
			bbIcons = TRUE;
		else
			bbIcons = FALSE;

		if(bBNoFog == TRUE)
			bbNoFog = TRUE;
		else
			bbNoFog = FALSE;

		if(bBDistanceTags == TRUE)
			bbDistanceTags = TRUE;
		else
			bbDistanceTags = FALSE;

		if(bBMinimap == TRUE)
			bbMinimap = TRUE;
		else
			bbMinimap = FALSE;

		if(bBMap == TRUE)
			bbMap = TRUE;
		else
			bbMap = FALSE;

		if(bBWeaponUnlocks == TRUE)
			bbWeaponUnlocks = TRUE;
		else
			bbWeaponUnlocks = FALSE;

		if(bBEnemyMines == TRUE)
			bbEnemyMines = TRUE;
		else
			bbEnemyMines = FALSE;

		if(bBBridgeIcons == TRUE)
			bbBridgeIcons = TRUE;
		else
			bbBridgeIcons = FALSE;

		if(bBFlash == TRUE)
			bbFlash = TRUE;
		else
			bbFlash = FALSE;

		if(bBCommands == TRUE)
			bbCommands = TRUE;
		else
			bbCommands = FALSE;

		if(bBShellShock == TRUE)
			bbShellShock = TRUE;
		else
			bbShellShock = FALSE;

		if(bBNoTVGuidedStatic == TRUE)
			bbNoTVGuidedStatic = TRUE;
		else
			bbNoTVGuidedStatic = FALSE;

		if(bBPanel == TRUE)
			bbPanel = TRUE;
		else
			bbPanel = FALSE;

		if(bBViewDistance == TRUE)
			bbViewDistance = TRUE;
		else
			bbViewDistance = FALSE;

		//==========================================

		if(bbNameTags == TRUE)
		{
			fNameTags(enableMemHacks);

			//add_log("1");
		}

		if(bbBars == TRUE)
		{
			fBars(enableMemHacks);
			//add_log("2");
		}

		if(bbIcons == TRUE)
		{
			fIcons(enableMemHacks);
			//add_log("3");
		}

		if(bbNoFog == TRUE)
		{
			fNoFog(enableMemHacks);
			//add_log("4");
		}

		if(bbDistanceTags == TRUE)
		{
			fDistanceTags(enableMemHacks);
			//add_log("5");
		}

		if(bbMinimap == TRUE)
		{
			fMinimap(enableMemHacks);
			//add_log("6");
		}

		if(bbMap == TRUE)
		{
			fMinimapKits(enableMemHacks);
			//add_log("7");
		}

		if(bbWeaponUnlocks == TRUE)
		{
			fWeaponUnlocks(enableMemHacks);
			//add_log("8");
		}

		if(bbEnemyMines == TRUE)
		{
			fEnemyMines(enableMemHacks);
			//add_log("9");
		}

		if(bbBridgeIcons == TRUE)
		{
			fRadarBridgeIcons(enableMemHacks);
			//add_log("10");
		}

		if(bbFlash == TRUE)
		{
			fFlash(enableMemHacks);
			//add_log("11");
		}

		if(bbCommands == TRUE)
		{
			fUnauthorizedCommands(enableMemHacks);
			//add_log("12");
		}

		if(bbShellShock == TRUE)
		{
			fShellShock(enableMemHacks);
			//add_log("13");
		}

		if(bbNoTVGuidedStatic == TRUE)
		{
			fNoTVGuidedStatic(enableMemHacks);
			//add_log("14");
		}

		if(bbPanel == TRUE)
		{
			fPanel(enableMemHacks);
			//add_log("15");
		}

		if(bbViewDistance == TRUE)
			fViewDistance(enableMemHacks);
	}

	else 
		if(enableMemHacks == TRUE)
		{
			//add_log("In enable");
			if(bbNameTags == TRUE)
			{
				fNameTags(enableMemHacks);
				//add_log("1-1");
			}

			if(bbBars == TRUE)
			{
				fBars(enableMemHacks);
				//add_log("1-2");
			}

			if(bbIcons == TRUE)
			{
				fIcons(enableMemHacks);
				//add_log("1-3");
			}

			if(bbNoFog == TRUE)
			{
				fNoFog(enableMemHacks);
				//add_log("1-4");
			}

			if(bbDistanceTags == TRUE)
			{
				fDistanceTags(enableMemHacks);
				//add_log("1-5");
			}

			if(bbMinimap == TRUE)
			{
				fMinimap(enableMemHacks);
				//add_log("1-6");
			}

			if(bbMap == TRUE)
			{
				fMinimapKits(enableMemHacks);
				//add_log("1-7");
			}

			if(bbWeaponUnlocks == TRUE)
			{
				fWeaponUnlocks(enableMemHacks);
				//add_log("1-8");
			}

			if(bbEnemyMines == TRUE)
			{
				fEnemyMines(enableMemHacks);
				//add_log("1-9");
			}

			if(bbBridgeIcons == TRUE)
			{
				fRadarBridgeIcons(enableMemHacks);
				//add_log("1-10");
			}

			if(bbFlash == TRUE)
			{
				fFlash(enableMemHacks);
				//add_log("1-11");
			}

			if(bbCommands == TRUE)
			{
				fUnauthorizedCommands(enableMemHacks);
				//add_log("1-12");
			}

			if(bbShellShock == TRUE)
			{
				fShellShock(enableMemHacks);
				//add_log("1-13");
			}

			if(bbNoTVGuidedStatic == TRUE)
			{
				fNoTVGuidedStatic(enableMemHacks);
				//add_log("1-14");
			}

			if(bbPanel == TRUE)
			{
				fPanel(enableMemHacks);
				//add_log("1-15");
			}

			if(bbViewDistance == TRUE)
				fViewDistance(enableMemHacks);
		}
}













//--------------
ID3DXLine *pLiner;
bool FontCreated=false; //For creating the font once
bool do_once = false;
LPD3DXFONT m_font; //You can make as many fonts as you want using different names

//=======================
bool backupfuncs = false;
//=======================

struct basehookcvar_s
{
	float gamespeed;
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
int menuItems = 38;// When adding more thing's/feat's, always make sure to update this
basehookmenu_s menu[38];//Along w/ this also =P

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
float menu34;
float menu35;
float menu36;
float menu37;


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
	menu34 = 0;
	menu35 = 0;
	menu36 = 0;
	menu37 = 0;
}


void InitMenu()
{
	strcpy( menu[0].title, "NameTags" );
	menu[0].value = menu0;
	menu[0].min = 0;
	menu[0].max = 1;
	menu[0].step = 1;

	strcpy( menu[1].title, "Distance Tags" );
	menu[1].value = menu1;
	menu[1].min = 0;
	menu[1].max = 1;
	menu[1].step = 1;

	strcpy( menu[2].title, "Health Bars" );
	menu[2].value = menu2;
	menu[2].min = 0;
	menu[2].max = 1;
	menu[2].step = 1;

	strcpy( menu[3].title, "Kit Icons" );
	menu[3].value = menu3;
	menu[3].min = 0;
	menu[3].max = 1;
	menu[3].step = 1;

	strcpy( menu[4].title, "Minimap" );
	menu[4].value = menu4;
	menu[4].min = 0;
	menu[4].max = 1;
	menu[4].step = 1;

	strcpy( menu[5].title, "3d Map" );
	menu[5].value = menu5;
	menu[5].min = 0;
	menu[5].max = 1;
	menu[5].step = 1;

	strcpy( menu[6].title, "Weapon Unlocks" );
	menu[6].value = menu6;
	menu[6].min = 0;
	menu[6].max = 1;
	menu[6].step = 1;

	strcpy( menu[7].title, "Enemy Mines" );
	menu[7].value = menu7;
	menu[7].min = 0;
	menu[7].max = 1;
	menu[7].step = 1;

	strcpy( menu[8].title, "Radar & Bridge Icons" );
	menu[8].value = menu8;
	menu[8].min = 0;
	menu[8].max = 1;
	menu[8].step = 1;

	strcpy( menu[9].title, "Shellshock Removal" );
	menu[9].value = menu9;
	menu[9].min = 0;
	menu[9].max = 1;
	menu[9].step = 1;

	strcpy( menu[10].title, "Flash Removal" );
	 menu[10].value = menu10;
	menu[10].min = 0;
	menu[10].max = 1;
	menu[10].step = 1;

	strcpy( menu[11].title, "TVG Static Removal" );
	menu[11].value = menu11;
	menu[11].min = 0;
	menu[11].max = 1;
	menu[11].step = 1;

	strcpy( menu[12].title, "Weapon Chams" );
	menu[12].value = menu12;
	menu[12].min = 0;
	menu[12].max = 1;
	menu[12].step = 1;

	strcpy( menu[13].title, "ASUS Wallhack" );
	 menu[13].value = menu13;
	menu[13].min = 0;
	menu[13].max = 1;
	menu[13].step = 1;

	strcpy( menu[14].title, "WhiteWalls" );
	 menu[14].value = menu14;
	menu[14].min = 0;
	menu[14].max = 1;
	menu[14].step = 1;

	strcpy( menu[15].title, "Unauthorized Commands" );
	menu[15].value = menu15;
	menu[15].min = 0;
	menu[15].max = 1;
	menu[15].step = 1;

	strcpy( menu[16].title, "XQZ Wallhack" );
	menu[16].value = menu16;
	menu[16].min = 0;
	menu[16].max = 1;
	menu[16].step = 1;

	strcpy( menu[17].title, "Wireframe" );
	menu[17].value = menu17;
	menu[17].min = 0;
	menu[17].max = 1;
	menu[17].step = 1;

	strcpy( menu[18].title, "Custom Xhair" );
	menu[18].value = menu18;
	menu[18].min = 0;
	menu[18].max = 1;
	menu[18].step = 1;

	strcpy( menu[19].title, "Player Chams" );
	 menu[19].value = menu19;
	menu[19].min = 0;
	menu[19].max = 1;
	menu[19].step = 1;

	strcpy( menu[20].title, "Vehicle Chams" );
	 menu[20].value = menu20;
	menu[20].min = 0;
	menu[20].max = 1;
	menu[20].step = 1;

	strcpy( menu[21].title, "View Distance" );
	 menu[21].value = menu21;
	menu[21].min = 0;
	menu[21].max = 1;
	menu[21].step = 1;

	strcpy( menu[22].title, "Mobile Object Chams" );
	 menu[22].value = menu22;
	menu[22].min = 0;
	menu[22].max = 1;
	menu[22].step = 1;

	strcpy( menu[23].title, "Commander Asset Chams" );
	 menu[23].value = menu23;
	menu[23].min = 0;
	menu[23].max = 1;
	menu[23].step = 1;

	strcpy( menu[24].title, "Mine Chams" );
	 menu[24].value = menu24;
	menu[24].min = 0;
	menu[24].max = 1;
	menu[24].step = 1;

	strcpy( menu[25].title, "No Fog" );
	 menu[25].value = menu25;
	menu[25].min = 0;
	menu[25].max = 1;
	menu[25].step = 1;

	strcpy( menu[26].title, "No Water" );
	 menu[26].value = menu26;
	menu[26].min = 0;
	menu[26].max = 1;
	menu[26].step = 1;

	strcpy( menu[27].title, "No Sky" );
	 menu[27].value = menu27;
	menu[27].min = 0;
	menu[27].max = 1;
	menu[27].step = 1;

	strcpy( menu[28].title, "No Scope" );
	menu[28].value = menu28;
	menu[28].min = 0;
	menu[28].max = 1;
	menu[28].step = 1;

	strcpy( menu[29].title, "No Tank Overlay" );
	 menu[29].value = menu29;
	menu[29].min = 0;
	menu[29].max = 1;
	menu[29].step = 1;

	strcpy( menu[30].title, "Force Commander" );
	 menu[30].value = menu30;
	menu[30].min = 0;
	menu[30].max = 1;
	menu[30].step = 1;

	strcpy( menu[31].title, "Force Squad Leader" );
	 menu[31].value = menu31;
	menu[31].min = 0;
	menu[31].max = 1;
	menu[31].step = 1;

	strcpy( menu[32].title, "Team Switch" );
	 menu[32].value = menu32;
	menu[32].min = 0;
	menu[32].max = 1;
	menu[32].step = 1;

	strcpy( menu[33].title, "Unlimited Assets" );
	 menu[33].value = menu33;
	menu[33].min = 0;
	menu[33].max = 1;
	menu[33].step = 1;

	strcpy( menu[34].title, "Squad Jumping" );
	 menu[34].value = menu34;
	menu[34].min = 0;
	menu[34].max = 1;
	menu[34].step = 1;

	strcpy( menu[35].title, "Aimbot" );
	menu[35].value = menu35;
	menu[35].min = 0;
	menu[35].max = 1;
	menu[35].step = 1;

	strcpy( menu[36].title, "Display Time" );
	menu[36].value = menu36;
	menu[36].min = 0;
	menu[36].max = 1;
	menu[36].step = 1;

	strcpy( menu[37].title, "Display FPS" );
	menu[37].value = menu37;
	menu[37].min = 0;
	menu[37].max = 1;
	menu[37].step = 1;
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
		bMenuInited = true;
		cvar.menu=!cvar.menu;
	}

	if( cvar.menu)
	{
		if(GetAsyncKeyState(0x26)&1) // up arrow
		{
			if( menuIndex>0 ) menuIndex--;
		}
		if(GetAsyncKeyState(0x28)&1) //downarrow
		{
			if( menuIndex<menuItems-1 ) menuIndex++;
		}

		if(GetAsyncKeyState(0x25)&1) //leftarrow
		{
			//if(menu[menuIndex].value == 1)
			//	menu[menuIndex].value = 0;

			if(menuIndex == 0)
			{
				fNameTags( false);
				menu0 = 0; 
				//fNameTagsDistance(50000);
			}

			if(menuIndex == 1)
			{
				fDistanceTags(false);
				menu1 = 0;
			}

			if(menuIndex == 2)
			{
				fBars(false);
				menu2 = 0;
			}

			if(menuIndex == 3)
			{
				fIcons(false);
				menu3 = 0;
			}

			if(menuIndex == 4)
			{
				fMinimap(false);
				menu4 = 0;
			}

			if(menuIndex == 5)
			{
				fMinimapKits(false);
				menu5 = 0;
			}

			if(menuIndex == 6)
			{
				fWeaponUnlocks(false);
				menu6 = 0;
			}

			if(menuIndex == 7)
			{
				fEnemyMines(false);
				menu7 = 0;
			}

			if(menuIndex == 8)
			{
				fRadarBridgeIcons(false);
				menu8 = 0;
			}

			if(menuIndex == 9)
			{
				fShellShock(false);
				menu9 = 0;
			}

			if(menuIndex == 10)
			{
				fFlash(false);
				menu10 = 0;
			}

			if(menuIndex == 11)
			{
				fNoTVGuidedStatic(false);
				menu11 = 0;
			}

			if(menuIndex == 12)
			{
				WeaponChams = false; //WeaponChams;
				menu12 = 0;
			}

			if(menuIndex == 13)
			{
				AsusWallhack = false; //AsusWallhack;
				menu13 = 0;
			}

			if(menuIndex == 14)
			{
				Whitewalls = false; //Whitewalls;
				menu14 = 0;
			}

			if(menuIndex == 15)
			{
				fUnauthorizedCommands(false);
				menu15 = 0;
			}

			if(menuIndex == 16)
			{
				Wallhack = false; //Wallhack;
				menu16 = 0;
			}

			if(menuIndex == 17) 
			{
				Wireframe = false; 
				menu17 = 0;
			} //Wireframe; }

			if(menuIndex == 18) 
			{
				Xhair = false; 
				menu18 = 0;
			} //Xhair; }

			if(menuIndex == 19) 
			{
				Chams = false; 
				menu19 = 0;
			} //Chams; }

			if(menuIndex == 20) 
			{
				VehicleChams = false; 
				menu20 = 0;
			} //VehicleChams; }

			if(menuIndex == 21) 
			{
				fViewDistance(false); 
				menu21 = 0;
			}//GroundWeaponChams = false; } //GroundWeaponChams; }

			if(menuIndex == 22) 
			{
				MobileObjects = false; 
				menu22 = 0;
			} //MobileObjects; }

			if(menuIndex == 23) 
			{
				AssetChams = false; 
				menu23 = 0;
			} //AssetChams; }

			if(menuIndex == 24) 
			{
				MineChams = false; 
				menu24 = 0;
			} //MineChams; }

			if(menuIndex == 25) 
			{
				NoFogo = false; 
				menu25 = 0;
			} //NoFogo; }

			if(menuIndex == 26) 
			{
				NoWater = false; 
				menu26 = 0;
			} //NoWater; }

			if(menuIndex == 27) 
			{
				NoSky = false; 
				menu27 = 0;
			} //NoSky; }

			if(menuIndex == 28) 
			{
				NoScope = false; 
				menu28 = 0;
			} //NoScope; }

			if(menuIndex == 29) 
			{
				NoTankOverlay = false; 
				menu29 = 0;
			} //NoTankOverlay; }

			if(menuIndex == 30)
			{
				bbbCommander = false; //bbbCommander;
				fCommander(false);
				menu30 = 0;
			}

			if(menuIndex == 31)
			{
				bbLeader = false; //bbLeader;
				fSquadLeader(false);
				menu31 = 0;
			}

			if(menuIndex == 32)
			{
				fTeam();
				menu32 = 0;
			}

			if(menuIndex == 33)
			{
				UnlimitedAssets = false; //UnlimitedAssets;
				fPanel(false);
				menu33 = 0;
			}

			if(menuIndex == 34)
			{
				SquadJumping = false;
				menu34 = 0;
			}//SquadJumping;

			if(menuIndex == 35)
			{
				aimbot.Allow(TRUE);
				aimbot.Toggle(FALSE);
				menu35 = 0;
				//aimbot.Allow(TRUE);
			}

			if(menuIndex == 36)
			{
				Time = false; //Time;
				menu36 = 0;
			}

			if(menuIndex == 37)
			{
				FPS = false; //FPS;
				menu37 = 0;
			}
		}

		if(GetAsyncKeyState(0x27)&1) //rightarrow
		{
			//if(menu[menuIndex].value == 0)
			//	menu[menuIndex].value = 1;

			if(menuIndex == 0)
			{
				fNameTags( true);
				menu0 = 1; 
				fNameTagsDistance(50000);
			}

			if(menuIndex == 1)
			{
				fDistanceTags(true);
				menu1 = 1;
			}

			if(menuIndex == 2)
			{
				fBars(true);
				menu2 = 1;
			}

			if(menuIndex == 3)
			{
				fIcons(true);
				menu3 = 1;
			}

			if(menuIndex == 4)
			{
				fMinimap(true);
				menu4 = 1;
			}

			if(menuIndex == 5)
			{
				fMinimapKits(true);
				menu5 = 1;
			}

			if(menuIndex == 6)
			{
				fWeaponUnlocks(true);
				menu6 = 1;
			}

			if(menuIndex == 7)
			{
				fEnemyMines(true);
				menu7 = 1;
			}

			if(menuIndex == 8)
			{
				fRadarBridgeIcons(true);
				menu8 = 1;
			}

			if(menuIndex == 9)
			{
				fShellShock(true);
				menu9 = 1;
			}

			if(menuIndex == 10)
			{
				fFlash(true);
				menu10 = 1;
			}

			if(menuIndex == 11)
			{
				fNoTVGuidedStatic(true);
				menu11 = 1;
			}

			if(menuIndex == 12)
			{
				WeaponChams = true; //WeaponChams;
				menu12 = 1;
			}

			if(menuIndex == 13)
			{
				AsusWallhack = true; //AsusWallhack;
				menu13 = 1;
			}

			if(menuIndex == 14)
			{
				Whitewalls = true; //Whitewalls;
				menu14 = 1;
			}

			if(menuIndex == 15)
			{
				fUnauthorizedCommands(true);
				menu15 = 1;
			}

			if(menuIndex == 16)
			{
				Wallhack = true; //Wallhack;
				menu16 = 1;
			}

			if(menuIndex == 17) 
			{
				Wireframe = true; 
				menu17 = 1;
			} //Wireframe; }

			if(menuIndex == 18) 
			{
				Xhair = true; 
				menu18 = 1;
			} //Xhair; }

			if(menuIndex == 19) 
			{
				Chams = true; 
				menu19 = 1;
			} //Chams; }

			if(menuIndex == 20) 
			{
				VehicleChams = true; 
				menu20 = 1;
			} //VehicleChams; }

			if(menuIndex == 21) 
			{
				fViewDistance(true); 
				menu21 = 1;
			}//GroundWeaponChams = true; } //GroundWeaponChams; }

			if(menuIndex == 22) 
			{
				MobileObjects = true; 
				menu22 = 1;
			} //MobileObjects; }

			if(menuIndex == 23) 
			{
				AssetChams = true; 
				menu23 = 1;
			} //AssetChams; }

			if(menuIndex == 24) 
			{
				MineChams = true; 
				menu24 = 1;
			} //MineChams; }

			if(menuIndex == 25) 
			{
				NoFogo = true; 
				menu25 = 1;
			} //NoFogo; }

			if(menuIndex == 26) 
			{
				NoWater = true; 
				menu26 = 1;
			} //NoWater; }

			if(menuIndex == 27) 
			{
				NoSky = true; 
				menu27 = 1;
			} //NoSky; }

			if(menuIndex == 28) 
			{
				NoScope = true; 
				menu28 = 1;
			} //NoScope; }

			if(menuIndex == 29) 
			{
				NoTankOverlay = true; 
				menu29 = 1;
			} //NoTankOverlay; }

			if(menuIndex == 30)
			{
				bbbCommander = true; //bbbCommander;
				fCommander(true);
				menu30 = 1;
			}

			if(menuIndex == 31)
			{
				bbLeader = true; //bbLeader;
				fSquadLeader(true);
				menu31 = 1;
			}

			if(menuIndex == 32)
			{
				fTeam();
				menu32 = 1;
			}

			if(menuIndex == 33)
			{
				UnlimitedAssets = true; //UnlimitedAssets;
				fPanel(true);
				menu33 = 1;
			}

			if(menuIndex == 34)
			{
				SquadJumping = true;
				menu34 = 1;
			}//SquadJumping;

			if(menuIndex == 35)
			{
				aimbot.Allow(TRUE);
				aimbot.Toggle(TRUE);
				menu35 = 1;
				//aimbot.Allow(TRUE);
			}

			if(menuIndex == 36)
			{
				Time = true; //Time;
				menu36 = 1;
			}

			if(menuIndex == 37)
			{
				FPS = true; //FPS;
				menu37 = 1;
			}

		}
	}
}

void __stdcall CustomPresent(IDirect3DDevice9* pDevice)
{
	//Menu
	if( cvar.menu )
	{
		int x = 35;    
		int y = 10;

		InitMenu();

		int r, g, b;
		int mr, mg, mb;
		int menur, menug, menub;

		r = 255;
		g = 0;
		b = 0;

		mr = 255;
		mg = 255;
		mb = 255;

		menur = 73;
		menug = 73;
		menub = 73;

		drawguibox(x - 10, y - 5, 170, (y - 5) + (15*menuItems) + 15, 255, 255, 240, 255, menur, menug, menub, 232);

		DrawConString( x + 30, y, 255, 255, 0, 255, "D100 - Hack Menu" );

		for(int i=0;i<menuItems;i++)
		{
			if( i!=menuIndex )
			{
				DrawConString( x, (y + 15) + (15*i), mr, mg, mb, 255,  menu[i].title );
				DrawConString( x + 135, (y + 15) + (15*i), mr, mg, mb, 255, "%2.2f", menu[i].value );
			}
			else
			{
				DrawConString( x, (y + 15) + (15*i), r, g, b, 255, menu[i].title );
				DrawConString( x + 135, (y + 15) + (15*i), r, g, b, 255, "%2.2f", menu[i].value );
			}
		}
	}

	/*if(*dwPlayerPointerBase){ //IF YOU ARE INGAME....CHANGE THIS IF YOU MUST
	//---------------------------------------------------
	if(!backupfuncs){
	cvar.gamespeed = 1.0f; //normal speed defualt
	backupfuncs = true;
	}*/
	//===================
	// float* fGameSpeed = (float*)0xB7CB64;
	//===================

	/*-----------------------------------------------------*/
	DoKeyCheck();
	/*-----------------------------------------------------*/

	//--------------------
//	*fGameSpeed = (float)cvar.gamespeed;
	//--------------------
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
			D3DXCreateFont(pDevice, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
		}
		do_once = true;
	}
}