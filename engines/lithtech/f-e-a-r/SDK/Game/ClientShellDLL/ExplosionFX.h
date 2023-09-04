// ----------------------------------------------------------------------- //
//
// MODULE  : ExplosionFX.h
//
// PURPOSE : Explosion special fx class - Definition
//
// CREATED : 12/29/99
//
// (c) 1999-2000 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __EXPLOSION_FX_H__
#define __EXPLOSION_FX_H__

#include "SpecialFX.h"
#include "SharedFXStructs.h"

class CExplosionFX : public CSpecialFX
{
	public :

        virtual bool Init(HLOCALOBJ hServObj, ILTMessage_Read *pMsg);
        virtual bool Init(SFXCREATESTRUCT* psfxCreateStruct);
        virtual bool CreateObject(ILTClient* pClientDE);

		// Apply a physical impulse to objects within the radius of the explosion...
		virtual void CreateAreaPhysicsImpulseForce( );

		// Callback for each object found within the radius of the explosion
		// to apply a physics impulse to...
		static void ApplyPhysicsImpulseToObject( HOBJECT hObject, void* pUserData );

		virtual uint32 GetSFXID() { return SFX_EXPLOSION_ID; }

	protected :

		// Creation data...

		EXPLOSIONCREATESTRUCT	m_cs;		// Holds all initialization data
};

#endif // __EXPLOSION_FX_H__