#ifndef AIMBOT_H
#define	AIMBOT_H

#include "sdk.h"

class Aimbot
{
public:
		
	void FindTarget();
	bool bIsDeathMatch(CLIENT_INFO* MyPlayer);
	
	const char* pNode;

private:
	HOBJECT Victim;
};


#endif