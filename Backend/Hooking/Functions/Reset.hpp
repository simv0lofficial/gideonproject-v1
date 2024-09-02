#pragma once
#include "../Hooks/Hooks.h"
#include "../../Include/GlobalIncludes.h"
#include "../../../Frontend/Rendering/Render.h"

#include "../../../Frontend/Menu/cmenu.h"

typedef long( __stdcall* fnReset )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
fnReset oReset = nullptr;

namespace Cheat
{
	namespace Hooked
	{
		long __stdcall ResetHook( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParams )
		{
			if ( !Graphics::Render->IsInitialized() || !c_menu::get()->is_menu_initialized())
				return oReset( pDevice, pParams );

			Graphics::Render->ReleaseObjects();
			ImGui_ImplDX9_InvalidateDeviceObjects();
			auto result = oReset( pDevice, pParams );
			ImGui_ImplDX9_CreateDeviceObjects();
			Graphics::Render->CreateObjects();

			return result;
		}
	}
}