#ifndef aimbot_helper_h
#define aimbot_helper_h

#include "../default_includes.h"
#include "../bf2/bf2.h"


#include <D3dx9math.h>
#pragma comment (lib, "D3dx9.lib")



#define GET_XAXIS_FROMMATRIX( out_vector, pMatrix) { D3DXVec3Normalize( out_vector, &D3DXVECTOR3((pMatrix)->_11,(pMatrix)->_12,(pMatrix)->_13)); }
#define GET_YAXIS_FROMMATRIX( out_vector, pMatrix) { D3DXVec3Normalize( out_vector, &D3DXVECTOR3((pMatrix)->_21,(pMatrix)->_22,(pMatrix)->_23)); }
#define GET_ZAXIS_FROMMATRIX( out_vector, pMatrix) { D3DXVec3Normalize( out_vector, &D3DXVECTOR3(((pMatrix))->_31,(pMatrix)->_32,(pMatrix)->_33)); }
#define GET_ORIGIN_FROMMATRIX( out_vector, pMatrix){ (out_vector)->x = (pMatrix)->_41; (out_vector)->y = (pMatrix)->_42; (out_vector)->z = (pMatrix)->_43; }



void SoldierAngleToPix( signed long* out_x, signed long* out_y, float angle_x, float angle_y );
void VehicleAngleToPix( signed long* out_x, signed long* out_y, float angle_x, float angle_y );



class LocalPlayerInformation
{
	CPlayer* player;

	float lag;

	CControlMap* control;

	CObject*  object;
	CObject_PlayerControlObject* vehicle;
	CObject_Soldier* soldier;
	CObject_Weapon* weapon;

	CTemplate_GenericProjectile* ammo_template;

	BOOL	weapon_high_drop;
	BOOL	weapon_explosive;
	int		weapon_target_system;

	float bullet_gravity;

	D3DXMATRIX* aim_matrix;

	D3DXVECTOR3 aim_pos;
	D3DXVECTOR3 aim_dir;
	D3DXVECTOR3 aim_up;
	D3DXVECTOR3 aim_right;

	D3DXVECTOR3* dislocation;

	float* vehicle_templ_accel_x;
	float* vehicle_templ_accel_y;
	float* vehicle_templ_mspeed_x;
	float* vehicle_templ_mspeed_y;

	//float* vehicle_obj_rot_x;
	//float* vehicle_obj_rot_y;


public:

	LocalPlayerInformation(): player(0), control(0), object(0), vehicle(0), soldier(0), weapon(0),
								lag(0.0f), ammo_template(0), weapon_high_drop(0), bullet_gravity(0.0f),
								weapon_explosive(0), weapon_target_system(0), aim_matrix(0),
								aim_pos(0,0,0), aim_dir(0,0,0), aim_up(0,0,0), aim_right(0,0,0), dislocation(0),
								vehicle_templ_accel_x(0), vehicle_templ_accel_y(0), vehicle_templ_mspeed_x(0),
								vehicle_templ_mspeed_y(0) //, vehicle_obj_rot_x(0), vehicle_obj_rot_y(0)
								{ }


	// this should be called every game loop to update the player's state. returns false if
	// no good for the aimbot's help
	BOOL Refresh(void);

	float		 GetLag(void)		 { return lag; }
	CControlMap* GetControlMap(void) { return control; }

	CObject*						 GetObject(void)		{ return object; }
	CObject_PlayerControlObject* GetVehicle (void)		{ return vehicle; }
	CObject_Soldier*				 GetSoldier (void)		{ return soldier; }
	CObject_Weapon*					 GetWeapon (void)		{ return weapon; }
	CTemplate_GenericProjectile*	 GetAmmoTemplate (void) { return ammo_template; }

	float	GetBulletGravity(void) { return bullet_gravity; }

	float GetWeaponDeviation(void)
	{ if(weapon && weapon->weapon_deviation) return weapon->weapon_deviation->total_deviation; else return 0.0f; }

