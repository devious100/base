
#include "../ogoid++.h"
#include "cheats.h"
#include "../misc/encrypted_strings.h"

#include "../d3d/d3dhook.h"
#include <D3dx9tex.h>

AntiPBSs::AntiPBSs(): cicles_active(0), good_frame(0), cicles_good(3), buffer_valid(FALSE)
{
	tags_nodelay.location = CodePatch::on_renddx9;
	tags_nodelay.offset = TAGSNOFADEDELAY_OFF;
	tags_nodelay.patched_code.resize(2,'\x90');

	encrypted_name = estr_screenshot;

	ConfigParam param;

	param.address = &cicles_good;
	param.type = ConfigParam::param_int;
	config_parameters.push_back(param);

	virtual_key = VK_PRINT;
	Enable(TRUE);
}

BOOL AntiPBSs::Configure( void )
{
	if(cicles_good < 1) return FALSE;
	else return TRUE;
}

AntiPBSs::~AntiPBSs()
{
	tags_nodelay.Apply(FALSE);
	if(good_frame)
		good_frame->Release();
}

BOOL AntiPBSs::Enable(BOOL enable)
{
	if(enable == enabled) 
		return TRUE;
	
	if(enabled == TRUE && cicles_active)
	{
		Cheats_AllowMemPointer(TRUE);
		tags_nodelay.Apply(FALSE);
	}

	enabled = enable;
	buffer_valid = FALSE;
	cicles_active = 0;

	return TRUE;
}

void AntiPBSs::Refresh(void)
{
	if( IsEnabled() && pbcl_base && !buffer_valid )
	{
		if(cicles_active > cicles_good && local_player && local_player->player_view)
		{
			GetBuffer();
			buffer_valid = TRUE;
			cicles_active = 0;
			tags_nodelay.Apply(FALSE);
		}
		else
		{
			if(cicles_active == 0)
			{
				tags_nodelay.Apply(TRUE);
			}
			cicles_active++;
		}
	}
}


BOOL AntiPBSs::GetBuffer(void)
{
	if( !enabled) return TRUE;

	IDirect3DDevice9* pD3DDev = d3d_hook.GetOriginal();
	IDirect3DSurface9* back_buffer = 0;

	if( !pD3DDev || FAILED(pD3DDev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &back_buffer)))
		goto FAIL;

	if(!good_frame)
	{
		D3DSURFACE_DESC desc;
		if(FAILED(back_buffer->GetDesc(&desc)))
			goto FAIL;

		if(FAILED(pD3DDev->CreateOffscreenPlainSurface(desc.Width, desc.Height,
			desc.Format, desc.Pool, &good_frame, NULL)))
			goto FAIL;
	}

	if(FAILED(pD3DDev->GetFrontBufferData(0,good_frame)))
			if(FAILED(pD3DDev->StretchRect(back_buffer, NULL, good_frame, NULL, D3DTEXF_NONE)))
			goto FAIL;

	back_buffer->Release();
	return TRUE;

FAIL:
	if(back_buffer) back_buffer->Release();
	return FALSE;
}


BOOL AntiPBSs::WriteBufferToScreen(void)
{
	if(!enabled) return TRUE;

	IDirect3DDevice9* pD3DDev = d3d_hook.GetOriginal();
	HRESULT hresult = -1;

	if(!pD3DDev)
		return FALSE;

	if(!good_frame)
		hresult = pD3DDev->Clear(0,NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 0.0, 0);
	else
	{
		IDirect3DSurface9* back_buffer = 0;
		if(FAILED(pD3DDev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &back_buffer)))
			return FALSE;

		hresult = pD3DDev->StretchRect(good_frame, NULL, back_buffer, NULL, D3DTEXF_NONE);
		if(FAILED(hresult))
			hresult = D3DXLoadSurfaceFromSurface(back_buffer, NULL, NULL, good_frame, NULL, NULL, D3DX_DEFAULT, 0);

		back_buffer->Release();
	}

	buffer_valid = FALSE;

	if(SUCCEEDED(hresult))
	{
		pD3DDev->Present(NULL,NULL,NULL,NULL);
		return TRUE;
	}
	else return FALSE;

}