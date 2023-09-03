#include "sdk.h"

/* 
	All this shit is, is just a bunch of functions that
	I added from the SDK to get rid of the unresolved
	externals										
														*/


CGameClientShell * g_pGameClientShell;
CWeaponDB * g_pWeaponDB;
IDatabaseMgr * g_pLTDatabase;
CInterfaceResMgr * g_pInterfaceResMgr;
ILTServer * g_pLTServer;
CInterfaceMgr * g_pInterfaceMgr;
ILTClient * g_pLTClient;
ILTCommon * g_pCommonLT;
ILTCSBase * g_pLTBase;
CSurfaceDB * g_pSurfaceDB;
ModelsDB*			g_pModelsDB;
//CWeaponDB*			g_pWeaponDB;
CPlayerMgr*			g_pPlayerMgr;
CPlayerStats*		g_pPlayerStats;
ILTModel*			g_pModelLT;
CStringEdit * g_pLTDBStringEdit;
IStringEditMgr * g_pLTIStringEdit;
IDatabaseCreatorMgr * g_pLTDatabaseCreator;

const char* const WDB_WeaponData_Default	= "Default";
const char* const WDB_WeaponData_Multi		= "MultiPlayer";
const char* const WDB_WeaponData_AI			= "AI";
const char* const WDB_AmmoData_Default		= "Player";
const char* const WDB_AmmoData_Multi		= "MultiPlayer";
const char* const WDB_AmmoData_AI			= "AI";

const uint16 g_BandwidthServer[eBandwidth_Custom] =
{
	//	30,			// eBandwidth_56K
	128,		// eBandwidth_DSL_Low
		256,		// eBandwidth_DSL_High
		256,		// eBandwidth_Cable
		1500,		// eBandwidth_T1
		10000		// eBandwidth_LAN
};

CCharacterFX* CSFXMgr::GetCharacterFX(HOBJECT hObject)
{
	CCharacterFX* pCharacterFX = NULL;
	int cCharacterFX  = m_dynSFXLists[SFX_CHARACTER_ID].GetSize();

	for ( int iCharacterFX = 0 ; iCharacterFX < cCharacterFX ; iCharacterFX++ )
	{
		pCharacterFX = (CCharacterFX*)m_dynSFXLists[SFX_CHARACTER_ID][iCharacterFX];
		if (pCharacterFX && pCharacterFX->GetServerObj() == hObject)
		{
			return pCharacterFX;
		}
	}

	return NULL;
}

void CPlayerMgr::Teleport(const LTVector & vPos)
{
	//#ifndef _FINAL // Don't allow player teleporing in the final version...

	CAutoMessage cMsg;
	cMsg.Writeuint8(MID_PLAYER_TELEPORT);
	cMsg.WriteLTVector(vPos);
	cMsg.WriteLTRotation(m_pPlayerCamera->GetCameraRotation( ));
	g_pLTClient->SendToServer(cMsg.Read(), MESSAGE_GUARANTEED);

	//#endif // _FINAL
}


CClientWeapon *CClientWeaponMgr::GetCurrentClientWeapon() const
{
	if ( (-1) == m_nCurClientWeaponIndex )
	{
		return 0;
	}

	// we should always have a current weapon
	ASSERT( CWM_WEAPON_INDEX_IS_VALID( m_nCurClientWeaponIndex ) );
	ASSERT( m_pCurrentWeapon == m_apClientWeapon[ m_nCurClientWeaponIndex ] );

	return m_pCurrentWeapon;
}

bool IsMultiplayerGameServer()
{
	// If we don't have a server (like when being run from worldedit), then assume no mp.
	if( !g_pLTServer )
		return false;

	return g_pLTServer->IsMultiplayerExe();
}


HWEAPONDATA CWeaponDB::GetWeaponData(HWEAPON hWeapon, bool bUseAIStats)
{
	if (!hWeapon) return NULL;

	if (bUseAIStats)
	{
		HWEAPONDATA hOver = GetRecordLink(hWeapon, WDB_WeaponData_AI );
		if (hOver)
			return hOver;
	}

#ifdef _CLIENTBUILD
	if (IsMultiplayerGameClient())
#else
	if (IsMultiplayerGameServer())
#endif
	{
		HWEAPONDATA hOver = GetRecordLink(hWeapon, WDB_WeaponData_Multi );
		if (hOver)
			return hOver;
	}

	return GetRecordLink(hWeapon, WDB_WeaponData_Default );
}





bool CInterfaceMgr::IsInGame( )
{
	switch( GetGameState( ))
	{
	case GS_PLAYING:
	case GS_EXITINGLEVEL:
		return true;
	default:
		return false;
	}
}

void CInterfaceResMgr::ScreenDimsChanged()
{
	if (!g_pLTClient) return;

	RMode currentMode;
	g_pLTClient->GetRenderMode(&currentMode);

	m_vfScale.x = (float)currentMode.m_Width / 640.0f;
	m_vfScale.y = (float)currentMode.m_Height / 480.0f;

	//if the resolution aspect ratio is closer to 16x9 than 14x3, the resolution is considered widescreen
	if (currentMode.m_Height > 0.0f)
	{
		float fAspectRatio = (float)currentMode.m_Width/(float)currentMode.m_Height;
		float fWidescreenLimit = 14.0f/9.0f; //halfway between 4x3 and 16x9
		m_bWidescreen = (fAspectRatio >= fWidescreenLimit);
	}

	m_dwScreenWidth = currentMode.m_Width;
	m_dwScreenHeight = currentMode.m_Height;

}

void CMenuMgr::ScreenDimsChanged()
{
	MenuArray::iterator iter = m_MenuArray.begin();
	while (iter != m_MenuArray.end())
	{
		(*iter)->SetScale(g_pInterfaceResMgr->GetScreenScale());
		iter++;
	}
}

