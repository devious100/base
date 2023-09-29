#include "main.h"
#include "NTDDK.h"

void EraseModuleFromPEB( HANDLE hModule )
{
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
		if( pDOSHeader->e_magic == IMAGE_DOS_SIGNATURE )
		{
			PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)( (DWORD)pDOSHeader + (DWORD)pDOSHeader->e_lfanew );
			if( pNTHeader->Signature == IMAGE_NT_SIGNATURE )
			{
				DWORD dwOld, dwSize = pNTHeader->OptionalHeader.SizeOfHeaders;
				if( VirtualProtect( (LPVOID)pDOSHeader, dwSize, PAGE_READWRITE, &dwOld ) )
				{
					memset( (void*)pDOSHeader, 0, dwSize );
					VirtualProtect( (LPVOID)pDOSHeader, dwSize, dwOld, &dwOld );
				}
			}
			_TEB *pTeb = NULL;
			_asm
			{
				mov eax, fs:[0x18]
				mov pTeb, eax
			}
			PLIST_ENTRY pList = &pTeb->Peb->LoaderData->InLoadOrderModuleList;
			for( PLIST_ENTRY pEntry = pList->Flink; pEntry != pList; pEntry = pEntry->Flink )
			{
				PLDR_MODULE pModule = (PLDR_MODULE)pEntry;
				if( pModule->BaseAddress == hModule )
				{
					pEntry = &pModule->InLoadOrderModuleList; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
					memset( pEntry, 0, sizeof( LIST_ENTRY ) );
					pEntry = &pModule->InMemoryOrderModuleList; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
					memset( pEntry, 0, sizeof( LIST_ENTRY ) );
					pEntry = &pModule->InInitializationOrderModuleList; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
					memset( pEntry, 0, sizeof( LIST_ENTRY ) );
					pEntry = &pModule->HashTableEntry; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
					memset( pEntry, 0, sizeof( LIST_ENTRY ) );
					memset( pModule->FullDllName.Buffer, 0, pModule->FullDllName.Length );
					memset( pModule, 0, sizeof( LDR_MODULE ) );
					break;
			}
		}
	}
}
BOOL WINAPI DllMain(HANDLE hModule,DWORD dwReason,LPVOID lpReserved)
{
	if( dwReason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( GetModuleHandle( NULL ) );

		CreateThread(0,0,dwHookMethod,0,0,0);
		EraseModuleFromPEB((HMODULE)hModule);
	}

	return TRUE;
}