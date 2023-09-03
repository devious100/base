#include <windows.h>
#include "detours.h"
#include "cRoute.h"

#pragma comment(lib,"detours.lib")
#pragma comment(lib,"psapi.lib")
BOOL GWP;

DETOUR_TRAMPOLINE(BOOL WINAPI fGetWindowPlacement(HWND hWnd,WINDOWPLACEMENT *lpwndpl),GetWindowPlacement);

BOOL WINAPI Fake_GetWindowPlacemt(HWND hWnd,WINDOWPLACEMENT *lpwndpl)
{

GWP = fGetWindowPlacement(hWnd,lpwndpl);

if (lpwndpl->showCmd && SW_SHOWMAXIMIZED)
{
	lpwndpl->showCmd = SW_MINIMIZE;
}

return GWP;
}