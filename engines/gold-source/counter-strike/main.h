#pragma once
#pragma comment(lib, "opengl32.lib")

#include <Windows.h>
#include <process.h>
#include <Vector>
#include <String>
#include <GL/GL.h>

#include <exception>

#include "SDK/wrect.h"
#include "SDK/cl_dll.h"
#include "SDK/pmtrace.h" 
#include "SDK/r_studioint.h"
#include "SDK/triangleapi.h"
#include "SDK/r_efx.h"
#include "SDK/studio.h"
#include "SDK/event_api.h"
#include "SDK/parsemsg.h"
#include "SDK/ref_params.h"
#include "SDK/com_model.h"
#include "SDK/screenfade.h"
#include "SDK/usercmd.h"
#include "SDK/pm_defs.h"
#include "SDK/keydefs.h"
#include "SDK/com_weapons.h"

#include "hook.h"
#include "client.h"
#include "engine.h"
#include "studio.h"
#include "sound.h"
#include "entinfo.h"
#include "utils.h"
#include "usermsgs.h"
#include "esp.h"
#include "drawing.h"

typedef void (*DynamicSound_orig)(int,int,char*,float*,float,float,int,int);

extern DynamicSound_orig oDynamicSound;

#define M_PI 3.14159265358979323846
#define POW(x)((x)*(x))
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}
#define VectorDistance(a,b)sqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}
#define IS_NAN(x)(((*(int*)&x)&(255<<23))==(255<<23))