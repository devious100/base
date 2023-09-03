

#include "Hooks.h"
#include "..\HL\Engine.h"
#include "..\HL\EntList.h"
#include "..\HL\Entity.h"
#include "..\HL\Local.h"
#include "..\HL\ModelInfo.h"
#include "..\HL\Weapon.h"
#include "..\SourceBase\SourceBase.h"
#include "..\SourceBase\SBVar.h"
#include "..\SourceBase\Console.h"

CSBVar esp_player( "esp_player", "Draw ESP on players", CValue(true) );
CSBVar esp_weapon( "esp_weapon", "Display the name of the weapon an entity is carrying", CValue(true) );

////////////////////////////////////////////////////////////////////////////////
// RunFrame
//! This is the main draw function, you will want to add your ESP/GUI etc in here
//! I have added some example code to get you started
//! 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void RunFrame()
{
	g_pMyFont->PrintString( 10, 10, 255, 255, 255, 255, kAlign_Left, "SourceBase by Chod" );

	// some example ESP code
	if( esp_player.GetValue().m_bool )
	{		
		int iEntCount = gEntList.GetHighestEntityIndex();

		for( int i = 0; i < iEntCount; ++i )
		{
			CBaseEntity* pEnt = gEntList.GetClientEntity(i);

			if( !pEnt )
				continue;

			if( pEnt == CLocalBaseEntity::Get() )
				continue;

			if( pEnt->IsDormant() || !pEnt->IsBaseCombatCharacter() )
				continue;

			if( pEnt->GetLifeState() != LIFE_ALIVE )
				continue;

			player_info_t pi;
			if( !gEngine.GetPlayerInfo( i, &pi ) )
				continue;

			CVector3 vScreen;
			if( !gEngine.WorldToScreen( pEnt->GetAbsOrigin(), vScreen ) )
				continue;

			int y = (int)vScreen.y;
#define INC_Y() y += g_pMyFont->GetTextHeight();

			g_pMyFont->PrintString( (int)vScreen.x, y, 255, 255, 255, 255, kAlign_CenterH|kAlign_CenterV, pi.name ); 
			INC_Y();

			// weapon ESP
			if( esp_weapon.GetValue().m_bool )
			{
				CBaseCombatWeapon* pWeapon = pEnt->GetBaseCombatWeapon();
				if( pWeapon )
				{
					char szWeaponName[128];
					if( pEnt->GetModelName( pWeapon->GetModel(), szWeaponName, sizeof( szWeaponName ) ) )
					{
						g_pMyFont->PrintString( (int)vScreen.x, y, 255, 255, 255, 255, kAlign_CenterH|kAlign_CenterV, szWeaponName ); 
					}
				}
			}
		}
	}

	// now render the console
	gConsole.Render();
}

////////////////////////////////////////////////////////////////////////////////
// Hook_PaintTraverse
//! Hooked PaintTraverse method. All drawing is done from here
//! 
//! \param vguiPanel - 
//! \param forceRepaint - 
//! \param allowForce - 
//! \return void __stdcall - 
////////////////////////////////////////////////////////////////////////////////
void __stdcall PaintTraverse( unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	typedef unsigned int ( __thiscall *tGetPanel )( void*, int type );

	// only render on the in-game panel
	if( vguiPanel == g_pVGUI.GetMethod< tGetPanel >( 1 )( g_pVGUI.GetInstance(), 4 ) )
	{
		RunFrame();
	}

	typedef void ( __thiscall *tPaintTraverse)( void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce );	
	g_pPanel.GetMethod< tPaintTraverse >( ID_PAINTTRAVERSE )( g_pPanel.GetInstance(), vguiPanel, forceRepaint, allowForce );
}