LTVector CInterfaceMgr::GetScreenFromWorldPos(const LTVector& vPos, HOBJECT hCamera, bool& bOnScreen)
{
	LTVector vScreenPos;

	LTRESULT res = g_pLTClient->GetRenderer()->WorldPosToScreenPos(hCamera, vPos, vScreenPos);

	bOnScreen = (LT_OK == res && vScreenPos.x >= 0.0f && vScreenPos.x <= 1.0f 
		&& vScreenPos.y >= 0.0f && vScreenPos.y <= 1.0f
		&& vScreenPos.z > 0.0f);

	//scale based upon the physical extents of the screen

	vScreenPos.x *= (float)g_pInterfaceResMgr->GetScreenWidth();
	vScreenPos.y *= (float)g_pInterfaceResMgr->GetScreenHeight();


	return vScreenPos;
}



namespace
{
	wchar_t wszUnknownPlayer[32] = L"";
	const int kNumTeams = 2;
}

CLIENT_INFO* CClientInfoMgr::GetClientByID(uint32 nID, bool bUpdateOnFailure)
{
	if( nID == INVALID_CLIENT )
		return NULL;

	CLIENT_INFO* ptr = m_pClients;
	while (ptr)
	{
		if (ptr->nID == nID)
			return ptr;

		ptr = ptr->pNext;
	}
	if (bUpdateOnFailure)
		g_pInterfaceMgr->UpdateClientList();
	return NULL;
}

bool IsMultiplayerGameClient()
{
	if( !g_pLTClient )
		return false;

	return g_pLTClient->IsMultiplayerExe( );
}

void DebugCPrint(int nLevel, const char* szFormat, ...)
{
	if (!g_pLTBase)
		return;

	if (nLevel > GetConsoleInt("DebugLevel",0))
		return;

	static char szBuffer[1024];
	va_list val;
	va_start(val, szFormat);
	LTVSNPrintF(szBuffer, LTARRAYSIZE(szBuffer), szFormat, val);
	va_end(val);
	g_pLTBase->CPrintNoArgs(szBuffer);

}

int GetConsoleInt(char* sKey, int nDefault)
{
	if (g_pLTServer)
	{
		HCONSOLEVAR hVar = g_pLTServer->GetConsoleVariable(sKey);
		if (hVar)
		{
			float fValue = g_pLTServer->GetConsoleVariableFloat(hVar);
			return((int)fValue);
		}
	}

	return(nDefault);
}

LTRESULT SendEmptyServerMsg(uint32 nMsgID, uint32 nFlags)
{
	LTRESULT nResult;

	CAutoMessage cMsg;

	cMsg.Writeuint8(nMsgID);

	nResult = g_pLTClient->SendToServer(cMsg.Read(), nFlags);

	return nResult;
}

int GetConsoleInt(char const* sKey, int nDefault)
{
	if (g_pLTClient)
	{
		HCONSOLEVAR hVar = g_pLTClient->GetConsoleVariable(( char* )sKey);
		if (hVar)
		{
			float fValue = g_pLTClient->GetConsoleVariableFloat(hVar);
			return((int)fValue);
		}
	}

	return(nDefault);
}

void CInterfaceMgr::ScreenDimsChanged()
{
	m_InterfaceResMgr.ScreenDimsChanged();
	GetScreenMgr( )->ScreenDimsChanged();
	m_UserMenuMgr.ScreenDimsChanged();
	m_SystemMenuMgr.ScreenDimsChanged();
	GetHUDMgr()->ScreenDimsChanged();

	g_pLTClient->SetCameraRect (m_hInterfaceCamera, g_pInterfaceMgr->GetViewportRect());

	UpdateInterfaceBackground();

}

void CInterfaceMgr::UpdateInterfaceBackground()
{
}

LTRect2f CInterfaceMgr::GetViewportRect()
{
	// Adjusts the viewport dimenstions for certain video modes.
	// GetDefaultFOVAspectRatioScale for the corresponding FOV aspect changes.

#ifdef PROJECT_DARK
#ifdef PLATFORM_XENON
	if( !(XGetVideoFlags() & XC_VIDEO_FLAGS_WIDESCREEN) )	
		return LTRect2f(0.0f, 0.125f, 1.0f, 0.875f);		// add letter-box to non-widescreen modes
	else
		return LTRect2f(0.0f, 0.0f, 1.0f, 1.0f);			// don't letterbox widescreen mode
#else
	return LTRect2f(0.0f, 0.125f, 1.0f, 0.875f);			// allows LDs to preview how it will look on xenon
#endif
#endif
	return LTRect2f(0.0f, 0.0f, 1.0f, 1.0f);				// normal PC viewport
}



void CInterfaceMgr::UpdateClientList()
{
	if (IsMultiplayerGameClient())
	{
		// Don't send update requests more than once per second
		double fCurTime = RealTimeTimer::Instance( ).GetTimerAccumulatedS( );
		if (( fCurTime - m_fLastUpdateRequestTime) > 1.0f)
		{
			DebugCPrint(2,"Asking for clientlist update");
			SendEmptyServerMsg(MID_MULTIPLAYER_UPDATE, MESSAGE_GUARANTEED);
			m_fLastUpdateRequestTime = fCurTime;
		}
	}
}

