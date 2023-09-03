#include "../ogoid++.h"


#include "cheats.h"

#include "../bf2/bf2.h"
#include "../misc/encrypted_strings.h"

#include "aimbot_helper.h"


#define _USE_MATH_DEFINES
#include <math.h>



//////////////////////////////////////////////////////////////////////////
AimbotCheat::AimbotCheat()
{
	encrypted_name = estr_aimbot;

	virtual_key = VK_NUMPAD0;
	disable_key = VK_SHIFT;

	ConfigParam param;

	param.type = ConfigParam::param_vkey;
	param.address = &disable_key;
	config_parameters.push_back(param);

	Enable(TRUE);
}


BOOL AimbotCheat::Apply(BOOL apply)
{
	if(apply && !player_manager) return FALSE;
	return TRUE;
}


void AimbotCheat::Refresh(void)
{
	if(!IsApplied()
		|| !local_player
		|| hud_manager->mouseActive
		|| !local_info.Refresh()
		)
		return;

	INPUT input;

	list<CPlayer*>::iterator target_iter;

	D3DXVECTOR3 temp_vector;

	CPlayer*	target_player;

	TargetInformation	target;
	D3DXVECTOR3			target_pos;
	D3DXVECTOR3			dir_to_target;
	float				target_importance;
	float				target_z;

	TargetInformation	best_target;
	D3DXVECTOR3			best_dir;
	D3DXVECTOR3			best_projected;
	float				best_importance;
	BOOL				got_target;

	if(GetAsyncKeyState(VK_SHIFT))
	{
		assert(player_manager && bf2_wnd_width && bf2_wnd_height);

		if( !local_player->player_object
			|| !local_player->player_object->ptr
			|| !local_player->player_alive
			||  local_player->player_down
			) return;


		best_importance = 20.0f;
		got_target = FALSE;


		// let's iterate through the bastards
		for( target_iter = player_manager->player_list.begin();
			target_iter != player_manager->player_list.end();
			target_iter++ )
		{
			target_player = *target_iter;

			if(		target_player == local_player
				||	!target_player->player_alive
				||	target_player->player_down
				||	target_player->player_team == local_player->player_team
				|| !target_player->player_object
				|| !target_player->player_object->ptr
				)
				continue;

			if( !target.NewTarget(target_player)
				|| !target.IsGoodDistance()
				|| !target.GetDirectionToTarget(&dir_to_target)
				)
				continue;

			target_z = D3DXVec3Dot( local_info.GetAimDir(), &dir_to_target);


			// the dot product is the cosine of the angle, that means, if the angle between
			// the viewing direction and the direction to the target is 0, target_z will
			// be 1.0.
			if(target_z < cosf(AIMBOT_COVER_ANGLE))
				continue;

			target_importance = 10.0f - target_z;

			// if not sniper the distance is important
			//if( !(local_player->player_object
			//	&& local_player->player_object == local_player->player_soldier
			//	&& local_player->kit_num == 1 && local_info.GetSoldier()->soldier_selected_weapon == 3)
			//	)
			//		target_importance += target.GetDistanceToTarget() / 800.0f;


			if( target_importance < best_importance )
			{
				best_importance = target_importance;
				best_target = target;
				best_dir = dir_to_target;
				got_target = TRUE;
			}
		}

		if(got_target && best_target.GetAimVector(&dir_to_target))
		{
			float angle_x, angle_y;

			angle_x = asin( D3DXVec3Dot(local_info.GetAimRight(), &dir_to_target) );
			angle_y = asin( D3DXVec3Dot(local_info.GetAimUp(), &dir_to_target) );

			if(local_info.GetObject() == local_info.GetSoldier())
				SoldierAngleToPix( &input.mi.dx, &input.mi.dy, angle_x, angle_y );
			else
				VehicleAngleToPix( &input.mi.dx, &input.mi.dy, angle_x, angle_y );


			// limit the aim change
#ifdef AIMBOT_MAX_PIXEL_CHANGE		//defined in configuration_switches_h
			if(input.mi.dx > AIMBOT_MAX_PIXEL_CHANGE)		input.mi.dx =  AIMBOT_MAX_PIXEL_CHANGE;
			else if(input.mi.dx < -AIMBOT_MAX_PIXEL_CHANGE)input.mi.dx = -AIMBOT_MAX_PIXEL_CHANGE;
			if(input.mi.dy > AIMBOT_MAX_PIXEL_CHANGE)		input.mi.dy =  AIMBOT_MAX_PIXEL_CHANGE;
			else if(input.mi.dy < -AIMBOT_MAX_PIXEL_CHANGE)input.mi.dy = -AIMBOT_MAX_PIXEL_CHANGE;
#endif

			input.mi.dx *= 0.5f + 0.5f * cosf(angle_x / AIMBOT_COVER_ANGLE * M_PI * 2);
			input.mi.dy *= 0.5f + 0.5f * cosf(angle_y / AIMBOT_COVER_ANGLE * M_PI * 2);

			input.mi.mouseData = 0;
			input.mi.dwFlags = 0;
			input.mi.time = 0;
			input.mi.dwExtraInfo = GetMessageExtraInfo();
			input.type = INPUT_MOUSE;

			SendInput( 1, &input, sizeof(input) );
		}
	}
}