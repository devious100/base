#ifndef configuration_switches_h
#define configuration_switches_h



//////////////////////////////////////////////////////////////////////////
// choose the version that the hack will be compiled for
#define BF2_VERSION_DEMO 1
#define BF2_VERSION_RETAIL141 2

#define TARGET_VERSION BF2_VERSION_RETAIL141



//////////////////////////////////////////////////////////////////////////
// comment to disable the console log
#define USE_CONSOLE


//////////////////////////////////////////////////////////////////////////
// the first define is used to limit the number of pixels in each axis the
// aimbot may change each frame (can be disabled by commenting it).
// the second is the angle that it will search for targets.
#define AIMBOT_MAX_PIXEL_CHANGE 40
#define AIMBOT_COVER_ANGLE		D3DXToRadian(30)



//////////////////////////////////////////////////////////////////////////
// I'm using the file sizes to check the game version. comment this if you
// don't want those checks (for instance, if using an unpacked exe).
// However I recommend that the pbcl check stays aways activated since
// punkbuster will update itself without warning.
#define CHECK_PBCL_SIZE
#define CHECK_BF2_FILENAME
#define CHECK_BF2_SIZE



//////////////////////////////////////////////////////////////////////////
// Punkbuster defines

#define PBCL_FILE_SIZE 479232

#define PBCL_CHECK_FNADD 0x31920
#define PBCL_SSHOT_FNADD 0x0E9E0




#endif //configuration_switches_h