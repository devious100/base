#include "sdk.h"


#define SAFE_RELEASE(release) if(release) {release->Release();}

LPDIRECT3DTEXTURE9 chamsTextures[9];

const BYTE blue[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00
};

const BYTE yellow[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00
};

const BYTE green[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00
};

const BYTE ltblue[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00
};

const BYTE purple[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00
};

const BYTE red[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00
};

const BYTE white[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00
};

const BYTE black[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const BYTE orange[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0xFF, 0x00
};

#define CHAMS (uiStride == 64 && ((NumVertices == 1578) || (NumVertices == 1338) || (NumVertices == 927) || (NumVertices == 1019) || (NumVertices == 1123) || (NumVertices == 700) || (NumVertices == 1564) || (NumVertices == 1359) || (NumVertices == 892) || (NumVertices == 1016) || (NumVertices == 1226) || (NumVertices == 813)))

unsigned int uiStride;

IDirect3DDevice9 * pGameDevice;
/**************************************************************************************************/

////////////////
///BeginScene///
////////////////
typedef HRESULT(WINAPI* BeginScene_)(LPDIRECT3DDEVICE9 pDevice);
BeginScene_ pBeginScene;
HRESULT WINAPI nBeginScene(LPDIRECT3DDEVICE9 pDevice)
{
	_asm NOP;
	HRESULT hRet = pBeginScene(pDevice);

	
	DO_ONCE(
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&blue, sizeof(blue), &chamsTextures[0]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&yellow, sizeof(yellow), &chamsTextures[1]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&green, sizeof(green), &chamsTextures[2]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&ltblue, sizeof(ltblue), &chamsTextures[3]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&purple, sizeof(purple), &chamsTextures[4]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&red, sizeof(red), &chamsTextures[5]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&white, sizeof(white), &chamsTextures[6]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&black, sizeof(black), &chamsTextures[7]);
		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&orange, sizeof(orange), &chamsTextures[8]);
		
		D3DXCreateFont( pDevice, 10, 0, FW_THIN, 1, 0, DEFAULT_CHARSET,  OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial Black", &FEAR.m_pCDraw->g_pFont );
		D3DXCreateFont( pDevice, 14, 0, FW_THIN, 1, 0, DEFAULT_CHARSET,  OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial Black", &FEAR.m_pCDraw->MenuFont );
		
		D3DXCreateLine(pDevice, &FEAR.m_pCDraw->pLine); 
	)


	pDevice->GetViewport(&FEAR.m_pCDraw->oViewport);  

	return hRet;
}

/**************************************************************************************************/

//////////////
///EndScene///
//////////////
typedef HRESULT(WINAPI* EndScene_)(LPDIRECT3DDEVICE9 pDevice);
EndScene_ pEndScene;
HRESULT WINAPI nEndScene(LPDIRECT3DDEVICE9 pDevice)
{	
	_asm NOP;
	HRESULT hRet = pEndScene(pDevice);

	DO_ONCE(
		FEAR.m_pMenu->Init();
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dwWait, NULL, 0, NULL);
	)
	
	StartHackLoop();

	return hRet;
}

/**************************************************************************************************/

//////////////////////////
///DrawIndexedPrimitive///
//////////////////////////
typedef HRESULT(WINAPI* DrawIndexedPrimitive_)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex,
											   UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);
DrawIndexedPrimitive_ pDrawIndexedPrimitive;
HRESULT WINAPI nDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex,
									 UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
{	
	_asm NOP;
	HRESULT hRet = pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

	if(CHAMS && pmenu[3][FEAR.m_pMenu->GetIndex(3, "Toggle")].Setting == true)
	{
		DWORD dwOldZ;
		pDevice->GetRenderState(D3DRS_ZENABLE, &dwOldZ);
		pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		for(int i=0; i<10; i++) pDevice->SetTexture(i, chamsTextures[2]);
		pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
		pDevice->SetRenderState(D3DRS_ZENABLE, dwOldZ);
		pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		for(int i=0; i<10; i++)	pDevice->SetTexture(i, chamsTextures[0]);
		pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
	}

	return hRet;
}

/**************************************************************************************************/

/////////////////////
///SetStreamSource///
/////////////////////
typedef HRESULT(WINAPI* SetStreamSource_)(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride);
SetStreamSource_ pSetStreamSource;
HRESULT WINAPI nSetStreamSource(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride)
{	
	_asm NOP;
	HRESULT hRet = pSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);

	if(StreamNumber == 0)
		uiStride = Stride;

	return hRet;
}

/**************************************************************************************************/

///////////
///Reset///
///////////
typedef HRESULT(WINAPI* Reset_)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
Reset_ pReset;
HRESULT WINAPI nReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	_asm NOP;
	HRESULT hRet = pReset(pDevice, pPresentationParameters);

	SAFE_RELEASE(FEAR.m_pCDraw->pLine);
	SAFE_RELEASE(chamsTextures[0]);
	SAFE_RELEASE(chamsTextures[1]);
	SAFE_RELEASE(chamsTextures[2]);
	SAFE_RELEASE(chamsTextures[3]);
	SAFE_RELEASE(chamsTextures[4]);
	SAFE_RELEASE(chamsTextures[5]);
	SAFE_RELEASE(chamsTextures[6]);
	SAFE_RELEASE(chamsTextures[7]);
	SAFE_RELEASE(chamsTextures[8]);
	SAFE_RELEASE(FEAR.m_pCDraw->g_pFont);
	SAFE_RELEASE(FEAR.m_pCDraw->MenuFont);

	return hRet;
}

/**************************************************************************************************/



DWORD dwWaitThread(LPVOID lpArgs)
{

	DWORD FearBase = NULL;

	for (;FearBase == NULL;Sleep(100))           
		FearBase = (DWORD)GetModuleHandle("FEARMP.exe");  //get base address for FEARMP.exe

	for(;pGameDevice == NULL; Sleep(500))  //do this to allow the game to get spun up, if we don't, pGameDevice will always equal 0x00000000
		PATCH_ReadMemory((void*)(FearBase + 0x176FF0), &pGameDevice, 4);
	

	DWORD* pdwNewDevice = (DWORD*)pGameDevice;
	pdwNewDevice = (DWORD*)pdwNewDevice[0];  //turn our pointer into an array for the vtable

	add_log("DrawIndexedPrimitive = 0x%.8x", pdwNewDevice[82]); 

	//Hook accordingly  
	//Note: Requires MS Detours v1.5
	//For further indexes, consult the d3d9.h
	
	pBeginScene = (BeginScene_)DetourFunction((PBYTE)pdwNewDevice[41],(PBYTE)nBeginScene);
	pEndScene = (EndScene_)DetourFunction((PBYTE)pdwNewDevice[42],(PBYTE)nEndScene);
	pDrawIndexedPrimitive = (DrawIndexedPrimitive_)DetourFunction((PBYTE)pdwNewDevice[82],(PBYTE)nDrawIndexedPrimitive);
	pSetStreamSource = (SetStreamSource_)DetourFunction((PBYTE)pdwNewDevice[100],(PBYTE)nSetStreamSource);
	pReset = (Reset_)DetourFunction((PBYTE)pdwNewDevice[16],(PBYTE)nReset);

	return 0;
}

void D3D::StartHooks()
{
	CreateThread( NULL, NULL,(LPTHREAD_START_ROUTINE) dwWaitThread , NULL, NULL, NULL);
}