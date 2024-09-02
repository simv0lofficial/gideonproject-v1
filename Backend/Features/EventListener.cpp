#include "EventListener.h"
#include "Visuals/Visuals.h"
#include "Visuals/EventLogger.h"

#include "../Configuration/Config.h"
#include "../SDK/player_info_t.h"
#include "../Include/GlobalIncludes.h"
#include "Skinchanger/cskin.h"

#include "Ragebot/c_ragebot.h"

#include "../../Other/sounds.h"
#include "../Lua/CLua.h"

CGameEvents g_pGameEvents;
CMyBulletTracer* BulletTracer = new CMyBulletTracer();
CSoundEsp* SoundEsp = new CSoundEsp();

#define ev(n) Interfaces::GameEvents->AddListener(this, #n, false)

void CGameEvents::InitializeEventListeners()
{
	ev(team_info);
	ev(team_score);
	ev(teamplay_broadcast_audio);
	ev(player_team);
	ev(player_class);
	ev(player_death);
	ev(player_chat);
	ev(player_score);
	ev(player_spawn);
	ev(player_shoot);
	ev(player_use);
	ev(player_changename);
	ev(player_hintmessage);
	ev(base_player_teleported);
	ev(game_init);
	ev(game_newmap);
	ev(game_start);
	ev(game_end);
	ev(round_start);
	ev(round_end);
	ev(game_message);
	ev(break_breakable);
	ev(break_prop);
	ev(entity_killed);
	ev(bonus_updated);
	ev(achievement_event);
	ev(achievement_increment);
	ev(physgun_pickup);
	ev(flare_ignite_npc);
	ev(helicopter_grenade_punt_miss);
	ev(user_data_downloaded);
	ev(ragdoll_dissolved);
	ev(hltv_changed_mode);
	ev(hltv_changed_target);
	ev(vote_ended);
	ev(vote_started);
	ev(vote_changed);
	ev(vote_passed);
	ev(vote_failed);
	ev(vote_cast);
	ev(vote_options);
	ev(replay_saved);
	ev(entered_performance_mode);
	ev(browse_replays);
	ev(replay_youtube_stats);
	ev(inventory_updated);
	ev(cart_updated);
	ev(store_pricesheet_updated);
	ev(gc_connected);
	ev(item_schema_initialized);
	ev(weapon_fire);
	ev(player_hurt);
	ev(item_purchase);
	ev(bullet_impact);
	ev(bomb_planted);
	ev(player_disconnect);
	ev(other_death);
	ev(bomb_beginplant);
	ev(bomb_abortplant);
	ev(bomb_defused);
	ev(bomb_exploded);
	ev(bomb_dropped);
	ev(bomb_pickup);
	ev(defuser_dropped);
	ev(defuser_pickup);
	ev(announce_phase_end);
	ev(cs_intermission);
	ev(bomb_begindefuse);
	ev(bomb_abortdefuse);
	ev(hostage_follows);
	ev(hostage_hurt);
	ev(hostage_killed);
	ev(hostage_rescued);
	ev(hostage_stops_following);
	ev(hostage_rescued_all);
	ev(hostage_call_for_help);
	ev(vip_escaped);
	ev(vip_killed);
	ev(player_radio);
	ev(bomb_beep);
	ev(weapon_fire_on_empty);
	ev(grenade_thrown);
	ev(weapon_outofammo);
	ev(weapon_reload);
	ev(weapon_zoom);
	ev(silencer_detach);
	ev(inspect_weapon);
	ev(weapon_zoom_rifle);
	ev(player_spawned);
	ev(item_pickup);
	ev(item_pickup_failed);
	ev(item_remove);
	ev(ammo_pickup);
	ev(item_equip);
	ev(enter_buyzone);
	ev(exit_buyzone);
	ev(buytime_ended);
	ev(enter_bombzone);
	ev(exit_bombzone);
	ev(enter_rescue_zone);
	ev(exit_rescue_zone);
	ev(silencer_off);
	ev(silencer_on);
	ev(buymenu_open);
	ev(buymenu_close);
	ev(round_prestart);
	ev(round_poststart);
	ev(grenade_bounce);
	ev(hegrenade_detonate);
	ev(flashbang_detonate);
	ev(smokegrenade_detonate);
	ev(smokegrenade_expired);
	ev(molotov_detonate);
	ev(decoy_detonate);
	ev(decoy_started);
	ev(tagrenade_detonate);
	ev(inferno_startburn);
	ev(inferno_expire);
	ev(inferno_extinguish);
	ev(decoy_firing);
	ev(player_footstep);
	ev(player_jump);
	ev(player_blind);
	ev(player_falldamage);
	ev(door_moving);
	ev(round_freeze_end);
	ev(mb_input_lock_success);
	ev(mb_input_lock_cancel);
	ev(nav_blocked);
	ev(nav_generate);
	ev(player_stats_updated);
	ev(achievement_info_loaded);
	ev(spec_target_updated);
	ev(spec_mode_updated);
	ev(cs_game_disconnected);
	ev(cs_win_panel_round);
	ev(cs_win_panel_match);
	ev(cs_match_end_restart);
	ev(cs_pre_restart);
	ev(show_freezepanel);
	ev(hide_freezepanel);
	ev(freezecam_started);
	ev(player_avenged_teammate);
	ev(achievement_earned);
	ev(achievement_earned_local);
	ev(item_found);
	ev(items_gifted);
	ev(repost_xbox_achievements);
	ev(match_end_conditions);
	ev(round_mvp);
	ev(player_decal);
	ev(teamplay_round_start);
	ev(client_disconnect);
	ev(gg_player_levelup);
	ev(ggtr_player_levelup);
	ev(assassination_target_killed);
	ev(ggprogressive_player_levelup);
	ev(gg_killed_enemy);
	ev(gg_final_weapon_achieved);
	ev(gg_bonus_grenade_achieved);
	ev(switch_team);
	ev(gg_leader);
	ev(gg_team_leader);
	ev(gg_player_impending_upgrade);
	ev(write_profile_data);
	ev(trial_time_expired);
	ev(update_matchmaking_stats);
	ev(player_reset_vote);
	ev(enable_restart_voting);
	ev(sfuievent);
	ev(start_vote);
	ev(player_given_c4);
	ev(player_become_ghost);
	ev(gg_reset_round_start_sounds);
	ev(tr_player_flashbanged);
	ev(tr_highlight_ammo);
	ev(tr_mark_complete);
	ev(tr_mark_best_time);
	ev(tr_exit_hint_trigger);
	ev(bot_takeover);
	ev(tr_show_finish_msgbox);
	ev(tr_show_exit_msgbox);
	ev(reset_player_controls);
	ev(jointeam_failed);
	ev(teamchange_pending);
	ev(material_default_complete);
	ev(cs_prev_next_spectator);
	ev(cs_handle_ime_event);
	ev(nextlevel_changed);
	ev(seasoncoin_levelup);
	ev(tournament_reward);
	ev(start_halftime);
	ev(ammo_refill);
	ev(parachute_pickup);
	ev(parachute_deploy);
	ev(dronegun_attack);
	ev(drone_dispatched);
	ev(loot_crate_visible);
	ev(loot_crate_opened);
	ev(open_crate_instr);
	ev(smoke_beacon_paradrop);
	ev(drone_cargo_detached);
	ev(choppers_incoming_warning);
	ev(firstbombs_incoming_warning);
	ev(dz_item_interaction);
	ev(snowball_hit_player_face);
}

