/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

#pragma once

typedef struct
{
	short			iTagNum;
	int				iWhere;
	int				iHeadPriority;
	int				iBodyPriority;
} tag_t;
#define HEAD			0
#define BODY			1
#define HEAD_PRIORITY	2
#define BODY_PRIORITY	3

typedef struct
{
	centity_t *		cent;
	clientInfo_t *	ci;
	float			distance;
	vec3_t			aimpoint;
	vec3_t			aimangles;
	vec3_t			esppoint;
	bool			GotAngles;
	bool			enemy;
	bool			visible;
} BotTarget_t;

typedef struct
{
	int				TotalKills;
	int				Headshots;
	int				Bashes;
	int				Deaths;
	int				Suicides;
} BotStats_t;

class CBot
{
public:
	CBot( int iTagPriority );
	~CBot( );

	playerState_t *	GetClientState		( int clientNum );

	void			DrawTargetInfo		( clientInfo_t * ci );

	void			SetTagPriority		( int priority );

	bool			AddTarget			( centity_t * cent, float prediction, int priority );
	void			ClearTargets		( );
	void			UpdateTargetAngles	( );

	short			GetTagNumber		( const char * tag_name );	
	bool			GetTagOrigin		( centity_t * cent, short tag, vec3_t origin );
	bool			GetTagOrientation	( centity_t * cent, short tag, orientation_t * or );

	bool			Aim					( );
	int				HumanAim			( float, float, float, float, float );

	void			Autobash			( );
	void			Autoshoot			( int weapon, bool toggle );
	void			Autosight			( bool toggle );
	void			DrawESP				( bool bName, bool bDistance );
	void			DrawRadar			( float x, float y, float w, float h, float range, float * BackColour, float * LineColour, float * FriendlyColour, float * EnemyColour );

	//void			SetInvalidPlayer	( int entNum );
	//void			SetValidPlayer		( int entNum );
	//bool			InvalidPlayer		( int entNum );

	BotStats_t		Stats;

protected:
	//BYTE			InvalidPlayers[MAX_CLIENTS];

	static bool		SortPredicateA		( BotTarget_t lhs, BotTarget_t rhs );
	static bool		SortPredicateB		( BotTarget_t lhs, BotTarget_t rhs );
	void			SortTargets			( bool bHuman );

	static bool		TagSortHead			( tag_t lhs, tag_t rhs );
	static bool		TagSortBody			( tag_t lhs, tag_t rhs );

	int				LerpTag				( centity_t * cent, orientation_t * or, short tag, int handle );
	int				GetHandle			( centity_t * cent );

	int									TagPriority;
	short								TagHead;
	std::vector<tag_t>					TagList;
	std::vector<BotTarget_t>			Targets;
};
