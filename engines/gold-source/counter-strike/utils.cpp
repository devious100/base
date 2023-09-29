#include "main.h"

CUtils gUtils;

void CUtils::GetScreenInfo(SCREENINFO *screeninfo)
{
	screeninfo->iSize=sizeof(SCREENINFO);
	gEngfuncs.pfnGetScreenInfo(screeninfo);
	iCenterX=screeninfo->iWidth/2;
	iCenterY=screeninfo->iHeight/2;
}

void CUtils::ConEcho(char *szText,...)
{
	char szBuf[500];
	va_list valist;
	va_start(valist,szText);
	_vsnprintf_s(szBuf,sizeof(szBuf),szText,valist);
	va_end(valist);
	strcat_s(szBuf,"\n");
	gEngfuncs.pfnConsolePrint(szBuf);
}

void CUtils::ConExec(char *szCommand,...)
{
	char szBuf[500];
	va_list valist;
	va_start(valist,szCommand);
	_vsnprintf_s(szBuf,sizeof(szBuf),szCommand,valist);
	va_end(valist);
	gEngfuncs.pfnClientCmd(szBuf);
}
void CUtils::VectorAngles(const float *flForward,float *flAngles)
{
	double tmp,yaw,pitch;
	if(flForward[1]==0 && flForward[0]==0)
	{
		if(flForward[2]>0)
			pitch=90.0;
		else
			pitch=270.0;
	}
	else
		yaw=(double)(atan2(flForward[1],flForward[0])*180.0/M_PI);
	if (yaw<0)
		yaw+=360.0;
	tmp=(double)sqrt(flForward[0]*flForward[0]+flForward[1]*flForward[1]);	
	pitch=(double)(atan2(flForward[2],(float)tmp)*180.0/M_PI);
	flAngles[0]=(float)pitch;
	flAngles[1]=(float)yaw;
	flAngles[2]=0;
}
void *CUtils::Detour(BYTE *pbFunc,BYTE *pbNewFunc,int iLen)
{
	BYTE *pbGate=new BYTE[iLen+5];
	DWORD dwOld;
	VirtualProtect(pbFunc,iLen,PAGE_EXECUTE_READWRITE,&dwOld);
	memcpy(pbGate,pbFunc,iLen);
	pbGate+=iLen;
	pbGate[0]=0xE9;
	*(DWORD*)(pbGate+1)=DWORD(pbFunc+iLen-pbGate)-5;
	pbFunc[0]=0xE9;
	*(DWORD*)(pbFunc+1)=DWORD(pbNewFunc-pbFunc)-5;
	for(int i=5;i<iLen;i++)
		pbFunc[i]=0xCC;
	VirtualProtect(pbFunc,iLen,dwOld,&dwOld);
	return (pbGate-iLen);
}
int CUtils::StrHeight()
{
	int h;
	gEngfuncs.pfnDrawConsoleStringLen("A",&h,&h);
	return h;
}
int CUtils::StrWidth(char *szText,...)
{
	int w,h;
	char szBuf[500];
	va_list valist;
	va_start(valist,szText);
	_vsnprintf_s(szBuf,sizeof(szBuf),szText,valist);
	va_end(valist);
	gEngfuncs.pfnDrawConsoleStringLen(szBuf,&w,&h);
	return w;
}
bool CUtils::CalcScreen(float *flOrigin,float *flVecScreen)
{
	int ret=gEngfuncs.pTriAPI->WorldToScreen(flOrigin,flVecScreen);
	if(flVecScreen[0]<1 && flVecScreen[1]<1 && flVecScreen[0]>-1 && flVecScreen[1]>-1 && !ret)
	{
		flVecScreen[0]=flVecScreen[0]*iCenterX+iCenterX;
		flVecScreen[1]=-flVecScreen[1]*iCenterY+iCenterY;
		return true;
	}
	return false;
}
bool CUtils::isValidEnt(int iIndex)
{
	cl_entity_s *ent=gEngfuncs.GetEntityByIndex(iIndex);
	if(ent && ent->player && ent->curstate.solid && ent->index!=g_Local.iIndex && !(ent->curstate.effects&EF_NODRAW) && !(ent->curstate.messagenum<gEngfuncs.GetLocalPlayer()->curstate.messagenum) && !(gEngfuncs.GetLocalPlayer()->curstate.iuser1==4 && gEngfuncs.GetLocalPlayer()->curstate.iuser2==iIndex))
		return true;
	else return false;
}
color3_s rgb(int r,int g,int b)
{
	color3_s ret={r,g,b};
	return ret;
}

color4_s rgba(int r,int g,int b,int a)
{
	color4_s ret={r,g,b,a};
	return ret;
}