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
//! \file	Vector3.h
//! \brief  3D Vector class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "..\include.h"

class CVector3
{
public:
	CVector3( );
	CVector3( float _x, float _y, float _z );

	CVector3&	operator = ( const CVector3& );
	CVector3&	operator +=( const CVector3& );
	CVector3&	operator -=( const CVector3& );
	CVector3&	operator *=( const CVector3& );
	CVector3&	operator *=( const float );
	CVector3	operator + ( const CVector3& );
	CVector3	operator - ( const CVector3& );
	CVector3	operator / ( const CVector3& );
	CVector3	operator / ( const float& );
	CVector3	operator * ( const CVector3& );
	CVector3	operator * ( const float&	 );
	float&		operator []( int index	 );
	bool		operator ==( const CVector3& );
	bool		operator !=( const CVector3& );

	float		Length( );
	float		LengthXY( );
	float		LengthXZ( );
	float		DotProduct( const CVector3& );
	CVector3	CrossProduct( const CVector3& );
	float		Normalise( );

	// some static helper functions
	static CVector3 AngleNormalize( CVector3 vAngle, float flMin, float flMax );

public:
	float x;
	float y;
	float z;
};

// for the sake of compatability...
class VectorAligned : public CVector3
{
public:
	VectorAligned( )
	{
		x = y = z = 0;
	}

	VectorAligned( const CVector3& in )
	{
		x = in.x;
		y = in.y;
		z = in.z;
	}

	float w;
};

#endif
