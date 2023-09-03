#include "aimbot_helper.h"

#include "../ogoid++.h"

#include <vector>
using namespace std;

#include "../bf2/bf2_general_functions.h"



#define _USE_MATH_DEFINES
#include <math.h>



LocalPlayerInformation local_info;




void SoldierAngleToPix( signed long* out_x, signed long* out_y, float angle_x, float angle_y )
{
	// this is the reversed routine of bf2's soldier class
	CControlMap* cmap = local_info.GetControlMap();
	float m = 1.1f / (local_player->view_fov * 5.0f * cmap->mouse_sensitivity);
	*out_x = (signed long) (angle_x * m / D3DXToRadian(cmap->deg_per_pixel_x));
	*out_y = (signed long) - (angle_y * m / D3DXToRadian(cmap->deg_per_pixel_y));
}




//
float deriv[2] = { 0.0, 0.0 };
float last_angle[2] = { 0.0f, 0.0f };
//double integral[2] = { 0.0, 0.0 };

void VehicleAngleToPix( signed long* out_x, signed long* out_y, float angle_x, float angle_y )
{
	float max_speed, accel;
	CControlMap* cmap = local_info.GetControlMap();

	float disp_x = 0.0f;
	float disp_y = 0.0f;

	angle_x = D3DXToDegree(angle_x);
	angle_y = -D3DXToDegree(angle_y);


	// need to take care of the local angular speed
	//CObject* vehicle = local_info.GetVehicle();
	//assert(vehicle);
	//while(vehicle != vehicle->object_root) vehicle = vehicle->object_root;
	//if(vehicle->object_physics)
	//{
	//	// not correct
	//	angle_x += vehicle->object_physics->rotation.y;
	//	angle_y += vehicle->object_physics->rotation.x;
	//}


	// first the x axis
	// get the template's speed multiplier and acceleration
	max_speed = local_info.GetTRotMSpeedX();
	accel	  =	local_info.GetTRotMAccelX();

	if( max_speed > 0.0f && accel > 0.0f)
	{

		//disp_x = (angle_x - PHY_FREQ*cur_speed/accel);
		//disp_x = (cur_speed - accel * angle_x) / (-2.0f * accel);

		deriv[0] = angle_x - last_angle[0];
		last_angle[0] = angle_x;
		//integral += angle_x;

		// i still couldn't find the formula to get the correct balance between distance and
		// speed, but this is doing reasonably well
		disp_x = 16 * angle_x - 1200* (-deriv[0] / frame_time / accel) * fabsf(deriv[0]) ;
		
		*out_x = disp_x / max_speed / cmap->deg_per_pixel_x / cmap->mouse_sensitivity;

	}
	else *out_x = 0;



	max_speed = local_info.GetTRotMSpeedY();
	accel	  =	local_info.GetTRotMAccelY();

	if( max_speed > 0.0f && accel > 0.0f)
	{
		//run_y = (angle_y - PHY_FREQ*cur_speed/accel );
		//run_y = (cur_speed - accel * angle_y) / (-2.0f * accel * max_speed);

		deriv[1] = angle_y - last_angle[1];
		last_angle[1] = angle_y;
		//integral[1] += angle_y;

		disp_y = 16 * angle_y - 1200* (-deriv[1] / frame_time /accel) * fabsf(deriv[1]) ;

		*out_y = disp_y / max_speed / cmap->deg_per_pixel_y / cmap->mouse_sensitivity;

	}
	else *out_y = 0;
}



