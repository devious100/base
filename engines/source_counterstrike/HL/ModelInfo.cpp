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
//! \file	ModelInfo.h
//! \brief  Model info class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#include "ModelInfo.h"
#include "..\Hooks\Hooks.h"

CModelInfo gModelInfo;

////////////////////////////////////////////////////////////////////////////////
// Utils::GetModelName
//! 
//! \param pModel - 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
const char*	CModelInfo::GetModelName( const model_t* pModel )
{
	typedef const char* ( __thiscall *tGetModelName )( void*, const model_t* );
	return GetMethod< tGetModelName >( 3 )( GetInstance(), pModel );
}

////////////////////////////////////////////////////////////////////////////////
// Utils::GetNumModelMaterials
//! 
//! \param pModel - 
//! \return - 
////////////////////////////////////////////////////////////////////////////////
int	CModelInfo::GetModelMaterialCount( const model_t* pModel )
{
	typedef int ( __thiscall *tGetModelMaterialCount )( void*, const model_t* );
	return GetMethod< tGetModelMaterialCount >( ID_GETMODELMATERIALCOUNT )( GetInstance(), pModel );
}