#ifndef BF2Hackerz_h
#define BF2Hackerz_h


// global vars used throughout the code, defined
// BF2Hackerz++.cpp

// for now these strings are used in initialization only
extern string BF2Hackerz_dir;
extern string BF2Hackerz_dll;
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


extern DWORD this_frame;
extern DWORD last_frame;
extern float frame_time;



class D3DHook;
extern D3DHook d3d_hook;


int GetModuleBase( HMODULE hModule );


/
void InitializeMem( void );
void MainInit();


BOOL GeneralInit(void);



);



#define CAST( type, func) ( ((type) (func)) )
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))



#endif