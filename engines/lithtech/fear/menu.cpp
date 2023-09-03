#include "sdk.h"


menu2 pmenu[GROUPS][MAX_ITEMS];

void Menu::Init()
{
	AddCvar();
	LoadSettings();
	Xpos = 0;
	Ypos = 0;
	bDrawAimMenu = false;
		
}

void Menu::CallMenus()
{
	
	static bool bClickOnce = false;
	
	if (GetAsyncKeyState(VK_INSERT) && !bClickOnce)
	{
		bDrawAimMenu = !bDrawAimMenu;
		bClickOnce = true;
	}
	else if(!GetAsyncKeyState(VK_INSERT))
		bClickOnce = false;

	if(bDrawAimMenu)
	{
		FEAR.m_pCDraw->FillRGBA(0, 0, FEAR.m_pCDraw->oViewport.Width, 20, MENU);
		FEAR.m_pCDraw->DrawLine(0, 20, FEAR.m_pCDraw->oViewport.Width, 20, BLACK); //Bottom Menu Line
		FEAR.m_pCDraw->DrawLine(0, 0, 0, 20, BLACK);//BEGIN
		FEAR.m_pCDraw->DrawLine(60, 0, 60, 20, BLACK);//Between A & E
		FEAR.m_pCDraw->DrawLine(100, 0, 100, 20, BLACK);//Between E & R
		FEAR.m_pCDraw->DrawLine(175, 0, 175, 20, BLACK);//Between R & C
		FEAR.m_pCDraw->DrawLine(232, 0, 232, 20, BLACK);//Between C & M
		FEAR.m_pCDraw->DrawLine(279, 0, 279, 20, BLACK);//Between M & S
		FEAR.m_pCDraw->DrawLine(357, 0, 357, 20, BLACK);//END
		FEAR.m_pCDraw->DrawText(10, 4, WHITE, "%s", pmenu[0][0].name); //AIMBOT 40
		FEAR.m_pCDraw->DrawText(70 , 4, WHITE, "%s", pmenu[1][0].name); //ESP 20
		FEAR.m_pCDraw->DrawText(110, 4, WHITE, "%s", pmenu[2][0].name); // REMOVALS 55
		FEAR.m_pCDraw->DrawText(185, 4, WHITE, "%s", pmenu[3][0].name); // CHAMS 37
		FEAR.m_pCDraw->DrawText(242, 4, WHITE, "%s", pmenu[4][0].name); // MISC 25
		FEAR.m_pCDraw->DrawText(290, 4, WHITE, "%s", pmenu[5][0].name); // SAVE/LOAD 25
		
		
		AimbotMenu();
		EspMenu();
		RemovalsMenu();
		ChamsMenu();
		MiscMenu();
		Save_Load();
	}

	
}

