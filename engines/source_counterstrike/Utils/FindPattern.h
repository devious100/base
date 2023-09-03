

#ifndef FINDPATTERN_H
#define FINDPATTERN_H

#include "..\Include.h"

DWORD_PTR FindPattern( DWORD_PTR dwAddress, DWORD_PTR dwLen, DWORD_PTR offset, bool deref, BYTE *bMask, char * szMask );

#endif