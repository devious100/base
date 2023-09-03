/*
#include "../ogoid++.h"

#include "../bf2/bf2.h"
#include "cheats.h"

#include "cheats_detours.h"


Detour input_detour;

typedef float	( CheatsDetours::* GETINPUTMOVEMENT)	(int a, int b);


extern ZoomCheat zoom;

float CheatsDetours::Mod_GetInputMovement(int a, int b)
{
	GETINPUTMOVEMENT RealGetInputMovement = SetMemberPtr<GETINPUTMOVEMENT> (input_detour.GetTarget());
	return zoom.multiplier * CALL_MEMBER_FN(*this, RealGetInputMovement) (a, b);

}

*/