	BOOL	WeaponIsExplosive (void)	 { return weapon_explosive; }
	BOOL	WeaponHasHighDrop (void)	 { return weapon_high_drop; }
	int		WeaponTargetSystem(void)	 { return weapon_target_system; }
	BOOL	WeaponCanBeatVehicle(CObject_PlayerControlObject* vehicle);


	D3DXMATRIX*  GetAimMatrix(void) { assert(aim_matrix); return aim_matrix; }
	D3DXVECTOR3* GetAimPos	(void)	{ assert(aim_pos); return &aim_pos; }
	D3DXVECTOR3* GetAimDir (void)	{ assert(aim_dir); return &aim_dir; }
	D3DXVECTOR3* GetAimUp (void)	{ assert(aim_up); return &aim_up; }
	D3DXVECTOR3* GetAimRight (void)	{ assert(aim_right); return &aim_right; }

	D3DXVECTOR3* GetMovementSpeed (void) { assert(dislocation); return dislocation; }

	// warning: values in degrees, and valid only when on a vehicle
	//float GetORotSpeedX(void) { if(vehicle_obj_rot_x) return *vehicle_obj_rot_x; else return 0.0f; }
	//float GetORotSpeedY(void) { if(vehicle_obj_rot_y) return *vehicle_obj_rot_y; else return 0.0f; }
	float GetTRotMSpeedX(void)
		{ if(vehicle_templ_mspeed_x) return *vehicle_templ_mspeed_x; else return 0.0f; }
	float GetTRotMSpeedY(void)
		{ if(vehicle_templ_mspeed_y) return *vehicle_templ_mspeed_y; else return 0.0f; }
	float GetTRotMAccelX(void) 
		{ if(vehicle_templ_accel_x)  return *vehicle_templ_accel_x; else return 0.0f; }
	float GetTRotMAccelY(void)
		{ if(vehicle_templ_accel_y)  return *vehicle_templ_accel_y; else return 0.0f; }

};

extern LocalPlayerInformation local_info;



//////////////////////////////////////////////////////////////////////////
class TargetInformation
{
	CPlayer* player;

	CObject*  object;
	CObject_PlayerControlObject* vehicle;
	CObject_Soldier* soldier;


	D3DXVECTOR3 base_aim;

	D3DXVECTOR3 head_pos;
	D3DXVECTOR3 stomach_pos;

	float		bullet_drop;
	float		bullet_delay;

	D3DXVECTOR3 direction_to_target;
	float		distance_to_target;


	BOOL good;

	BOOL got_head;
	BOOL got_stomach;
	BOOL got_dir_dist;
	BOOL got_bullet;

	BOOL GetBonePos(int bone, D3DXVECTOR3* output);
	inline BOOL GetHeadPos(D3DXVECTOR3* output);
	inline BOOL GetStomachPos(D3DXVECTOR3* output);
	inline BOOL GetFootPos(D3DXVECTOR3* output);

	BOOL CalculateBullet(D3DXVECTOR3* aim);

public:
	TargetInformation(): player(0), object(0), vehicle(0), soldier(0), 
		good(FALSE), got_head(FALSE), got_stomach(FALSE), got_dir_dist(FALSE),
		got_bullet(FALSE) { }

	// this sets a new player as target (AimbotCheat::Refresh() iterates through
	// all players calling this for each). returns true if is a good target
	BOOL NewTarget(CPlayer* new_target);

	CPlayer* GetPlayer(void) { assert(player); return player; }

	BOOL GetBulletInfo(float* drop, float* delay);

	void GetPosition(D3DXVECTOR3* output)		{ assert(player && good); *output = base_aim; }

	// used to determine if the player is close enough to aim
	BOOL IsGoodDistance(void);
	BOOL IsGoodHeadDistance(void);

	// these two are just to determine a rough estimate, for accurate aiming
	// GetAimVector() should be used
	BOOL GetDirectionToTarget(D3DXVECTOR3* output);
	float GetDistanceToTarget(void);

	// unlike GetDirectionToTarget(), this routine retrieves the exact location to aim at, with
	// bullet drop and delay compensation.
	BOOL GetAimVector(D3DXVECTOR3* output);
};






#endif //aimbot_helper_h