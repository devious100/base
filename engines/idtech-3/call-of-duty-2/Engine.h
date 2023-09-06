/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/************/
/* Engine.h */
/************/

#pragma once

#define REF_API_VERSION 60

#define	RF_MINLIGHT			1
#define	RF_THIRD_PERSON		2
#define	RF_FIRST_PERSON		4
#define	RF_DEPTHHACK		8
#define	RF_NOSHADOW			64
#define RF_LIGHTING_ORIGIN	128
#define	RF_SHADOW_PLANE		256
#define	RF_WRAP_FRAMES		512

#define ET_PLAYER			1
#define EF_CROUCHING		0x00000004
#define EF_PRONE			0x00000008
#define EF_UNKNOWN			0x00000020
#define EF_DEAD				0x00020000
#define EF_SIGHT			0x00040000

#define STRING_TYPE			0x7FFFFFFF
#define TEXTSTYLE_SHADOW	3
#define TEXTSTYLE_OUTLINE	4

#define	MAX_CLIENTS				64
#define	GENTITYNUM_BITS			10
#define	MAX_GENTITIES			(1<<GENTITYNUM_BITS)
#define	ENTITYNUM_NONE			(MAX_GENTITIES-1)
#define	ENTITYNUM_WORLD			(MAX_GENTITIES-2)
#define	ENTITYNUM_MAX_NORMAL	(MAX_GENTITIES-2)

#define KEYCATCH_CONSOLE		0x0001
#define	KEYCATCH_UI				0x0002
#define	KEYCATCH_MESSAGE		0x0004
#define	KEYCATCH_CGAME			0x0008

typedef enum {
	qfalse,
	qtrue
} qboolean;

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

typedef int qhandle_t;

typedef struct {
	vec3_t axis[3];
	vec3_t origin;
} orientation_t;

typedef struct {
	unsigned char padding[44];
} gfxconfig_t;

// paramters for command buffer stuffing
typedef enum {
	EXEC_NOW,			// don't return until completed, a VM should NEVER use this,
						// because some commands might cause the VM to be unloaded...
	EXEC_INSERT,		// insert at current position, but don't run yet
	EXEC_APPEND			// add to end of the command buffer (normal case)
} cbufExec_t;

typedef void (*xcommand_t)( );
typedef struct {
	const char *	name;
	int				flags;
	LPVOID			value;
	LPVOID			value2;
	LPVOID			value3;
	LPVOID			min;
	LPVOID			max;
	int				pad[2];
} dvar_t;

typedef enum {
	h_high,
	h_low,
	h_dontcare
} ha_pref;

typedef struct {
	int				reType;
	int				renderfx;
	vec3_t			lightingOrigin;
	vec3_t			axis[3];
	float			axisScale; //normally 1.0
	vec3_t			origin;

	long			padding1[4];

	BYTE			shaderRGBA[4];

	long			padding2[6];
} refEntity_t; //size=116, dw=29

typedef struct {
	int					pad[20];
	int *				unknown;
	int					pad1[4];
	//int				pad[25];
} shaderInfo_t;

typedef struct {
	int				x;
	int				y;
	int				width;
	int				height;
	float			fov_x;
	float			fov_y;
	vec3_t			vieworg;
	vec3_t			viewaxis[3];
	int				time;
	int				rdflags;
	BYTE			areamask[8]; //?
} refdef_t; //size=88, dw=22

typedef struct {
	int		pad[42];

	int		clientNum;

	int		pad0[2435];

	int		health;

	int		pad1[122];
} playerState_t; //size=10404, dw=2601

typedef struct {
	int		trType;
	int		trTime;
	int		trDuration;
	vec3_t	trBase;
	vec3_t	trDelta;
} trajectory_t;

typedef struct {
	int				number;
	int				eType;
	int				eFlags;
	trajectory_t	pos;
	trajectory_t	apos;

	int				pad[8];

	int				otherEntityNum;
	int				otherEntityNum2;

	int				pad4[4];

	int				unknown;
	int				clientNum;

	int				pad2[2];

	int				solid;
	int				eventParm;

	int				pad5[9];

	int				weapon;
	int				legsAnim;
	int				torsoAnim;

	int				pad3[7];
} entityState_t; //size=240, dw=60