const wchar_t* CClientInfoMgr::GetPlayerName (uint32 nID)
{
	if (!m_pClients) return NULL;

	CLIENT_INFO* ptr = GetClientByID(nID);
	if (ptr)
		return ptr->sName.c_str( );

	return wszUnknownPlayer;
}
void WriteConsoleInt(char const* sKey, int nValue)
{
	if (g_pLTServer)
	{
		g_pLTServer->SetConsoleVariableFloat(sKey, (float)nValue);
	}
}
void CCursorMgr::UseCursor(bool bUseCursor, bool bLockCursorToCenter)
{
	m_bUseCursor = bUseCursor;

	// New hardware code:
	// if the cursor is visible and being used
	if (m_bUseCursor && m_bUseHardwareCursor)
	{
		g_pLTClient->Cursor()->SetCursorMode(CM_Hardware);
		// copied the following 4 lines from Init()
		if (g_pLTClient->Cursor()->SetCursor(m_hCursor) != LT_OK)
		{
			DebugCPrint(1,"can't set cursor.");
		}
	}
	else
	{
		g_pLTClient->Cursor()->SetCursorMode(CM_None);

	}

	// Lock or don't lock the cursor to the center of the screen
	if(bLockCursorToCenter)
	{
		g_pLTClient->SetConsoleVariableFloat("CursorCenter", 1.0f);
	}
	else
	{
		g_pLTClient->SetConsoleVariableFloat("CursorCenter", 0.0f);
	}
}
void CClientWeapon::SendFireMessage( float fPerturb, 
									LTVector const &vFirePos,
									LTVector const &vDir )
{
	CAutoMessage cMsg;
	LTRESULT msgResult;

	cMsg.Writeuint8( MID_WEAPON_FIRE );

	// The server knows the weapon the client is currently using so that doesn't
	// need to be sent.  Grenades and melee are handled differently, so they will be sent...

	bool bSendWeaponRecord = false;
	HWEAPONDATA hWeaponData = g_pWeaponDB->GetWeaponData( m_hWeapon, !USE_AI_DATA );
	if( g_pWeaponDB->GetBool( hWeaponData, WDB_WEAPON_bIsGrenade ) || (m_hWeapon == g_pWeaponDB->GetUnarmedRecord( )) )
		bSendWeaponRecord = true;

	cMsg.Writebool( bSendWeaponRecord );
	if( bSendWeaponRecord )
	{
		// Record of the weapon that is firing
		cMsg.WriteDatabaseRecord( g_pLTDatabase, m_hWeapon );
	}

	// The ammo should only be required to be sent if the weapon has multiple ammo types...
	bool bSendAmmoRecord = false;
	if( g_pWeaponDB->GetNumValues( hWeaponData, WDB_WEAPON_rAmmoName ) > 1 )
		bSendAmmoRecord = true;

	cMsg.Writebool( bSendAmmoRecord );
	if( bSendAmmoRecord )
	{
		// Record of the ammo that is firing
		cMsg.WriteDatabaseRecord( g_pLTDatabase, m_hAmmo );
	}

	// weapon fire position (point where the bullets come from)
	cMsg.WriteLTVector( vFirePos );

	// vector pointing in the direction of travel
	cMsg.WriteLTVector( vDir );

	// random seed
	cMsg.Writeuint8( m_nRandomSeed );

	//perturb count
	cMsg.Writeuint8( m_nPerturbCount );


	// perturb (random range from true center that bullet can travel)
	cMsg.Writeuint8( static_cast< uint8 >( fPerturb * 255.0f ) );

	// time the weapon fired, in milliseconds
	uint32 nFireTime = g_pGameClientShell->GetServerRealTimeMS( );
	cMsg.Writeuint32( nFireTime );

	//which hand did the firing... needed to sync animation and FX
	cMsg.Writebool(m_bFireLeftHand);

	// send the message
	msgResult = g_pLTClient->SendToServer( cMsg.Read(), MESSAGE_GUARANTEED );
	ASSERT( LT_OK == msgResult );

#ifndef _FINAL
	if (g_vtEnableSimulationLog.GetFloat())
	{
		WriteFireMessageToSimulationLog(vFirePos, vDir, m_nRandomSeed, m_nPerturbCount, fPerturb, nFireTime);
	}
#endif

}
uint8 CClientInfoMgr::GetPlayerTeam(uint32 nID)
{
	if (!m_pClients) return INVALID_TEAM;
	if (!GameModeMgr::Instance( ).m_grbUseTeams) return INVALID_TEAM;

	CLIENT_INFO* ptr = GetClientByID(nID);
	if (ptr)
		return ptr->nTeamID;

	return INVALID_TEAM;
}

