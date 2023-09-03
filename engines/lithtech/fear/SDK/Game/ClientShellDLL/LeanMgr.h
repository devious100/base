// ----------------------------------------------------------------------- //
//
// MODULE  : LeanMgr.cpp
//
// PURPOSE : Lean mgr - Definition
//
// CREATED : 1/8/02
//
// (c) 2002 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __LEANMGR_H__
#define __LEANMGR_H__

#include "LeanNodeController.h"

class CLeanMgr
{
	public:

		enum	eLeanDirection
		{
			kLean_Right = -1,
			kLean_Center = 0,
			kLean_Left = 1,
		};


	public:  // Methods...

		CLeanMgr();
		~CLeanMgr();

		void	Init( HOBJECT hObject, ModelsDB::HMODEL hModel );
		void	Update( );

		bool	IsLeaning() const { return !!(m_kLeanDir != kLean_Center); }
		
		uint32	GetControlFlags() const { return m_dwControlFlags; }

		eLeanDirection GetLeanDir( ) const { return m_kLeanDir; }
		
		float	GetCameraLeanAngle( ) const;

		float	GetLeanAngle( ) const { return m_fLeanAngle; }


	private: // Methods...

		void	UpdateControlFlags( );

		void	BeginLean( eLeanDirection kDir );
		void	EndLean( eLeanDirection kDir );
		void	BeginCenter( );
		
		void	UpdateLean( );
		void	UpdateCenter( );

		bool	CalcAngle( float &fAngle, float fInitial, float fTarget, eLeanDirection kDir, float fTotalTime, float fPercent );


	private: // Members...

		eLeanDirection m_kLeanDir;

		uint32		m_dwControlFlags;
		uint32		m_dwLastControlFlags;
	
		float		m_fLeanAngle;
		float		m_fLastLeanAngle;
		float		m_fMaxLeanAngle;
		float		m_fLeanFromAngle;
		float		m_fCenterFromAngle;
		
		float		m_fStartTime;
		float		m_fEndTime;

		bool		m_bFailedToCenter;		// Did we try to center last update but failed?
		bool		m_bFailedToLean;		// Did we try to lean last update but failed?
		bool		m_bLeanedOut;			// Are we completely leaned out?	
		bool		m_bDoneMoving;

		// Object being controlled by the lean... 
		LTObjRef	m_hObject;

		// Node controller to modify the lean nodes...
		CLeanNodeController	m_LeanNodeController;
};

#endif // __LEANMGR_H__