typedef struct {
	entityState_t	currentState;
	entityState_t	nextState;
	qboolean		currentValid;
	int				pad[2];
	vec3_t			lerpOrigin;
	int				pad2[11];
} centity_t; //size=548, dw=137

typedef struct {
	int				pad[12];
	playerState_t	ps;
} snapshot_t;

typedef struct {
	int				clientFrame;
	int				clientNum;

	int				padding[7];

	snapshot_t *	snap;
	snapshot_t *	nextSnap;

	int				padding0[41297];

	//41308
	refdef_t		refdef;
	vec3_t			refdefViewAngles;

	//int				padding2[3761];
	int				padding2[3582];
	int				crosshairClientNum;
	int				crosshairClientTime;
	int				padding3[3];
	int				crosshairClientHealth; //?
	int				padding4[173];

	vec3_t			kick_angles;
	int				padding1[205363];

	//int				padding1[209139];
} cg_t; //size=1001888, dw=250472

typedef enum {
	TEAM_FREE,
	TEAM_ALLIES,
	TEAM_AXIS,
	TEAM_SPECTATOR,

	TEAM_NUM_TEAMS
} team_t;

typedef struct {
	qboolean	infoValid;
	qboolean	unknown;
	int			clientNum;
	char		name[32];
	team_t		team; 
	int			pad[4];

	char		bodymodel[64];
	char		headmodel[64];
	char		helmetmodel[64];

	//int			pad1[215];
	int			pad1[203];
	int *		pointer;
	int			pad4[11];

	int			movement;

	int			pad7[3];

	qboolean	usingMountedWeapon;

	int			pad3[13];

	int *		unknown_ptr;
	int			curWeapon;	

	int			pad5[3];
} clientInfo_t; //size=1208, dw=302

typedef struct {
	int pad0[43605];
} cgs_t; //size=174420, dw=43605
//start=014C3700
//end  =014EE054

typedef struct {
	float		fraction;
	vec3_t		endpos;
	int			surfaceFlags;
	qboolean	allSolid;
	int			contents;
	short		entityNum;
	short		pad0;
	int			pad1;
} trace_t;

typedef const char *	enum_type;
typedef int *			fontInfo_t;

/* Tag stuff */
//typedef WORD			tag_t;
/*************/

typedef struct
{
	float	x, y;
	float	w, h;
	float	s1, t1, s2, t2;
} rect_t;

typedef struct
{
	vec3_t	xyz;
	float	st[2];
	float	pad;
	int		negative1;
	float	modulate[4];
	int		pad2[6];
	//int pad[17];
} polyVert_t; //size=68, dw=17

