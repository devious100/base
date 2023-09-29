#include "main.h"

void DynamicSound(int index,int entchannel,char *sample,float *origin,float volume,float attenuation,int timeofs,int pitch)
{
	oDynamicSound(index,entchannel,sample,origin,volume,attenuation,timeofs,pitch);
	if(index>=1 && index<=gEngfuncs.GetMaxClients())
	{
		SoundUpdate(g_Player[index].currentSound,g_Player[index].previousSound,origin)
	}
}