uint8 CClientInfoMgr::GetLocalTeam()
{
	uint8 nTeam = INVALID_TEAM;
	const CLIENT_INFO* pLocal = GetLocalClient();
	if (pLocal)
		nTeam = pLocal->nTeamID;
	return nTeam;
}
CLIENT_INFO* CClientInfoMgr::GetLocalClient()
{
	return GetClientByID(m_nLocalID);
}
GameModeMgr& GameModeMgr::Instance()
{
	if (GetCurExecutionShellContext() == eExecutionShellContext_Client)
	{
		static GameModeMgr instance;
		return instance;
	}
	else
	{
		static GameModeMgr instance;
		return instance;
	}
}
GameModeMgr::GameModeMgr( )
{
	m_hGameModeRecord = NULL;
	m_bBrowserDirty = false;

	m_grfRunSpeed.Init( "RunSpeed", 1.0f );
	m_grwsSessionName.Init( "SessionName" );
	m_grwsBriefingStringId.Init( "BriefingStringId" );
	m_grbFriendlyFire.Init( "FriendlyFire", false );
	m_grbWeaponsStay.Init( "WeaponsStay", false );
	m_grbUseLoadout.Init( "UseLoadout", true );
	m_grfTeamReflectDamage.Init( "TeamReflectDamage", 0.0f );
	m_grfTeamDamagePercent.Init( "TeamDamagePercent", 1.0f );
	m_grnScoreLimit.Init( "ScoreLimit", 0 );
	m_grnTimeLimit.Init( "TimeLimit", 0 );
	m_grnSuddenDeathTimeLimit.Init( "SuddenDeathTimeLimit", 0 );
	m_grnMaxWeapons.Init( "MaxWeapons", 2 );
	m_grnNumRounds.Init( "NumRounds", 1 );
	m_grnFragScorePlayer.Init( "FragScorePlayer", 1 );
	m_grnFragScoreTeam.Init( "FragScoreTeam", 1 );
	m_grnDeathScorePlayer.Init( "DeathScorePlayer", 0 );
	m_grnDeathScoreTeam.Init( "DeathScoreTeam", 0 );
	m_grnTKScore.Init( "TeamKillScore", 0 );
	m_grnSuicideScorePlayer.Init( "SuicideScorePlayer", 0 );
	m_grnSuicideScoreTeam.Init( "SuicideScoreTeam", 0 );
	m_grnMaxPlayers.Init( "MaxPlayers", MAX_MULTI_PLAYERS );
	m_grbUseTeams.Init( "UseTeams", true );
	m_grbSwitchTeamsBetweenRounds.Init( "SwitchTeamsBetweenRounds", false );
	m_grnRespawnWaitTime.Init( "RespawnWaitTime", 8 );
	m_grbUseRespawnWaves.Init( "UseRespawnWaves", false );
	m_grnTeamKillRespawnPenalty.Init( "TeamKillRespawnPenalty", 0 );
	m_grbAccumulateRespawnPenalty.Init( "AccumulateRespawnPenalty", false );
	m_grbUseWeaponRestrictions.Init( "UseWeaponRestrictions", false );
	m_greSpawnPointSelect.Init( "SpawnPointSelect", "First" );
	m_grbUsesDifficulty.Init( "UsesDifficulty", true );
	m_grbAllowRespawnFromDeath.Init( "AllowRespawnFromDeath", true );
	m_grbEliminationWin.Init( "EliminationWin", false );
	m_grnJoinGracePeriod.Init( "JoinGracePeriod", 0 );
	m_grsRestrictedWeapons.Init( "RestrictedWeapons" );
	m_grsRestrictedGear.Init( "RestrictedGear" );
	m_grbUseSlowMo.Init( "UseSlowMo", false );
	m_grbSlowMoPersistsAcrossDeath.Init( "SlowMoPersistsAcrossDeath", false );
	m_grbSlowMoRespawnAfterUse.Init( "SlowMoRespawnAfterUse", false );
	m_grbSlowMoNavMarker.Init( "SlowMoNavMarker", false );
	m_grnSlowMoHoldScorePeriod.Init( "SlowMoHoldScorePeriod", 0 );
	m_grnSlowMoHoldScorePlayer.Init( "SlowMoHoldScorePlayer", 0 );
	m_grnSlowMoHoldScoreTeam.Init( "SlowMoHoldScoreTeam", 0 );
	m_grsBroadcastSet.Init( "BroadcastSet" );
	m_grbAllowSpectatorToLiveChatting.Init( "AllowSpectatorToLiveChatting", true );
	m_grbAllowDeadVoting.Init( "AllowDeadVoting", true );

	m_grsCTFRules.Init( "CTFRules" );
	m_grnCTFDefendFlagBaseScorePlayer.Init( "CTFDefendFlagBaseScorePlayer" );
	m_grnCTFDefendFlagBaseScoreTeam.Init( "CTFDefendFlagBaseScoreTeam" );
	m_grnCTFDefendFlagBaseRadius.Init( "CTFDefendFlagBaseRadius" );
	m_grnCTFDefendFlagCarrierScorePlayer.Init( "CTFDefendFlagCarrierScorePlayer" );
	m_grnCTFDefendFlagCarrierScoreTeam.Init( "CTFDefendFlagCarrierScoreTeam" );
	m_grnCTFDefendFlagCarrierRadius.Init( "CTFDefendFlagCarrierRadius" );
	m_grnCTFKillFlagCarrierScorePlayer.Init( "CTFKillFlagCarrierScorePlayer" );
	m_grnCTFKillFlagCarrierScoreTeam.Init( "CTFKillFlagCarrierScoreTeam" );
	m_grnCTFTeamKillFlagCarrierPenalty.Init( "CTFTeamKillFlagCarrierPenalty" );
	m_grnCTFReturnFlagScorePlayer.Init( "CTFReturnFlagScorePlayer" );
	m_grnCTFReturnFlagScoreTeam.Init( "CTFReturnFlagScoreTeam" );
	m_grnCTFStealFlagScorePlayer.Init( "CTFStealFlagScorePlayer" );
	m_grnCTFStealFlagScoreTeam.Init( "CTFStealFlagScoreTeam" );
	m_grnCTFPickupFlagScorePlayer.Init( "CTFPickupFlagScorePlayer" );
	m_grnCTFPickupFlagScoreTeam.Init( "CTFPickupFlagScoreTeam" );
	m_grnCTFCaptureFlagScorePlayer.Init( "CTFCaptureFlagScorePlayer" );
	m_grnCTFCaptureFlagScoreTeam.Init( "CTFCaptureFlagScoreTeam" );
	m_grfCTFCaptureAssistTimeout.Init( "CTFCaptureAssistTimeout" );
	m_grnCTFCaptureAssistScorePlayer.Init( "CTFCaptureAssistScorePlayer" );
	m_grnCTFCaptureAssistScoreTeam.Init( "CTFCaptureAssistScoreTeam" );
	m_grfCTFFlagLooseTimeout.Init( "CTFFlagLooseTimeout" );
	m_grfCTFFlagMovementLimit.Init( "CTFFlagMovementLimit" );
	m_grsRequiredMapFeatures.Init( "RequiredMapFeatures" );

	m_grsCPRules.Init( "CPRules" );
	m_grfCPCapturingTime.Init( "CPCapturingTime" );
	m_grfCPGroupCaptureFactor.Init( "CPGroupCaptureFactor" );
	m_grnCPDefendScorePlayer.Init( "CPDefendScorePlayer" );
	m_grnCPDefendScoreTeam.Init( "CPDefendScoreTeam" );
	m_grnCPDefendRadius.Init( "CPDefendRadius" );
	m_grnCPOwnedScoreAmountTeam.Init( "CPOwnedScoreAmountTeam" );
	m_grfCPOwnedScorePeriod.Init( "CPOwnedScorePeriod" );
	m_grnCPScoreLoseTeam.Init( "CPScoreLoseTeam" );
	m_grnCPScoreNeutralizeTeam.Init( "CPScoreNeutralizeTeam" );
	m_grnCPScoreCaptureTeam.Init( "CPScoreCaptureTeam" );
	m_grnCPScoreNeutralizePlayer.Init( "CPScoreNeutralizePlayer" );
	m_grnCPScoreCapturePlayer.Init( "CPScoreCapturePlayer" );
	m_grbCPConquestWin.Init( "CPConquestWin" );

	m_grsHUDTeamScoreLayout.Init("HUDTeamScoreLayout");

	m_greTeamSizeBalancing.Init( "TeamSizeBalancing", "Never" );
	m_greTeamScoreBalancing.Init( "TeamScoreBalancing", "Never" );
	m_grfTeamScoreBalancingPercent.Init( "TeamScoreBalancingPercent", 2.0f );
	m_grfEndRoundMessageTime.Init( "EndRoundMessageTime", 5.0f );
	m_grfEndRoundScoreScreenTime.Init( "EndRoundScoreScreenTime", 10.0f );

	m_grbAllowKillerTrackSpectating.Init( "AllowKillerTrackSpectating", true );

}