typedef struct {
	void			(*Printf)					( int print_level, const char * fmt, ... );
	void			(*Error)					( int level, const char * error, ... );
	int				(*Milliseconds)				( );
	int				(*iswcntrl)					( unsigned short wc );
	void *			(*Malloc)					( int size );
	void *			(*Malloc2)					( int size );
	void			(*Free)						( void * ptr );
	void *			(*Hunk_Alloc)				( int size, ha_pref preference );
	void *			(*Hunk_VirtualAlloc)		( unsigned long size );
	void			(*Hunk_VirtualAllocEx)		( void * address, unsigned long size );
	void			(*Hunk_VirtualFreeEx)		( void * address, unsigned long size );
	void			(*Hunk_VirtualFree)			( void * address );
	void			(*Hunk_FreeTempMemory)		( void * buf );
	void *			(*Hunk_AllocateTempMemory)	( int size );
	void			(*Hunk_Unknown1)			( );
	void			(*Hunk_Unknown2)			( );
	void			(*CL_Unknown1)				( int arg1 );
	void			(*CL_Unknown2)				( int arg1 );
	void *			(*CL_Unknown3)				( int index );
	void			(*Win_Init)					( );
	void			(*Win_Show)					( );
	void			(*Win_Hide)					( );
	void			(*Win_GetMessage)			( MSG msg );
	void			(*func24)					( );
	void			(*func25)					( );
	unsigned long	(*GenerateStringHash)		( const char * string );
	dvar_t *		(*Dvar_Registerb)			( const char * var_name, qboolean var_value, int flags ); //boolean
	dvar_t *		(*Dvar_Registerir)			( const char * var_name, int var_value, int min, int max, int flags ); //integer range
	dvar_t *		(*Dvar_Registerf)			( const char * var_name, float var_value, float min, float max, int flags ); //float range
	dvar_t *		(*Dvar_Registers)			( const char * var_name, const char * var_value, int flags ); //string
	dvar_t *		(*Dvar_Registere)			( const char * var_name, enum_type enum_values[], int initial, int flags ); //enum
	dvar_t *		(*Dvar_Register4f)			( const char * var_name, float a, float b, float c, float d, int flags); //colour
	dvar_t *		(*Dvar_Register2fr)			( const char * var_name, float a, float b, float min, float max, int flags ); //2D vector
	dvar_t *		(*Dvar_Register3fr)			( const char * var_name, float a, float b, float c, float min, float max, int flags ); //3D vector
	dvar_t *		(*Dvar_Register4fr)			( const char * var_name, float a, float b, float c, float d, float min, float max, int flags ); //4D vector
	void			(*Dvar_SetCheatState)		( dvar_t * dv );
	void			(*Dvar_UnsetCheatState)		( dvar_t * dv );
	void			(*Dvar_ChangeEnum)			( dvar_t * dv, enum_type enum_values[] );
	void			(*Dvar_Unknown1)			( int flags );
	void			(*Dvar_Unknown2)			( dvar_t * dv, int value1 );
	void			(*Dvar_Unknown3)			( dvar_t * dv );
	void			(*Dvar_Setb)				( dvar_t * dv, bool value );
	void			(*Dvar_Seti)				( dvar_t * dv, int value );
	void			(*Dvar_Setf)				( dvar_t * dv, float value );
	void			(*Dvar_Sets)				( dvar_t * dv, const char * value );
	void			(*Dvar_Set4fc)				( dvar_t * dv, float r, float g, float b, float a ); //colour
	void			(*Dvar_Set2f)				( dvar_t * dv, float a, float b );
	void			(*Dvar_Set3f)				( dvar_t * dv, float a, float b, float c );
	void			(*Dvar_Set4f)				( dvar_t * dv, float a, float b, float c, float d );
	dvar_t *		(*Dvar_Get)					( dvar_t * dv, const char * value );
	dvar_t *		(*Dvar_Getb)				( dvar_t * dv, bool value );
	dvar_t *		(*Dvar_Geti)				( dvar_t * dv, int value );
	dvar_t *		(*Dvar_Getf)				( dvar_t * dv, float value );
	dvar_t *		(*Dvar_Gets)				( dvar_t * dv, const char * value );
	dvar_t *		(*Dvar_Get4fc)				( dvar_t * dv, float r, float g, float b, float a );
	dvar_t *		(*Dvar_Get2f)				( dvar_t * dv, float a, float b );
	dvar_t *		(*Dvar_Get3f)				( dvar_t * dv, float a, float b, float c );
	dvar_t *		(*Dvar_Get4f)				( dvar_t * dv, float a, float b, float c, float d );
	dvar_t *		(*Dvar_Get2)				( dvar_t * dv, const char * value );
	qboolean		(*Dvar_BoolValue)			( dvar_t * dv );
	int				(*Dvar_IntValue)			( dvar_t * dv );
	float			(*Dvar_FloatValue)			( dvar_t * dv );
	char *			(*Dvar_StringValue)			( dvar_t * dv );
	char *			(*Dvar_FormattedValue)		( dvar_t * dv ); //works with all types of dvars
	char *			(*Dvar_EnumValue)			( dvar_t * dv );
	void			(*Dvar_Set)					( dvar_t * dv, void * value);
	void			(*Cmd_AddCommand)			( const char * cmd_name, xcommand_t function );
	void			(*Cmd_RemoveCommand)		( const char * cmd_name );
	int				(*Cmd_Argc)					( );
	char *			(*Cmd_Argv)					( int arg );
	void			(*Cmd_ExecuteText)			( int exec_when, const char * text );
	qboolean		(*func72)					( char * list, int num, char * buffer, int size );
	void			(*func73)					( );
	void			(*func74)					( );
	void			(*func75)					( );
	void			(*func76)					( );
	void			(*func77)					( );
	void			(*func78)					( );
	int				(*FS_ReadFile)				( const char * qpath, void ** buffer );
	void			(*Hunk_FreeTempMemory2)		( void * buf );
	void *			(*FS_FOpenFileRead)			( const char * qpath, int arg2, int mode );
	void			(*FS_WriteFile)				( const char * qpath, void * buffer, int size );
	void			(*FS_FreeFile)				( void * buffer );
	void			(*FS_Unknown1)				( int, int, int, int, int );
	qboolean		(*FS_DoesFileExist)			( const char * qpath );
	void *			(*FS_FOpenFile)				( const char * qpath, int arg2, int mode );
	void			(*FS_Unknown2)				( int );
	void *			(*FS_Read)					( void * buffer, int count, int mode );
	int				(*FS_Write)					( void * buffer, int count, int mode );
	void			(*FS_Unknown3)				( int, int, int, int );
	void			(*BoxTrace)					( trace_t * result, vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, qhandle_t model, int mask );
	void			(*func92)					( );
	void			(*func93)					( );
	void			(*func94)					( );
	void			(*func95)					( );
	void			(*func96)					( );
	void			(*func97)					( );
	void			(*func98)					( );
	void			(*func99)					( );
	void			(*func100)					( );
	void			(*func101)					( );
	void			(*func102)					( );
	void			(*func103)					( );
	void			(*func104)					( );
	void			(*func105)					( );
	void			(*func106)					( );
	void			(*func107)					( );
	void			(*func108)					( );
	void			(*func109)					( );
	void			(*func110)					( );
	void			(*func111)					( );
	void			(*func112)					( );
	void			(*func113)					( );
	void			(*func114)					( );
	void			(*func115)					( );
	void			(*func116)					( );
	void			(*func117)					( );
	void			(*func118)					( );
	void			(*func119)					( );
	void			(*func120)					( );
	void			(*func121)					( );
	void			(*func122)					( );
	void			(*func123)					( );
	void			(*func124)					( );
	void			(*func125)					( );
	qboolean		(*SetEvent)					( HANDLE hEvent );
	void			(*func127)					( );
	void			(*func128)					( );
	void			(*func129)					( );
	void			(*func130)					( );
	void			(*func131)					( );
	void			(*func132)					( );
	void			(*func133)					( );
	void			(*func134)					( );
	void			(*func135)					( );
	void			(*func136)					( );
	void			(*func137)					( );
	void			(*func138)					( );
	void			(*func139)					( );
	void			(*func140)					( );
	void			(*func141)					( );
	void			(*func142)					( );
	void			(*func143)					( );
	void			(*func144)					( );
	void			(*func145)					( );
	void			(*func146)					( );
	void			(*func147)					( );
	void			(*func148)					( );
	void			(*func149)					( );
	void			(*func150)					( );
	void			(*func151)					( );
	void			(*func152)					( );
	void			(*func153)					( );
	void			(*func154)					( );
	void			(*func155)					( );
	void			(*func156)					( );
	void			(*func157)					( );
	void			(*func158)					( );
	void			(*func159)					( );
	void			(*func160)					( );
	void			(*func161)					( );
	void			(*func162)					( );
	void			(*func163)					( );
	void			(*func164)					( );
	void			(*func165)					( );
	void			(*func166)					( );
	void			(*func167)					( );
	void			(*func168)					( );
	void			(*func169)					( );
	void			(*func170)					( );
	void			(*func171)					( );
	void			(*func172)					( );
	void			(*func173)					( );
	void			(*func174)					( );
	void			(*func175)					( );
	void			(*func176)					( );
	void			(*func177)					( );
	void			(*func178)					( );
	void			(*func179)					( );
	void			(*func180)					( );
	void			(*func181)					( );
} refimport_t; //size: 181 * 4 = 724 (0x2D4)

