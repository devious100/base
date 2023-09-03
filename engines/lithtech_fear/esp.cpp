#include "sdk.h"





void Esp::StartEsp()
{
		
	if(!g_pPlayerMgr)
		return;
		
	if(!g_pInterfaceMgr->IsInGame())
		return;
	
	CSpecialFXList* pList = g_pGameClientShell->GetSFXMgr()->GetFXList(SFX_CHARACTER_ID);
	
	int nNumChars = pList->GetSize();
	
	LTVector vCharPos;
	LTVector vScreen;
	LTTransform MyPos;
	bool bOnScreen = false;
	uint32 MyID = 0;
	uint32 ThisID = 0;
	uint32 ThisTeamID = 0;
	uint32 MyTeamID = 0;
	D3DCOLOR PlayerColor;
	
	for (int i=0; i < nNumChars; i++)
	{
		if ((*pList)[i])
		{
			
			CCharacterFX* pChar = (CCharacterFX*)(*pList)[i];
			
			CLIENT_INFO *pPlayer = g_pInterfaceMgr->GetClientInfoMgr()->GetClientByID(pChar->m_cs.nClientID); 
			if (pPlayer)
			{
				ThisID = pPlayer->nID;
				ThisTeamID = pPlayer->nTeamID;
			}
			
			CLIENT_INFO* pMyPlayer = g_pInterfaceMgr->GetClientInfoMgr()->GetLocalClient();
			if(pMyPlayer)
			{
				MyID = pMyPlayer->nID;
				MyTeamID = pMyPlayer->nTeamID;
			}
			
			if (MyID != ThisID && pChar->m_cs.bIsPlayer && !pChar->m_cs.bIsDead && !pChar->IsSpectating())
			{
				
				if(pmenu[4][FEAR.m_pMenu->GetIndex(4, "VoteKick")].Setting)
				{
					CAutoMessage cMsg;
					cMsg.Writeuint8( MID_VOTE );
					cMsg.Writeuint32( pChar->m_cs.nClientID );
					g_pLTClient->SendToServer( cMsg.Read(), MESSAGE_GUARANTEED );
				}

				if(MyTeamID == ThisTeamID && MyTeamID != 255)
				{
					if(IsVisible( g_pGameClientShell->GetPlayerMgr()->GetPlayerCamera()->GetCameraActivePos(), pChar->GetServerObj() ))
						PlayerColor = BLUE;
					else
						PlayerColor = GREEN;
				}
				else
				{
					if(IsVisible( g_pGameClientShell->GetPlayerMgr()->GetPlayerCamera()->GetCameraActivePos(), pChar->GetServerObj() ))
						PlayerColor = RED;
					else
						PlayerColor = ORANGE;
				}
				
				if(pmenu[1][FEAR.m_pMenu->GetIndex(1,"BoneChams")].Setting)
					DrawSkeleton(pChar->GetServerObj(), PlayerColor);

				g_pLTClient->GetObjectPos(pChar->GetServerObj(), &vCharPos);
				
				if(pmenu[4][FEAR.m_pMenu->GetIndex(4,"Teleport")].Setting)
				{
					LTVector vNewPos = LTVector(vCharPos.x,vCharPos.y,vCharPos.z-150);
					g_pLTClient->SetObjectPos(g_pGameClientShell->GetPlayerMgr()->GetMoveMgr()->GetObject(),vNewPos);
					pitem[FEAR.m_pMenu->GetIndex("Teleport")].Setting = false;
				}

				vScreen = GetScreenFromWorldPos2(vCharPos, g_pGameClientShell->GetPlayerMgr()->GetPlayerCamera()->GetCamera(), bOnScreen);
								
				if(bOnScreen)
				{
					const wchar_t * name = g_pInterfaceMgr->GetClientInfoMgr()->GetPlayerName(pChar->m_cs.nClientID);
					char bufret[255];
					WideCharToMultiByte(CP_ACP, 0, name, -1, bufret, sizeof(bufret), NULL, NULL);
					if(pmenu[1][FEAR.m_pMenu->GetIndex(1,"Name")].Setting)
						FEAR.m_pCDraw->DrawText(vScreen.x, vScreen.y, PlayerColor, FEAR.m_pCDraw->g_pFont, "%s", bufret);
						
				
					vScreen.z /= 74;
					
					if(pmenu[1][FEAR.m_pMenu->GetIndex(1,"Distance")].Setting)
						FEAR.m_pCDraw->DrawText(vScreen.x, vScreen.y + 12, PlayerColor, FEAR.m_pCDraw->g_pFont, "[%2.2f]", vScreen.z);
					
					//Was going to be some bounding boxes but i never finished with it
					//FEAR.m_pCDraw->DrawLine(vScreen.x - 10, vScreen.y - 20, vScreen.x - 10, vScreen.y + 20, PlayerColor); //Left
					//FEAR.m_pCDraw->DrawLine(vScreen.x + 10, vScreen.y - 20, vScreen.x + 10, vScreen.y + 20, PlayerColor); //Right
					//FEAR.m_pCDraw->DrawLine(vScreen.x - 10, vScreen.y - 20, vScreen.x + 10, vScreen.y - 20, PlayerColor); //Top
					//FEAR.m_pCDraw->DrawLine(vScreen.x - 10, vScreen.y + 20, vScreen.x + 10, vScreen.y + 20, PlayerColor); //Bottom

				}
                				
			}
			if(pPlayer)
				if (pPlayer->bIsAdmin == true)
					FEAR.m_pCDraw->DrawText(200, 200, RED, "ADMIN PRESENT!");
				

		}
	}
}
bool pausegame = false;
void Esp::Pause()
{

	if(!g_pInterfaceMgr)
		return;


	if(!g_pInterfaceMgr->IsInGame())
		return;
	
		
	if(pmenu[4][FEAR.m_pMenu->GetIndex(4,"Pause")].Setting && !pausegame)
	{
		CAutoMessage cMsg;
		cMsg.Writeuint8(MID_GAME_PAUSE);
		g_pLTClient->SendToServer(cMsg.Read(), MESSAGE_GUARANTEED);
		pausegame = true;
	}
	else if(!pmenu[4][FEAR.m_pMenu->GetIndex(4,"Pause")].Setting && pausegame)
	{
		CAutoMessage cMsg;
		cMsg.Writeuint8(MID_GAME_UNPAUSE);
		g_pLTClient->SendToServer(cMsg.Read(), MESSAGE_GUARANTEED);
		pausegame = false;
	}
	
	
}

