
#include "../ogoid++.h"

#include "cheat_base.h"


#include <vector>
using namespace std;


vector<Cheat*> cheat_list;
BOOL hotkeys_enabled;



void CheatRefresh (Cheat* cheat)
{
	/*if( hotkeys_enabled && cheat->virtual_key != 0 )
	{
		if( GetAsyncKeyState(cheat->virtual_key) & ~1 )
		{
			if( cheat->key_pressed == FALSE )
			{
				MessageBeep(-1);
				cheat->Toggle();
				cheat->key_pressed = TRUE;
			}
		}
		else cheat->key_pressed = FALSE;
	}

	cheat->Refresh();*/
}

//////////////////////////////////////////////////////////////////////////
Cheat::Cheat(): key_pressed(FALSE), enabled(FALSE), allowed(FALSE), virtual_key(0)
{
	cheat_list.push_back(this);

	encrypted_name = 0;

	ConfigParam parameter;

	//by default two config parameters
	parameter.type = ConfigParam::param_bool;
	parameter.address = &enabled;
	config_parameters.push_back(parameter);

	parameter.type = ConfigParam::param_vkey;
	parameter.address = &virtual_key;
	config_parameters.push_back(parameter);
}



BOOL Cheat::Enable( BOOL enable )
{
	if(enabled == enable) 
		return TRUE;

	enabled = enable;

	// only make any change if allowed
	//if(allowed)
	//	Apply(enable);

	return TRUE;
}

BOOL Cheat::Allow( BOOL allow )
{
	allowed = TRUE;
	/*if(allowed == allow) return TRUE;

	allowed = allow;

	// only make any change if it is enabled
	if(enabled)
		Apply(allow);*/

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
BOOL CodeCheat::Apply(BOOL apply)
{
	BOOL result = TRUE;
	vector<CodePatch>::iterator vec_iter;

	// just Apply() all the patches
	for(vec_iter = patches.begin();
		vec_iter != patches.end();
		vec_iter++ )
	{
		result &= vec_iter->Apply(apply);
		assert(result);
	}

	return result;
}


//////////////////////////////////////////////////////////////////////////
// here we have to deal with windows memory access protections
BOOL CodePatch::Apply( BOOL enable )
{
	DWORD oldprotect;
	unsigned int i;
	BOOL result;

	char* out;

	if(applied == enable) return TRUE;

	applied = enable;

	assert(offset);
	unsigned int address = offset;

	if(location == on_bf2)
	{
		assert(bf2_base);
		address += bf2_base;
	}
	else if(location == on_renddx9)
	{
		assert(renddx9_base);
		address += renddx9_base;
	}

	result = VirtualProtect( (LPVOID) address,
		patched_code.size(),
		PAGE_READWRITE,
		&oldprotect );
	assert(result);


	if( enable )
	{
		// make sure we have a backup of the original code
		if( original_code.size() == 0)
		{
			original_code.reserve(patched_code.size());
			for(i = 0; i < patched_code.size(); i++)
				original_code.push_back((char) *((char*) address + i));
		}

		// apply the patch
		for(i = 0; i < patched_code.size(); i++)
		{
			out = ((char*) address + i);
			*out = patched_code[i];
		}
	}
	// restore the original
	else
	{
		for(i = 0; i < original_code.size(); i++)
		{
			out = ((char*) address + i);
			*out = original_code[i];
		}
	}


	// set back the original access protection
	VirtualProtect( (LPVOID) address, patched_code.size(),
		oldprotect, &oldprotect );

	return TRUE;
}





