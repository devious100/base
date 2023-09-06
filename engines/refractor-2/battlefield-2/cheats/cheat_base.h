#ifndef cheat_base_h
#define cheat_base_h

#include "../default_includes.h"




// the basic cheats types, every new cheat should be derived from Cheat,
// or from CodeCheat if it only do changes to bf2's code (like nops).
class Cheat;
class CodeCheat;

// a cheat may contain one of this for each modification to bf2's code
class CodePatch;

// a class used to hold information of the cheat's parameters in the config file
class ConfigParam;



// a list with all the created cheats (this allow easy iteration through them)
extern vector<Cheat*> cheat_list;


// if true, CheatRefresh() will call GetAsyncKeyState(virtual_key) for each cheat
extern BOOL hotkeys_enabled;


//////////////////////////////////////////////////////////////////////////
class Cheat
{
protected:
	// the cheats can be 'enabled' by the user, but not 'allowed' because of
	// pb checks.
	BOOL enabled;
	BOOL allowed;

	// Apply() MUST be implemented in every derived class, and is called
	// when the cheat should really make any change to the game. it may
	// also implement a new Refresh() that will be called at every new
	// game loop.
	virtual BOOL Apply(BOOL apply) = 0;

public:
	// a keyboard keycode used to enable/disable the cheat, 0 if no key
	int virtual_key;
	
	// indicates if the key was pressed in the last check (so we won't toggle 
	// the cheat until the user releases it.)
	BOOL key_pressed;

	// a name for the cheat (used by Config). its better to keep it encrypted, see
	// misc/encrypted_strings.h
	const char* encrypted_name;

	// the parameters to be used in the config file. the cheat's constructor must push/pop
	// the desired parameters here. a default set is defined in Cheat::Cheat() with a bool
	// to enable/disable and a virtual key.
	vector<ConfigParam> config_parameters;

	// called by Configure (in config_file.cpp) after setting the cheat's parameters, may be
	// used by derived classes to do some initialization.
	virtual BOOL Configure(void) {return TRUE;}


	Cheat();

	virtual BOOL IsEnabled(void)	{ return enabled; }
	virtual BOOL IsAllowed(void)	{ return allowed; }
	virtual BOOL IsApplied(void)	{ return enabled; }

	// used when the user want to enable or disable the cheat
	virtual BOOL Enable( BOOL enable );
	virtual BOOL Toggle(BOOL enable)  { return Enable(enable);}
	
	// used to disallow when punkbuster is doing its checks
	virtual BOOL Allow( BOOL allow );

	// called once every game loop, by default it does nothing. note also that this will be called
	// even if the cheat is not applied, so the new implementation should check this before making
	// any changes (I decided for this behavior so the anti-pb-screenshots could be implemented as
	// one normal cheat, since it will have to disable all cheats before grabing a new frame.)
	virtual void Refresh(void) { };
};




// used as parameter to for_each algorithm
inline void CheatEnable(Cheat* cheat)	{ cheat->Enable(TRUE); }
inline void CheatDisable(Cheat* cheat)	{ cheat->Enable(FALSE); }
inline void CheatAllow(Cheat* cheat)	{ cheat->Allow(TRUE); }
inline void CheatDisallow(Cheat* cheat)	{ cheat->Allow(FALSE); }

void CheatRefresh (Cheat* cheat);



//////////////////////////////////////////////////////////////////////////
// implements cheats that do code changes. the classes derived from this
// should on their constructors insert the changes to the code in the
// patches member, and don't need to derive a Apply() member.
class CodeCheat: public Cheat
{
protected:
	vector<CodePatch> patches;

	virtual BOOL Apply(BOOL apply);

};


//////////////////////////////////////////////////////////////////////////
// Any cheat may have some of these. before use, location, offset (from the
// module base indicated by 'location') and patched_code should be initialized
class CodePatch
{
private:

	vector<char> original_code;
	BOOL		 applied;

public:

	enum { linear, on_bf2, on_renddx9 } location;
	unsigned int offset;

	vector<char> patched_code;


	CodePatch(): applied(FALSE), offset(0), location(linear) {}
	BOOL Apply(BOOL apply);

	BOOL IsApplied(void) {return applied;}

};


//////////////////////////////////////////////////////////////////////////
// used by Configure() to read parameters from the config file.
// see config_file.cpp
class ConfigParam
{
public:
	enum { param_bool, param_int, param_float, param_vkey } type;
	void* address;
};

#endif //cheat_base_h