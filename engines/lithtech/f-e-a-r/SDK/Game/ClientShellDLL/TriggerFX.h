// ----------------------------------------------------------------------- //
//
// MODULE  : TriggerFX.h
//
// PURPOSE : Trigger special fx class - Definition
//
// CREATED : 5/6/02
//
// (c) 2002 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __TRIGGER_FX_H__
#define __TRIGGER_FX_H__

//
// Includes...
//

#include "SpecialFX.h"
#include "SharedFXStructs.h"


class CTriggerFX : public CSpecialFX
{
	public: // Methods...

		CTriggerFX();
		~CTriggerFX();

		virtual bool		Init( HLOCALOBJ hServObj, ILTMessage_Read *pMsg );
		virtual bool		Update();
		virtual bool		OnServerMessage( ILTMessage_Read *pMsg );

		virtual uint32		GetSFXID() { return SFX_TRIGGER_ID; }

		virtual bool		WithinIndicatorRadius() const { return m_bWithinIndicatorRadius; }
		virtual float		GetDistancePercentage()	const { return m_fDistPercent; }
		virtual HTEXTURE	GetIcon() const { return m_hIcon; }


	protected: // Methods...

		void	CalcLocalClientDistance();
		void	CheckPlayersWithinTrigger();

	
	protected: // Members...

		typedef std::vector<CCharacterFX*, LTAllocator<CCharacterFX*, LT_MEM_TYPE_CLIENTSHELL> > CharFXList;
		CharFXList	m_lstCurPlayersInTrigger;
		CharFXList	m_lstNewPlayersInTrigger;
		CharFXList	m_lstPlayersNotInTrigger;
		
		TRIGGERCREATESTRUCT m_cs;

		bool		m_bWithinIndicatorRadius;
		float		m_fDistPercent;

		HLOCALOBJ	m_hDimsObject;
		
		TextureReference	m_hIcon;
};

#endif //__TRIGGER_FX_H__