//////////////////////////////////////////////////////////////////////////
BOOL LocalPlayerInformation::Refresh(void)
{
	CObject_Weapon* cur_weapon;

	player = local_player;

	if( !player
		|| !player->player_object
		|| !player->player_object->ptr
		|| !player->player_soldier
		|| player->vehicle_seat_num < 0
		|| !player->player_view
		)
		return FALSE;


	lag = (float) player->player_ping / 1000.f;

	// if the player has changed the vehicle since the last
	// Refresh() we have to rebind the mouse sensibility and
	// movement pointers
	if(object != player->player_object->ptr)
	{
		control = controlmap_manager->active_controlmap;
		if(!control) return FALSE;

		CObject_RotationalBundle* rb_mouse_x = 0;
		CObject_RotationalBundle* rb_mouse_y = 0;


		vehicle = 0;
		soldier = 0;

		vehicle_templ_accel_x = vehicle_templ_accel_y
			= vehicle_templ_mspeed_x = vehicle_templ_mspeed_y
			//= vehicle_obj_rot_x = vehicle_obj_rot_y
			= 0;

		object = player->player_object->ptr;
		switch(object->object_template->GetTemplateType())
		{
		case TEMPLATE_SOLDIER_ID:
			soldier = (CObject_Soldier*) object;
			break;

		case TEMPLATE_PARACHUTE_ID:
			vehicle = (CObject_PlayerControlObject*) object;
			assert( vehicle->vehicle_soldier && vehicle->vehicle_soldier->object );

			if(!vehicle->vehicle_soldier || !vehicle->vehicle_soldier->ptr)
				return FALSE;

			soldier = (CObject_Soldier*) vehicle->vehicle_soldier->ptr;
			assert(soldier &&
				soldier->object_template->GetTemplateType() == TEMPLATE_SOLDIER_ID);

			vehicle = 0;
			object = soldier;
			break;


		case TEMPLATE_PLAYERCONTROLOBJECT_ID:
			vehicle = (CObject_PlayerControlObject*) object;
			assert( vehicle->vehicle_soldier && vehicle->vehicle_soldier->object );

			soldier = (CObject_Soldier*) vehicle->vehicle_soldier->ptr;
			assert(soldier &&
				soldier->object_template->GetTemplateType() == TEMPLATE_SOLDIER_ID);


			// only try to aim if the vehicle has rotational controls (the pilot's seat
			// generally doesn't have)
			if(!FindVehicleMouseObjects(vehicle, &rb_mouse_x, &rb_mouse_y))
				return FALSE;

			else
			{
				// bind the x axis
				CTemplate_RotationalBundle* templ_rot = (CTemplate_RotationalBundle*) rb_mouse_x->object_template;
				if(templ_rot)
				{
					if(templ_rot->inputToYaw == PIMouseLookX)
					{
						vehicle_templ_accel_x	= &(templ_rot->max_acceleration.x);
						vehicle_templ_mspeed_x	= &(templ_rot->max_speed.x);
						//vehicle_obj_rot_x		= &(rb_mouse_x->rot_speed.x);
					}
					else if(templ_rot->inputToPitch == PIMouseLookX)
					{
						vehicle_templ_accel_x	= &(templ_rot->max_acceleration.y);
						vehicle_templ_mspeed_x	= &(templ_rot->max_speed.y);
						//vehicle_obj_rot_x		= &(rb_mouse_x->rot_speed.y);
					}
					else if(templ_rot->inputToRoll == PIMouseLookX)
					{
						vehicle_templ_accel_x	= &(templ_rot->max_acceleration.z);
						vehicle_templ_mspeed_x	= &(templ_rot->max_speed.z);
						//vehicle_obj_rot_x		= &(rb_mouse_x->rot_speed.z);
					}
				}

				// now the y
				templ_rot = (CTemplate_RotationalBundle*) rb_mouse_y->object_template;
				if(templ_rot)
				{
					if(templ_rot->inputToYaw == PIMouseLookY)
					{
						vehicle_templ_accel_y	= &(templ_rot->max_acceleration.x);
						vehicle_templ_mspeed_y	= &(templ_rot->max_speed.x);
						//vehicle_obj_rot_y		= &(rb_mouse_y->rot_speed.x);

					}
					else if(templ_rot->inputToPitch == PIMouseLookY)
					{
						vehicle_templ_accel_y	= &(templ_rot->max_acceleration.y);
						vehicle_templ_mspeed_y	= &(templ_rot->max_speed.y);
						//vehicle_obj_rot_y		= &(rb_mouse_y->rot_speed.y);
					}
					else if(templ_rot->inputToRoll == PIMouseLookY)
					{
						vehicle_templ_accel_y	= &(templ_rot->max_acceleration.z);
						vehicle_templ_mspeed_y	= &(templ_rot->max_speed.z);
						//vehicle_obj_rot_y		= &(rb_mouse_y->rot_speed.z);
					}
				}

				break;
			}

		default:
			object = 0;
			soldier = 0;
			weapon = 0;

			return FALSE;
		}


		// now the speed vector
		if(object->object_physics)
			dislocation = &object->object_physics->speed;

	}


	//now check the weapon. on vehicle?
	if(object == vehicle)
	{
		// some vehicles, like the tank's driver, have two weapons, so we'll
		// use the secondary if it is being fired. this is important since in
		// the case of the tank the primary weapon have a low speed and need a
		// big time/drop correction, but the secondary doesn't.
		if(player->secondary_fire_down && vehicle->vehicle_secondary_weapon)
			 cur_weapon = vehicle->vehicle_secondary_weapon;
		
		// else get the primary weapon
		else if(vehicle->vehicle_primary_weapon)
			 cur_weapon = vehicle->vehicle_primary_weapon;
		
		// i assume that if there is no weapon in the vehicle, then the soldier's
		// weapon is activated (but i'm not sure if this always happens.)
		else if(soldier) cur_weapon = soldier->soldier_weapon;

		else cur_weapon = 0;
	}
	else cur_weapon = soldier->soldier_weapon;

	// no need of an aimbot if you have nothing to shoot, right?
	if(!cur_weapon || !cur_weapon->object_template) return FALSE;


	// if changed weapon, we must update it
	if(cur_weapon != weapon)
	{
		weapon = cur_weapon;

		CTemplate_GenericFireArm* weapon_template;
		weapon_template = (CTemplate_GenericFireArm*) weapon->object_template;
		ammo_template = weapon_template->projectile_template;

		if(!ammo_template) return FALSE;

		// if is explosive TargetInformation will always aim on target's foot
		CTComponent_Detonation* det = (CTComponent_Detonation*) ammo_template->GetComponent(COMPONENT_DETONATION_ID);
		if(det && det->explosionMaterial > 0 && det->explosionRadius > 0.0f)
			weapon_explosive = TRUE;
		else weapon_explosive = FALSE;


		// some items, like the car horn and the health/ammo kits, are constructed
		// as weapons, but they have the damage member set to zero.
		if( !weapon_explosive && ammo_template->damage == 0.0f)
			return FALSE;

		// the missiles for instance have what bf calls a 'target system'
		CTComponent_Target* weapon_ts = (CTComponent_Target*) weapon_template->GetComponent(COMPONENT_TARGET_ID);
		if(weapon_ts)
		{
			weapon_target_system = weapon_ts->targetSystem;

			// but we don't support them yet.
			return FALSE;
		}
		else weapon_target_system = TSNone;

		

		// grab the gravity. some weapons, like the tank's cannon, have a low multiplier
		// to have low speed with low drop.
		if(physics_manager) bullet_gravity = physics_manager->gravity * ammo_template->gravity_modifier;
		else bullet_gravity = 0.0f;


		float t = ammo_template->timeToLive_1;
		if(t > 1.0f) t = 1.0f;
		if((weapon_template->fire_velocity / ammo_template->gravity_modifier / t) < 100.0f)
			weapon_high_drop = TRUE;
		else weapon_high_drop = FALSE;

		
	}

	// no target system
	if(weapon_target_system != TSNone)
		return FALSE;


	switch(player->player_view->perspective)
	{
	case 1:												//first person
	case 2:												//nose cam
		aim_matrix = &player->player_view->matrix;
		break;
	default:
		aim_matrix = &weapon->object_matrix;			// actually, z axis of the object matrix
														// doesn't point exactly in the aim dir.
		break;											// i need to take care of this sometime.
	}


	GET_XAXIS_FROMMATRIX(&aim_right, aim_matrix);
	GET_YAXIS_FROMMATRIX(&aim_up,	 aim_matrix);
	GET_ZAXIS_FROMMATRIX(&aim_dir,	 aim_matrix);
	GET_ORIGIN_FROMMATRIX(&aim_pos,  aim_matrix);


	return TRUE;
}


