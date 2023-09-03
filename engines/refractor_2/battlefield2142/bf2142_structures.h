#ifndef bf2142_structures_h
#define bf2142_structures_h

CPlayerManager*			Player_manager;
CViewMatrix*			viewmatrix;
CInfo*					info;
CPlayerInfo*			playerinfo;
CViewDistance_holder*	viewdistance_holder;
CPlayer*				local_player;


class CPlayerManager
{
public:
	virtual void AddRef();
	virtual int GetRef();
	virtual void Release();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual CPlayer* GetPlayerByIndex(int Index);
	virtual void Function8();
	virtual void Function9();
	virtual void Function10();
	virtual CPlayer* GetLocalPlayer();

	char unknown0[16];
	__int32 TotalPlayers; //0014
	char unknown1[84];
	CPlayer* LocalPlayer; //006C
};

class CPlayer
{
public:
	virtual void Function0();
	char unknown0[84];
	__int32 Kit; //0058
	char unknown1[8];
	char Name[16]; //0064
	char unknown2[12];
	CInfo* Info; //0080
	CViewMatrix* ViewMatrix; //0084
	char unknown3[5];
	BYTE CommanderMenu; //008D
	char unknown4[38];
	__int32 Spawned; //00B4
	__int32 player_code; //00B8
	char unknown5[4];
	float FOV; //00C0
	float Zoom; //00C4
	float Zoomback; //00C8
	char unknown6[9];
	BYTE Alive; //00D5
	char unknown7[2];
	__int32 Team; //00D8
	char unknown8[4];
	BYTE ManDown; //00E0
	char unknown9[15];
	__int32 Ping; //00F0
	char unknown10[20];
	__int32 SquadNumber; //0108
	BYTE Commander; //010C
	BYTE SquadLeader; //010D
	char unknown11[142];
	BYTE Shooting; //019C
	char unknown12[2];
	BYTE Sprinting; //019F
	BYTE Sprinting_cur; //01A0
	char unknown13[115];
	__int32 NumberOfReloads; //0214
	char unknown14[56];
	__int32 TimesPressedShootButton; //0250
	__int32 Ammo; //0254
	char unknown15[8];
	__int32 WeaponFireMode; //0260
	char unknown16[8];
	BYTE CanReload; //026C
	char unknown17[3];
	float WeaponHeatPrimary; //0270
};

class CViewMatrix
{
public:
	char unknown0[200];
	float Matrix[16]; //00C8
};

class CPlayerInfo
{
public:
	virtual void Function0();
	char unknown0[32];
	CVehicle* Vehicle; //0024
	char unknown1[32];
	CHealth* HealthInfo; //0048
	char unknown2[84];
	float BodyMatrix[16]; //00A0
	char unknown10[312];
	BYTE SelectedWeapon; //0218
	char unknown11[67];
	float BodyYaw; //025C
	float RightSpeed; //0260
	float ForwardSpeed; //0264
	__int32 Posture; //0268
	__int32 Posture_cur; //026C
	float HeadPitch; //0270
	float HeadElevationChange; //0274
	float HeadYaw; //0278
	float HeadYawChange; //027C
	char unknown12[4];
	__int32 PostureKey; //0284
};

#endif//bf142 classes
