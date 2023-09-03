#ifndef MENU_H
#define MENU_H

#include "sdk.h"


class Menu
{
public:
	void Init();
	void DrawCursor();
	void CursorPos();
	bool IsCursorOver(int x1, int x2, int y1, int y2);
	void AddCvar();
	int GetIndex(int group, char* pszString);
	void CallMenus();
	void AimbotMenu();
	void EspMenu();
	void RemovalsMenu();
	void ChamsMenu();
	void MiscMenu();
	void Save_Load();
	void LoadSettings();
	void SaveSettings();
		
	int Xpos;
	int Ypos;
	bool bDrawAimMenu;
	
};


#define GROUPS 7
#define MAX_ITEMS 7

struct menu2
{
	char name[255];	
	bool Setting;
};

extern menu2 pmenu[GROUPS][MAX_ITEMS];

#endif