std::string getHitgroup(int hg) {
	switch (hg) {
	case HITGROUP_HEAD: return xorstr_("head");
	case HITGROUP_CHEST: return xorstr_("chest");
	case HITGROUP_STOMACH: return xorstr_("stomach");
	case HITGROUP_LEFTARM: return xorstr_("left arm");
	case HITGROUP_RIGHTARM: return xorstr_("right arm");
	case HITGROUP_LEFTLEG: return xorstr_("left leg");
	case HITGROUP_RIGHTLEG: return xorstr_("right leg");
	default: return xorstr_("unknown");
	}
}

void CGameEvents::FireGameEvent(IGameEvent* game_event)
{
	const char* szEventName = game_event->GetName();

	if (!szEventName || !G::LocalPlayer)
		return;

	for (auto hk : c_lua::get()->hooks->getHooks(szEventName))
		hk.func(game_event);

	SoundEsp->CollectSteps(game_event);
	BulletTracer->CollectTracers(game_event);
	//g_pSkin->OnEvents(game_event);
	if (!strcmp(szEventName, xorstr_("bullet_impact"))) {
		auto userid = game_event->GetInt("userid");
		auto index = Interfaces::Engine->GetPlayerForUserID(userid);
		Vector position(game_event->GetFloat(xorstr_("x")), game_event->GetFloat(xorstr_("y")), game_event->GetFloat(xorstr_("z")));

		if (c_config::get()->b["vis_bulletimpacts"])
			Interfaces::DebugOverlay->AddBoxOverlay(position, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), 0, 0, 255, 127, 4);
	}
	if (!strcmp(szEventName, xorstr_("weapon_fire"))) {
		auto userid = Interfaces::Engine->GetPlayerForUserID(game_event->GetInt(xorstr_("userid")));

		if (G::RagebotTarget && G::LocalPlayer->GetIndex() == userid && G::RagebotIsShooting) {
			G::MissingDueToSpread = true;
			G::RagebotIsShooting = false;
		}
	}

	if (!strcmp(szEventName, xorstr_("round_start")))
	{
		c_ragebot::get()->clear_bonelist();

		G::PK_RoundStarted = true;
		SoundEsp->ClearSteps();
		BulletTracer->ClearTracers();

		for (int i = 0; i < 64; i++)
			G::ResolverAdd[i] = 0;

		for (int i = 0; i < 64; i++)
			G::MissedShots[i] = 0;
	}
	if (!strcmp(szEventName, xorstr_("player_hurt")))
	{
		auto attacker = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt(xorstr_("attacker"))));
		auto damage = game_event->GetInt(xorstr_("dmg_health"));
		auto remain = game_event->GetInt(xorstr_("health"));

		if (!attacker)
			return;

		if (attacker != G::LocalPlayer)
			return;

		if (c_config::get()->b["vis_hitmarker"])
			Features::Visuals->RunHitmarker();

		auto userid = Interfaces::Engine->GetPlayerForUserID(game_event->GetInt(xorstr_("userid")));
		if (Interfaces::EntityList->GetClientEntity(userid) != G::LocalPlayer) {
			G::RageBotHurting = true;
			G::MissingDueToSpread = false;
		}

		if (c_config::get()->i["vis_hitsound"] != 0) // хуй
		{
			switch (c_config::get()->i["vis_hitsound"])
			{
			case 1:
				PlaySoundA(snd_default, nullptr, SND_MEMORY | SND_ASYNC);
				break;

			case 2:
				Interfaces::Engine->ClientCmd_Unrestricted(xorstr_("play buttons/arena_switch_press_02"));
				break;

			case 3:
				PlaySoundA(snd_flick, nullptr, SND_MEMORY | SND_ASYNC);
				break;

			case 4:
				PlaySoundA(snd_bubble, nullptr, SND_MEMORY | SND_ASYNC);
				break;

			case 5:
				PlaySoundA(snd_ooo, nullptr, SND_MEMORY | SND_ASYNC);
				break;

			case 6:
				Interfaces::Engine->ClientCmd_Unrestricted(xorstr_("play hostage/hpain/hpain1"));
				break;
			}
		}

		if (c_config::get()->b["misc_logevents"] && c_config::get()->b["misc_showdamage"])
		{
			player_info_t inf;
			Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt(xorstr_("userid"))), &inf);

			if (Interfaces::Engine->GetPlayerForUserID(game_event->GetInt(xorstr_("userid"))) == Interfaces::Engine->GetLocalPlayerIndex())
				return;

			if (!lstrcmp(inf.name, xorstr_("")))
				return;

			Utilities->Game_Msg(xorstr_("Hit %s in the %s for %i damage (%i health remaining)"), inf.name, getHitgroup(game_event->GetInt(xorstr_("hitgroup"))).c_str(), damage, remain);
		}
	
	}

	if (!strcmp(szEventName, xorstr_("item_purchase")) && c_config::get()->b["misc_showbuylog"])
	{
		auto userid = game_event->GetInt(xorstr_("userid"));
		auto weapon = game_event->GetString(xorstr_("weapon"));

		if (c_config::get()->b["misc_logevents"] && c_config::get()->b["misc_showbuylog"])
		{
			player_info_t inf;
			Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetPlayerForUserID(userid), &inf);

			if (Interfaces::Engine->GetPlayerForUserID(game_event->GetInt(xorstr_("userid"))) == Interfaces::Engine->GetLocalPlayerIndex())
				return;
			if (!lstrcmp(inf.name, xorstr_("")))
				return;

			Utilities->Game_Msg(xorstr_("%s bought %s"), inf.name, weapon);
		}
	}

	if (!strcmp(szEventName, xorstr_("bomb_planted")) && c_config::get()->b["misc_logevents"])
	{
		auto site = game_event->GetInt(xorstr_("site"));

		Utilities->Game_Msg(xorstr_("Bomb has been planted at site %s"), site ? xorstr_("B") : xorstr_("A"));
	}

	if (!strcmp(szEventName, xorstr_("bomb_defused")) && c_config::get()->b["misc_logevents"])
	{
		auto site = game_event->GetInt(xorstr_("site"));

		Utilities->Game_Msg(xorstr_("Bomb has been defused at site %s"), site ? xorstr_("B") : xorstr_("A"));
	}

	if (!strcmp(szEventName, xorstr_("game_newmap")) || !strcmp(szEventName, xorstr_("game_start")) || !strcmp(szEventName, xorstr_("game_end")))
	{
		c_ragebot::get()->clear_bonelist();

		G::RagebotTarget = nullptr;
		memset(&G::MissedShots, 0, sizeof(G::MissedShots));
		G::ResetCheatState = true;
		G::PK_Reset = true;

		for (int i = 0; i < 64; i++)
			G::ResolverAdd[i] = 0;

		for (int i = 0; i < 64; i++)
			G::MissedShots[i] = 0;
	}
}