typedef struct
{
	void			(*Shutdown)					( );
	void			(*BeginRegistration)		( gfxconfig_t * glConfig );
	qhandle_t		(*RegisterModel)			( const char * name );
	int				(*func4)					( int );
	qhandle_t		(*RegisterShader)			( const char * name, int, int );
	int				(*RegisterShaderNoMip)		( const char * name, int );
	qboolean		(*func7)					( int );
	void			(*RegisterGraphics)			( int, int );
	void			(*func9)					( );
	void			(*func10)					( );
	void			(*func11)					( );
	int				(*func12)					( );
	void			(*func13)					( );
	void			(*func14)					( );
	void			(*func15)					( );
	void			(*func16)					( );
	void			(*func17)					( );
	void			(*func18)					( );
	void			(*func19)					( );
	int				(*AddRefEntityToScene)		( refEntity_t * ent, shaderInfo_t * si, centity_t * cent );
	void			(*func21)					( );
	void			(*func22)					( );
	void			(*func23)					( );
	void			(*func24)					( );
	void			(*func25)					( );
	void			(*func26)					( );
	void			(*AddPolyToScene)			( qhandle_t shader, short numPolys, short numVerts, const polyVert_t * verts );
	int *			(*AddDLightToScene)			( vec3_t origin, float intensity, float r, float g, float b );
	void			(*func29)					( );
	void			(*func30)					( );
	void			(*func31)					( );
	void			(*func32)					( );
	void			(*func33)					( );
	void			(*func34)					( );
	void			(*func35)					( );
	void			(*func36)					( );
	void			(*func37)					( );
	void			(*RenderScene)				( refdef_t * fd );
	void			(*func39)					( );
	void			(*func40)					( );
	void			(*func41)					( );
	void			(*func42)					( );//nullsub
	void			(*SetColour)				( float * colour );
	void			(*DrawStretchPic)			( float x, float y, float width, float height, float s1, float t1, float s2, float t2, float * colour, qhandle_t shader );
	void			(*DrawRotatedPic)			( float x, float y, float width, float height, float s1, float t1, float s2, float t2, float angle, float * colour, qhandle_t shader );
	void			(*DrawPicture)				( qhandle_t shader, float x, float y, float width, float height, int zero1, int zero2, float useColour1, float useColour2 );
	void			(*DrawRect)					( rect_t * rect, float * colour, qhandle_t shader );
	void			(*DrawSomething)			( qhandle_t shader, rect_t * rect, vec3_t vec1 );
	void			(*func49)					( );
	void			(*func50)					( );
	void			(*func51)					( );
	void			(*func52)					( );
	void			(*BeginFrame)				( int zero );
	void			(*EndFrame)					( );
	void			(*func55)					( );
	void			(*func56)					( );
	void			(*func57)					( );
	void			(*func58)					( );
	void			(*func59)					( );
	void			(*func60)					( );
	void			(*func61)					( );
	void			(*func62)					( );
	fontInfo_t *	(*RegisterFont)				( const char * fontName, int pointSize );
	void			(*func64)					( );
	void			(*func65)					( );
	void			(*func66)					( );
	void			(*func67)					( );
	void			(*func68)					( );
	void			(*func69)					( );
	void			(*func70)					( );
	void			(*func71)					( );
	void			(*func72)					( vec3_t, int, vec3_t, int );
	void			(*func73)					( );
	void			(*SCR_Unknown1)				( int, int );
	fontInfo_t *	(*FontScale)				( fontInfo_t * font, float scale );
	int				(*StringWidth)				( const char * text, int limit, fontInfo_t * font );
	int				(*StringHeight)				( fontInfo_t * font );
	void			(*DrawStringExt)			( const char * text, int type, fontInfo_t * font, float x, float y, float scalex, float scaley, float * colour, int style );
	void			(*DrawBigString_)			( const char * text, int type, vec3_t vec1, vec3_t vec2, vec3_t vec3, float * colour );
	void			(*func80)					( );
	void			(*func81)					( );
	void			(*func82)					( );
	void			(*func83)					( );
	void			(*func84)					( );
	void			(*func85)					( );
	void			(*func86)					( );
	void			(*DuplicateFont)			( fontInfo_t * in, fontInfo_t * out );
	void			(*func88)					( );
	void			(*func89)					( );
	void			(*func90)					( );
	void			(*func91)					( );
	void			(*func92)					( );
	void			(*func93)					( );
	void			(*func94)					( );
	void			(*func95)					( );
	void			(*func96)					( );
	void			(*func97)					( );
	void			(*func98)					( );
	void			(*func99)					( );
	void			(*func100)					( );
	void			(*func101)					( );
	void			(*func102)					( );
	void			(*func103)					( );
	void			(*func104)					( );
	void			(*func105)					( );
	void			(*func106)					( );
	void			(*func107)					( );
	void			(*RenderScene2)				( refdef_t * fd );
} refexport_t; //size: 108 * 4 = 432 (0x1B0)

