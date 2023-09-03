#ifndef d3dhook_h
#define d3dhook_h


#include "d3d9dev.h"

class D3DHook
{
	IDirect3DDevice9*		original;
	IDirect3DDevice9hook*	hook;

	BOOL enabled;

public:
	D3DHook(): original(0), hook(0), enabled(0) {}
	~D3DHook();

	BOOL SetHook(BOOL enable);

	IDirect3DDevice9* GetOriginal(void) {return original;}

	BOOL IsHookEnabled(void) { return enabled; }
};


#endif