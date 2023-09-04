#include "sdk.h"


static bool VisibleFilterFn ( HOBJECT hTest, void *pUserData )
{
	uint32 dwFlags;
	g_pLTClient->Common( )->GetObjectFlags ( hTest, OFT_Flags, dwFlags );
	if (!( dwFlags & FLAG_VISIBLE )) return 0;
	if (!( dwFlags & FLAG_RAYHIT ))  return 0;
	HOBJECT hClientHitBox = NULL;
	CCharacterFX* pCharacter = g_pGameClientShell->GetSFXMgr( )->GetCharacterFX ( g_pLTClient->GetClientObject( ) );
	if ( pCharacter )
		hClientHitBox = pCharacter->GetHitBox( );
	HOBJECT hFilterList[] = 
	{
		g_pLTClient->GetClientObject( ),
			g_pGameClientShell->GetPlayerMgr( )->GetMoveMgr( )->GetObject( ),
			hClientHitBox,
			NULL
	};
	if ( !ObjListFilterFn ( hTest, (void*) hFilterList ) )
		return 0;
	return 1;
}
/*============================================================================*/
bool IsVisible ( LTVector pStart, HOBJECT m_hObject )
{
	LTVector pOrigin;
	g_pLTClient->GetObjectPos ( m_hObject, &pOrigin );
	IntersectQuery IQuery;
	IntersectInfo IInfo;
	IQuery.m_From  = pStart;	
	IQuery.m_To = pOrigin;
	IQuery.m_Flags = INTERSECT_HPOLY | INTERSECT_OBJECTS | IGNORE_NONSOLID;
	IQuery.m_FilterActualIntersectFn  = VisibleFilterFn;
	IQuery.m_pActualIntersectUserData = ( void* )&IQuery;
	IQuery.m_PolyFilterFn = NULL;
	if ( g_pLTClient->IntersectSegment ( IQuery, &IInfo ))
	{
		if ( IInfo.m_hObject == m_hObject )
			return 1;
	}
	return 0;
}
void Aimbot::FindTarget()
{
	if(!pmenu[0][FEAR.m_pMenu->GetIndex(0,"Toggle")].Setting)
		return;
	
	if(!g_pGameClientShell)
		return;

	if ( !g_pGameClientShell->GetPlayerMgr()->GetClientWeaponMgr()->m_pCurrentWeapon )return;

	CSpecialFXList* CharList = g_pGameClientShell->GetSFXMgr()->GetFXList( SFX_CHARACTER_ID );
	LTVector vEnemy;
	static LTVector UpDist( 0.0f, 1.0f, 0.0f );
	bool bMyTeam = true;
	uint32 ThisID = NULL;
	uint32 MyID = NULL;
	HOBJECT hDamager = NULL;

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
			{
				bMyTeam = g_pInterfaceMgr->GetClientInfoMgr()->IsLocalTeam( pThis->nTeamID );
				ThisID = pThis->nID;
			}
			CLIENT_INFO* pMyPlayer = g_pInterfaceMgr->GetClientInfoMgr()->GetLocalClient();
			if(pMyPlayer)
			{
				MyID = pMyPlayer->nID;
				
				if(bIsDeathMatch(pMyPlayer))
					bMyTeam = false;
			}
				
				

			if (MyID != ThisID && CharFX->m_cs.bIsPlayer && !CharFX->IsPlayerDead() && IsVisible( g_pGameClientShell->GetPlayerMgr()->GetPlayerCamera()->GetCameraActivePos(), CharFX->GetServerObj() ) && bMyTeam == false  && !CharFX->IsSpectating())
			{					
				Victim = CharFX->GetServerObj();
				g_pLTClient->GetModelLT()->GetNode(CharFX->GetServerObj(), pNode, hNode);
			
				if(LT_OK == g_pLTClient->GetModelLT()->GetNodeTransform(CharFX->GetServerObj(), hNode, ltBoneTran, true))
				{
					
					BonePos = ltBoneTran.m_vPos;
		
					LTVector vFinal = (BonePos - g_pGameClientShell->GetPlayerMgr()->GetPlayerCamera()->GetCameraPos() );
					vFinal.Normalize();
					LTRotation rNewRot( vFinal, UpDist );

					if ( !g_pGameClientShell->GetPlayerMgr()->GetClientWeaponMgr()->GetCurrentClientWeapon()->HasAmmo())return;

					g_pGameClientShell->GetPlayerMgr()->GetPlayerCamera()->SetCameraRotation( rNewRot );		
						
					SFMpos = BonePos;
					SFMpos.x -= 15;
																		
					if ( pmenu[0][FEAR.m_pMenu->GetIndex(0,"AutoShoot")].Setting )
					{ 
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
}

bool Aimbot::bIsDeathMatch(CLIENT_INFO* MyPlayer)
{
	if (MyPlayer)
	{
		if (MyPlayer->nTeamID == 255)
			return true;
		else 
			return false;
	}

	return false;
}

