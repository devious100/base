////////////////////////////////////////////////////////////////////////////////
//! SourceBase By Chod
//! -------------------------------------------------------------------------
//! SourceBase is free software: you can redistribute it and/or modify 
//! it under the terms of the GNU Lesser General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//! 
//! SourceBase is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//! GNU Lesser General Public License for more details.
//! 
//! You should have received a copy of the GNU Lesser General Public License
//! along with SourceBase. If not, see <http://www.gnu.org/licenses/>.
//! -------------------------------------------------------------------------
//! \file	SDK.h
//! \brief  Custom SDK header
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef SDK_H
#define SDK_H

#include "..\Include.h"
#include "Vector3.h"

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

struct matrix3x4_t;
class model_t;
class CBaseEntity;

#define	MAX_EDICT_BITS			11
#define NUM_ENT_ENTRY_BITS		(MAX_EDICT_BITS + 1)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)

// m_lifeState values
#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

class CGlobalVarsBase
{
public:
	
	float			realtime;
	int				framecount;
	float			absoluteframetime;
	float			curtime;
	float			frametime;
	int				maxClients;
	int				tickcount;
	float			interval_per_tick;
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;	
};


enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class CUserCmd
{
public:
	virtual void dtor() = 0;

	int			command_number; // 0x4
	int			tick_count;		// 0x8
	CVector3	viewangles;		// 0xC
#ifdef HACK_CSGO
	float		u1[3];			// 0x24
#endif
	float		forwardmove;	// 0x18
	float		sidemove;		// 0x1C
	float		upmove;         // 0x20

	int			buttons;		// 0x30
	BYTE		impulse;        // 0x34
	int			weaponselect;	
	int			weaponsubtype;
	int			random_seed;
	short		mousedx;
	short		mousedy;
	bool		hasbeenpredicted;
};

typedef struct player_info_s
{
#ifdef HACK_CSGO
	unsigned char	pad[0x8];
#endif
	char			name[32];
	int				userID;
	char			guid[32 + 1];
	unsigned int	friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	unsigned long	customFiles[4];
	unsigned char	filesDownloaded;
} player_info_t;

class DVariant
{
public:
	union
	{
		float	m_Float;
		long	m_Int;
		char	*m_pString;
		void	*m_pData;
		float	m_Vector[3];
	};
};

struct RecvProp;

class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;
	DVariant		m_Value;
	int				m_iElement;
	int				m_ObjectID;
};


typedef void (*RecvVarProxyFn)( const CRecvProxyData *pData, void *pStruct, void *pOut );

struct RecvTable
{
	RecvProp		*m_pProps;
	int				m_nProps;
	void			*m_pDecoder;
	char			*m_pNetTableName;
	bool			m_bInitialized;
	bool			m_bInMainList;
};

struct RecvProp
{
	char					*m_pVarName;
	int						m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;
	bool					m_bInsideArray;
	const void				*m_pExtraData;
	RecvProp				*m_pArrayProp;
	void*					m_ArrayLengthProxy;	
	void*					m_ProxyFn;
	void*					m_DataTableProxyFn;
	RecvTable				*m_pDataTable;
	int						m_Offset;	
	int						m_ElementStride;
	int						m_nElements;
	const char				*m_pParentArrayPropName;
};

struct ClientClass
{
	void*			m_pCreateFn;
	void*			m_pCreateEventFn;
	char			*m_pNetworkName;
	RecvTable		*m_pRecvTable;
	ClientClass		*m_pNext;
	int				m_ClassID;
};

struct Ray_t
{
	VectorAligned  m_Start;	// starting point, centered within the extents
	VectorAligned  m_Delta;	// direction + length of the ray
	VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
#ifdef HACK_CSGO
	const matrix3x4_t*	m_pTransform;
#endif
	bool	m_IsRay;	// are the extents zero?
	bool	m_IsSwept;	// is delta != 0?

	void Init( CVector3 & start, CVector3 & end )
	{
		VectorAligned vStart( start );
		VectorAligned vEnd( end );

		m_Delta = vEnd - vStart;

		m_IsSwept = m_Delta.Length() != 0;

		m_Extents[0] = 0;
		m_Extents[1] = 0;
		m_Extents[2] = 0;
		m_Extents[3] = 0;

		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset[0] = 0;
		m_StartOffset[1] = 0;
		m_StartOffset[2] = 0;
		m_StartOffset[3] = 0;

		m_Start = start;
	}

private:
};

struct cplane_t
{
	float normal[3];
	float dist;
	BYTE type;
	BYTE signBits;
	BYTE pad[2];
};

struct csurface_t
{
	const char*		name; 
	short			surfaceProps;
	unsigned short	flags;
};

struct trace_t
{
	CVector3	start;
	CVector3	endpos;
	cplane_t	plane;
	float		fraction;
	int			contents;
	WORD		dispFlags;
	bool		allSolid;
	bool		startSolid;
	float		fractionLeftSolid;
	csurface_t	surface;
	int			hitgroup;
	short		physicsBone;
#ifdef HACK_CSGO
	WORD		m_nWorldSurfaceIndex;
#endif
	CBaseEntity* m_pEnt;
	int			hitbox;
};

class CBaseHandle
{
public:
	int GetEntryIndex()
	{
		return m_Index & ENT_ENTRY_MASK;
	}

protected:

	unsigned long	m_Index;
};

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle( const CBaseHandle &handle ) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class ITraceFilter
{
public:
	virtual bool		ShouldHitEntity( IHandleEntity *pEntity, int contentsMask ) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t( 
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23 )
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	float *operator[]( int i )				{ return m_flMatVal[i]; }
	const float *operator[]( int i ) const	{ return m_flMatVal[i]; }
	float *Base()							{ return &m_flMatVal[0][0]; }
	const float *Base() const				{ return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

class VMatrix
{
public:
	// array access
	inline float* operator[](int i)
	{ 
		return m[i]; 
	}

	inline const float* operator[](int i) const
	{ 
		return m[i]; 
	}

	// Get a pointer to m[0][0]
	inline float *Base()
	{
		return &m[0][0];
	}

	inline const float *Base() const
	{
		return &m[0][0];
	}

public:
	// The matrix.
	float		m[4][4];
};

#endif