BYTE Jmp = 0xEB; // recoil
BYTE JmpBack = 0x74; // recoil back
BYTE NopSix[6] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90}; // spread
BYTE NopSixBack[6] = {}; // spread back
BYTE JmpTwo[6] = {0xE9, 0x74, 0x04, 0x00, 0x00, 0x90}; // HeadBob
BYTE JmpTwoBack[6] = {0x0F, 0x84, 0x73, 0x04, 0x00, 0x00}; // HeadBob back

void Esp::WeaponAccuracy()
{
	if(!g_pInterfaceMgr)
		return;

	static bool recoil = false;
	static bool spread = false;
	static bool weaponsway = false;
	
	DO_ONCE(
	DWORD WSwayAddy = pClient + 0x1E85;
	for (int i = 0; i < 6; i++, (BYTE)WSwayAddy++)
	{
			BYTE* TheByte = (BYTE*)WSwayAddy;
			PATCH_ReadMemory((void*)TheByte, &NopSixBack[i], 1);
	})
	//recoil
	if (pmenu[2][FEAR.m_pMenu->GetIndex(2,"Recoil")].Setting && !recoil)
	{
		recoil = true;
		Patch_WriteMemory( (void*)(pClient + 0x0B0DC1), &Jmp, 1); // recoil
	}
	else if (!pmenu[2][FEAR.m_pMenu->GetIndex(2,"Recoil")].Setting && recoil)
	{
		recoil = false;
		Patch_WriteMemory( (void*)(pClient + 0x0B0DC1), &JmpBack, 1); // recoil
	}
	//spread
	if (pmenu[2][FEAR.m_pMenu->GetIndex(2,"Spread")].Setting && !spread)
	{
		spread = true;
		Patch_WriteMemory( (void*)(pClient + 0x1E85), &NopSix, 6); // spread
	}
	else if (!pmenu[2][FEAR.m_pMenu->GetIndex(2,"Spread")].Setting && spread)
	{
		spread = false;
		Patch_WriteMemory( (void*)(pClient + 0x1E85), &NopSixBack, 6); // spread
	}
	//weaponsway
	if (pmenu[2][FEAR.m_pMenu->GetIndex(2,"WeaponSway")].Setting && !weaponsway)
	{
		weaponsway = true;
		Patch_WriteMemory( (void*)(pClient + 0x5EAAD), &JmpTwo, 6); // WeaponSway
	}
	else if (!pmenu[2][FEAR.m_pMenu->GetIndex(2,"WeaponSway")].Setting && weaponsway)
	{
		weaponsway = false;
		Patch_WriteMemory( (void*)(pClient + 0x5EAAD), &JmpTwoBack, 6); // WeaponSway
	}


}