void CGameEvents::Register()
{
	EVENT_HOOK(FireEvent);
}

int CGameEvents::GetEventDebugID()
{
	return 42;
}

void CSoundEsp::CollectSteps(IGameEvent* e) {
	if (!c_config::get()->b["vis_soundesp"])
		return;

	if (!strcmp(e->GetName(), xorstr_("player_footstep"))) {
		static float lasttime[64] = { 0.f };

		auto ent = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(e->GetInt(xorstr_("userid"))));
		if (ent == G::LocalPlayer || G::LocalPlayer->GetTeam() == ent->GetTeam() || ent->IsDormant() || !ent->IsAlive())
			return;

		if (Interfaces::Globals->curtime - lasttime[ent->GetIndex()] > .5f) { // pFix задержка
			StepsData.push_back(CSoundEsp_info(ent->GetAbsOrigin(), .5f, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_soundesp_color"])));

			lasttime[ent->GetIndex()] = Interfaces::Globals->curtime;
		}
	}
}

void CSoundEsp::AddStepManually(int ent, Vector o) {
	static float lasttime[4096] = { 0.f };

	if (Interfaces::Globals->curtime - lasttime[ent] > .5f) { // pFix задержка
		StepsData.push_back(CSoundEsp_info(o, .5f, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_soundesp_color"])));

		lasttime[ent] = Interfaces::Globals->curtime;
	}
}

void CSoundEsp::ClearSteps() {
	StepsData.clear();
}

void CSoundEsp::DrawSteps() {
	if (c_config::get()->b["vis_soundesp"]) {
		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		{
			BeamInfo_t beamInfo;

			for (size_t i = 0; i < StepsData.size(); i++)
			{
				auto current = StepsData.at(i);
				beamInfo.m_nType = TE_BEAMRINGPOINT;
				beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
				beamInfo.m_nModelIndex = Interfaces::ModelInfo->GetModelIndex("sprites/purplelaser1.vmt");
				//beam_info.m_pszHaloName   = "sprites/purplelaser1.vmt";
				beamInfo.m_nHaloIndex = -1;
				beamInfo.m_flHaloScale = 5;
				beamInfo.m_flLife = 1.f; //
				beamInfo.m_flWidth = 10.f;
				beamInfo.m_flFadeLength = 1.0f;
				beamInfo.m_flAmplitude = 0.f;
				beamInfo.m_flRed = current.color.r();
				beamInfo.m_flGreen = current.color.g();
				beamInfo.m_flBlue = current.color.b();
				beamInfo.m_flBrightness = current.color.a();
				beamInfo.m_flSpeed = 0.f;
				beamInfo.m_nStartFrame = 0.f;
				beamInfo.m_flFrameRate = 60.f;
				beamInfo.m_nSegments = -1;
				//beam_info.m_bRenderable   = true;
				beamInfo.m_nFlags = FBEAM_FADEOUT;
				beamInfo.m_vecCenter = current.origin + Vector(0, 0, 5);
				beamInfo.m_flStartRadius = 20.f;
				beamInfo.m_flEndRadius = 640.f;
				auto myBeam = Interfaces::RenderBeams->CreateBeamRingPoint(beamInfo);

				if (myBeam)
					Interfaces::RenderBeams->DrawBeam(myBeam);

				StepsData.erase(StepsData.begin() + i);
			}
		}
	}
}

void CMyBulletTracer::ClearTracers() {
	BulletTracerData.clear();
}

void CMyBulletTracer::CollectTracers(IGameEvent* pEvent)
{
	if (!c_config::get()->b["vis_bullettracer"])
		return;

	if (strstr(pEvent->GetName(), xorstr_("bullet_impact")))
	{
		static float tracers[64] = { 0 };

		auto index = Interfaces::Engine->GetPlayerForUserID(pEvent->GetInt(xorstr_("userid")));
		auto ent = Interfaces::EntityList->GetClientEntity(index);
		bool isEnemy = ent->GetTeam() != G::LocalPlayer->GetTeam();

		if (ent->GetTeam() == G::LocalPlayer->GetTeam() && ent != G::LocalPlayer)
			return;

		Vector dist = G::LocalPlayer->GetOrigin() - ent->GetOrigin();
		float flDist = sqrtf(dist.Length());

		if (flDist > 50.f)
			return;

		if (Interfaces::Globals->curtime - tracers[index] < 0.1f)
			return;

		tracers[index] = Interfaces::Globals->curtime;

		Vector position(pEvent->GetInt(xorstr_("x")), pEvent->GetInt(xorstr_("y")), pEvent->GetInt(xorstr_("z")));
		auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_bullettracer_color"]);

		if (isEnemy)
			BulletTracerData.push_back(CMyBulletTracer_info(ent->GetEyePosition() - Vector(0, 0, 1), position, Interfaces::Globals->curtime, col, col));
	}
}

void CMyBulletTracer::DrawTracers()
{
	if (c_config::get()->b["vis_bullettracer"])
	{
		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		{
			if (!G::LocalPlayer)
				return;

			BeamInfo_t beamInfo;

			for (size_t i = 0; i < BulletTracerData.size(); i++)
			{
				auto current = BulletTracerData.at(i);
				beamInfo.m_nType = TE_BEAMPOINTS;
				beamInfo.m_pszModelName = xorstr_("sprites/purplelaser1.vmt");
				beamInfo.m_nModelIndex = -1;
				beamInfo.m_flHaloScale = 0.0f;
				beamInfo.m_flLife = 3.0f;
				beamInfo.m_flWidth = 2.0f;
				beamInfo.m_flEndWidth = 2.0f;
				beamInfo.m_flFadeLength = 0.0f;
				beamInfo.m_flAmplitude = 2.0f;
				beamInfo.m_flBrightness = current.color1.a();
				beamInfo.m_flSpeed = 0.2f;
				beamInfo.m_nStartFrame = 0;
				beamInfo.m_flFrameRate = 0.f;
				beamInfo.m_flRed = current.color1.r();
				beamInfo.m_flGreen = current.color1.g();
				beamInfo.m_flBlue = current.color1.b();
				beamInfo.m_nSegments = 2;
				beamInfo.m_bRenderable = true;
				beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
				beamInfo.m_vecStart = current.src;
				beamInfo.m_vecEnd = current.dst;
				auto myBeam = Interfaces::RenderBeams->CreateBeamPoints(beamInfo);

				if (myBeam)
					Interfaces::RenderBeams->DrawBeam(myBeam);

				BulletTracerData.erase(BulletTracerData.begin() + i);
			}
		}
	}


}
