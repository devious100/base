#include "../ogoid++.h"
#include "../bf2/bf2.h"

#include "cheats.h"

#include "../misc/detour_wrapper.h"
#include "../misc/encrypted_strings.h"


/*WeaponsUnlockCheat	weaponsunlock;
MinimapCheat		minimap;
EnemytagsCheat		enemytags;
ViewDistCheat		viewdist;
TagsDistCheat		tagsdist;
ForceCommanderCheat	forcecommander;
ChamsCheat			chams;
NoFogCheat			nofog;
NoSkyCheat			nosky;
TeamSwitchCheat		teamswitch;
NoPostProductionCheat nopostprod;

ZoomCheat			zoom;
UnlockConsoleCheat	unlock_console;*/
AimbotCheat			aimbot;
AntiPBSs			screenshot;



/*//////////////////////////////////////////////////////////////////////////
MinimapCheat::MinimapCheat( )
{
	CodePatch nop2;
	CodePatch nop6;

	nop2.patched_code.resize(2, '\x90' );
	nop6.patched_code.resize(6, '\x90' );

	nop2.location = CodePatch::on_bf2;
	nop6.location = CodePatch::on_bf2;

	nop2.offset = MINIMAP1_OFF;
	patches.push_back(nop2);

	nop2.offset = MINIMAP2_OFF;
	patches.push_back(nop2);

	nop6.offset = MINIMAP3_OFF;
	patches.push_back(nop6);

	nop2.offset = MINIMAP4_OFF;
	patches.push_back(nop2);

	nop2.offset = MINIMAP5_OFF;
	patches.push_back(nop2);

	nop2.offset = MINIMAP6_OFF;
	patches.push_back(nop2);

	nop2.offset = MINIMAP7_OFF;
	patches.push_back(nop2);

	encrypted_name = estr_minimap;

	virtual_key = VK_NUMPAD7;
	Enable(TRUE);
}


//////////////////////////////////////////////////////////////////////////
EnemytagsCheat::EnemytagsCheat( void )
{
	CodePatch nop2;
	CodePatch nop6;
	CodePatch nop_jmp;

	nop2.patched_code.resize(2, '\x90' );
	nop6.patched_code.resize(6, '\x90' );

	nop_jmp.patched_code.push_back('\x90');
	nop_jmp.patched_code.push_back('\xE9');


	nop_jmp.location = CodePatch::on_renddx9;
	nop2.location = CodePatch::on_renddx9;
	nop6.location = CodePatch::on_renddx9;


	nop_jmp.offset = ENEMYTAGS1_OFF;
	patches.push_back(nop_jmp);

	nop2.offset = KITICONS1_OFF;
	patches.push_back(nop2);
	nop2.offset = KITICONS2_OFF;
	patches.push_back(nop2);
	nop2.offset = KITICONS3_OFF;
	patches.push_back(nop2);

	nop6.offset = HEALTHARMOR1_OFF;
	patches.push_back(nop6);

	nop6.offset = HEALTHARMOR2_OFF;
	patches.push_back(nop6);
	nop6.offset = HEALTHARMOR3_OFF;
	patches.push_back(nop6);

	nop2.offset = ENEMYMINES_OFF;
	patches.push_back(nop2);

	nop2.offset = DIST2PLAYER_OFF;
	patches.push_back(nop2);

	encrypted_name = estr_enemy_tags;

	virtual_key = VK_NUMPAD8;
	Enable(TRUE);

}


//////////////////////////////////////////////////////////////////////////
WeaponsUnlockCheat::WeaponsUnlockCheat( void )
{
	CodePatch nop2;

	nop2.patched_code.resize( 2, '\x90' );
	nop2.location = CodePatch::on_bf2;

	nop2.offset = WEAPONSUNLOCK1_OFF;
	patches.push_back(nop2);
	nop2.offset = WEAPONSUNLOCK2_OFF;
	patches.push_back(nop2);

	encrypted_name = estr_weapons_unlock;

	virtual_key = 0;
	Enable(TRUE);

}




//////////////////////////////////////////////////////////////////////////
UnlockConsoleCheat::UnlockConsoleCheat(void)
{
	CodePatch nop2;
	CodePatch nop6;

	CodePatch jmp36;

	nop2.location = nop6.location = jmp36.location = CodePatch::on_bf2;

	nop2.patched_code.resize(2, '\x90');
	nop6.patched_code.resize(6, '\x90');

	nop2.offset = CONSOLE1_OFF;
	patches.push_back(nop2);
	nop2.offset = CONSOLE2_OFF;
	patches.push_back(nop2);

	jmp36.patched_code.push_back('\xEB');
	jmp36.patched_code.push_back('\x36');
	jmp36.offset = CONSOLE3_OFF;
	patches.push_back(jmp36);

	nop6.offset = CONSOLE4_OFF;
	patches.push_back(nop6);
	nop6.offset = CONSOLE5_OFF;
	patches.push_back(nop6);

	encrypted_name = estr_console_unlock;

	// remove the virtual key parameter (no need to change this
	// while playing)
	config_parameters.pop_back();

	virtual_key = 0;
	Enable(TRUE);
}






//////////////////////////////////////////////////////////////////////////
ViewDistCheat::ViewDistCheat(): original_viewdist(-1.0f), patched_viewdist(2.0f)
{

	encrypted_name = estr_view_dist;

	ConfigParam parameter;

	// let's add another config parameter for the desired view distance
	parameter.type = ConfigParam::param_float;
	parameter.address = &patched_viewdist;
	config_parameters.push_back(parameter);

	virtual_key = VK_NUMPAD9;
	//Enable(TRUE);
}

BOOL ViewDistCheat::Apply( BOOL enable )
{
	if(!renddx9_base) return FALSE;

	if(!renderer) return FALSE;

	if(enable)
	{
		if(original_viewdist < 0.0f)
			original_viewdist = renderer->maxViewDistance;

		renderer->maxViewDistance = patched_viewdist;
	}
	else if(original_viewdist > 0.0f)
			renderer->maxViewDistance = original_viewdist;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
TagsDistCheat::TagsDistCheat(): original_tagsdist(-1.0f), patched_tagsdist(2.0f)
{

	encrypted_name = estr_tags_dist;

	// config parameter for the tags distance
	ConfigParam parameter;
	parameter.type = ConfigParam::param_float;
	parameter.address = &patched_tagsdist;
	config_parameters.push_back(parameter);

	virtual_key = VK_NUMPAD9;
	//Enable(FALSE);
}

BOOL TagsDistCheat::Apply( BOOL enable )
{
	float* tagdist_add = CAST(float*, renddx9_base + RENDDX9_TAGSDIST);

	if(enable)
	{
		if(original_tagsdist < 0.0f)
			original_tagsdist = *tagdist_add;

		*tagdist_add = patched_tagsdist;
	}
	else if(original_tagsdist > 0.0f)
		*tagdist_add = original_tagsdist;

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
ForceCommanderCheat::ForceCommanderCheat(void): original_commander(0)
{
	unlimited_supplies.patched_code.push_back( '\x00' );
	unlimited_supplies.offset = UNLIMITEDSUPPLIES_OFF;
	unlimited_supplies.location = CodePatch::on_bf2;
	
	unlimited_artillery.patched_code.push_back( '\x00' );
	unlimited_artillery.offset = UNLIMITEDARTILLERY_OFF;
	unlimited_artillery.location = CodePatch::on_bf2;

#ifdef UNLIMITEDCARS_OFF
	unlimited_cars.patched_code.push_back('\x00');
	unlimited_cars.offset = UNLIMITEDCARS_OFF;
	unlimited_cars.location = CodePatch::on_bf2;
#endif

	encrypted_name = estr_force_commander;

	// for the force commander just a virtual key (no sense in keeping this enabled
	// from start)
	ConfigParam parameter = config_parameters[1];
	config_parameters.clear();
	config_parameters.push_back(parameter);

	virtual_key = VK_NUMPAD4;
	//Enable(FALSE);
}

BOOL ForceCommanderCheat::Apply(BOOL enable)
{
	if(local_player)
	{
		unlimited_supplies.Apply(enable);
		unlimited_artillery.Apply(enable);

#ifdef UNLIMITEDCARS_OFF
		unlimited_cars.Apply(enable);
#endif

		if(enable)
		{
			original_commander = local_player->commander;
			local_player->commander = 1;
		}
		else local_player->commander = original_commander;
	}

	return TRUE;
}

void ForceCommanderCheat::Refresh(void)
{
	if(IsApplied() && local_player)
	{
		if(!hud_manager) return;

		local_player->commander = 1;
		hud_manager->assets->ArtilleryLoading = hud_manager->assets->SuppliesLoading = 0;

#ifdef UNLIMITEDCARS_OFF
		hud_manager->CarsHealth = 1.0f;
		hud_manager->assets->CarsLoadTime = 0.0f;
#endif
	}
}


//////////////////////////////////////////////////////////////////////////
TeamSwitchCheat::TeamSwitchCheat(): original_team(0)
{

	encrypted_name = estr_team_switch;

	ConfigParam parameter = config_parameters[1];
	config_parameters.clear();
	config_parameters.push_back(parameter);

	virtual_key = VK_NUMPAD1;
	//Enable(FALSE);
}

BOOL TeamSwitchCheat::Apply(BOOL apply)
{
	if(!local_player) return FALSE;

	if(apply)
	{
		original_team = local_player->player_team;

		if(original_team == 1)
			local_player->player_team = 2;
		else
			local_player->player_team = 1;
	}
	else if(original_team)
	{
		local_player->player_team = original_team;
		original_team = 0;
	}

	return TRUE;

}


NoPostProductionCheat::NoPostProductionCheat()
{
	encrypted_name = estr_no_postproduction;

	CodePatch patch;


#ifdef SHELLSHOCK_OFF
	patch.patched_code.clear();
	patch.location = CodePatch::on_renddx9;
	patch.offset = SHELLSHOCK_OFF;
	patch.patched_code.push_back(0xEB);
	patches.push_back(patch);
#endif


#ifdef FLASHBANG_OFF
	patch.location = CodePatch::on_bf2;
	patch.offset = FLASHBANG_OFF;
	patch.patched_code.push_back(0x00);
	patches.push_back(patch);
#endif

#ifdef TEARGAS_OFF
	patch.patched_code.clear();
	patch.location = CodePatch::on_renddx9;
	patch.offset = TEARGAS_OFF;
	patch.patched_code.push_back(0x00);
	patches.push_back(patch);
#endif

#ifdef NIGHTVISION_OFF
	patch.patched_code.clear();
	patch.location = CodePatch::on_renddx9;
	patch.offset = NIGHTVISION_OFF;
	patch.patched_code.push_back(0xEB);
	patches.push_back(patch);
#endif



	virtual_key = 0;
	Enable(TRUE);
}

BOOL NoPostProductionCheat::Apply(BOOL apply)
{
	if(!renderer) return FALSE;

	for(vector<CodePatch>::iterator iter = patches.begin(); iter != patches.end(); iter++)
		iter->Apply(apply);

	if(apply)
	{
		original_post = renderer->drawPostProduction;
		renderer->drawPostProduction = FALSE;
	}
	else renderer->drawPostProduction = original_post;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
ChamsCheat::ChamsCheat()
{

	encrypted_name = estr_chams;


	virtual_key = VK_NUMPAD5;
	Enable(TRUE);
}

NoFogCheat::NoFogCheat()
{

	encrypted_name = estr_nofog;

	virtual_key = VK_NUMPAD2;
	Enable(TRUE);
}

BOOL NoFogCheat::Apply(BOOL apply)
{
	if(!postproduction)
		return FALSE;

	if(apply)
	{
		fog_start = postproduction->fogStart;
		fog_end = postproduction->fogEnd;
	}
	else
	{
		postproduction->fogStart = fog_start;
		postproduction->fogEnd = fog_end;
	}

	return TRUE;
}

void NoFogCheat::Refresh(void)
{
	if(!postproduction)
		return;

	if(IsApplied())
	{
		postproduction->fogStart = 2000.0f;
		postproduction->fogEnd = 3000.0f;
	}
}

NoSkyCheat::NoSkyCheat(): got_original(FALSE)
{

	encrypted_name = estr_nosky;

	virtual_key = VK_NUMPAD2;
	Enable(TRUE);
};

BOOL NoSkyCheat::Apply(BOOL apply)
{
	if(!renderer) return FALSE;

	if(!got_original)
	{
		original_sky = renderer->drawSkyDome;
		original_flare = renderer->drawSunFlare;
		got_original = TRUE;
	}

	if(apply)
		renderer->drawSkyDome = renderer->drawSunFlare = 0;
	else
	{
		renderer->drawSkyDome = original_sky;
		renderer->drawSunFlare = original_flare;
	}
	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
ZoomCheat::ZoomCheat(void): forced_zoom(1.1f), multiplier(0.5)
{
 	zoom_unlock.patched_code.push_back('\xDD');
 	zoom_unlock.patched_code.push_back('\xD8');
 	zoom_unlock.patched_code.insert(zoom_unlock.patched_code.end(), 4, '\x90');
 	zoom_unlock.patched_code.push_back('\x5F');
 	zoom_unlock.patched_code.push_back('\xDD');
 	zoom_unlock.patched_code.push_back('\xD8');
 	zoom_unlock.patched_code.insert(zoom_unlock.patched_code.end(), 4, '\x90');
 
 	zoom_unlock.location = CodePatch::on_bf2;
 	zoom_unlock.offset = ZOOMUNLOCK_OFF;
 
	encrypted_name = estr_zoom;

	ConfigParam parameter;
	parameter.type = ConfigParam::param_float;
	parameter.address = &multiplier;
	config_parameters.push_back(parameter);

	virtual_key = VK_NUMPAD3;
	//Enable(FALSE);
}

BOOL ZoomCheat::Apply(BOOL apply)
{
 	zoom_unlock.Apply(apply);
 
 	//if(!input_detour.IsInitialized())
 	//	input_detour.Initialize( CAST(PVOID, bf2_base + BF2_GETINPUTMOVEMENT_FNADD), GetMemberPtr(&CheatsDetours::Mod_GetInputMovement) );
 
 	//input_detour.Attach(apply);
 
 	if(apply)
 	{
 		original_fov = local_player->view_fov;
 		local_player->view_fov = forced_zoom = original_fov * multiplier;
 	}
 	else local_player->view_fov = original_fov;

	return TRUE;
}

void ZoomCheat::Refresh(void)
{
	if(IsApplied())
	{
		local_player->view_fov = forced_zoom;

	}
}*/
