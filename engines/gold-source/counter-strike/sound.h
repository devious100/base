void DynamicSound(int index,int entchannel,char *sample,float *origin,float volume,float attenuation,int timeofs,int pitch);

#define SoundUpdate(a,b,origin) {b=a; a.vOrigin=origin; a.bValid=true; a.dwTime=GetTickCount();}
#define SoundClear(a) {a.vOrigin[0]=0; a.vOrigin[1]=0; a.vOrigin[2]=0; a.dwTime=0; a.bValid=false;}