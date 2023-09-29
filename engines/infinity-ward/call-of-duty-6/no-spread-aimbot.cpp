void (__cdecl *R_WeaponSpread)( cg_t*, weapon_t*, vec3_t MaxSpread, vec3_t BaseSpread ) = (void (__cdecl *)(cg_t* ,weapon_t* , vec3_t , vec3_t))0x4249C0;
void (__cdecl *R_RandomFloats)( int iSeed,float* flSpreadRight,float* flSpreadUp ) = (void (__cdecl*)(int,float*,float*))0x82E1F0;
weapon_t*(__cdecl *GetWeaponIndex)( int iIndex ) = ( weapon_t* ( __cdecl* )( int ) )0x57E9C0;
void (__cdecl *R_AngleVectors)( float*weapon_angles,float*forward,float*right,float*up) = ( void(__cdecl*)(float*,float*,float*,float*) )0x6913C0;

void GetWeaponAngles( vec3_t Angles )
{
	float WeaponAngleX = viewMatrix->WeaponAngleX;
	float WeaponAngleY = viewMatrix->WeaponAngleY;

	if( Angles )
	{
	    Angles[PITCH] = WeaponAngleX;
	    Angles[YAW] = WeaponAngleY;
	}
}
void GetSpreadMultiplier( float &spreadMultiplier )
{
	float fMulti = 0;

	int iSeed = cg->Seed;

	__asm
	{
	    mov esi, cg;
	    add esi, [iSeed]//[cg+0x4069C]
	    movss xmm0, [esi];	
	    mov eax, [0xAADD38]//spreadmultiplier
	    mulss xmm0, [eax]
	    movss [fMulti], xmm0;
	}

	spreadMultiplier = fMulti;
}
void CG_WeaponSpread ( weapon_t *pWeapon, float* WeaponSpread )
{
	float SpreadMultiplier = 0.0f;

	float MaxSpread = 0.0f;
	float BaseSpread = 0.0f;

	if( pWeapon )
	{
		GetSpreadMultiplier(SpreadMultiplier);

	    R_WeaponSpread(cg,pWeapon,&MaxSpread,&BaseSpread);
		
		*WeaponSpread = BaseSpread + ( ( MaxSpread - BaseSpread ) * SpreadMultiplier );
	}
}
void ApplySpread()
{
	input_t *g_pInput = (input_t*)0xEB1468;
	usercmd_t *pCmd = g_pInput->GetUserCmd( g_pInput->currentCmdNum -1 );
	//pCmd->servertime = *(int*)0x2C3D820C + 1;

    float flSpread = 0.0f;
	float flSpreadRight,flSpreadUp = 0.0f;
	float max_distance = (float)8192.0f;

	vec3_t forward,right,up,calc_spread_view,spread_view,dest,weapon_angles,spread_angle;

	GetWeaponAngles( weapon_angles );

	R_AngleVectors( weapon_angles,forward,right,up );

	weapon_t* pWeapon = GetWeaponIndex( clientInfo[cg->clientNum].weapon );

	CG_WeaponSpread( pWeapon,&flSpread );

	flSpread *= 0.01745329238474369f;
    flSpread = tan(flSpread) * max_distance; 

	R_RandomFloats( cg->Seed,&flSpreadRight,&flSpreadUp );
	flSpreadRight *= flSpread;
    flSpreadUp    *= flSpread;

	spread_view[ 0 ] = forward[ 0 ] * 8192.0f + ( refdef->vOrigin[ 0 ] + flSpreadRight * right[ 0 ] + flSpreadUp * up[ 0 ] );
	spread_view[ 1 ] = forward[ 1 ] * 8192.0f + ( refdef->vOrigin[ 1 ] + flSpreadRight * right[ 1 ] + flSpreadUp * up[ 1 ] );
	spread_view[ 2 ] = forward[ 2 ] * 8192.0f + ( refdef->vOrigin[ 2 ] + flSpreadRight * right[ 2 ] + flSpreadUp * up[ 2 ] );

	calc_spread_view[ 0 ] = spread_view[ 0 ] - refdef->vOrigin[ 0 ];
	calc_spread_view[ 1 ] = spread_view[ 1 ] - refdef->vOrigin[ 1 ];
	calc_spread_view[ 2 ] = spread_view[ 2 ] - refdef->vOrigin[ 2 ];

	gUtils.VectorAngles( calc_spread_view,dest );

	dest[0] *= -1;

	spread_angle[0] = refdef->refdefViewAngles[0] - dest[0];
	spread_angle[1] = refdef->refdefViewAngles[1] - dest[1];
	spread_angle[2] = 0;

	pCmd->viewangles[0] += ANGLE2SHORT( spread_angle[0] );
	pCmd->viewangles[1] += ANGLE2SHORT( spread_angle[1] );
	pCmd->viewangles[2] += ANGLE2SHORT( spread_angle[2] );
	
}