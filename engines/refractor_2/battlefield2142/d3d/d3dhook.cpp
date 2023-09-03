#include "d3dhook.h"



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
{               _asm NOP; _asm NOP;

	if( !renderer
		|| (int) renderer < bf2_base
		)
		return FALSE;

	if(enable)
	{               _asm NOP; _asm NOP;
		if(!original)
		{               _asm NOP; _asm NOP;
			original = (IDirect3DDevice9*) renderer->m_pDevice;
			if(!original) return FALSE;

			hook = new IDirect3DDevice9hook(original);

		}

		renderer->m_pDevice = (void*) hook;

	}
	else
	{               _asm NOP; _asm NOP;
		if(!original) return TRUE;

		renderer->m_pDevice = (void*) original;

		delete hook;

		original = hook = 0;
	}

	enabled = enable;

	return TRUE;
}
