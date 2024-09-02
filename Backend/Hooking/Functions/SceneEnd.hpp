#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../SDK/ModelRender.h"
#include "../../SDK/CMaterialHelper.h"
#include "../../DLL/Engine.h"

#include <fstream>

enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,

	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,

	STUDIORENDER_DRAW_ACCURATETIME = 0x10,		// Use accurate timing when drawing the model.
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,

	STUDIORENDER_DRAW_WIREFRAME = 0x80,

	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,

	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,

	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,

	STUDIORENDER_GENERATE_STATS = 0x8000,
};

typedef void( __thiscall* fnSceneEnd )( CRenderView* );
fnSceneEnd oSceneEnd = nullptr;

inline void MakeDefNormal( GlowObjectDefinition_t* def, float color[4] )
{
	def->m_bPulsatingChams = false;
	def->m_flGlowAlpha = color[3];
	def->m_flRed = color[0];
	def->m_flGreen = color[1];
	def->m_flBlue = color[2];
	def->m_bRenderWhenOccluded = true;
	def->m_bRenderWhenUnoccluded = false;
}

inline void MakeDefPulse( GlowObjectDefinition_t* def, float color[4] )
{
	def->m_bPulsatingChams = true;
	def->m_flGlowAlpha = color[3];
	def->m_flRed = color[0];
	def->m_flGreen = color[1];
	def->m_flBlue = color[2];
	def->m_bRenderWhenOccluded = true;
	def->m_bRenderWhenUnoccluded = false;
}

