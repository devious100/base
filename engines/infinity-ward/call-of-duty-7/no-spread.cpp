    void (__cdecl *GetRandomFloats)( int iSeed, float* flSpreadRight, float* flSpreadUp ) = (void (__cdecl*)(int, float*, float*))0x7FC000;
     
    void GetSpreadMultiplier( float &spreadMultiplier )
    {
            float fMulti = 0;
     
            _asm mov esi,[ cg ];
            _asm add esi, [0x6FB10]//seed
            _asm movss xmm0, [esi];
            _asm mov eax, [0xA60D78]//spread multiplier
            _asm mulss xmm0, [eax]
            _asm movss [fMulti], xmm0;
     
            spreadMultiplier = fMulti;
    }
    void CNoSpread::GetWeaponViewAngles ( vec3_t pAngles )
    {
            pAngles[PITCH] = viewMatrix->WeaponAngleY;
            pAngles[YAW] = viewMatrix->WeaponAngleX;
    }
    void CNoSpread::GetWeaponSpread ( weapon_t* pWeapon, float* WeaponSpread )
    {
            DWORD dwCall = 0x57D080;
     
            float flMaxSpread = 0.0f;
            float flBaseSpread = 0.0f;
            float flSpreadMultiplier = 0.0f;
     
            GetSpreadMultiplier(flSpreadMultiplier);
     
            _asm
            {
                    mov edi, [ cg ];
                    mov esi, [ pWeapon ];
                    lea eax, [ flMaxSpread ];
                    push eax;
                    lea ecx, [ flBaseSpread ];
                    push ecx;
                    lea edx, [ edi+0x4069C ];
                    push esi;
                    push edx;
                    call [ dwCall ];
                    add esp, 0x10;
            }
     
            *WeaponSpread = flBaseSpread + ( ( flMaxSpread - flBaseSpread ) * flSpreadMultiplier );
    }
    void CNoSpread::ApplySpread()
    {
            input_t* input = (input_t*)0xEAA4E8;
        usercmd_t * cmd = input->GetUserCmd( input->currentCmdNum - 1 );
            cmd->servertime++;
            cmd->servertime -= -1;
     
            if( gAimbot.bHasTarget )
            {
                    float spread = 0.0f;
                float spread_x = 0.0f;
                float spread_y = 0.0f;
                float distance = (float)8192.0f;
     
                vec3_t weapon_angles,spread_view,calc_spread_view,spread_angle,forward,right,up,dest;
     
                GetWeaponViewAngles( weapon_angles );
     
                gUtils.AngleVectors( weapon_angles,forward,right,up );
     
                weapon_t* pWeapon = GetWeaponIndex( clientInfo[cg->clientNum].weapon );
     
                GetWeaponSpread( pWeapon,&spread );
     
                spread *= 0.01745329238474369f;
                spread = tan( spread ) * distance;
     
                GetRandomFloats(cg->Seed,&spread_x,&spread_y);
     
                spread_x *= spread;
                spread_y *= spread;
             
                spread_view[ 0 ] = refdef->Vieworg[ 0 ] + forward[ 0 ] * distance + right[ 0 ] * spread_x + up[ 0 ] * spread_y;
                spread_view[ 1 ] = refdef->Vieworg[ 1 ] + forward[ 1 ] * distance + right[ 1 ] * spread_x + up[ 1 ] * spread_y;
                spread_view[ 2 ] = refdef->Vieworg[ 2 ] + forward[ 2 ] * distance + right[ 2 ] * spread_x + up[ 2 ] * spread_y;
     
                calc_spread_view[ 0 ] = spread_view[ 0 ] - refdef->Vieworg[ 0 ];
                calc_spread_view[ 1 ] = spread_view[ 1 ] - refdef->Vieworg[ 1 ];
                calc_spread_view[ 2 ] = spread_view[ 2 ] - refdef->Vieworg[ 2 ];
     
                gUtils.VectorAngles( calc_spread_view,dest );
     
                spread_angle[ 0 ] = refdef->refdefViewAngles[ 0 ] - dest[ 0 ];
                spread_angle[ 1 ] = refdef->refdefViewAngles[ 1 ] - dest[ 1 ];
                spread_angle[ 2 ] = 0;
     
                if( !clientInfo[cg->clientNum].Zoomed )
                    {
                            cmd->viewangles[ 0 ] += ANGLE2SHORT( spread_angle[ 0 ] );
                    cmd->viewangles[ 1 ] += ANGLE2SHORT( spread_angle[ 1 ] );
                    cmd->viewangles[ 2 ] += ANGLE2SHORT( spread_angle[ 2 ] );
                    }
            }
    }

