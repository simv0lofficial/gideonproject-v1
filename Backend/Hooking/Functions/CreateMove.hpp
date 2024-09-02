#pragma once
#include "../../../Frontend/Menu/cmenu.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../Features/Ragebot/c_ragebot.h"
#include "../../SDK/CBaseEntity.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Legitbot/Aimbot.h"
#include "../../Features/Misc/LagCompensation.h"
#include "../../Features/Ragebot/AntiAims.h"
#include "../../Features/Ragebot/Resolver.h"
#include "../Hooks/Hooks.h"
#include "SendDataGram.hpp"

#include "../../Lua/CLua.h"

typedef bool( __thiscall* fnCreateMove )( IClientMode*, float, CUserCmd* );
fnCreateMove oCreateMove = nullptr;
bool netchannel_hook1 = false;
bool tagreset = false;

typedef void(__thiscall* fnDoExtraBonesProcessing)(CBaseEntity*, studiohdr_t*, Vector*, Quaternion*, matrix3x4_t&, uint8_t*, void*);
fnDoExtraBonesProcessing oDEBP;

namespace Cheat
{
	namespace Hooked
	{
		void __fastcall DoExtraBonesProcessingHook(CBaseEntity* ecx, uint32_t smt, studiohdr_t* a, Vector* b, Quaternion* c, matrix3x4_t& d, uint8_t* e, void* f) {
			const auto state = ecx->GetAnimState();

			if (!state || !state->m_pBaseEntity) {
				oDEBP(ecx, a, b, c, d, e, f);
				return;
			}

			const auto backup_onground = state->m_bOnGround;
			state->m_bOnGround = false;
			oDEBP(ecx, a, b, c, d, e, f);
			state->m_bOnGround = backup_onground;
		}

		bool __stdcall CreateMoveHook( float flInputSampleTime, CUserCmd* UserCmd )
		{
			oCreateMove(Interfaces::ClientMode, flInputSampleTime, UserCmd);
			if (!UserCmd || !Interfaces::Engine->IsInGame() || !Interfaces::Engine->IsConnected() || !UserCmd->tick_count)
				return false;

			static Hooking::VTableHook* playerHooks;
			static bool hooked = false;
			auto ent = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex());
			auto unhook = false;

			/*if (ent) {
				if (ent->IsAlive()) {
					if (!hooked) {
						playerHooks = new Hooking::VTableHook();
						playerHooks->Setup(ent);
						playerHooks->Hook(192, DoExtraBonesProcessingHook);
						oDEBP = playerHooks->GetOriginal<fnDoExtraBonesProcessing>(192);
						hooked = true;
					}
				}
				else
					unhook = true;

				if (unhook && playerHooks) {
					playerHooks->Unhook(192);
					delete playerHooks;
					hooked = false;
				}
			}
			else {
				if (playerHooks) {
					delete playerHooks;
					hooked = false;
				}
			}*/

			if (c_config::get()->b["misc_clantagspammer"]) {
				Features::Misc->ClanTagChanger();

				tagreset = false;
			}
			else {
				if (!tagreset)
					Features::Misc->SetClanTag("", "");

				tagreset = true;
			}

			G::UnpredictedVelocity = G::LocalPlayer->GetVelocity();
			if (!G::LocalPlayer->IsAlive() || G::LocalPlayer->GetHealth() <= 0)
				return true;

			if (G::LocalPlayer->GetActiveWeapon()) {
				auto w = G::LocalPlayer->GetActiveWeapon();
				G::VisualLastCone = w->GetSpreadCone();
				G::VisualLastInaccuracy = w->GetInaccuracy();
				G::VisualLastKnife = w->is_knife();
				G::VisualLastNade = w->is_grenade();
				G::VisualLastRange = w->get_full_info()->range;
			}

			Vector wish_angle = UserCmd->viewangles;
			G::UserCmd = UserCmd;

			Features::Misc->Run();
			if (c_config::get()->b["rage_fakeduck"])
				c_antiaims::get()->fake_duck();

			engineprediction::get()->start();	
	
			Features::LagCompensation->CompensateLegit(UserCmd, G::LocalPlayer);

			c_ragebot::get()->create_move();
			c_antiaims::get()->create_move();

