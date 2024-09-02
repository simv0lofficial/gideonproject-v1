#pragma once
#include "../../Include/GlobalIncludes.h"
#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../SDK/ModelRender.h"
#include "../../SDK/CMaterialHelper.h"

#include "../Hooks/Hooks.h"

#include <fstream>

typedef void( __thiscall* fnDrawModelExecute )( void*, IMatRenderContext*, const DrawModelState_t &, const ModelRenderInfo_t &, matrix3x4_t* );
fnDrawModelExecute oDrawModelExecute = nullptr;

namespace Cheat
{
	namespace Hooked
	{
		void __fastcall DrawModelExecuteHook( void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t &state, const ModelRenderInfo_t &render_info, matrix3x4_t* matrix )
		{
			const char* ModelName = Interfaces::ModelInfo->GetModelName( ( model_t* )render_info.pModel );
			auto ent = Interfaces::EntityList->GetClientEntity( render_info.entity_index );

			if ( ent )
			{
				if ( G::LocalPlayer == ent )
				{
					if ( G::LocalPlayer->IsScoped() )
						Interfaces::RenderView->SetBlend( c_config::get()->i["vis_alphainscope"] );

					else
						Interfaces::RenderView->SetBlend( ( int )( 1.f * 255.f ) );
				}

				else
					if ( c_config::get()->b["vis_dontrenderteam"] && ent->GetTeam() == G::LocalPlayer->GetTeam() && ent != G::LocalPlayer )
						Interfaces::RenderView->SetBlend( 0 );
			}

			for (auto hk : c_lua::get()->hooks->getHooks("on_draw_model_execute"))
				hk.func(render_info);

			oDrawModelExecute( ecx, context, state, render_info, matrix );
		}
	}
}