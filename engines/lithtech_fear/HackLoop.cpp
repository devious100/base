#include "sdk.h"

/*	
	I made this function to alleviate the clutter in EndScene. 
	Just a preference thing thats all. 
																*/

void StartHackLoop()
{
	
	FEAR.m_pEsp->StartEsp();
	
	FEAR.m_pEsp->AutoKill();
	
	/*MENU FUNCTIONS*/
	FEAR.m_pMenu->CallMenus();
	FEAR.m_pMenu->CursorPos();
	FEAR.m_pMenu->DrawCursor();
	/*END MENU*/
	
	static bool bTime, bTime2, bTime3;
	if (pmenu[6][FEAR.m_pMenu->GetIndex(6, "Head")].Setting && !bTime)
	{
		bTime = true;
		bTime2 = false;
		bTime3 = false;
		FEAR.m_pAimbot->pNode = BONE_Head;
		pmenu[6][FEAR.m_pMenu->GetIndex(6, "Chest")].Setting = false;
		pmenu[6][FEAR.m_pMenu->GetIndex(6, "Stomach")].Setting = false;
	}
	if (pmenu[6][FEAR.m_pMenu->GetIndex(6, "Chest")].Setting && !bTime2)
	{
		bTime = false;
		bTime2 = true;
		bTime3 = false;
		FEAR.m_pAimbot->pNode = BONE_Upper_torso;
		pmenu[6][FEAR.m_pMenu->GetIndex(6, "Head")].Setting = false;
		pmenu[6][FEAR.m_pMenu->GetIndex(6, "Stomach")].Setting = false;
	}
	if (pmenu[6][FEAR.m_pMenu->GetIndex(6, "Stomach")].Setting && !bTime3)
	{
		bTime = false;
		bTime2 = false;
		bTime3 = true;
		FEAR.m_pAimbot->pNode = BONE_Torso;
		pmenu[6][FEAR.m_pMenu->GetIndex(6, "Head")].Setting = false;
		pmenu[6][FEAR.m_pMenu->GetIndex(6, "Chest")].Setting = false;
	}	
	if (!pmenu[6][FEAR.m_pMenu->GetIndex(6, "Head")].Setting && !pmenu[6][FEAR.m_pMenu->GetIndex(6, "Chest")].Setting && !pmenu[6][FEAR.m_pMenu->GetIndex(6, "Stomach")].Setting)
	{
		FEAR.m_pAimbot->pNode = BONE_Head;
		pmenu[6][FEAR.m_pMenu->GetIndex(6, "Head")].Setting = true;
	}

	FEAR.m_pAimbot->FindTarget();
	
	FEAR.m_pEsp->Pause();
	
	FEAR.m_pEsp->CrossHair();
	
	FEAR.m_pEsp->WeaponAccuracy();
		
	FEAR.m_pEsp->PickUps();
	
		
	
}