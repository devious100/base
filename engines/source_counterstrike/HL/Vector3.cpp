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
//! \file	Vector3.cpp
//! \brief  3D Vector class
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////


#include "Vector3.h"

CVector3::CVector3( )
{
	x = y = z = 0.0f;
}

CVector3::CVector3( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

CVector3& CVector3::operator = ( const CVector3& in )
{
	x = in.x;
	y = in.y;
	z = in.z;

	return *this;
}

CVector3& CVector3::operator += ( const CVector3& in )
{
	x += in.x;
	y += in.y;
	z += in.z;

	return *this;
}

CVector3& CVector3::operator -= ( const CVector3& in )
{
	x -= in.x;
	y -= in.y;
	z -= in.z;

	return *this;
}

CVector3& CVector3::operator *= ( const CVector3& in )
{
	x *= in.x;
	y *= in.y;
	z *= in.z;

	return *this;
}

CVector3& CVector3::operator *= ( const float in )
{
	x *= in;
	y *= in;
	z *= in;

	return *this;
}

CVector3 CVector3::operator + ( const CVector3& in )
{
	CVector3 temp;

	temp.x = x + in.x;
	temp.y = y + in.y;
	temp.z = z + in.z;

	return temp;
}

CVector3 CVector3::operator - ( const CVector3& in )
{
	CVector3 temp;

	temp.x = x - in.x;
	temp.y = y - in.y;
	temp.z = z - in.z;

	return temp;
}

CVector3 CVector3::operator / ( const CVector3& in )
{
	CVector3 temp;

	// Use epsilon to avoid division by zero
	temp.x = x / ( ( in.x == 0.0f ) ? FLT_EPSILON : in.x );
	temp.y = y / ( ( in.y == 0.0f ) ? FLT_EPSILON : in.y );
	temp.z = z / ( ( in.x == 0.0f ) ? FLT_EPSILON : in.z );

	return temp;
}

CVector3 CVector3::operator / ( const float& in )
{
	CVector3 temp;

	float flDiv = in == 0.0f ? FLT_EPSILON : in;

	// Use epsilon to avoid division by zero
	temp.x = x / flDiv;
	temp.y = y / flDiv;
	temp.z = z / flDiv;

	return temp;
}

CVector3 CVector3::operator * ( const CVector3& in )
{
	CVector3 temp;

	temp.x = x * in.x;
	temp.y = y * in.y;
	temp.z = z * in.z;

	return temp;
}

CVector3 CVector3::operator * ( const float& in )
{
	CVector3 temp;

	temp.x = x * in;
	temp.y = y * in;
	temp.z = z * in;

	return temp;
}

bool CVector3::operator == ( const CVector3& in )
{
	if( fabs(in.x - x) < FLT_EPSILON &&
		fabs(in.y - y) < FLT_EPSILON &&
		fabs(in.z - z) < FLT_EPSILON )
	{
		return true;
	}

	return false;
}

bool CVector3::operator != ( const CVector3& in )
{
	if( fabs(in.x - x) > FLT_EPSILON &&
		fabs(in.y - y) > FLT_EPSILON &&
		fabs(in.z - z) > FLT_EPSILON )
	{
		return true;
	}

	return false;
}

float& CVector3::operator [] ( int index )
{	
	return ( (&x)[index] );
}

float CVector3::Length( )
{
	return sqrt( (x*x) + (y*y) + (z*z) );
}

float CVector3::LengthXY( )
{
	return sqrt( (x*x) + (y*y) );
}

float CVector3::LengthXZ( )
{
	return sqrt( (x*x) + (z*z) );
}

float CVector3::Normalise( )
{
	float flLength = Length( );

	float flLengthNormal = 1.0f / ( FLT_EPSILON + flLength );

	x = x * flLengthNormal;
	y = y * flLengthNormal;
	z = z * flLengthNormal;

	return flLength;
}

float CVector3::DotProduct( const CVector3& a )
{
	return ( this->x * a.x + this->y * a.y + this->z * a.z );
}

CVector3 CVector3::CrossProduct( const CVector3& vIn )
{
	return CVector3( ( this->y * vIn.z ) - ( this->z * vIn.y ),
					 ( this->z * vIn.x ) - ( this->x * vIn.z ),
					 ( this->x * vIn.y ) - ( this->y * vIn.x ) );
}

CVector3 CVector3::AngleNormalize( CVector3 vAngles, float flMax, float flMin )
{
	float flDelta = flMax - flMin;

	if( vAngles.x > flMax )		vAngles.x -= flDelta;
	if( vAngles.y > flMax )		vAngles.y -= flDelta;
	if( vAngles.x < -flMin )	vAngles.x += flDelta;
	if( vAngles.y < -flMin )	vAngles.y += flDelta;

	return vAngles;
}