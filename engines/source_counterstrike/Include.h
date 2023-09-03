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
//! \file	Include.h
//! \brief 
//! \date	01 October 2012
//! \author Cal
//! Copyright 2012 Callum MacGregor
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_H
#define INCLUDE_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <time.h>

#include "Utils\VMethod.h"
#include "Error\Error.h"

// set this to 0 to disable threaded startup
#ifndef USE_THREADS
	#define USE_THREADS 1
#endif

class CFont;
extern CFont* g_pMyFont;

// Structure to hold the base addresses of various required modules
enum
{
	MODULE_CLIENT,
	MODULE_ENGINE,
	MODULE_VSTDLIB,
	MODULE_MATSURFACE,
	MODULE_LAUNCHER,
	MODULE_SHADER,
	MODULE_TIER0,
	MODULE_VGUI,
	MODULE_PHYSICS,
	MODULE_MATERIAL,
	MODULE_OVERLAY,

	MODULE_MAX
};

typedef struct Module_s
{
	DWORD_PTR dwValue;
	DWORD_PTR dwHash;
} Module_t;

extern Module_t gModules[MODULE_MAX];

extern CVMethod g_pClient;
extern CVMethod g_pVGUI;
extern CVMethod g_pPrediction;
extern CVMethod g_pMovement;
extern CVMethod g_pInput;
extern CVMethod g_pSurface;
extern CVMethod g_pPanel;
extern CVMethod g_pSound;
extern CVMethod g_pModelRender;
extern CVMethod g_pCmdInput;
extern CVMethod g_pMaterial;
extern CVMethod g_pRenderView;
extern CVMethod g_pPlatTime;

#ifndef SAFE_DELETE
	#define SAFE_DELETE( x ) if( x ){ delete x; x = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY( x ) if( x ){ delete [] x; x = NULL; }
#endif

#endif // INCLUDE_H