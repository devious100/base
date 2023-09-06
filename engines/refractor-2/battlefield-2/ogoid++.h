#ifndef ogoid_h
#define ogoid_h


#include "default_includes.h"

#include "bf2/bf2.h"


#include "cheats/cheats.h"


// global vars used throughout the code, defined
// ogoid++.cpp

// for now these strings are used in initialization only
extern string ogoid_dir;
extern string ogoid_dll;
extern string config_file;
extern string sshot_dir;

extern string game_dir;
extern string bf2_exe;
extern string pbcl_dll;


//
extern HMODULE  bf2_hModule;
extern int		bf2_base;
extern int		bf2_entry;
extern HANDLE	bf2_hThread;

extern BOOL		pbcl_initialized;
extern BOOL		renddx9_initialized;

extern int		pbcl_base;
extern HMODULE	pbcl_hModule;
extern int		renddx9_base;
extern HMODULE	renddx9_hModule;


extern HWND  bf2_hWnd;
//extern int	 bf2_wnd_width;
//extern int	 bf2_wnd_height;
//extern float bf2_wnd_aspect;


extern DWORD this_frame;
extern DWORD last_frame;
extern float frame_time;



class D3DHook;
extern D3DHook d3d_hook;


int GetModuleBase( HMODULE hModule );


/*#ifdef USE_CONSOLE
#define LOGMSG( x ) cout << x << endl;
#else
#define LOGMSG( x ) ;
#endif*/

// defined in detoured_functions.cpp
void Cheats_AllowMemPointer(BOOL enableMemHacks);
void ScrCheatsAllow(BOOL enable);
void InitializeMem( void );
//void InitDetours();
void MainInit();

BOOL GeneralInit(void);



// defined in detoured_functions.cpp
void InitDetours();
BOOL SetGeneralDetours( BOOL enable );
BOOL SetMainLoopDetours(BOOL enable);
BOOL SetPbclDetours(BOOL enable);


// in config_file.cpp
//BOOL Configure( void );



#define CAST( type, func) ( ((type) (func)) )
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))



#endif // ogoid_h