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
//! \file	CmdBase.cpp
//! \brief  Base command class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "CmdBase.h"
#include "SourceBase.h"

CCmdBase* CCmdBase::sCmdList = NULL;

void CCmdBase::Create( IVarBase* pCmd )
{
	if( !pCmd )
		return;

	LOG( "Created new IVarBase [%s]", pCmd->GetName() );

	m_next = sCmdList;
	sCmdList = this;
}

CCmdBase::CCmdBase()
{
	m_flags = NULL;
}

char* CCmdBase::GetName()
{
	return m_name;
}

char* CCmdBase::GetDesc()
{
	return m_desc;
}

DWORD_PTR CCmdBase::GetFlags()
{
	return m_flags;
}