			if (c_config::get()->b["rage_aimbot"] && G::LocalPlayer && !(UserCmd->buttons & IN_ATTACK)) {
				if (G::LocalPlayer->GetActiveWeapon()->is_revolver())
				{
					UserCmd->buttons |= IN_ATTACK;
					float flPostponeFireReady = G::LocalPlayer->GetActiveWeapon()->GetPostponeFireReadyTime();
					if (flPostponeFireReady > 0 && flPostponeFireReady - float(Interfaces::Engine->GetNetChannel()->m_nChokedPackets) / 16.f < Interfaces::Globals->curtime)
					{
						UserCmd->buttons &= ~IN_ATTACK;
						if (flPostponeFireReady + Interfaces::Globals->interval_per_tick * 16 > Interfaces::Globals->curtime)
							UserCmd->buttons |= IN_ATTACK2;
					}
				}
			}

			engineprediction::get()->end();

			for (auto hk : c_lua::get()->hooks->getHooks("on_create_move"))
				hk.func(UserCmd, &G::SendPacket);

			static auto angle_vectors = [](const Vector &angles, Vector *forward, Vector *right, Vector *up) {
				static auto sincos = [](float radians, float *sine, float *cosine) {
					*sine = sin(radians);
					*cosine = cos(radians);
				};

				float sr, sp, sy, cr, cp, cy;

				sincos(DEG2RAD(angles[1]), &sy, &cy);
				sincos(DEG2RAD(angles[0]), &sp, &cp);
				sincos(DEG2RAD(angles[2]), &sr, &cr);

				if (forward) {
					forward->x = cp * cy;
					forward->y = cp * sy;
					forward->z = -sp;
				}

				if (right) {
					right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
					right->y = (-1 * sr*sp*sy + -1 * cr*cy);
					right->z = -1 * sr*cp;
				}

				if (up) {
					up->x = (cr*sp*cy + -sr * -sy);
					up->y = (cr*sp*sy + -sr * cy);
					up->z = cr * cp;
				}
			};

			Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
			auto viewangles = UserCmd->viewangles;
			viewangles.Normalized();

			angle_vectors(wish_angle, &view_fwd, &view_right, &view_up);
			angle_vectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

			float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
			float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
			float v12 = sqrtf(view_up.z * view_up.z);

			Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
			Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
			Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

			float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
			float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
			float v18 = sqrtf(cmd_up.z * cmd_up.z);

			Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
			Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
			Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

			float v22 = norm_view_fwd.x * UserCmd->forwardmove;
			float v26 = norm_view_fwd.y * UserCmd->forwardmove;
			float v28 = norm_view_fwd.z * UserCmd->forwardmove;
			float v24 = norm_view_right.x * UserCmd->sidemove;
			float v23 = norm_view_right.y * UserCmd->sidemove;
			float v25 = norm_view_right.z * UserCmd->sidemove;
			float v30 = norm_view_up.x * UserCmd->upmove;
			float v27 = norm_view_up.z * UserCmd->upmove;
			float v29 = norm_view_up.y * UserCmd->upmove;

			UserCmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
				+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
				+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
			UserCmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
				+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
				+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
			UserCmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
				+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
				+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

			UserCmd->forwardmove = std::clamp<float>(UserCmd->forwardmove, -450.f, 450.f);
			UserCmd->sidemove = std::clamp<float>(UserCmd->sidemove, -450.f, 450.f);
			UserCmd->upmove = std::clamp<float>(UserCmd->upmove, -320.f, 320.f);


			Cheat::Utilities->Math_Normalize(UserCmd->viewangles);

			if (UserCmd->forwardmove > 450.0f)
				UserCmd->forwardmove = 450.0f;
			else if (UserCmd->forwardmove < -450.0f)
				UserCmd->forwardmove = -450.0f;

			if (UserCmd->sidemove > 450.0f)
				UserCmd->sidemove = 450.0f;
			else if (UserCmd->sidemove < -450.0f)
				UserCmd->sidemove = -450.0f;

			if (c_config::get()->b["misc_slidewalk"])
				Features::Misc->SlideWalk();
			if (G::SendPacket)
				G::FakeAngle = UserCmd->viewangles;
			else
				G::RealAngle = UserCmd->viewangles;

			if (G::SendPacket) {
				G::FakePosition = G::LocalPlayer->GetAbsOrigin();
				G::DisplayFakeAngle = G::FakeAngle;
				G::DisplayRealAngle = G::RealAngle;
			}

			Features::grenade_prediction::get()->Tick(UserCmd->buttons);

			uintptr_t *frame_ptr;
			__asm mov frame_ptr, ebp;
			*(bool*)(*frame_ptr - 0x1C) = G::SendPacket;
			return false;
		}
	}
}