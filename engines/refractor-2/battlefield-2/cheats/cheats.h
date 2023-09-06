#ifndef cheats_h
#define cheats_h

#include "cheat_base.h"
#include "../bf2/bf2.h"

#include "cheats_detours.h"


// all the cheats are actually defined as global
// objects in cheats.cpp
/*class WeaponsUnlockCheat;
extern WeaponsUnlockCheat	weaponsunlock;
class MinimapCheat;
extern MinimapCheat			minimap;
class EnemytagsCheat;
extern EnemytagsCheat		enemytags;
class ViewDistCheat;
extern ViewDistCheat		viewdist;
class TagsDistCheat;
extern TagsDistCheat		tagsdist;
class ForceCommanderCheat;
extern ForceCommanderCheat	forcecommander;
class TeamSwitchCheat;
extern TeamSwitchCheat		teamswitch;
class NoPostProductionCheat;
extern NoPostProductionCheat nopostprod;

class AllowFreeCamCheat;
extern AllowFreeCamCheat	freecam;

class NoSkyCheat;
extern NoSkyCheat			nosky;*/
class AimbotCheat;
extern AimbotCheat			aimbot;

class AntiPBSs;
extern AntiPBSs screenshot;

/*class UnlockConsoleCheat;
extern UnlockConsoleCheat	unlock_console;
class ZoomCheat;
extern ZoomCheat			zoom;


// d3d cheats, most part of them are really implemented in d3d/d3d9dev.cpp
class ChamsCheat;
extern ChamsCheat			chams;
class NoFogCheat;
extern NoFogCheat			nofog;
*/


//////////////////////////////////////////////////////////////////////////
/*class MinimapCheat: public CodeCheat
{
public:
	MinimapCheat();
};


//////////////////////////////////////////////////////////////////////////
class EnemytagsCheat: public CodeCheat
{
public:
	EnemytagsCheat();
};

class NoSkyCheat: public Cheat
{
protected:
	char original_sky;
	char original_flare;
	BOOL got_original;

	virtual BOOL Apply(BOOL apply);

public:
	NoSkyCheat();
};

//////////////////////////////////////////////////////////////////////////
class WeaponsUnlockCheat: public CodeCheat
{
public:
	WeaponsUnlockCheat();
};


//////////////////////////////////////////////////////////////////////////
class ViewDistCheat: public Cheat
{
private:
	float original_viewdist;

protected:
	virtual BOOL Apply( BOOL enable );

public:
	float patched_viewdist;
	ViewDistCheat();

};

class TagsDistCheat: public Cheat
{
private:
	float original_tagsdist;

protected:
	virtual BOOL Apply( BOOL enable );

public:
	float patched_tagsdist;
	TagsDistCheat();

};


//////////////////////////////////////////////////////////////////////////
class ForceCommanderCheat: public Cheat
{
private:
	BYTE original_commander;

	CodePatch unlimited_supplies;
	CodePatch unlimited_artillery;

#ifdef UNLIMITEDCARS_OFF
	CodePatch unlimited_cars;
#endif

protected:
	virtual BOOL Apply(BOOL enable);

public:
	ForceCommanderCheat();

	virtual void Refresh(void);
};


//////////////////////////////////////////////////////////////////////////
class TeamSwitchCheat: public Cheat
{
private:
	int original_team;

protected:
	virtual BOOL Apply(BOOL apply);

public:
	TeamSwitchCheat();

};

class NoPostProductionCheat: public Cheat
{
private:
	BYTE original_post;

	vector<CodePatch> patches;

protected:
	virtual BOOL Apply(BOOL apply);

public:
	NoPostProductionCheat();
};


//////////////////////////////////////////////////////////////////////////
class ChamsCheat: public Cheat
{
protected:
	virtual BOOL Apply(BOOL apply) {return TRUE;}
public:
	ChamsCheat();
};

class NoFogCheat: public Cheat
{
private:
	float	fog_start;
	float	fog_end;

protected:
	virtual BOOL Apply(BOOL apply);
public:
	NoFogCheat();
	virtual void Refresh(void);
};
*/


//////////////////////////////////////////////////////////////////////////
class AimbotCheat: public Cheat
{
protected:
	int disable_key;

	virtual BOOL Apply(BOOL apply);

public:

	AimbotCheat();
	virtual void Refresh(void);

};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct IDirect3DSurface9;

class AntiPBSs: public Cheat
{
	IDirect3DSurface9*		good_frame;
	CodePatch				tags_nodelay;

	// number of frames before caching a new screen
	int		cicles_good;

	BOOL	buffer_valid;
	int		cicles_active;

	virtual BOOL Apply(BOOL apply) {return TRUE;}

public:
	AntiPBSs();
	~AntiPBSs();

	virtual BOOL Configure(void);

	virtual BOOL Enable(BOOL enable);
	virtual void Refresh(void);

	BOOL GetBuffer(void);
	BOOL WriteBufferToScreen(void);

	BOOL WaitingToFetchScreen(void) { return !buffer_valid; }

};

/*//////////////////////////////////////////////////////////////////////////
class UnlockConsoleCheat: public CodeCheat
{
public:
	UnlockConsoleCheat();
};


//////////////////////////////////////////////////////////////////////////
class ZoomCheat: public Cheat
{
protected:
	virtual BOOL Apply(BOOL apply);

	CodePatch zoom_unlock;

	float original_fov;
	float forced_zoom;

public:
	float multiplier;

	ZoomCheat();

	virtual void Refresh(void);
};
*/
#endif //cheats_h