GameModeMgr::~GameModeMgr( )
{
}
HATTRIBUTE GameRule::GetStruct() const
{
	return g_pLTDatabase->GetAttribute( GameModeMgr::Instance().GetGameModeRecord(), GetAttribName( ));
}
void GameRule::SetDirty( bool bDirty ) 
{ 
	m_bDirty = bDirty; 
	if( m_bDirty )
		GameModeMgr::Instance().SetBrowserDirty( true );
}
void GameRuleBool::ToString( wchar_t* pszString, uint32 nStringLen, bool bLocalized ) const 
{ 
	if( bLocalized )
	{
		LTStrCpy( pszString, LoadString( g_pLTDatabase->GetString( CGameDatabaseReader::GetStructAttribute( 
			GetStruct( ), 0, m_bValue ? "TrueString" : "FalseString" ), 0, "" )), nStringLen );
	}
	else
	{
		LTSNPrintF( pszString, nStringLen, L"%d", m_bValue ? 1 : 0 ); 
	}
}

void GameRuleBool::FromString( wchar_t const* pszString, bool bLocalized ) 
{ 
	if( bLocalized )
	{
		m_bValue = LTStrEquals( pszString, LoadString( g_pLTDatabase->GetString( 
			CGameDatabaseReader::GetStructAttribute( GetStruct( ), 0, "TrueString" ), 0, "" )));
	}
	else
	{
		m_bValue = ( LTStrToLong( pszString ) != 0 ) ? true : false;
	}
	SetDirty( true );
}

void GameRuleEnum::ToString( wchar_t* pszString, uint32 nStringLen, bool bLocalized ) const 
{ 
	if( bLocalized )
	{
		// Get the struct of values.
		HATTRIBUTE hValues = CGameDatabaseReader::GetStructAttribute( GetStruct( ), 0, "Values" );
		if( !hValues )
		{
			LTStrCpy( pszString, L"", nStringLen );
			return;
		}

		// Iterate through the values and find the label that matches our current value.
		uint32 nNumValues = g_pLTDatabase->GetNumValues( hValues );
		for( uint32 nIndex = 0; nIndex < nNumValues; nIndex++ )
		{
			char const* pszLabel = g_pLTDatabase->GetString( 
				CGameDatabaseReader::GetStructAttribute( hValues, nIndex, "Label" ), 0, "" );
			if( LTStrEquals( *this, pszLabel ))
			{
				// Return the localize string.
				HATTRIBUTE hString = CGameDatabaseReader::GetStructAttribute( hValues, nIndex, "String" );
				LTStrCpy( pszString, LoadString( g_pLTDatabase->GetString( hString, 0, "" )), nStringLen );
				break;
			}
		}
	}
	else
	{
		// Return the current value label.
		GameRuleString::ToString( pszString, nStringLen, bLocalized );
	}
}

