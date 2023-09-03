#include "FEAR.h"

MyClasses FEAR;


MyClasses::MyClasses()
{
	FEAR.m_pD3D		=	new D3D();
	FEAR.m_pAimbot	=	new Aimbot();
	FEAR.m_pEsp		=	new Esp();
	FEAR.m_pMenu	=	new Menu();
	FEAR.m_pCDraw	=	new	Draw();
}