//////////////////////////////////////////////////
// Globals
extern refimport_t *	ri;
extern refexport_t *	re;
extern refimport_t *	o_ri;
extern refexport_t *	o_re;

extern cg_t *			cg;
extern centity_t *		cg_entities;
extern clientInfo_t *	clientinfo;
extern float *			viewangles;
extern short *			delta_angles;
extern int *			ping;
extern int *			r_numentities;
extern int *			keycatchers;

extern float			fZoomFactor;
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Engine Functions
typedef char *		(*t_va)						( const char *, ... );
typedef DWORD		(*t_Com_Milliseconds)		( );
typedef void		(*t_Con_Close)				( );
typedef void		(*t_DrawStringExt)			( const char * text, int type, float x, float y, float scale, float * colour, int style );
typedef void		(*t_CG_Trace)				( trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask );
typedef void		(*t_CM_BoxTrace)			( trace_t *result, const vec3_t start, qhandle_t model, int brushmask );
typedef void		(*t_CG_ClipMoveToEntities)	( const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask, trace_t * tr );
typedef int			(*t_CG_PointContents)		( vec3_t point, int passEntityNum, int andContents );
typedef void		(*t_FX_PlayEffect)			( int effect, vec3_t origin, int zero );

typedef void		(*t_CG_Obituary)			( );
typedef void		(*t_CG_FireRecoil)			( );
typedef void		(*t_CG_Player)				( );