void Esp::AutoKill()
{
	if ( !pmenu[4][FEAR.m_pMenu->GetIndex(4,"AutoKill")].Setting )
		return;
	
	if(!g_pGameClientShell)
		return;

	if ( !g_pGameClientShell->GetPlayerMgr()->GetClientWeaponMgr()->m_pCurrentWeapon ) return;
	CSpecialFXList* CharList = g_pGameClientShell->GetSFXMgr()->GetFXList( SFX_CHARACTER_ID );
	
	bool bMyTeam = true;
	int iPlayers = CharList->GetSize();
	/////////
	HMODELNODE hNode;
	LTTransform ltBoneTran;
	LTVector BonePos;
	LTVector SFMpos;
	/////////
	for ( unsigned int K = 0; K < iPlayers; K++ )
	{
		if (( *CharList )[ K ])
		{			
			CCharacterFX* CharFX = ( CCharacterFX* )( *CharList )[ K ];
						
			CLIENT_INFO* pThis = g_pInterfaceMgr->GetClientInfoMgr()->GetClientByID( CharFX->m_cs.nClientID );
			if ( pThis )
				bMyTeam = g_pInterfaceMgr->GetClientInfoMgr()->IsLocalTeam( pThis->nTeamID );
				
			CLIENT_INFO* pMyPlayer = g_pInterfaceMgr->GetClientInfoMgr()->GetLocalClient();
			if(pMyPlayer)
				if(FEAR.m_pAimbot->bIsDeathMatch(pMyPlayer))
					bMyTeam = false;
							
			if (CharFX->m_cs.bIsPlayer && !CharFX->IsPlayerDead() && bMyTeam == false  && !CharFX->IsSpectating())
			{					
			
				g_pLTClient->GetModelLT()->GetNode(CharFX->GetServerObj(), BONE_Head, hNode);/////////
				if(LT_OK == g_pLTClient->GetModelLT()->GetNodeTransform(CharFX->GetServerObj(), hNode, ltBoneTran, true))///////
				{

					BonePos = ltBoneTran.m_vPos;
					SFMpos = BonePos;
					SFMpos.x -= 15;
											
					CAutoMessage cMsg;
					cMsg.Writeuint8(MID_OBJECT_MESSAGE);
					cMsg.WriteObject(g_pPlayerMgr->GetMoveMgr()->GetServerObject());
					cMsg.Writeuint32(MID_MELEEATTACK);
					cMsg.WriteObject(CharFX->GetServerObj());
					cMsg.Writeuint32(hNode);
					cMsg.WriteLTVector(SFMpos);
					cMsg.WriteLTVector(BonePos);
					cMsg.Writeint32(g_pGameClientShell->GetServerRealTimeMS());
					g_pLTClient->SendToServer(cMsg.Read(), MESSAGE_GUARANTEED);
				}
			}
		}
	}
}



