

#include "findpattern.h"
#include "crc32.h"

////////////////////////////////////////////////////////////////////////////////
// DataCompare
//! Compare data
//! 
//! \param pData - 
//! \param bMask - 
//! \param szMask - 
//! \return bool - 
////////////////////////////////////////////////////////////////////////////////
bool DataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
    for(int i = 0;*szMask;++szMask,++pData,++bMask)
	{
        if(*szMask=='x' && *pData!=*bMask ) 
		{
            return false;
		}
	}
    return (*szMask) == NULL;
}

////////////////////////////////////////////////////////////////////////////////
// FindPattern
//! Find a pattern of bytes in memory
//! 
//! \param dwAddress - 
//! \param dwLen - 
//! \param offset - 
//! \param deref - 
//! \param *bMask - 
//! \param szMask - 
//! \return DWORD_PTR - 
////////////////////////////////////////////////////////////////////////////////
DWORD_PTR FindPattern( DWORD_PTR dwAddress, DWORD_PTR dwLen, DWORD_PTR offset, bool deref, BYTE *bMask, char * szMask )
{
    for(DWORD i=0; i < dwLen; i++)
	{
        if( DataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
		{
			if( deref )
			{
				DWORD dwOut;
				memcpy( &dwOut, *(void**)(dwAddress+i+offset), 4 );
				return dwOut;
			}
			return (DWORD)(dwAddress+i+offset);
		}
	}    
    return 0;
}  