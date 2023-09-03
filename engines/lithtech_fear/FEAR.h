#ifndef _FEAR_H_
#define _FEAR_H_

#include "sdk.h"


class D3D;
class Aimbot;
class Esp;
class Menu;
class Draw;


class MyClasses
{
public:
	MyClasses::MyClasses();
	
	D3D*		m_pD3D;
	Aimbot*		m_pAimbot;
	Esp*		m_pEsp;
	Menu*		m_pMenu;
	Draw*		m_pCDraw;
};

extern MyClasses FEAR;

#endif //_FEAR_H_