////////////////////////////////////////////////////////////////////////////////
//! SourceBase By Chod
//! -------------------------------------------------------------------------
//! SourceBase is free software: you can redistribute it and/or modify 
//! it under the terms of the GNU Lesser General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//! 
//! SourceBase is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//! GNU Lesser General Public License for more details.
//! 
//! You should have received a copy of the GNU Lesser General Public License
//! along with SourceBase. If not, see <http://www.gnu.org/licenses/>.
//! -------------------------------------------------------------------------
//! \file	Trace.h
//! \brief  Define some trace methods
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "Trace.h"
#include "..\Hooks\Hooks.h"
#include "..\Utils\FindPattern.h"
#include "Local.h"

CTrace gTrace;

////////////////////////////////////////////////////////////////////////////////
// CTrace::TraceFilterSkipTwoEntities
//! 
//! \param pEnt1 - 
//! \param pEnt2 - 
//! \return ITraceFilter* - 
////////////////////////////////////////////////////////////////////////////////
ITraceFilter* CTrace::TraceFilterSkipTwoEntities( CBaseEntity* pEnt1, CBaseEntity* pEnt2 )
{
	static DWORD_PTR dwTrace[5];

#if HACK == HACK_CSGO
	static DWORD_PTR dwTraceFilter = FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x0, false, (BYTE*)"\x55\x8B\xEC\x8B\x55\x00\x8B\xC1\x8B\x4D\x00\x89\x48\x00\x8B\x4D\x00\x89\x50\x00\xC7\x40\x00\x00\x00\x00\x00", "xxxxx?xxxx?xx?xx?xx?xx????" );
#else
	static DWORD_PTR dwTraceFilter = FindPattern( gModules[MODULE_CLIENT].dwValue, 0x0091C000, 0x0, false, (BYTE*)"\x55\x8B\xEC\x8B\x55\x00\x8B\xC1\x8B\x4D\x00\x89\x48\x00\x8B\x4D\x00\x89\x50\x00\xC7\x40\x00\x00\x00\x00\x00\xC7\x0\x00\x00\x00\x00\x89\x48\x00\x5D\xC2\x00\x00", "xxxxx?xxxx?xx?xx?xx?xx?????xx????xx?xx?" );
#endif

	__asm
	{
		PUSH 0
		PUSH pEnt2
		PUSH pEnt1
		LEA ECX, dwTrace
		MOV EAX, dwTraceFilter
		CALL EAX
	}

	return reinterpret_cast< ITraceFilter* >( dwTrace );
}

////////////////////////////////////////////////////////////////////////////////
// CTrace::TraceRay
//! 
//! \param Ray_t &ray - 
//! \param fMask - 
//! \param *pTraceFilter - 
//! \param *pTrace - 
//! \return void - 
////////////////////////////////////////////////////////////////////////////////
void CTrace::TraceRay( const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace )
{
	typedef void ( __thiscall *tTraceRay )( void*, const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace );
	GetMethod< tTraceRay >( ID_TRACERAY )( GetInstance(), ray, fMask, pTraceFilter, pTrace );
}

////////////////////////////////////////////////////////////////////////////////
// CTrace::IsVisible
//! Determine if a point or entity is visible
//! 
//! \param vStart - start of trace
//! \param vEnd - end of trace
//! \param dwFlags - trace flags
//! \param pEnt - entity to trace to (if any)
//! \return bool - True if an point in 3D space is visible, false if not
////////////////////////////////////////////////////////////////////////////////
bool CTrace::IsVisible( CVector3& vStart, CVector3& vEnd, DWORD_PTR dwFlags, CBaseEntity* pEnt )
{
	trace_t tr;	
	Ray_t	ray;		
	ray.Init( vStart, vEnd );
	ITraceFilter* tf = NULL;
	
	if( pEnt )
		tf = TraceFilterSkipTwoEntities( CLocalBaseEntity::Get(), pEnt );

	TraceRay( ray, dwFlags, tf, &tr );

	if( pEnt )
	{
		return ( tr.m_pEnt == pEnt || tr.fraction >= 1.0f );
	}

	return ( tr.fraction >= 1.0f );
}