void GameRuleEnum::FromString( wchar_t const* pszString, bool bLocalized ) 
{ 
	if( bLocalized )
	{
		// Get the struct of values.
		HATTRIBUTE hValues = CGameDatabaseReader::GetStructAttribute( GetStruct( ), 0, "Values" );
		if( !hValues )
		{
			return;
		}

		// Iterate through the values and find the localized string that matches the input value.
		uint32 nNumValues = g_pLTDatabase->GetNumValues( hValues );
		for( uint32 nIndex = 0; nIndex < nNumValues; nIndex++ )
		{
			char const* pszStringId = g_pLTDatabase->GetString( 
				CGameDatabaseReader::GetStructAttribute( hValues, nIndex, "String" ), 0, "" );
			if( LTStrEquals( pszString, LoadString( pszStringId )))
			{
				// Match found, set our current value to the value label.
				HATTRIBUTE hLabel = CGameDatabaseReader::GetStructAttribute( hValues, nIndex, "Label" );
				GameRuleString::FromString( MPA2W( g_pLTDatabase->GetString( hLabel, 0, "" )).c_str( ), bLocalized );
				break;
			}
		}
	}
	else
	{
		// Set our value label.
		GameRuleString::FromString( pszString, bLocalized );
	}
}
void GameRuleFloat::ToString( wchar_t* pszString, uint32 nStringLen, bool bLocalized ) const 
{ 
	uint32 nPrecision = g_pLTDatabase->GetInt32( CGameDatabaseReader::GetStructAttribute( GetStruct( ), 0, "Precision" ), 0, 0 );
	LTSNPrintF( pszString, nStringLen, L"%.*f", nPrecision, m_fValue ); 
}
CGameDatabaseMgr::CGameDatabaseMgr()
:	m_sDatabaseFile		( ),
m_hDatabase			( NULL )
{
}
ServerSettings::ServerSettings( )
{
#if !defined(PLATFORM_LINUX)
	Clear( );
#endif
}
CGameDatabaseReader::CGameDatabaseReader()
{
	LoadDatabaseInterface( );
}
bool CategoryDB::Init( char const* pszCategory, char const* pszDatabaseFile )
{
	if( !OpenDatabase( pszDatabaseFile ))
		return false;

	// Get handles to all of the category in the database.
	m_hCategory = g_pLTDatabase->GetCategory(m_hDatabase, pszCategory);
	if( !m_hCategory )
		return false;

	return true;
}
void CategoryDB::Term( )
{
	m_hCategory = NULL;
}
void ServerSettings::Clear( )
{
	m_sGameMode = GameModeMgr::GetSinglePlayerRecordName( );

	m_sServerMessage = L"";
	m_sBriefingOverrideMessage = L"";
	m_sPassword = L"";
	m_bUsePassword = false;
	m_sScmdPassword = L"";
	m_bAllowScmdCommands = false;

	m_bDedicated = false;
	m_bLANOnly = false;
	m_nPort = DEFAULT_PORT;
	m_sBindToAddr = L"";
	m_nBandwidthServer = 3;
	m_nBandwidthServerCustom = g_BandwidthServer[m_nBandwidthServer];
	m_bAllowContentDownload = true;
	m_nMaxDownloadRatePerClient = 40960;
	m_nMaxDownloadRateAllClients = GetMaxPlayersForBandwidth() * m_nMaxDownloadRatePerClient;
	m_nMaxSimultaneousDownloads = GetMaxPlayersForBandwidth();
	m_nMaxDownloadSize = (uint32)-1;
	m_sRedirectURLs.clear();
	m_sContentDownloadMessage = L"";
	m_bEnableScoringLog = false;
	m_nMaxScoringLogFileAge = 0;

	for (uint8 i =0; i < kNumVoteTypes; ++i)
	{
		m_bAllowVote[i] = true;
	}

	m_nMinPlayersForVote = 5;
	m_nMinPlayersForTeamVote = 3;
	m_nVoteLifetime = 30;		//seconds
	m_nVoteBanDuration = 60;	//minutes
	m_nVoteDelay = 0;		//seconds
	m_bUsePunkbuster = false;
}
bool CGameDatabaseMgr::OpenDatabase( const char *szDatabaseFile, bool bInRezTree /* = true  */ )
{
	if( !g_pLTDatabase || !szDatabaseFile )
		return false;

	// Check if we already have one open.
	if( m_hDatabase )
	{
		// Check if it's the same database.
		if( LTStrIEquals( m_sDatabaseFile.c_str( ), szDatabaseFile ))
		{
			return true;
		}
		// Different database, we'll need to toss the old one.
		else
		{
			g_pLTDatabase->ReleaseDatabase( m_hDatabase );
			m_hDatabase = NULL;
		}
	}

	m_sDatabaseFile = szDatabaseFile;

	//try and open an existing database first
	m_hDatabase = g_pLTDatabase->OpenExistingDatabase(szDatabaseFile);
	if(m_hDatabase)
		return true;

	// Open the file... 
	if( g_pLTBase && bInRezTree)
	{
		// If we have a base interface we know we are running the game and thus need to 
		// open the database file through a stream since its in a REZ file...

		ILTInStream	*pDBStream = g_pLTBase->FileMgr()->OpenFile( szDatabaseFile );
		if( !pDBStream )
		{
			g_pLTBase->CPrint( "ERROR CGameDatabaseMgr couldn't open file: %s!", szDatabaseFile );
			return false;
		}

		// Open the database...

		m_hDatabase = g_pLTDatabase->OpenNewDatabase( szDatabaseFile, *pDBStream );
		if( !m_hDatabase )
			return false;

		// Free up the stream...

		pDBStream->Release( );
	}
	else
	{
		// We are trying to open a database while not in game. (ie. WorldEdit)
		// So the database can be opened from the full path...

		CLTFileToILTInStream InFile;
		if(!InFile.Open(szDatabaseFile))
			return false;

		m_hDatabase = g_pLTDatabase->OpenNewDatabase( szDatabaseFile, InFile );
		if( !m_hDatabase )
			return false;
	}

	return true;
}
bool CWin32_LTFileRead::Read(void* const pBuffer, const uint32 nBytesToRead)
{
	LTASSERT(GetFileHandle() != INVALID_HANDLE_VALUE, "invalid file handle");

	// check to see if we have enough in the buffer to satisfy the request
	uint32 nRemainingBytesInBuffer = GetRemainingBytesInBuffer();
	if (nRemainingBytesInBuffer >= nBytesToRead)
	{
		// get the data from the buffer
		GetDataFromBuffer(pBuffer, nBytesToRead);
	}
	else
	{
		// we need more than what's currently available in the buffer to satisfy
		// this request.  First, copy the remaining data (if any) from the read buffer
		// to the user buffer
		if (nRemainingBytesInBuffer)
		{
			if (!GetDataFromBuffer(pBuffer, nRemainingBytesInBuffer))
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}
		}

		// calculate the amount left to read
		uint32 nAmountRemainingToRead = nBytesToRead - nRemainingBytesInBuffer;

		// if the amount we need to read is more than half of our buffer allocation size,
		// then read it directly into the user buffer
		if (nAmountRemainingToRead >= (k_nWin32FileBufferSize / 2))
		{
			uint32 nAmountReadFromFile = 0;
			if (!ReadFromFile((uint8*)pBuffer + (nBytesToRead - nAmountRemainingToRead), nAmountRemainingToRead, nAmountReadFromFile))
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}

			// check to see if we read the full amount
			if (nAmountReadFromFile != nAmountRemainingToRead)
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}
		}
		else
		{
			// perform a full read into the internal buffer
			if (!FillBufferFromFile())
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}

			// check to see if the file actually had enough data remaining to satisfy the request
			if (m_nBufferAmount < nAmountRemainingToRead)
			{
				// not enough left in the file
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}

			// get the remaining amount from the buffer
			if (!GetDataFromBuffer((uint8*)pBuffer + (nBytesToRead - nAmountRemainingToRead), nAmountRemainingToRead))
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}
		}
	}

	// success
	return true;
}
static uint32	 s_nGameDatabaseRef	= 0;
static HLTMODULE s_hDatabaseInst		= NULL;
bool LoadDatabaseInterface( const char *pszDllFile /* = GDB_DLL_NAME  */ )
{
	// Increase the ref...

	++s_nGameDatabaseRef;

	if ( !g_pLTDatabase || !g_pLTDatabaseCreator )
	{
#if defined(PLATFORM_SEM)
		g_pLTDatabase = GetIDatabaseMgr();
		g_pLTDatabaseCreator = GetIDatabaseCreatorMgr();
#else // PLATFORM_SEM
		// The database was not setup yet.  Do so now...
		if( !s_hDatabaseInst && pszDllFile )
			s_hDatabaseInst = LTLibraryLoader::OpenLibrary( pszDllFile );

		if( s_hDatabaseInst )
		{
			// Get the databse interface function...

			HLTPROC hProc = LTLibraryLoader::GetProcAddress( s_hDatabaseInst, "GetIDatabaseMgr" );

			LTASSERT( hProc != NULL, "Unable to retrieve the DatabaseMgr function!" );
			if( hProc )
			{
				TGetIDatabaseMgrFn DBfn = (TGetIDatabaseMgrFn)hProc;
				g_pLTDatabase = DBfn();
			}
			else
			{
				FreeDatabaseInterface( );
			}

			// Get the database creator interface function...
			hProc = LTLibraryLoader::GetProcAddress( s_hDatabaseInst, "GetIDatabaseCreatorMgr" );

			LTASSERT( hProc != NULL, "Unable to retrieve the DatabaseCreatorMgr function!" );
			if( hProc )
			{
				TGetIDatabaseCreatorMgrFn DBfn = (TGetIDatabaseCreatorMgrFn)hProc;
				g_pLTDatabaseCreator = DBfn();
			}
			else
			{
				FreeDatabaseInterface( );
			}

		}
#endif // PLATFORM_SEM
	}

	return (g_pLTDatabase != NULL && g_pLTDatabaseCreator != NULL);
}
void FreeDatabaseInterface( )
{

	// Decrease the ref...
	--s_nGameDatabaseRef;
	LTASSERT( s_nGameDatabaseRef >= 0, "Freed more gamedatabases than created!" );

	if( s_nGameDatabaseRef == 0 )
	{
		// No more refs so free it...

		if( s_hDatabaseInst )
		{
			LTLibraryLoader::CloseLibrary( s_hDatabaseInst );
		}

		s_hDatabaseInst		= NULL;
		g_pLTDatabase		= NULL;
		g_pLTDatabaseCreator = NULL;

	}
}
HLTPROC LTLibraryLoader::GetProcAddress(HLTMODULE hModule, const char* pszProcName)
{
	return ::GetProcAddress((HMODULE)hModule, pszProcName);
}
HLTMODULE LTLibraryLoader::OpenLibrary(const char* pszFileName)
{
	return (HLTMODULE)::LoadLibrary(pszFileName);
}
static const uint32 knFileBufferStartSize  = 5;
static const uint32 knFileBufferGrowSize = 5;
static ObjectBank<CWin32_LTFileBuffer, LT_MEM_TYPE_MISC> g_FileBufferObjectBank(knFileBufferGrowSize, knFileBufferStartSize);
CLTCriticalSection	g_FileBufferObjectBankCS;
CWin32_LTFileBuffer* CWin32_LTFileBuffer::Allocate()
{
	//restrict access to prevent race conditions from corrupting our bank
	CLTAutoCriticalSection AutoCS(g_FileBufferObjectBankCS);

	// allocate a stream from the bank
	CWin32_LTFileBuffer* pFileBuffer = g_FileBufferObjectBank.Allocate();

	if(!pFileBuffer)
	{
		return NULL;
	}

	return pFileBuffer;
}
CGameDatabaseMgr::~CGameDatabaseMgr()
{
	// Clean up...

	if( m_hDatabase )
		g_pLTDatabase->ReleaseDatabase( m_hDatabase );
}
uint8 ServerSettings::GetMaxPlayersForBandwidth() const
{
	uint16 nBandwidth = 0;
	if (m_bLANOnly)
	{
		nBandwidth = g_BandwidthServer[eBandwidth_LAN];
	}
	else
	{
		if ( m_nBandwidthServer >= eBandwidth_Custom )
		{
			nBandwidth = m_nBandwidthServerCustom;
		}
		else
		{
			nBandwidth = g_BandwidthServer[m_nBandwidthServer];
		}
	}

	// Find the bandwidth in the table.  Keep upping the max players
	// until our bandwidth can't reach the bandwidth in the table entry.  Which
	// means we'll just use the player count from the previous entry.
	uint8 nMaxPlayers = 1;
	uint8 nIndex = 0;
	for( uint8 nIndex = 0; nIndex < g_BandwidthMaxPlayersSize; nIndex++ )
	{
		nMaxPlayers = g_BandwidthMaxPlayers[nIndex].m_nMaxPlayers;
		if( nBandwidth <= g_BandwidthMaxPlayers[nIndex].m_nBandwidth )
		{
			break;
		}
	}

	// Clamp to the hardcoded max.
	nMaxPlayers = LTMIN( nMaxPlayers, MAX_MULTI_PLAYERS );

	return (nMaxPlayers);
}
void sb_Init(StructBank *pBank, uint32 structSize, uint32 cacheSize)
{
	pBank->m_StructSize = structSize;

	pBank->m_AlignedStructSize = structSize;
	if(pBank->m_AlignedStructSize < sizeof(StructLink))
		pBank->m_AlignedStructSize = sizeof(StructLink);

	if(pBank->m_AlignedStructSize % 4 != 0)
	{
		pBank->m_AlignedStructSize += (4 - (pBank->m_AlignedStructSize % 4));
	}

	pBank->m_nPages = 0;
	pBank->m_nTotalObjects = 0;
	pBank->m_CacheSize = cacheSize;
	pBank->m_PageHead = NULL;
	pBank->m_FreeListHead = NULL;
}
void sb_Term(StructBank *pBank)
{
#ifndef _FINAL
	// Make sure everything is freed.
	uint32 freeListCount = sb_GetFreeObjectCount(pBank);
	LTASSERT(freeListCount == pBank->m_nTotalObjects, "Not all objects were properly freed from this struct or object bank");
#endif

	StructBankPage *pPage = pBank->m_PageHead;
	while(pPage)
	{
		StructBankPage *pNext = pPage->m_pNext;
		delete [] (uint8*)pPage;
		pPage = pNext;
	}

	pBank->m_PageHead = NULL;
	pBank->m_FreeListHead = NULL;
	pBank->m_nPages = 0;
	pBank->m_nTotalObjects = 0;
}
const BandwidthMaxPlayers g_BandwidthMaxPlayers[] = 
{
	{ 0, 1 },
	{ 32, 2 },
	{ 80, 3 },
	{ 128, 4 },
	{ 192, 5 },
	{ 256, 6 },
	{ 384, 7 },
	{ 512, 8 },
	{ 768, 12 },
	{ 1500, 16 },
};
const uint32 g_BandwidthMaxPlayersSize = LTARRAYSIZE( g_BandwidthMaxPlayers );
CGameDatabaseReader::~CGameDatabaseReader()
{
	// Clean up...

	FreeDatabaseInterface( );
}

