#ifndef encrypted_strings_h
#define encrypted_strings_h

#include "../default_includes.h"

#include <string>
using namespace std;


//the following strings are in strings.cxr

// global objects
extern const char* estr_class_manager;
extern const char* estr_physics_system;
extern const char* estr_inputdevice_manager;
extern const char* estr_controlmap_manager;
extern const char* estr_object_manager;
extern const char* estr_template_manager;
extern const char* estr_player_manager;
extern const char* estr_hud_manager;
extern const char* estr_renderer;
extern const char* estr_postproduction;
extern const char* estr_nametag_manager;


// cheats names
extern const char* estr_minimap;
extern const char* estr_enemy_tags;
extern const char* estr_weapons_unlock;
extern const char* estr_view_dist;
extern const char* estr_tags_dist;
extern const char* estr_force_commander;
extern const char* estr_team_switch;
extern const char* estr_no_postproduction;;

extern const char* estr_chams;
extern const char* estr_nofog;
extern const char* estr_nosky;
extern const char* estr_aimbot;
extern const char* estr_screenshot;


extern const char* estr_zoom;
extern const char* estr_nostatic;
extern const char* estr_console_unlock;
extern const char* estr_norecoil;
extern const char* estr_nodeviation;



#include "cxr_inc.h"


//////////////////////////////////////////////////////////////////////////
class EncryptedString
{
	BOOL valid;
	string str;

public:


	EncryptedString(unsigned int buffer_size = 256) { valid = 0; str.reserve(buffer_size); }

	~EncryptedString() { ClearString(); }

	BOOL IsBufferValid(void) { return valid; }

	BOOL	Decrypt(const char* encrypted)	{ ClearString(); return valid = __CXRDecrypt(&str, encrypted); }
	void	ClearString(void)				{ fill(str.begin(), str.end(), 0); str.clear(); valid = FALSE; }
	string* GetString(void)					{ return &str; }
};


#endif //encrypted_strings_h