void Esp::CrossHair()
{
	if(!pmenu[4][FEAR.m_pMenu->GetIndex(4, "Crosshair")].Setting)
		return;
	
	FEAR.m_pCDraw->FillRGBA(FEAR.m_pCDraw->oViewport.Width / 2, (FEAR.m_pCDraw->oViewport.Height / 2) - 5, 1, 11, 255, 0, 0, 255);
	FEAR.m_pCDraw->FillRGBA((FEAR.m_pCDraw->oViewport.Width / 2) - 5, (FEAR.m_pCDraw->oViewport.Height / 2), 11, 1, 255, 0, 0, 255);
	
}

void Esp::PickUps()
{
	if(!pmenu[4][FEAR.m_pMenu->GetIndex(4,"PickupItems")].Setting) 
		return;

	if(!g_pInterfaceMgr)
		return;

	if(!g_pInterfaceMgr->IsInGame())
		return;

	CSpecialFXList*	WepList	= g_pGameClientShell->GetSFXMgr()->GetFXList( SFX_PICKUPITEM_ID );
	int iWeapons = WepList->GetSize();
	for ( int K = 0; K < iWeapons; ++K )
	{
		if (( *WepList )[ K ] )
		{
			CPickupItemFX* pWepPickup = ( CPickupItemFX* )( *WepList )[ K ];
			if ( pWepPickup->GetPickupItemType() == kPickupItemType_Gear || pWepPickup->GetPickupItemType() == kPickupItemType_Weapon ) 
			{
				CAutoMessage cMsg;
				cMsg.Writeuint8( MID_PICKUPITEM_ACTIVATE );
				cMsg.WriteObject( pWepPickup->GetServerObj() );
				cMsg.Writebool( 0 ); 
				g_pLTClient->SendToServer( cMsg.Read(), MESSAGE_GUARANTEED ); 
				
			}
		}
	}
	pmenu[4][FEAR.m_pMenu->GetIndex(4,"PickupItems")].Setting = false;
}

