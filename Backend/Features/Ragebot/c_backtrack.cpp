#include "c_backtrack.h"

void c_backtrack::frame_stage_notify(int stage) {
	for (int i = 0; i < Interfaces::Globals->maxclients; i++) {
		auto& bt = players[i];

		auto ent = Interfaces::EntityList->GetClientEntity(i);
		if (!ent) {
			bt.data.clear();
			continue;
		}

		if (!ent->IsPlayer() || !ent->IsAlive() || ent->IsDormant()) {
			bt.data.clear();
			continue;
		}

		if (ent == G::LocalPlayer) {
			bt.data.clear();
			continue;
		}

		if (ent->GetTeam() == G::LocalPlayer->GetTeam() && c_config::get()->b["rage_friendlyfire"]) {
			bt.data.clear();
			continue;
		}

		auto animstate = ent->GetAnimState();
		bt.entity = ent;

		switch (stage) {
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			ent->SetAbsOrigin(ent->GetOrigin());
			ent->SetAbsAngles(Vector(0, animstate->m_flGoalFeetYaw, 0));
			break;
			
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			ent->UpdateClientSideAnimation();
			bt.add_record();
			break;
		}
	}
}

float c_backtrack::get_lerp_time() {
	static ConVar* updaterate = Interfaces::Var->FindVar("cl_updaterate");
	static ConVar* minupdate = Interfaces::Var->FindVar("sv_minupdaterate");
	static ConVar* maxupdate = Interfaces::Var->FindVar("sv_maxupdaterate");
	static ConVar* lerp = Interfaces::Var->FindVar("cl_interp");
	static ConVar* cmin = Interfaces::Var->FindVar("sv_client_min_interp_ratio");
	static ConVar* cmax = Interfaces::Var->FindVar("sv_client_max_interp_ratio");
	static ConVar* ratio = Interfaces::Var->FindVar("cl_interp_ratio");

	float lerpurmom = lerp->GetFloat();
	float maxupdateurmom = maxupdate->GetFloat();
	int updaterateurmom = updaterate->GetInt();
	float ratiourmom = ratio->GetFloat();
	int sv_maxupdaterate = maxupdate->GetInt();
	int sv_minupdaterate = minupdate->GetInt();
	float cminurmom = cmin->GetFloat();
	float cmaxurmom = cmax->GetFloat();

	if (sv_maxupdaterate&& sv_minupdaterate)
		updaterateurmom = maxupdateurmom;

	if (ratiourmom == 0)
		ratiourmom = 1.0f;

	if (cmin&& cmax&& cmin->GetFloat() != 1)
		ratiourmom = std::clamp(ratiourmom, cminurmom, cmaxurmom);

	return max(lerpurmom, ratiourmom / updaterateurmom);
}

std::deque<record> track::get_valid_records() {
	static auto tick_delta = [](float simtime) -> float {
		auto channel = Interfaces::Engine->GetNetChannelInfo();
		if (!channel)
			return FLT_MAX;

		auto tick = TIME_TO_TICKS(simtime);
		auto lerptime = c_backtrack::get()->get_lerp_time();
		auto lerp = TIME_TO_TICKS(lerptime);

		auto prev_arrived_tick = Interfaces::Globals->tickcount + 1 + TIME_TO_TICKS(channel->GetLatency(FLOW_INCOMING) + channel->GetLatency(FLOW_OUTGOING));
		auto correct = std::clamp(lerptime + channel->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(prev_arrived_tick + lerp - (tick + TIME_TO_TICKS(lerptime)));
		return fabs(correct);
	};

	std::deque<record> rc;

	for (auto rd : data) {
		if (tick_delta(rd.simtime) <= 0.2f)
			rc.push_back(rd);
	}

	return rc;
}

void track::add_record() {
	if (data.size() == 0) {
		record rd;
		rd.set_data(entity);

		data.push_front(rd);
	}

	auto& front = data.front();
	if (front.simtime != entity->GetSimTime()) {
		record rd;
		rd.set_data(entity);

		data.push_front(rd);
	}

	auto back = data.back();
	if (Interfaces::Globals->curtime - back.time > 1.f)
		data.pop_back();
}

void record::set_data(CBaseEntity* ent) {
	this->time = Interfaces::Globals->curtime;
	this->simtime = ent->GetSimTime();
	this->pos = ent->GetAbsOrigin();
	this->abs = ent->GetAbsAngles();

	ent->SetupBones(bones, 126, BONE_USED_BY_ANYTHING, this->time);
}