void Menu::AimbotMenu()
{
	
	static bool bAimMenu = false;
	static bool bAimAt = false;
	static bool bClickOnce = false;
	if (IsCursorOver(0, 59, 0, 20))
		bAimMenu = true;
	else if(!IsCursorOver(0, 100, 21, 63)/* || IsCursorOver(280, 380, 0, 20) || IsCursorOver(61, 99, 0, 20) || IsCursorOver(101, 174, 0, 20) || IsCursorOver(176, 231, 0, 20) || IsCursorOver(233, 279, 0, 20)*/)
	{
		if (bAimAt && IsCursorOver(100, 175, 35, 91))
			bAimMenu = true;	
		else
			bAimMenu = false;
	}
	
	if (bAimMenu)
	{
		FEAR.m_pCDraw->DrawText(10, 4, BLACK, "%s", pmenu[0][0].name); 
		//FEAR.m_pCDraw->FillRGBA(0, 21, 100, 100, MENU);
		int x = 0;
		for (int i = 1; i < 4; i++, x++)
		{
			FEAR.m_pCDraw->FillRGBA(0, 21 + (14*x), 100, 14, IsCursorOver(0, 100, 21 + (14*x), 13 +(21 + (14*x))) ? MENU2 : MENU);
			FEAR.m_pCDraw->DrawText(5, 21 + (14*x), pmenu[0][i].Setting ? GREEN : RED, "%s", pmenu[0][i].name); 
			FEAR.m_pCDraw->DrawLine(0, 21 + (14*i), 100, 21 + (14*i), BLACK);//bottom
			FEAR.m_pCDraw->FillRGBA(0, 21 + (14*x), 1, 14, BLACK);//left
			FEAR.m_pCDraw->FillRGBA(100, 21 + (14*x), 1, 14, BLACK);//right
			if (IsCursorOver(0, 100, 21 + (14*x), 13 +(21 + (14*x))) && GetAsyncKeyState(VK_LBUTTON) && !bClickOnce && i != 2)
			{
				bClickOnce = true;
				pmenu[0][i].Setting = !pmenu[0][i].Setting;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				bClickOnce = false;
		}
				
	}

	if (IsCursorOver(0, 100, 35, 48))
		bAimAt = true;
	else if(!IsCursorOver(100, 175, 35, 91))
		bAimAt = false;
	
	if (bAimAt && bAimMenu)
	{
		FEAR.m_pCDraw->FillRGBA(101, 35, 75, 14, MENU);
		FEAR.m_pCDraw->FillRGBA(101, 35, 75, 14, MENU2);
		FEAR.m_pCDraw->DrawText(105, 35, BLACK, "%s", pmenu[6][0].name);
		int x = 0;
		for (int i = 1; i < 4; i++, x++)
		{
			FEAR.m_pCDraw->FillRGBA(100, 35 + (14*i), 75, 14, IsCursorOver(100, 175, 35 + (14*i), 13 +(35 + (14*i))) ? MENU2 : MENU);
			FEAR.m_pCDraw->DrawText(105, 35 + (14*i), pmenu[6][i].Setting ? GREEN : RED, "%s", pmenu[6][i].name); 
			FEAR.m_pCDraw->DrawLine(100, 35 + (14*i), 175, 35 + (14*i), BLACK);//bottom
			FEAR.m_pCDraw->DrawLine(100, 35 + (14*4), 175, 35 + (14*4), BLACK);//bottom2
			FEAR.m_pCDraw->FillRGBA(100, 35 + (14*i), 1, 14, BLACK);//left
			FEAR.m_pCDraw->FillRGBA(175, 35 + (14*i), 1, 14, BLACK);//right
			if (IsCursorOver(100, 175, 35 + (14*i), 13 +(35 + (14*i))) && GetAsyncKeyState(VK_LBUTTON) && !bClickOnce)
			{
				bClickOnce = true;
				pmenu[6][i].Setting = !pmenu[6][i].Setting;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				bClickOnce = false;
		}
	}
}
void Menu::EspMenu()
{
	
	static bool bClickOnce = false;
	static bool bEspMenu = false;
	if (IsCursorOver(61, 99, 0, 20))
		bEspMenu = true;
	else if(!IsCursorOver(61, 161, 21, 77)/* || IsCursorOver(280, 380, 0, 20) || IsCursorOver(0, 59, 0, 20) || IsCursorOver(101, 174, 0, 20) || IsCursorOver(176, 231, 0, 20) || IsCursorOver(233, 279, 0, 20)*/)
		bEspMenu = false;

	if (bEspMenu)
	{
		FEAR.m_pCDraw->DrawText(70 , 4, BLACK, "%s", pmenu[1][0].name);
		//FEAR.m_pCDraw->FillRGBA(61, 21, 100, 100, MENU);
		int x = 0;
		for (int i = 1; i < 5; i++, x++)
		{
			FEAR.m_pCDraw->FillRGBA(60, 21 + (14*x), 100, 14, IsCursorOver(61, 161, 21 + (14*x), 13 +(21 + (14*x))) ? MENU2 : MENU);
			FEAR.m_pCDraw->DrawText(65, 21 + (14*x), pmenu[1][i].Setting ? GREEN : RED, "%s", pmenu[1][i].name); 
			FEAR.m_pCDraw->DrawLine(60, 21 + (14*i), 160, 21 + (14*i), BLACK);//bottom
			FEAR.m_pCDraw->FillRGBA(60, 21 + (14*x), 1, 14, BLACK);//left
			FEAR.m_pCDraw->FillRGBA(160, 21 + (14*x), 1, 14, BLACK);//right
			if (IsCursorOver(61, 161, 21 + (14*x), 13 +(21 + (14*x))) && GetAsyncKeyState(VK_LBUTTON) && !bClickOnce)
			{
				bClickOnce = true;
				pmenu[1][i].Setting = !pmenu[1][i].Setting;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				bClickOnce = false;

		}
	}
}
void Menu::RemovalsMenu()
{
	
	static bool bClickOnce = false;
	static bool bRemovalsMenu = false;
	if (IsCursorOver(101, 174, 0, 20))
		bRemovalsMenu = true;
	else if(!IsCursorOver(101, 201, 21, 63)/* || IsCursorOver(280, 380, 0, 20) || IsCursorOver(61, 99, 0, 20) || IsCursorOver(0, 59, 0, 20) || IsCursorOver(176, 231, 0, 20) || IsCursorOver(233, 279, 0, 20)*/)
		bRemovalsMenu = false;

	if (bRemovalsMenu)
	{
		FEAR.m_pCDraw->DrawText(110, 4, BLACK, "%s", pmenu[2][0].name);	
		//FEAR.m_pCDraw->FillRGBA(101, 21, 100, 100, MENU);
		int x = 0;
		for (int i = 1; i < 4; i++, x++)
		{
			FEAR.m_pCDraw->FillRGBA(100, 21 + (14*x), 100, 14, IsCursorOver(101, 201, 21 + (14*x), 13 +(21 + (14*x))) ? MENU2 : MENU);
			FEAR.m_pCDraw->DrawText(105, 21 + (14*x), pmenu[2][i].Setting ? GREEN : RED, "%s", pmenu[2][i].name); 
			FEAR.m_pCDraw->DrawLine(100, 21 + (14*i), 200, 21 + (14*i), BLACK);//bottom
			FEAR.m_pCDraw->FillRGBA(100, 21 + (14*x), 1, 14, BLACK);//left
			FEAR.m_pCDraw->FillRGBA(200, 21 + (14*x), 1, 14, BLACK);//right
			if (IsCursorOver(101, 201, 21 + (14*x), 13 +(21 + (14*x))) && GetAsyncKeyState(VK_LBUTTON) && !bClickOnce)
			{
				bClickOnce = true;
				pmenu[2][i].Setting = !pmenu[2][i].Setting;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				bClickOnce = false;
		}
	}
}
void Menu::ChamsMenu()
{
	
	static bool bClickOnce = false;
	static bool bChamsMenu = false;
	if (IsCursorOver(176, 231, 0, 20))
		bChamsMenu = true;
	else if(!IsCursorOver(176, 276, 21, 35)/* || IsCursorOver(280, 380, 0, 20) || IsCursorOver(61, 99, 0, 20) || IsCursorOver(101, 174, 0, 20) || IsCursorOver(0, 59, 0, 20) || IsCursorOver(233, 279, 0, 20)*/)
		bChamsMenu = false;

	if (bChamsMenu)
	{
		FEAR.m_pCDraw->DrawText(185, 4, BLACK, "%s", pmenu[3][0].name);
		//FEAR.m_pCDraw->FillRGBA(176, 21, 100, 100, MENU);
		int x = 0;
		for (int i = 1; i < 2; i++, x++)
		{
			FEAR.m_pCDraw->FillRGBA(175, 21 + (14*x), 100, 14, IsCursorOver(176, 276, 21 + (14*x), 13 +(21 + (14*x))) ? MENU2 : MENU);
			FEAR.m_pCDraw->DrawText(180, 21 + (14*x), pmenu[3][i].Setting ? GREEN : RED, "%s", pmenu[3][i].name); 
			FEAR.m_pCDraw->DrawLine(175, 21 + (14*i), 275, 21 + (14*i), BLACK);//bottom
			FEAR.m_pCDraw->FillRGBA(175, 21 + (14*x), 1, 14, BLACK);//left
			FEAR.m_pCDraw->FillRGBA(275, 21 + (14*x), 1, 14, BLACK);//right
			if (IsCursorOver(176, 276, 21 + (14*x), 13 +(21 + (14*x))) && GetAsyncKeyState(VK_LBUTTON) && !bClickOnce)
			{
				bClickOnce = true;
				pmenu[3][i].Setting = !pmenu[3][i].Setting;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				bClickOnce = false;
		}
	}
}
void Menu::MiscMenu()
{
	static bool bClickOnce = false;
	static bool bMiscMenu = false;
	if (IsCursorOver(233, 279, 0, 20))
		bMiscMenu = true;
	else if(!IsCursorOver(233, 333, 21, 103)/* || IsCursorOver(280, 380, 0, 20) || IsCursorOver(61, 99, 0, 20) || IsCursorOver(101, 174, 0, 20) || IsCursorOver(176, 231, 0, 20) || IsCursorOver(0, 59, 0, 20)*/)
		bMiscMenu = false;

	if (bMiscMenu)
	{
		FEAR.m_pCDraw->DrawText(242, 4, BLACK, "%s", pmenu[4][0].name);
		//FEAR.m_pCDraw->FillRGBA(233, 21, 100, 100, MENU);
		int x = 0;
		for (int i = 1; i < 7; i++, x++)
		{
			FEAR.m_pCDraw->FillRGBA(232, 21 + (14*x), 100, 14, IsCursorOver(233, 333, 21 + (14*x), 13 +(21 + (14*x))) ? MENU2 : MENU);
			FEAR.m_pCDraw->DrawText(237, 21 + (14*x), pmenu[4][i].Setting ? GREEN : RED, "%s", pmenu[4][i].name); 
			FEAR.m_pCDraw->DrawLine(232, 21 + (14*i), 332, 21 + (14*i), BLACK);//bottom
			FEAR.m_pCDraw->FillRGBA(232, 21 + (14*x), 1, 14, BLACK);//left
			FEAR.m_pCDraw->FillRGBA(332, 21 + (14*x), 1, 14, BLACK);//right
			if (IsCursorOver(233, 333, 21 + (14*x), 13 +(21 + (14*x))) && GetAsyncKeyState(VK_LBUTTON) && !bClickOnce)
			{
				bClickOnce = true;
				pmenu[4][i].Setting = !pmenu[4][i].Setting;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				bClickOnce = false;
		}
	}
}
void Menu::Save_Load()
{
	static bool bClickOnce = false;
	static bool bSLMenu = false;
	if (IsCursorOver(280, 357, 0, 20))
		bSLMenu = true;
	else if(!IsCursorOver(280, 380, 21, 49)/* || IsCursorOver(233, 279, 0, 20) || IsCursorOver(61, 99, 0, 20) || IsCursorOver(101, 174, 0, 20) || IsCursorOver(176, 231, 0, 20) || IsCursorOver(0, 59, 0, 20)*/)
		bSLMenu = false;

	if (bSLMenu)
	{
		FEAR.m_pCDraw->DrawText(290, 4, BLACK, "%s", pmenu[5][0].name);
		//FEAR.m_pCDraw->FillRGBA(280, 21, 100, 100, MENU);
		int x = 0;
		for (int i = 1; i < 3; i++, x++)
		{
			FEAR.m_pCDraw->FillRGBA(279, 21 + (14*x), 100, 14, IsCursorOver(280, 380, 21 + (14*x), 13 +(21 + (14*x))) ? MENU2 : MENU);
			FEAR.m_pCDraw->DrawText(284, 21 + (14*x), pmenu[5][i].Setting ? GREEN : RED, "%s", pmenu[5][i].name); 
			FEAR.m_pCDraw->DrawLine(279, 21 + (14*i), 379, 21 + (14*i), BLACK);//bottom
			FEAR.m_pCDraw->FillRGBA(279, 21 + (14*x), 1, 14, BLACK);//left
			FEAR.m_pCDraw->FillRGBA(379, 21 + (14*x), 1, 14, BLACK);//right
			if (IsCursorOver(280, 380, 21 + (14*x), 13 +(21 + (14*x))) && GetAsyncKeyState(VK_LBUTTON) && !bClickOnce)
			{
				bClickOnce = true;
				pmenu[5][i].Setting = !pmenu[5][i].Setting;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				bClickOnce = false;
		}
		SaveSettings();
		if(pmenu[5][GetIndex(5, "LoadSettings")].Setting)
			LoadSettings();
	}
}
bool Menu::IsCursorOver(int x1, int x2, int y1, int y2)
{
	return (Xpos >= x1 && Xpos <= x2 && Ypos >= y1 && Ypos <= y2);
}


void Menu::DrawCursor()
{
	
	if(!bDrawAimMenu)
		return;
	
	FEAR.m_pCDraw->FillRGBA(Xpos - 5, Ypos, 11, 1, WHITE);
	FEAR.m_pCDraw->FillRGBA(Xpos, Ypos - 5, 1, 11, WHITE);

}

void Menu::CursorPos()
{
	if(!bDrawAimMenu)
		return;
		
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	if(lpPoint.x >= 0 && lpPoint.x <= FEAR.m_pCDraw->oViewport.Width && lpPoint.y >= 0 && lpPoint.y <= FEAR.m_pCDraw->oViewport.Height)
	{
		Xpos = lpPoint.x;
		Ypos = lpPoint.y;
	}
	
		
}

void Menu::AddCvar()
{
	
	strcpy(pmenu[0][0].name, "Aimbot");		strcpy(pmenu[0][1].name, "Toggle");		pmenu[0][1].Setting = true;//
											strcpy(pmenu[0][2].name, "AimAt");		pmenu[0][2].Setting = true;//
											strcpy(pmenu[0][3].name, "AutoShoot");	pmenu[0][3].Setting = true;//

	strcpy(pmenu[1][0].name, "ESP");		strcpy(pmenu[1][1].name, "Name");		pmenu[1][1].Setting = true;//
											strcpy(pmenu[1][2].name, "Distance");	pmenu[1][2].Setting = true;//
											strcpy(pmenu[1][3].name, "Boxes");		pmenu[1][3].Setting = true;
											strcpy(pmenu[1][4].name, "BoneChams");	pmenu[1][4].Setting = true;//

	strcpy(pmenu[2][0].name, "Removals");	strcpy(pmenu[2][1].name, "Recoil");		pmenu[2][1].Setting = true;//
											strcpy(pmenu[2][2].name, "Spread");		pmenu[2][2].Setting = true;//
											strcpy(pmenu[2][3].name, "WeaponSway");	pmenu[2][3].Setting = true;//

	strcpy(pmenu[3][0].name, "Chams");		strcpy(pmenu[3][1].name, "Toggle");		pmenu[3][1].Setting = true;//

	strcpy(pmenu[4][0].name, "Misc");		strcpy(pmenu[4][1].name, "Crosshair");	pmenu[4][1].Setting = true;//
											strcpy(pmenu[4][2].name, "VoteKick");	pmenu[4][2].Setting = false;//
											strcpy(pmenu[4][3].name, "Pause");		pmenu[4][3].Setting = false;//
											strcpy(pmenu[4][4].name, "PickupItems");pmenu[4][4].Setting = false;//
											strcpy(pmenu[4][5].name, "AutoKill");	pmenu[4][5].Setting = false;//
											strcpy(pmenu[4][6].name, "Teleport");	pmenu[4][6].Setting = false;//
	
	strcpy(pmenu[5][0].name, "Save/Load");	strcpy(pmenu[5][1].name, "SaveSettings");	pmenu[5][1].Setting = false;
											strcpy(pmenu[5][2].name, "LoadSettings");	pmenu[5][2].Setting = false;

	strcpy(pmenu[6][0].name, "Bones Menu");	strcpy(pmenu[6][1].name, "Head");		pmenu[6][1].Setting = true;//
											strcpy(pmenu[6][2].name, "Chest");		pmenu[6][2].Setting = false;//
											strcpy(pmenu[6][3].name, "Stomach");	pmenu[6][3].Setting = false;//

}

int Menu::GetIndex(int group, char* pszString)
{
	for (int i = 1; i < 6; i++)
	{
		if(strstr(pmenu[group][i].name, pszString))
			return i;
	}

	return 0;
}

BOOL WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nInteger, LPCTSTR lpFileName)
{
	TCHAR lpString[ 1024 ];
	wsprintf( lpString, "%d", nInteger );
	return WritePrivateProfileString( lpAppName, lpKeyName, lpString, lpFileName );
}

void Menu::SaveSettings()
{
	if(!pmenu[5][GetIndex(5, "SaveSettings")].Setting)
		return;
	
	ofstream ifile;

	ifile.open(SETTINGSLOCATION);
	if (ifile.fail()) return;
    
	WritePrivateProfileInt("AIMBOT", "Toggle", pmenu[0][1].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("AIMBOT", "AutoShoot", pmenu[0][3].Setting, SETTINGSLOCATION);

	WritePrivateProfileInt("BONES", "Head", pmenu[6][1].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("BONES", "Chest", pmenu[6][2].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("BONES", "Stomach", pmenu[6][3].Setting, SETTINGSLOCATION);

	WritePrivateProfileInt("ESP", "Name", pmenu[1][1].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("ESP", "Distance", pmenu[1][2].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("ESP", "Boxes", pmenu[1][3].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("ESP", "BoneChams", pmenu[1][4].Setting, SETTINGSLOCATION);

	WritePrivateProfileInt("REMOVALS", "Recoil", pmenu[2][1].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("REMOVALS", "Spread", pmenu[2][2].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("REMOVALS", "WeaponSway", pmenu[2][3].Setting, SETTINGSLOCATION);

	WritePrivateProfileInt("CHAMS", "Toggle", pmenu[3][1].Setting, SETTINGSLOCATION);

	WritePrivateProfileInt("MISC", "Crosshair", pmenu[4][1].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("MISC", "VoteKick", pmenu[4][2].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("MISC", "Pause", pmenu[4][3].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("MISC", "PickupItems", pmenu[4][4].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("MISC", "AutoKill", pmenu[4][5].Setting, SETTINGSLOCATION);
	WritePrivateProfileInt("MISC", "Teleport", pmenu[4][6].Setting, SETTINGSLOCATION);
	
	ifile.close();

	pmenu[5][GetIndex(5, "SaveSettings")].Setting = false;
}

void Menu::LoadSettings()
{
	
	ifstream ofile;
	ofile.open(SETTINGSLOCATION);
	if (ofile.fail()) return;

	pmenu[0][1].Setting = GetPrivateProfileInt("AIMBOT", "Toggle", 1, SETTINGSLOCATION);
	pmenu[0][3].Setting = GetPrivateProfileInt("AIMBOT", "AutoShoot", 1, SETTINGSLOCATION);

	pmenu[6][1].Setting = GetPrivateProfileInt("BONES", "Head", 1, SETTINGSLOCATION);
	pmenu[6][2].Setting = GetPrivateProfileInt("BONES", "Chest", 0, SETTINGSLOCATION);
	pmenu[6][3].Setting = GetPrivateProfileInt("BONES", "Stomach", 0, SETTINGSLOCATION);

	pmenu[1][1].Setting = GetPrivateProfileInt("ESP", "Name", 1, SETTINGSLOCATION);
	pmenu[1][2].Setting = GetPrivateProfileInt("ESP", "Distance", 1, SETTINGSLOCATION);
	pmenu[1][3].Setting = GetPrivateProfileInt("ESP", "Boxes", 1, SETTINGSLOCATION);
	pmenu[1][4].Setting = GetPrivateProfileInt("ESP", "BoneChams", 1, SETTINGSLOCATION);

	pmenu[2][1].Setting = GetPrivateProfileInt("REMOVALS", "Recoil", 1, SETTINGSLOCATION);
	pmenu[2][2].Setting = GetPrivateProfileInt("REMOVALS", "Spread", 1, SETTINGSLOCATION);
	pmenu[2][3].Setting = GetPrivateProfileInt("REMOVALS", "WeaponSway", 1, SETTINGSLOCATION);

	pmenu[3][1].Setting = GetPrivateProfileInt("CHAMS", "Toggle", 1, SETTINGSLOCATION);

	pmenu[4][1].Setting = GetPrivateProfileInt("MISC", "Crosshair", 1, SETTINGSLOCATION);
	pmenu[4][2].Setting = GetPrivateProfileInt("MISC", "VoteKick", 0, SETTINGSLOCATION);
	pmenu[4][3].Setting = GetPrivateProfileInt("MISC", "Pause", 0, SETTINGSLOCATION);
	pmenu[4][4].Setting = GetPrivateProfileInt("MISC", "PickupItems", 0, SETTINGSLOCATION);
	pmenu[4][5].Setting = GetPrivateProfileInt("MISC", "AutoKill", 0, SETTINGSLOCATION);
	pmenu[4][6].Setting = GetPrivateProfileInt("MISC", "Teleport", 0, SETTINGSLOCATION);
		
	pmenu[5][GetIndex(5, "LoadSettings")].Setting = false;
}