namespace Cheat
{
	namespace Hooked
	{
		void __fastcall SceneEndHook( CRenderView* thisptr, void* edx )
		{
			/*oSceneEnd( thisptr );*/

			if ((c_config::get()->i["vis_glow"] != 0 || (c_config::get()->i["vis_glowself"] != 0 && c_config::get()->i["vis_self"]) || c_config::get()->i["vis_dropped_glow"] ||
				c_config::get()->i["vis_grenades"] || c_config::get()->b["vis_bomb"]))
			{
				for (auto i = 0; i < Interfaces::GlowObjectManager->max_size; i++)
				{
					auto def = &Interfaces::GlowObjectManager->m_GlowObjectDefinitions[i];

					if (!def || def->IsUnused() || !def->m_pEntity || !def->m_pEntity->GetClientClass())
						continue;

					auto classid = def->m_pEntity->GetClientClass()->m_ClassID;

					if (c_playerlist::get()->should_disable_visuals(def->m_pEntity))
						continue;

					if (classid == (int)ClassID::CCSPlayer)
					{
						if (def->m_pEntity->GetTeam() == G::LocalPlayer->GetTeam() && !c_config::get()->b["vis_teammates"] && def->m_pEntity != G::LocalPlayer)
							continue;

						if (def->m_pEntity == G::LocalPlayer && !c_config::get()->i["vis_glowself"])
							continue;

						if (def->m_pEntity == G::LocalPlayer)
						{
							if (c_config::get()->i["vis_glowself"] == 1)
								MakeDefNormal(def, c_config::get()->c["vis_glowself_color"]);

							else
								if (c_config::get()->i["vis_glowself"] == 2 && !c_config::get()->b["vis_fakeangles"])
									MakeDefPulse(def, c_config::get()->c["vis_glowself_color"]);
						}

						else
							if (def->m_pEntity->GetTeam() == G::LocalPlayer->GetTeam())
							{
								if (c_config::get()->i["vis_glow"] == 1)
									MakeDefNormal(def, c_config::get()->c["vis_glow_color"]);

								else
									if (c_config::get()->i["vis_glow"] == 2)
										MakeDefPulse(def, c_config::get()->c["vis_glow_color"]);
							}

							else
							{
								if (c_config::get()->i["vis_glow"] == 1)
									MakeDefNormal(def, c_config::get()->c["vis_glow_color"]);

								else
									if (c_config::get()->i["vis_glow"] == 2)
										MakeDefPulse(def, c_config::get()->c["vis_glow_color"]);
							}
					}

					if (c_config::get()->m["vis_droppedwpns"][1] && def->m_pEntity->IsWeapon())
						MakeDefNormal(def, c_config::get()->c["vis_droppedwpns_color"]);
					else if (!c_config::get()->m["vis_droppedwpns"][1] && def->m_pEntity->IsWeapon())
						def->m_flGlowAlpha = 0;

					if ((classid == (int)ClassID::CBaseCSGrenadeProjectile ||
						classid == (int)ClassID::CDecoyProjectile ||
						classid == (int)ClassID::CMolotovProjectile ||
						classid == (int)ClassID::CSensorGrenadeProjectile ||
						classid == (int)ClassID::CSmokeGrenadeProjectile ||
						classid == (int)ClassID::CTEClientProjectile) && c_config::get()->i["vis_grenades"])
						MakeDefNormal(def, c_config::get()->c["vis_grenades_color"]);

					if (classid == (int)ClassID::CPlantedC4 && c_config::get()->b["vis_bomb"])
						MakeDefNormal(def, c_config::get()->c["vis_bomb_color"]);
				}
			}

			if ( c_config::get()->b["vis_chamself"] || c_config::get()->b["vis_chamally"] || c_config::get()->b["vis_chamenemy"])
			{
				static IMaterial* covered = nullptr;

				switch ( c_config::get()->i["vis_chams"] )
				{
					case 0:
						covered = MaterialHelper->CoveredLit;
						break;

					case 1:
						covered = MaterialHelper->CoveredFlat;
						break;

					case 2:
						covered = Interfaces::MaterialSystem->FindMaterial( "simple_ignorez_reflective", TEXTURE_GROUP_MODEL );
						break;
				}

				static IMaterial* open = nullptr;

				switch ( c_config::get()->i["vis_chams"] )
				{
					case 0:
						open = MaterialHelper->OpenLit;
						break;

					case 1:
						open = MaterialHelper->OpenFlat;
						break;

					case 2:
						open = Interfaces::MaterialSystem->FindMaterial( "simple_regular_reflective", TEXTURE_GROUP_MODEL );
						break;
				}

				static IMaterial* shadow = MaterialHelper->CoveredFlat;

				auto b_IsThirdPerson = *reinterpret_cast<bool*>( reinterpret_cast<DWORD>( Interfaces::Input ) + 0xA5 );

				if ( open != nullptr && covered != nullptr )
				{
					for ( int i = 0; i < Interfaces::Engine->GetMaxClients(); i++ )
					{
						auto ent = Interfaces::EntityList->GetClientEntity( i );

						if ( !ent || ent->IsDormant() || !ent->IsAlive() )
							continue;

						if ( ent->GetClientClass()->m_ClassID != ( int )ClassID::CCSPlayer )
							continue;

						if (c_playerlist::get()->should_disable_visuals(ent))
							continue;

						if ( ent == G::LocalPlayer && G::LocalPlayer && (c_config::get()->b["vis_chamself"]) )
						{
							if ( ent->IsAlive())
							{
								if (c_config::get()->b["vis_chamself"]) {
									auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamself_color"]);
									Interfaces::RenderView->SetColorModulation(col);
									Interfaces::RenderView->SetBlend(col.a() / 255.f);
									Interfaces::ModelRender->ForcedMaterialOverride(open);
									ent->DrawModel(STUDIORENDER_DRAW_OPAQUE_ONLY, 255);
									Interfaces::RenderView->SetBlend(255.f);
									Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
								}
							}
						}

						if (ent == G::LocalPlayer && G::LocalPlayer && c_config::get()->b["vis_fakeposition"] && G::LocalPlayer->GetAbsOrigin().DistTo(G::FakePosition) > 5.f) {
							Vector OrigAng;
							OrigAng = G::LocalPlayer->GetAbsOrigin();

							G::LocalPlayer->SetAbsOrigin(G::FakePosition);
							auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_fakeposition_color"]);
							Interfaces::RenderView->SetColorModulation(col);
							Interfaces::RenderView->SetBlend(col.a() / 255.f);
							Interfaces::ModelRender->ForcedMaterialOverride(open);

							G::LocalPlayer->DrawModel(STUDIORENDER_DRAW_OPAQUE_ONLY, 255);
							Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
							G::LocalPlayer->SetAbsOrigin(OrigAng);
						}

						if (ent == G::LocalPlayer && G::LocalPlayer && c_config::get()->b["vis_fakeangles"] && G::FakeAngle != G::RealAngle) {
							Vector OrigAng;
							OrigAng = G::RealAngle;
						
							G::LocalPlayer->SetAngle2(Vector(0, G::FakeAngle.y, 0));
							auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_fakeangles_color"]);
							Interfaces::RenderView->SetColorModulation(col);
							Interfaces::RenderView->SetBlend(col.a() / 255.f);
							Interfaces::ModelRender->ForcedMaterialOverride(open);

							G::LocalPlayer->DrawModel(STUDIORENDER_DRAW_OPAQUE_ONLY, 255);
							Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
							G::LocalPlayer->SetAngle2(OrigAng);
						}

						if ( ent != G::LocalPlayer && ent->GetTeam() == G::LocalPlayer->GetTeam() && c_config::get()->b["vis_chamally"] )
						{
							if ( ent->IsAlive() )
							{
								auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamally_color"]);
								Interfaces::RenderView->SetColorModulation( col);
								Interfaces::RenderView->SetBlend( col.a() / 255.f );
								Interfaces::ModelRender->ForcedMaterialOverride( covered );
								ent->DrawModel( STUDIORENDER_DRAW_OPAQUE_ONLY, 255 );
								Interfaces::RenderView->SetBlend( 255.f );
								Interfaces::ModelRender->ForcedMaterialOverride( nullptr );
							}
						}

						if ( ent != G::LocalPlayer && ent->GetTeam() != G::LocalPlayer->GetTeam() && (c_config::get()->b["vis_chamenemy"] || c_config::get()->b["vis_chamenemy_xqz"]) )
						{
							if ( ent->IsAlive() )
							{
								if (c_config::get()->b["vis_chamshadow"] && c_config::get()->i["rage_fakelagfix"] > 0) {
									auto player = c_backtrack::get()->players[i];
									Vector OrigPos;
									Vector OrigAng;
									if (player.entity && !player.get_valid_records().empty() && ent->GetAbsOrigin().DistTo(player.get_valid_records().back().pos) > 5.f) {
										OrigPos = ent->GetAbsOrigin();
										OrigAng = ent->GetAbsAngles();
										player.entity->SetAbsOrigin(player.get_valid_records().back().pos);
										player.entity->SetAngle2(Vector(0, player.get_valid_records().back().abs.y, 0));
										auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamshadow_color"]);
										Interfaces::RenderView->SetColorModulation(col);
										Interfaces::RenderView->SetBlend(col.a() / 255.f);
										Interfaces::ModelRender->ForcedMaterialOverride(shadow);

										ent->DrawModel(STUDIORENDER_DRAW_OPAQUE_ONLY, 255);
										player.entity->SetAngle2(OrigAng);
										player.entity->SetAbsOrigin(OrigPos);
									}

								}

								if (c_config::get()->b["vis_chamenemy_xqz"]) {
									auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamenemy_xqz_color"]);
									Interfaces::RenderView->SetColorModulation(col);
									Interfaces::RenderView->SetBlend(col.a() / 255.f);
									Interfaces::ModelRender->ForcedMaterialOverride(covered);
									ent->DrawModel(STUDIORENDER_DRAW_OPAQUE_ONLY, 255);
								}

								if (c_config::get()->b["vis_chamenemy"]) {
									auto col1 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamenemy_color"]);
									Interfaces::RenderView->SetColorModulation(col1);
									Interfaces::RenderView->SetBlend(col1.a() / 255.f);
									Interfaces::ModelRender->ForcedMaterialOverride(open);
									ent->DrawModel(STUDIORENDER_DRAW_OPAQUE_ONLY, 255);
									
								}

								Interfaces::RenderView->SetBlend(255.f);
								Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
							}
						}
					}
				}
			}

			for (auto hk : c_lua::get()->hooks->getHooks("on_scene_end"))
				hk.func();

			oSceneEnd( thisptr );
		}
	}
}
