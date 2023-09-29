#include "main.h"

usermsgs_s g_UserMsgs;

int TeamInfo(const char *pszName,int iSize,void *pbuf)
{
	BEGIN_READ(pbuf,iSize);
	int iIndex=READ_BYTE();
	char *szTeam=READ_STRING();
	_strlwr(szTeam);
	if( !strcmp( szTeam, "terrorist" ) )
	{
		g_Player[iIndex].clrTeam=rgb(255,0,0);
		g_Player[iIndex].clrVisTeam=rgb(255,255,0);
		g_Player[iIndex].clrTeam2=rgba(255,0,0,125);
		g_Player[iIndex].iTeam = 1;
		if( iIndex == g_Local.iIndex ) { g_Local.iTeam = 1; }
	}
	else if( !strcmp( szTeam, "ct" ) )
	{
		g_Player[iIndex].clrTeam=rgb(0,0,255);
		g_Player[iIndex].clrVisTeam=rgb(0,255,0);
		g_Player[iIndex].clrTeam2=rgba(0,60,245,125);
		g_Player[iIndex].iTeam = 2;
		if( iIndex == g_Local.iIndex ) { g_Local.iTeam = 2; }
	}
	else
	{
		g_Player[iIndex].clrTeam=rgb(255,255,255);
		g_Player[iIndex].clrVisTeam=rgb(255,255,255);
		g_Player[iIndex].iTeam = 0;
		if( iIndex == g_Local.iIndex ) { g_Local.iTeam = 0; }
	}

	return g_UserMsgs.TeamInfo(pszName,iSize,pbuf);
}
int DeathMsg(const char *pszName,int iSize,void *pbuf)
{
	BEGIN_READ(pbuf,iSize);
	int iKiller=READ_BYTE();
	int iVictim=READ_BYTE();
	int iHS=READ_BYTE();
	char *szWeapon=READ_STRING();
	g_Player[iVictim].bAlive=false;
	g_Player[iVictim].fldmFixTime=gEngfuncs.GetClientTime();
	return g_UserMsgs.DeathMsg(pszName,iSize,pbuf);
}
int CurWeapon( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	int iState = READ_BYTE();
	int iWeaponID = READ_CHAR();
	int iClip = READ_CHAR();
	if( iState )
	{
		g_Local.iClip = iClip;
		g_Local.iWeaponId = iWeaponID;
	}
	return g_UserMsgs.CurWeapon( pszName, iSize, pbuf );
}
int SetFOV( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	g_Local.flFOV = READ_BYTE();
	if( !g_Local.flFOV ) { g_Local.flFOV = 90; }
	return g_UserMsgs.SetFOV( pszName, iSize, pbuf );
}
int ResetHUD( const char*pszName,int iSize,void*pbuf )
{
	BEGIN_READ( pbuf,iSize );

	for( int i = 1;i <= gEngfuncs.GetMaxClients();i++ )
	{
		g_Player[i].vStand.clear();
		g_Player[i].vDuck.clear();
		g_Player[i].vJump.clear();
		g_Player[i].vHitbox.clear();
	}

	return g_UserMsgs.ResetHUD(pszName,iSize,pbuf);
}
void HookUserMsgs( void )
{
	// Credits: LanceVorgin
	DWORD dwUserMsgList = (DWORD)*gEngfuncs.pfnHookUserMsg + 0x1B;
	dwUserMsgList += *(DWORD*)( dwUserMsgList ) + 0x4;
	dwUserMsgList += 0xD;
	usermsg_t **ppUserMsgList = (usermsg_t**)( *(DWORD*)dwUserMsgList );
	usermsg_t *pUserMsg = *ppUserMsgList;
	while( pUserMsg )
	{
#define GMSG( n ) if( !strcmp( pUserMsg->name, #n ) ) { g_UserMsgs.n = pUserMsg->pfn; }
#define HMSG( n ) if( !strcmp( pUserMsg->name, #n ) ) { g_UserMsgs.n = pUserMsg->pfn; pUserMsg->pfn = ##n; }
		//GMSG( HideWeapon )
		HMSG( TeamInfo )
		HMSG( DeathMsg )
		HMSG( CurWeapon )
		HMSG( SetFOV )
		HMSG( ResetHUD )
		pUserMsg = pUserMsg->next;
	}
}