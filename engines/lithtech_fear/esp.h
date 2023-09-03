#ifndef ESP_H
#define ESP_H

#include "sdk.h"

#define BONES 19

//To be more organized i should have made another class for non esp shit but i got lazy

class Esp
{
public:
	void StartEsp();
	void Pause();
	void CrossHair();
	void AutoKill();
	void PickUps();
	void DrawSkeleton(HOBJECT hObject, D3DCOLOR Color);
	LTVector GetScreenFromWorldPos2(const LTVector& vPos, HOBJECT hCamera, bool& bOnScreen);
	LTVector GetScreenFromWorldPos2(const LTVector& vPos, HOBJECT hCamera);
	void WeaponAccuracy();

private:
	//Id For the player i'm kicking
	uint32 KickID;
};




#endif