#pragma once

#include "main.h"

typedef struct usermsg_s
{
	int number;
	int size;
	char name[16];
	struct usermsg_s* next;
	pfnUserMsgHook pfn;
} 
usermsg_t;

struct usermsgs_s
{
	pfnUserMsgHook HideWeapon;
	pfnUserMsgHook TeamInfo;
	pfnUserMsgHook CurWeapon;
	pfnUserMsgHook SetFOV;
	pfnUserMsgHook DeathMsg;
	pfnUserMsgHook ResetHUD;
};

extern usermsgs_s g_UserMsgs;

void HookUserMsgs( void );