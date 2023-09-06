/**************************************\
*                 cuBot                *
*    Call of Duty 2 (1.3) Clienthook   *
*                                      *
*               By Shard               *
*                                      *
*	        www.mpcforum.com           *
*	     www.cheatersutopia.com        *
\**************************************/

/*
	I think RainerStoff is to thank for the original GetAngleToOrigin and WorldToScreen functions.
	(I can't remember exactly where I got them from.)

	http://www.netcoders.cc/
	http://www.rshook.org.uk/
*/

/*************/
/* Maths.cpp */
/*************/
#include "stdafx.h"

float ScreenW = 1152.0f;
float ScreenH = 864.0f;

vec3_t vec3_origin = { 0.0f, 0.0f, 0.0f };

void CalcAimVectors( orientation_t * or, vec3_t vectors, vec3_t out )
{
	vec3_t aX, aY, aZ;

	VectorScale( or->axis[1], vectors[0], aX );
	VectorScale( or->axis[2], vectors[1], aY );
	VectorScale( or->axis[0], vectors[2], aZ );

	out[0] = or->origin[0] + aX[0] + aY[0] + aZ[0];
	out[1] = or->origin[1] + aX[1] + aY[1] + aZ[1];
	out[2] = or->origin[2] + aX[2] + aY[1] + aZ[2];
}

float GetDistance( vec3_t A, vec3_t B )
{
	float dx = A[0] - B[0];
	float dy = A[1] - B[1];
	float dz = A[2] - B[2];

	return (float)sqrt( ( dx * dx ) + ( dy * dy ) + ( dz * dz ) );
}

float Get2dDistance( vec2_t A, vec2_t B )
{
	float dx = A[0] - B[0];
	float dy = A[1] - B[1];

	return (float)sqrt( ( dx * dx ) + ( dy * dy ) );
}

bool IsPointVisible( vec3_t point )
{
	trace_t t;

	CG_Trace( &t, cg->refdef.vieworg, vec3_origin, vec3_origin, point, cg->snap->ps.clientNum, MASK_TARGET & ~CONTENTS_BODY );

	return ( t.fraction == 1.0f );
}

bool IsEntityVisible( vec3_t point, int num )
{
	trace_t t;

	CG_Trace( &t, cg->refdef.vieworg, vec3_origin, vec3_origin, point, cg->snap->ps.clientNum, MASK_TARGET );

	if( t.fraction != 1.0f )
		return ( t.entityNum == num );
	else
		return true;
}

centity_t * AimingAtEntity( )
{
	trace_t	t;
	vec3_t	end;

	VectorMA( cg->refdef.vieworg, 131072, cg->refdef.viewaxis[0], end );

	CG_Trace( &t, cg->refdef.vieworg, vec3_origin, vec3_origin, end, cg->snap->ps.clientNum, MASK_TARGET | CONTENTS_BODY );

	if( t.fraction != 1.0f )
	{
		centity_t * cent = &cg_entities[ t.entityNum ];

		return cent;
	}

	return NULL;
}

clientInfo_t * AimingAtClient( )
{
	centity_t * cent = AimingAtEntity( );

	if( cent && cent->currentState.eType == ET_PLAYER )
		return &clientinfo[ cent->currentState.clientNum ];

	return NULL;
}

/* Credits to RainerStoff */
qboolean WorldToScreen( vec3_t origin, float *ScreenX, float *ScreenY ) {
	float	xzi, yzi;
	vec3_t	local, transformed;
	vec3_t	vfwd, vright, vup;
	#define xcenter ( ScreenW / 2.f )
	#define ycenter ( ScreenH / 2.f )

	AngleVectors( cg->refdefViewAngles, vfwd, vright, vup );
	VectorSubtract( origin, cg->refdef.vieworg, local );
	transformed[0] = DotProduct( local, vright );
	transformed[1] = DotProduct( local, vup );
	transformed[2] = DotProduct( local, vfwd );

	if( transformed[2] < 0.01f )
		return qfalse;

	xzi			= xcenter / transformed[2] * ( 90.0f / cg->refdef.fov_x );
	yzi			= ycenter / transformed[2] * ( 90.0f / cg->refdef.fov_y );
	*ScreenX	= xcenter + ( xzi * transformed[0] );
	*ScreenY	= ycenter - ( yzi * transformed[1] );

	return qtrue;
}

void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]) {
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
}

void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float			angle;
	static float	sr, sp, sy, cr, cp, cy;

	angle = angles[YAW] * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);

	angle = angles[PITCH] * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[ROLL] * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}

void vectoangles( const vec3_t value1, vec3_t angles )
{
	float	forward;
	float	yaw, pitch;
	
	if( value1[1] == 0 && value1[0] == 0 )
	{
		yaw = 0;
		if ( value1[2] > 0 )
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		if ( value1[0] )
			yaw = ( atan2 ( value1[1], value1[0] ) * ( 180.0f / M_PI ) );
		else if ( value1[1] > 0 )
			yaw = 90.0f;
		else
			yaw = 270.0f;

		if ( yaw < 0.0f )
			yaw += 360.0f;

		forward = sqrt ( ( value1[0] * value1[0] ) + ( value1[1] * value1[1] ) );
		pitch = ( atan2( value1[2], forward ) * ( 180.0f / M_PI ) );

		if ( pitch < 0.0f )
			pitch += 360;
	}

	angles[PITCH]	= -pitch;
	angles[YAW]		= yaw;
	angles[ROLL]	= 0;
}

/* Credits to RainerStoff */
void GetAngleToOrigin( vec3_t origin, vec3_t angles )
{
	vec3_t entity;
	VectorSubtract( origin, cg->refdef.vieworg, entity );
	vectoangles( entity, angles );

	if( angles[PITCH] > 180.0f)				angles[PITCH] -= 360.0f;
	else if( angles[PITCH] < -180.0f )		angles[PITCH] += 360.0f;
	if( angles[YAW] > 180.0f )				angles[YAW] -= 360.0f;
	else if( angles[YAW] < -180.0f )		angles[YAW] += 360.0f;
	angles[PITCH]	-= cg->refdefViewAngles[PITCH];
	angles[YAW]		-= cg->refdefViewAngles[YAW];
	if( angles[PITCH] > 180.0f )			angles[PITCH] -= 360.0f;
	else if( angles[PITCH] < -180.0f )		angles[PITCH] += 360.0f;
	if( angles[YAW] > 180.0f )				angles[YAW] -= 360.0f;
	else if( angles[YAW] < -180.0f )		angles[YAW] += 360.0f;
}