bool sb_Init2(StructBank *pBank, uint32 structSize, uint32 cacheSize, uint32 nPreallocations)
{
	sb_Init(pBank, structSize, cacheSize);
	return sb_AllocateNewStructPage(pBank, nPreallocations);
}
void CWin32_LTFileBuffer::Release()
{
	//restrict access to prevent race conditions from corrupting our bank
	CLTAutoCriticalSection AutoCS(g_FileBufferObjectBankCS);

	g_FileBufferObjectBank.Free(this);
}
void LTLibraryLoader::CloseLibrary(const HLTMODULE hModule)
{
	::FreeLibrary((HMODULE)hModule);
}
bool sb_AllocateNewStructPage(StructBank *pBank, uint32 nAllocations)
{
	if(nAllocations == 0)
		return true;

	// Allocate a new page.
	uint32 nPageSize = (pBank->m_AlignedStructSize * nAllocations) + (sizeof(StructBankPage)-sizeof(uint32));
	StructBankPage *pPage = (StructBankPage*)(new uint8 [nPageSize]);
	if(!pPage)
		return false;

	pPage->m_pNext = pBank->m_PageHead;
	pPage->m_nObjects = nAllocations;

	pBank->m_PageHead = pPage;

	++pBank->m_nPages;
	pBank->m_nTotalObjects += nAllocations;

	// Put its contents into the free list.
	uint8 *pDataPos = (uint8*)pPage->m_Data;
	uint32 count = nAllocations;
	while(count--)
	{
		StructLink *pStruct = (StructLink*)pDataPos;
		pStruct->m_pSLNext = pBank->m_FreeListHead;
		pBank->m_FreeListHead = pStruct;
		pDataPos += pBank->m_AlignedStructSize;
	}

	return true;
}
GameRule::GameRuleList& GameRule::GetGameRuleList()
{
	static GameRuleList lstGameRuleList;

	return lstGameRuleList;
}
bool CClientInfoMgr::IsLocalTeam( uint8 nTeamID )
{
	if (!GameModeMgr::Instance( ).m_grbUseTeams) return false;
	uint8 nLocalTeam = INVALID_TEAM;
	const CLIENT_INFO* pLocal = GetLocalClient();
	if (pLocal)
		nLocalTeam = pLocal->nTeamID;
	return (nLocalTeam == nTeamID);
}
LTVector const& CPlayerCamera::GetCameraActivePos( )
{
	if (GetCameraMode() == kCM_Cinematic)
	{
		CSpecialFXList* pCameraList = g_pGameClientShell->GetSFXMgr()->GetCameraList();
		if( pCameraList )
		{
			int nNumCameras = pCameraList->GetSize();

			for( int nCamera = 0; nCamera < nNumCameras; ++nCamera )
			{
				CCameraFX* pCamFX = (CCameraFX*)(*pCameraList)[nCamera];
				if( !pCamFX )
					continue;

				HOBJECT hCamObj = pCamFX->GetServerObj();

				if( hCamObj )
				{
					uint32 dwUsrFlags;
					g_pCommonLT->GetObjectFlags( hCamObj, OFT_User, dwUsrFlags );

					if( dwUsrFlags & USRFLG_CAMERA_LIVE )
					{
						g_pLTClient->GetObjectPos(hCamObj, &m_vActivePos);
					}
				}
			}
		}
	}
	else
	{
		g_pLTClient->GetObjectPos( m_hCamera, &m_vActivePos );
	}

	return m_vActivePos;

}
HOBJECT	CMoveMgr::GetServerObject() const
{
	if (!GetCharacterFX())
		return NULL;
	return GetCharacterFX()->GetServerObj();
}