void Esp::DrawSkeleton(HOBJECT hObject, D3DCOLOR Color)
{
	HMODELNODE hNode[BONES];
	LTTransform ltBoneTran[BONES];
	LTVector BonePos[BONES];
	LTVector PlayerPos[BONES];
	
	bool bOnScreen = false;
		
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Pelvis, hNode[0]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Torso, hNode[1]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Upper_torso, hNode[2]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Neck, hNode[3]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Head, hNode[4]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Left_shoulder, hNode[5]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Left_armu, hNode[6]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Left_arml, hNode[7]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Left_hand, hNode[8]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Right_shoulder, hNode[9]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Right_armu, hNode[10]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Right_arml, hNode[11]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Right_hand, hNode[12]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Left_legu, hNode[13]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Left_legl, hNode[14]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Left_foot, hNode[15]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Right_legu, hNode[16]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Right_legl, hNode[17]);
	g_pLTClient->GetModelLT()->GetNode(hObject, BONE_Right_foot, hNode[18]);


	for(int i = 0; i < BONES; i++)
	{
		
		if(LT_OK == g_pLTClient->GetModelLT()->GetNodeTransform(hObject, hNode[i], ltBoneTran[i], true))
		{
		
			BonePos[i] = ltBoneTran[i].m_vPos;
			PlayerPos[i] = GetScreenFromWorldPos2(BonePos[i], g_pGameClientShell->GetPlayerMgr()->GetPlayerCamera()->GetCamera(), bOnScreen);
		}
	
	}
	
	if(bOnScreen)
	{
		FEAR.m_pCDraw->DrawLine(PlayerPos[4].x, PlayerPos[4].y, PlayerPos[3].x, PlayerPos[3].y, Color); //Head -> Neck
		FEAR.m_pCDraw->DrawLine(PlayerPos[3].x, PlayerPos[3].y, PlayerPos[5].x, PlayerPos[5].y, Color); //Neck -> LeftShoulder
		FEAR.m_pCDraw->DrawLine(PlayerPos[5].x, PlayerPos[5].y, PlayerPos[6].x, PlayerPos[6].y, Color); //LeftShoulder -> Left Armu
		FEAR.m_pCDraw->DrawLine(PlayerPos[6].x, PlayerPos[6].y, PlayerPos[7].x, PlayerPos[7].y, Color); //Left Armu -> Left Arml
		FEAR.m_pCDraw->DrawLine(PlayerPos[7].x, PlayerPos[7].y, PlayerPos[8].x, PlayerPos[8].y, Color); //Left Arml -> Left Hand
		FEAR.m_pCDraw->DrawLine(PlayerPos[3].x, PlayerPos[3].y, PlayerPos[9].x, PlayerPos[9].y, Color); //Neck -> RightShoulder
		FEAR.m_pCDraw->DrawLine(PlayerPos[9].x, PlayerPos[9].y, PlayerPos[10].x, PlayerPos[10].y, Color); //RightShoulder -> Right Armu
		FEAR.m_pCDraw->DrawLine(PlayerPos[10].x, PlayerPos[10].y, PlayerPos[11].x, PlayerPos[11].y, Color); //Right Armu -> Right Arml
		FEAR.m_pCDraw->DrawLine(PlayerPos[11].x, PlayerPos[11].y, PlayerPos[12].x, PlayerPos[12].y, Color); //Right Arml -> Right Hand
		FEAR.m_pCDraw->DrawLine(PlayerPos[3].x, PlayerPos[3].y, PlayerPos[2].x, PlayerPos[2].y, Color); //Neck -> Upper Torso
		FEAR.m_pCDraw->DrawLine(PlayerPos[2].x, PlayerPos[2].y, PlayerPos[1].x, PlayerPos[1].y, Color); //Upper Torso -> Torso
		FEAR.m_pCDraw->DrawLine(PlayerPos[1].x, PlayerPos[1].y, PlayerPos[0].x, PlayerPos[0].y, Color); //Torso -> Pelvis
		FEAR.m_pCDraw->DrawLine(PlayerPos[0].x, PlayerPos[0].y, PlayerPos[13].x, PlayerPos[13].y, Color); //Pelvis -> Left Legu
		FEAR.m_pCDraw->DrawLine(PlayerPos[13].x, PlayerPos[13].y, PlayerPos[14].x, PlayerPos[14].y, Color); //Left Legu -> Left Legl
		FEAR.m_pCDraw->DrawLine(PlayerPos[14].x, PlayerPos[14].y, PlayerPos[15].x, PlayerPos[15].y, Color); //Left Legl -> Left foot
		FEAR.m_pCDraw->DrawLine(PlayerPos[0].x, PlayerPos[0].y, PlayerPos[16].x, PlayerPos[16].y, Color); //Pelvis -> Right Legu
		FEAR.m_pCDraw->DrawLine(PlayerPos[16].x, PlayerPos[16].y, PlayerPos[17].x, PlayerPos[17].y, Color); //Right Legu -> Right Legl
		FEAR.m_pCDraw->DrawLine(PlayerPos[17].x, PlayerPos[17].y, PlayerPos[18].x, PlayerPos[18].y, Color); //Right Legl -> Right foot
			
	}

}


LTVector Esp::GetScreenFromWorldPos2(const LTVector& vPos, HOBJECT hCamera, bool& bOnScreen)
{
	LTVector vScreenPos;
	LTRESULT res = g_pLTClient->GetRenderer()->WorldPosToScreenPos(hCamera, vPos, vScreenPos);

	bOnScreen = (LT_OK == res && vScreenPos.x >= 0.0f && vScreenPos.x <= 1.0f 
		&& vScreenPos.y >= 0.0f && vScreenPos.y <= 1.0f
		&& vScreenPos.z > 0.0f);

	//scale based upon the physical extents of the screen
	vScreenPos.x *= FEAR.m_pCDraw->oViewport.Width;
	vScreenPos.y *= FEAR.m_pCDraw->oViewport.Height;

	return vScreenPos;
}

LTVector Esp::GetScreenFromWorldPos2(const LTVector& vPos, HOBJECT hCamera)
{
	LTVector vScreenPos;
	LTRESULT res = g_pLTClient->GetRenderer()->WorldPosToScreenPos(hCamera, vPos, vScreenPos);

	//scale based upon the physical extents of the screen
	vScreenPos.x *= FEAR.m_pCDraw->oViewport.Width;
	vScreenPos.y *= FEAR.m_pCDraw->oViewport.Height;

	return vScreenPos;
}