BOOL LocalPlayerInformation::WeaponCanBeatVehicle( CObject_PlayerControlObject* vehicle )
{
	// this function will return true if the vehicle can be taken down within 12 seconds


	if(vehicle->object_root)
		vehicle = (CObject_PlayerControlObject*) vehicle->object_root;

	if(!vehicle->object_armor) return FALSE;

	int rounds = 1;
	float max_damage = ammo_template->damage;

	CTComponent_Detonation* det = (CTComponent_Detonation*) ammo_template->GetComponent(COMPONENT_DETONATION_ID);
	if(det && det->explosionMaterial > 0)
		max_damage = det->explosionDamage;

	CTComponent_Animation* ani =
		(CTComponent_Animation*) weapon->object_template->GetComponent(COMPONENT_ANIMATION_ID);

	// some weapons (mainly the sniper rifles) have it's fire rate attached to the fire animation
	if(ani && ani->useShiftAnimation)
		rounds = 10.0f / ani->shiftDelay;
	else
	{
		// the others to the fire's roundPerMinute
		CTComponent_Fire* fire = (CTComponent_Fire*) weapon->object_template->GetComponent(COMPONENT_FIRE_ID);
		if(fire)
			rounds = fire->roundsPerMinute / 6;
	}
	
	// can't fire more than on the cartridge
	if(player->weapon_ammo > 0
		&& rounds > player->weapon_ammo) rounds = player->weapon_ammo;


	// how much damage can the player do in 10 seconds?
	return vehicle->object_armor->Health < max_damage * rounds;

}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL TargetInformation::NewTarget(CPlayer* new_target)
{ 
	assert(new_target);
	player = new_target;

	assert(player->player_object);
	object = player->player_object->ptr;
	assert(object && object->object_template);


	good = FALSE;
	switch(object->object_template->GetTemplateType())
	{
	case TEMPLATE_SOLDIER_ID:
		soldier = (CObject_Soldier*) object;
		vehicle = 0;
		good = TRUE;
		break;

	case TEMPLATE_LADDERCONTAINER_ID:
	case TEMPLATE_PARACHUTE_ID:
	case TEMPLATE_PLAYERCONTROLOBJECT_ID:
		vehicle = (CObject_PlayerControlObject*) object;

		if(((CTemplate_PlayerControlableObject*) vehicle->object_template)->pco_seat_animation
			&& vehicle->vehicle_soldier
			&& vehicle->vehicle_soldier->ptr)
		{
			soldier = vehicle->vehicle_soldier->ptr;

			assert(soldier->object_template);

			if(soldier->object_template->GetTemplateType() == TEMPLATE_SOLDIER_ID)
			{
				good = TRUE;
				break;
			}
		}
		else
		{
			soldier = 0;

			float max_damage = local_info.GetAmmoTemplate()->damage;

			CTemplate_GenericProjectile* projectile = local_info.GetAmmoTemplate();
			assert(projectile);

			CTComponent_Detonation* det =
				(CTComponent_Detonation*) projectile->GetComponent(COMPONENT_DETONATION_ID);
			if(det && det->explosionMaterial > 0)
				max_damage = det->explosionDamage;


			// only aim if can damage more than a third of the target's armor (no sense in shooting at a tank
			// with a pistol)
			if(vehicle->object_armor &&  max_damage > vehicle->object_armor->Health / 3.0f)
			{
				good = TRUE;
				break;
			}
		}
	}


	if(good)
	{
		got_stomach = FALSE;
		got_head = FALSE;
		got_dir_dist = FALSE;
		got_bullet = FALSE;

		// by default aim at the soldier's stomach or at the object's origin
		if(!soldier || !GetStomachPos(&base_aim))
			GET_ORIGIN_FROMMATRIX(&base_aim, &object->object_matrix);
	}


	return good;
}


