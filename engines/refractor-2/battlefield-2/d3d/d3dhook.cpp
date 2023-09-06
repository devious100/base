#include "d3dhook.h"

#include "../bf2/bf2.h"
#include "../ogoid++.h"

#include <D3dx9tex.h>

#include <iostream>
using namespace std;


extern int renddx9_base;


//////////////////////////////////////////////////////////////////////////

D3DHook::~D3DHook()
{
	delete hook;
}


BOOL D3DHook::SetHook(BOOL enable)
{
	renddx9_hModule = GetModuleHandle("RendDx9.dll");
	renddx9_base = GetModuleBase(renddx9_hModule);

	if( !renderer
		|| (int) renderer < bf2_base
		)
		return FALSE;

	if(enable)
	{
		if(!original)
		{
			original = (IDirect3DDevice9*) renderer->m_pDevice;
			if(!original) return FALSE;

			hook = new IDirect3DDevice9hook(original);

		}

		renderer->m_pDevice = (void*) hook;

	}
	else
	{
		if(!original) return TRUE;

		renderer->m_pDevice = (void*) original;

		delete hook;

		original = hook = 0;
	}

	enabled = enable;

	return TRUE;
}
