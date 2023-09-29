// Credits : Shad0w, Kingorgy, Twice, Robbin, Crx, sfab

void CNoSpread::R_WeaponSpread( weaponInfo_t* pWeapon,float*flSpread )
{
	DWORD dwWeaponSpread = 0x420620;

	float SpreadMultiplier = *( float* )0xA8D5F8 / 255.f;
	float MaxSpread = 0.0f;
	float BaseSpread = 0.0f;

	__asm
	{
		MOV ECX, DWORD PTR DS:[ pWeapon ];
		PUSH ECX;
		MOV ESI, [ 0x9E2AB4 ];
		LEA ECX, DWORD PTR DS:[ MaxSpread ];
		LEA EDX, DWORD PTR DS:[ BaseSpread ];
		CALL DWORD PTR DS:[ dwWeaponSpread ];
		ADD ESP, 0x4;
	}

	*flSpread = BaseSpread + ( ( MaxSpread - BaseSpread ) *SpreadMultiplier );
}
void CNoSpread::R_RandomSpread( int iSeed,float*spread_x,float*spread_y )
{
	__asm
	{
		PUSH [ spread_y ];
		PUSH [ spread_x ];
		PUSH [ iSeed ];
		MOV EAX, 0x47E630;
		CALL EAX;
		ADD ESP,0xC;
	}
}
void CNoSpread::R_AngleVectors( vec3_t* pViewAngles, vec3_t* pReturnVector, vec3_t* pESIReturnVector, vec3_t* pEDIReturnVector )
{
	DWORD TransformByAngles = 0x5B9CC0;

	__asm
	{
		MOV EDI, DWORD PTR DS:[ pEDIReturnVector ];
		MOV ESI, DWORD PTR DS:[ pESIReturnVector ];
		MOV EDX, DWORD PTR DS:[ pViewAngles ];
		MOV ECX, DWORD PTR DS:[ pReturnVector ];
		PUSH ECX;
		CALL DWORD PTR DS:[ TransformByAngles ];
		ADD ESP,0x4;
	}
}
void CNoSpread::ApplySpread()
{
	input_t* g_pInput = ( input_t* )0xFAB334;
	usercmd_t* cmd = g_pInput->GetUserCmd( g_pInput->currentCmdNum -1 );
	cmd->servertime = *(int*)0x9E2AA4 + 1;

	int seed = *( int* )0x9E2AB4;

	float spread = 0.0f; 
	float spread_x = 0.0f; 
	float spread_y = 0.0f; 
	float distance = 8192.0f; 

	vec3_t bullet_angles,spread_view,calc_spread_view,spread_angle,forward,right,up,dest; 

	bullet_angles[ 0 ] = *( float* )0x9EEBB0;
	bullet_angles[ 1 ] = *( float* )0x9EEBB4;
	bullet_angles[ 2 ] = *( float* )0x9EEBB8;

	R_AngleVectors(  &bullet_angles, &up, &forward, &right );
	 
	weaponInfo_t* pWeapon = gUtils.GetWeapon( cg->predictedPlayerState.Weapon );

	R_WeaponSpread( pWeapon,&spread ); 
	
	spread *= 0.01745329238474369f; 
	
    spread = tan( spread ) * distance; 
	
	R_RandomSpread( seed,&spread_x,&spread_y ); 
	
	spread_x *= spread; 
	spread_y *= spread; 

	spread_view[ 0 ] = viewMatrix->vOrigin[ 0 ] + forward[ 0 ] * 8192 + right[ 0 ] * spread_x + up[ 0 ] * spread_y; 
	spread_view[ 1 ] = viewMatrix->vOrigin[ 1 ] + forward[ 1 ] * 8192 + right[ 1 ] * spread_x + up[ 1 ] * spread_y; 
	spread_view[ 2 ] = viewMatrix->vOrigin[ 2 ] + forward[ 2 ] * 8192 + right[ 2 ] * spread_x + up[ 2 ] * spread_y; 
	
	VectorSubtract( spread_view,viewMatrix->vOrigin,calc_spread_view ); 
	
	gUtils.VectorAngles( calc_spread_view,dest ); 

	spread_angle[ 0 ] = bullet_angles[ 0 ] - dest[ 0 ]; 
	spread_angle[ 1 ] = bullet_angles[ 1 ] - dest[ 1 ]; 
	spread_angle[ 2 ] = bullet_angles[ 2 ] - dest[ 2 ]; 
	
	cmd->viewangles[ 0 ] += ANGLE2SHORT( spread_angle[ 0 ] ); 
	cmd->viewangles[ 1 ] += ANGLE2SHORT( spread_angle[ 1 ] ); 
	cmd->viewangles[ 2 ] += ANGLE2SHORT( spread_angle[ 2 ] ); 
}

void (__cdecl *GetBulletTrajectory)(int po, bulletTrace_t* MapTrace, weaponInfo_t* Weapon, entity_t* cent, vec3_t vieworg, int B_On, int T_On ) = (void (__cdecl *)(int po, bulletTrace_t* MapTrace, weaponInfo_t* Weapon, entity_t* cent, vec3_t ViewOrg, int B_On, int T_On  ))0x47DAB0;

void CAutowall::SetupBulletTrace( bulletTrace_t* btr, vec3_t start, vec3_t end, int skipNum )
{
	btr->worldEntNum = 1022;
	btr->skipNum = skipNum;
	btr->power = 1.0f;
	btr->bulletType = 1;

	VectorCopy( refdef->Vieworg, btr->vieworg );
	VectorCopy( start, btr->start );
	VectorCopy( end, btr->end );

	vec3_t delta;
	VectorSubtract( end, start, delta );

	vec3_t angles;
	gUtils.VectorAngles( delta, angles );
	gUtils.AngleVectors( angles, btr->viewDirection, NULL, NULL );
}
void CAutowall::PatchFX( bool bState )
{
	DWORD dword_1EB3FB4 = *(DWORD*)0x1EB3FB4; // fx_enable
	DWORD dword_1EB3FE0 = *(DWORD*)0x1EB3FE0; // fx_draw
	
	if(!bState) {
		*(BYTE *)(dword_1EB3FB4 + 16) = 0x0;
		*(BYTE *)(dword_1EB3FE0 + 16) = 0x0;
	} else {
		*(BYTE *)(dword_1EB3FB4 + 16) = 0x1;
		*(BYTE *)(dword_1EB3FE0 + 16) = 0x1;
	}
}
void CAutowall::GetRemainingPower( vec3_t vieworg, entity_t* cent, clientInfo_t* pClient, vec3_t origin, int iCheckNum, float* remainingPower )
{
	bulletTrace_t bulletTrace;
	SetupBulletTrace( &bulletTrace, vieworg, origin, cg->clientNum );

	weaponInfo_t* pLocalWeapon = gUtils.GetWeapon(cg_entities[cg->clientNum].weapon);

	PatchFX( false );
	GetBulletTrajectory(NULL, &bulletTrace, pLocalWeapon, &cg_entities[cg->clientNum], vieworg, NULL, cg->clientNum);

	if(bulletTrace.skipNum == iCheckNum)
		*remainingPower = bulletTrace.power;
	else 
		*remainingPower = 0.0f;
}