//////////////////////////////////////////////////////////////////////////
BOOL TargetInformation::GetBonePos(int bone, D3DXVECTOR3* output)
{
	assert(player && good);

	if(!soldier) return FALSE;

	D3DXMATRIX transformation = soldier->soldier_skeleton3p->matrices[bone];
	
	D3DXMatrixMultiply(&transformation, &transformation, &soldier->object_matrix);

	GET_ORIGIN_FROMMATRIX( output, &transformation );

	return TRUE;
}


BOOL TargetInformation::GetHeadPos(D3DXVECTOR3* output)
{
	assert(player && good);

	if(!soldier) return FALSE;

	if(!got_head && !(got_head = GetBonePos(SKELETON_BONE_HEAD, &head_pos)))
		return FALSE;

	*output = head_pos;

	return TRUE;
}

BOOL TargetInformation::GetStomachPos(D3DXVECTOR3* output)
{
	assert(player && good);

	if(!soldier) return FALSE;

	if(!got_stomach && !(got_stomach = GetBonePos(SKELETON_BONE_STOMACH, &stomach_pos)))
		return FALSE;

	*output = stomach_pos;

	return TRUE;
}

BOOL TargetInformation::GetFootPos(D3DXVECTOR3* output)
{
	assert(player && good);

	if(!soldier) return FALSE;

	*output = base_aim;
	output->y -= 1.0f;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL TargetInformation::CalculateBullet(D3DXVECTOR3* aim_pos)
{
	assert(player && good);
	CTemplate_GenericFireArm* weapon_template =
		(CTemplate_GenericFireArm*) local_info.GetWeapon()->object_template;

	D3DXVECTOR3* local_origin = local_info.GetAimPos();

	float dist_aim = D3DXVec3Length(&(*aim_pos - *local_origin));

	if(local_info.WeaponTargetSystem() == TSNone)
	{
		float dy = aim_pos->y - local_origin->y;
		float dx2 = dist_aim * dist_aim - dy * dy;
		float dx = sqrtf(dx2);

		float g = - local_info.GetBulletGravity();

		float v2 = weapon_template->fire_velocity
				 * weapon_template->fire_velocity;

		float sq = v2 * v2 - g * (g * dx2 + 2.0f * dy * v2);

		if(sq < 0.0f) return FALSE;

		float angle = M_PI_2 - atan( (v2 + sqrtf(sq)) / (g * dx));

		bullet_drop = tanf(angle) * dx;
		bullet_delay = dx / (weapon_template->fire_velocity * cosf(angle));
	}
	else
	{
		bullet_drop = 0.0f;
		bullet_delay = weapon_template->fire_velocity * dist_aim;
	}


	got_bullet = TRUE;

	return TRUE;
}

BOOL TargetInformation::GetBulletInfo(float* drop, float* delay)
{
	if(!got_bullet && !CalculateBullet(&base_aim))
		return FALSE;

	*drop = bullet_drop;
	*delay = bullet_delay;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL TargetInformation::GetDirectionToTarget(D3DXVECTOR3* output)
{
	assert(player && good);


	// we have to force full calculation if using weapons like the m203
	// grenade launcher because of the high projectile drop
	if(local_info.WeaponHasHighDrop())
		return GetAimVector(output);

	if(!got_dir_dist) GetDistanceToTarget();

	*output = direction_to_target;
	return TRUE;
}

float TargetInformation::GetDistanceToTarget(void)
{
	assert(player && good);

	if(got_dir_dist) return distance_to_target;

	D3DXVECTOR3 dir_vector;

	D3DXVec3Subtract(&dir_vector, &base_aim, local_info.GetAimPos());

	distance_to_target = D3DXVec3Length(&dir_vector);
	D3DXVec3Normalize(&direction_to_target, &dir_vector);

	got_dir_dist = TRUE;
	return distance_to_target;
}

BOOL TargetInformation::IsGoodDistance(void)
{

	assert(player && good);

	CObject_Soldier* local_soldier = local_info.GetSoldier();
	CTemplate_GenericFireArm* weapon_template =
		(CTemplate_GenericFireArm*) local_info.GetWeapon()->object_template;

	CTComponent_Fire* fire = (CTComponent_Fire*) weapon_template->GetComponent(COMPONENT_FIRE_ID);

	if(!fire) return FALSE;

	if(!got_dir_dist) GetDistanceToTarget();

	float max_bullet_dist = D3DXVec3Dot(local_info.GetMovementSpeed(), local_info.GetAimDir())
					+ weapon_template->fire_velocity * local_info.GetAmmoTemplate()->timeToLive_1;

	if(distance_to_target > max_bullet_dist )
		return FALSE;


	//if(local_soldier && local_soldier->soldier_weapon == local_info.GetWeapon())
	//{
	//	if(local_player->kit_num == 2 && local_soldier->soldier_selected_weapon == 4)
	//		return distance_to_target < 150.0f;
	//}


	CTComponent_Detonation* det =
		(CTComponent_Detonation*) weapon_template->projectile_template->GetComponent(COMPONENT_DETONATION_ID);

	// if explosive, good if the max deviation is less than a quarter of the explosion radius
	if(det && det->explosionMaterial >= 0 && det->explosionRadius > 0.0f)
	{
		if( distance_to_target * tanf( 0.8f * D3DXToRadian(local_info.GetWeaponDeviation()/1.5f) )
			< det->explosionRadius / 4.0f )
			return TRUE;
		else return FALSE;

	}
	else
	{
		CTComponent_Animation* ani =
			(CTComponent_Animation*) weapon_template->projectile_template->GetComponent(COMPONENT_ANIMATION_ID);
		
		float  rpm = (ani && ani->useShiftAnimation) ? 60.0f / ani->shiftDelay : fire->roundsPerMinute;

		// shotguns and lmgs have big deviation, so i use some multipliers to correct for
		// the extra bullets
		if( distance_to_target / (1.5f *fire->batchSize) / rpm * 800
			* tanf( 0.8f * D3DXToRadian(local_info.GetWeaponDeviation()/1.5f) )
			< 0.80f )
			return TRUE;
		else return FALSE;
	}
}

BOOL TargetInformation::IsGoodHeadDistance(void)
{
	assert(player && good);

	if(!got_dir_dist) GetDistanceToTarget();

	if(local_info.GetObject() == local_info.GetVehicle()) return FALSE;

	// it shouldn't pass 15 cm away
	if( distance_to_target * tanf( D3DXToRadian(local_info.GetWeaponDeviation())) < 0.15f )
		return TRUE;
	else return FALSE;

}


BOOL TargetInformation::GetAimVector(D3DXVECTOR3* output)
{
	if(!player || !good)
		return FALSE;


	// on foot
	if(!vehicle)
	{
		if(local_info.WeaponIsExplosive())
		{
			if(!GetFootPos(output))
				return FALSE;
		}
		else if( (local_info.WeaponTargetSystem() == TSNone) && IsGoodHeadDistance())
		{
			if(!GetHeadPos(output))
				return FALSE;
		}
		else
		{
			if(!GetStomachPos(output))
				return FALSE;
		}
	}
	// on vehicle
	else
	{
		// if the target soldier isn't visible, or the local weapon is explosive, or it can
		// destroy the vehicle, aim at the vehicle's origin
		if( !soldier
			|| local_info.WeaponIsExplosive()
			|| local_info.WeaponCanBeatVehicle(vehicle)
			)
		{
			GET_ORIGIN_FROMMATRIX(output, &vehicle->object_root->object_matrix);
		}

		else
		{
			// aim at the head (the stomach generally has cover)
			if(!GetHeadPos(output))
				return FALSE;
		}
	}

	if(!CalculateBullet(output))
		return FALSE;

	// the bullet drop
	output->y += bullet_drop;

	// and for the delay
	// first find the root's speed
	CObject* t_obj;
	if(vehicle) t_obj = vehicle->object_root;
	else t_obj = soldier;

	if(t_obj->object_physics)
	{
		// correct for target's dislocation
		*output += t_obj->object_physics->speed* (local_info.GetLag() + bullet_delay);
	}

	// and local's
	*output -= *local_info.GetMovementSpeed() * (local_info.GetLag() + bullet_delay);


	D3DXVec3Normalize( output, D3DXVec3Subtract(output, output, local_info.GetAimPos()) );

	return TRUE;
}