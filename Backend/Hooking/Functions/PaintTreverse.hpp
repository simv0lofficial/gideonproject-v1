#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Features/Visuals/Drawing.h"
#include "../../Features/Visuals/EventLogger.h"
#include "../../Features/Legitbot/Aimbot.h"
#include "../../Features/Ragebot/CAutoWall.h"
#include "../../Lua/CLua.h"

typedef void( __thiscall* fnPaintTraverse )( void*, unsigned int, bool, bool );
fnPaintTraverse oPaintTraverse = nullptr;

using namespace Graphics;

namespace Cheat
{
	namespace Hooked
	{
		void __fastcall PaintTraverseHook( void* thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
		{
			const char* panel_name = Interfaces::GuiPanel->GetName( vguiPanel );
			Graphics::Drawing->Initialize();

			if ( !( strstr( panel_name, "HudZoom" ) && c_config::get()->i["vis_noscope"] > 0 ) )
			{
				oPaintTraverse( thisptr, vguiPanel, forceRepaint, allowForce );

				if ( strstr( panel_name, "FocusOverlayPanel" ) )
				{
					Graphics::Drawing->Initialize();

					if ( Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && G::LocalPlayer )
					{
						static auto svcheatsspoof = new SpoofedConvar( "sv_cheats" );
						svcheatsspoof->SetInt( 1 );
						for (int i = 0; i < 4; i++) {
							for (int j = 0; j < 4; j++) {
								G::matrix[i][j] = Interfaces::Engine->WorldToScreenMatrix()[i][j];
							}
						}

						Visuals->Run();
						Visuals->RunSpectators();

						if ( G::LocalPlayer->GetHealth() > 0 && G::LocalPlayer->IsAlive() )
						{
							auto recoilspoof = Interfaces::Var->FindVar( "cl_crosshair_recoil" );

							if ( c_config::get()->b["vis_recoiloverlay"] )
								recoilspoof->SetValue( 1 );

							else
								recoilspoof->SetValue( 0 );

							if (c_config::get()->b["vis_soundesp"])
								SoundEsp->DrawSteps();

							if ( c_config::get()->b["vis_bullettracer"] )
								BulletTracer->DrawTracers();

							if ( c_config::get()->i["vis_noscope"] > 0 && G::LocalPlayer->IsScoped() )
							{
								int w, h = 0;
								Interfaces::Engine->GetScreenSize( w, h );

								int gOffy = h / 2 / 2;
								switch (c_config::get()->i["vis_noscope"]) {
								case 1:					
									Drawing->GradientHorizontal(w / 2 - gOffy, h / 2, gOffy - 8, 1, CColor(0, 0, 0, 0), CColor(0, 0, 0, 255));
									Drawing->GradientHorizontal(w / 2 + 8, h / 2, gOffy, 1, CColor(0, 0, 0, 255), CColor(0, 0, 0, 0));
									Drawing->GradientVertical(w / 2, h / 2 - gOffy, 1, gOffy - 8, CColor(0, 0, 0, 0), CColor(0, 0, 0, 255));
									Drawing->GradientVertical(w / 2, h / 2 + 8, 1, gOffy, CColor(0, 0, 0, 255), CColor(0, 0, 0, 0));
									break;
								case 2:
									Drawing->Line(w / 2, 0, w / 2, h, CColor(0, 0, 0, 255));
									Drawing->Line(0, h / 2, w, h / 2, CColor(0, 0, 0, 255));
									break;
								}
							}
							
							if (c_config::get()->b["vis_penetrationreticle"])
							{
								Vector direction, EyeAng, dst, forward, angles;
								Interfaces::Engine->GetViewAngles(&EyeAng);

								Utilities->Math_AngleVectors(EyeAng, forward);
								dst = G::LocalPlayer->GetEyePosition() + (forward * 8912.f);

								static auto percent_col = [](int per) -> CColor {
									int red = per < 50 ? 255 : floorf(255 - (per * 2 - 100) * 255.f / 100.f);
									int green = per > 50 ? 255 : floorf((per * 2) * 255.f / 100.f);

									return CColor(red, green, 0);
								};

								int w, h = 0;
								Interfaces::Engine->GetScreenSize(w, h);

								auto dmg = c_autowall::get()->get_estimated_point_damage(dst);
								auto max_dmg = 50.f;

								if (c_config::get()->b["rage_automaticpenetration"])
									max_dmg = float(c_config::get()->i["rage_automaticpenetration_damage"]);
								if (max_dmg == 0.f)
									max_dmg = 50.f;

								auto col = percent_col(std::clamp(dmg, 0.f, max_dmg) / max_dmg * 100.f);

								Drawing->Rect(w / 2 - 1, h / 2 - 1, 1, 1, CColor(0, 0, 0, 150));
								Drawing->Rect(w / 2 - 1, h / 2 + 1, 1, 1, CColor(0, 0, 0, 150));
								Drawing->Rect(w / 2 + 1, h / 2 - 1, 1, 1, CColor(0, 0, 0, 150));
								Drawing->Rect(w / 2 + 1, h / 2 + 1, 1, 1, CColor(0, 0, 0, 150));

								Drawing->Line(w / 2 - 1, h / 2, w / 2 + 1, h / 2, col);
								Drawing->Line(w / 2, h / 2 - 1, w / 2, h / 2 + 1, col);
							}
						}
					}

					Features::grenade_prediction::get()->Paint();
					Features::EventLogger->Draw();

					for (auto hk : c_lua::get()->hooks->getHooks("on_paint"))
						hk.func();
				}
			}
		}
	}
}