extern const		t_va					va;
extern const		t_Com_Milliseconds		Com_Milliseconds;
extern const		t_Con_Close				Con_Close;
extern const		t_DrawStringExt			DrawStringExt;
extern const		t_CG_Trace				CG_Trace;
extern const		t_CM_BoxTrace			CM_BoxTrace;
extern const		t_CG_ClipMoveToEntities	CG_ClipMoveToEntities;
extern const		t_CG_PointContents		CG_PointContents;

extern				t_CG_Obituary			o_CG_Obituary;
extern				t_CG_FireRecoil			o_CG_FireRecoil;
extern				t_CG_Player				o_CG_Player;

extern const		xcommand_t				StartAttack;
extern const		xcommand_t				StopAttack;
extern const		xcommand_t				ToggleCrouch;
extern const		xcommand_t				ToggleProne;

void				RE_Shutdown				( );
void				RE_EndFrame				( );
void				RE_RenderScene			( refdef_t * fd );
int					RE_AddRefEntityToScene	( refEntity_t * ent, shaderInfo_t * si, centity_t * cent );
void				RE_BeginRegistration	( gfxconfig_t * gfxConfig );
void				h_CG_Obituary			( );
void				h_CG_FireRecoil			( );
void				h_CG_Player				( );
//////////////////////////////////////////////////