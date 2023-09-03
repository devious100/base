// ----------------------------------------------------------------------- //
//
// MODULE  : HUDBar.h
//
// PURPOSE : Definition of "Bar" HUD component
//
// (c) 2002 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __HUD_BAR_H
#define __HUD_BAR_H

#include "ltbasedefs.h"
#include "LTPoly.h"

class CHUDBar
{
public:
	CHUDBar();

	void Init(HTEXTURE hBar);
	void Update(float x,float y, float fillW, float maxW, float h);
	void Render();
	void SetAlpha(float fAlpha);
	void SetColor(uint32 cColor);

private:
	bool		m_bInitted;

	LTPoly_GT4	m_Poly[6];
	TextureReference	m_Bar;
	uint32		m_cColor;

};

#endif