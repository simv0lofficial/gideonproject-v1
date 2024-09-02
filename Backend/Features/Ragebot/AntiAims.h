#pragma once
class c_antiaims : public singleton<c_antiaims> {
public:
	__forceinline void create_move();
	__forceinline void slowwalk();
	__forceinline void fake_duck();
private:
	__forceinline void pitch(float pitch);
	__forceinline void yaw(float yaw);
	__forceinline void yaw_static(float yaw);
	__forceinline void pitch_emotion();
	__forceinline void pitch_up();
	__forceinline void pitch_minimal();
	__forceinline void yaw_stay_180();
	__forceinline void yaw_stay_jitter();
	__forceinline void yaw_stay_static();
	__forceinline void yaw_move_180();
	__forceinline void yaw_move_jitter();
	__forceinline void yaw_move_static();
	__forceinline bool desync();
	__forceinline float freestand();
private:
	bool side = false;
	int desync_autodir = false;
};

using namespace Cheat;

__forceinline bool IsAbleToShoot2()
{
	if (!Interfaces::Globals)
		return false;

	if (!G::LocalPlayer || !G::LocalPlayer->GetActiveWeapon())
		return false;

	auto flServerTime = (float)G::LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
	auto flNextPrimaryAttack = G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack();

	return(!(flNextPrimaryAttack > flServerTime));
}

__forceinline bool is_throwing() {
	if (G::LocalPlayer->GetActiveWeapon()->is_grenade()) {
		auto g = G::LocalPlayer->GetActiveWeapon();
		if (!g->IsPinPulled() || G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2)
		{
			float fThrowTime = g->GetThrowTime();

			if ((fThrowTime > 0) && (fThrowTime < Interfaces::Globals->curtime))
				return true;
		}
	}
	return false;
}

__forceinline void rotate_movement2(float yaw) {
	Vector viewangles;
	Interfaces::Engine->GetViewAngles(&viewangles);

	float rotation = DEG2RAD(viewangles.y - yaw);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * G::UserCmd->forwardmove) - (sin_rot * G::UserCmd->sidemove);
	float new_sidemove = (sin_rot * G::UserCmd->forwardmove) + (cos_rot * G::UserCmd->sidemove);

	G::UserCmd->forwardmove = new_forwardmove;
	G::UserCmd->sidemove = new_sidemove;
}

inline bool lby() {
	static bool m_bWillUpd;
	static bool m_bWasMove;
	static float m_flLastLbyTime;

	float flTime = Interfaces::Globals->curtime;

	if (G::LocalPlayer->GetVelocity().Length2D() > 110 || !(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		m_bWasMove = true;
	else if (flTime >= m_flLastLbyTime + 1 && G::LocalPlayer->GetVelocity().Length2D() <= 110 && G::LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		if (m_bWasMove) {
			m_flLastLbyTime = flTime + 0.22f;
			m_bWillUpd = true;
		}
		else {
			m_flLastLbyTime = flTime + 1.1f;
			m_bWillUpd = true;
		}
	}
	else
		m_bWillUpd = false;

	return m_bWillUpd;
}

float realAngles = 0.f;
__forceinline void c_antiaims::create_move() {
	this->slowwalk();

	if ((G::UserCmd->buttons& IN_ATTACK) && IsAbleToShoot2() || G::LocalPlayer->GetActiveWeapon()->is_grenade())
		return;

	if ((G::UserCmd->buttons& IN_USE) || (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER))
		return;

	if (c_config::get()->i["rage_antiaim_pitch"] == 1)
		this->pitch_emotion();
	if (c_config::get()->i["rage_antiaim_pitch"] == 2)
		this->pitch_up();
	if (c_config::get()->i["rage_antiaim_pitch"] == 3)
		this->pitch_minimal();

	bool desisenkisleft = false;
	if (c_config::get()->i["rage_antiaim_desync"] > 0) {
		if (!G::SendPacket) {
			desync_autodir = 0;

			if (G::LocalPlayer->GetVelocity().Length() <= 110.f) {
				if (c_config::get()->i["rage_antiaim_yaw"] == 1)
					this->yaw_stay_180();
				if (c_config::get()->i["rage_antiaim_yaw"] == 2)
					this->yaw_stay_jitter();
				if (c_config::get()->i["rage_antiaim_yaw"] == 3)
					this->yaw_stay_static();

				if (G::LocalPlayer->GetFlags() & FL_ONGROUND && G::UserCmd->sidemove < 3 && G::UserCmd->sidemove > -3) {
					static auto sw = false;
					if (sw)
						G::UserCmd->sidemove = 2;
					else
						G::UserCmd->sidemove = -2;
					sw = !sw;
				}
			}
			else {
				if (c_config::get()->i["rage_antiaim_yaw_move"] == 1)
					this->yaw_move_180();
				if (c_config::get()->i["rage_antiaim_yaw_move"] == 2)
					this->yaw_move_jitter();
				if (c_config::get()->i["rage_antiaim_yaw_move"] == 3)
					this->yaw_move_static();
			}
			realAngles = G::UserCmd->viewangles.y;
			G::RealAngle.y = G::UserCmd->viewangles.y;
			G::LocalPlayer->SetAbsAngles(Vector(0, G::UserCmd->viewangles.y, 0));
		}
		else
			desisenkisleft = this->desync();
	}
	else {
		if (G::LocalPlayer->GetVelocity().Length() <= 110.f) {
			if (c_config::get()->i["rage_antiaim_yaw"] == 1)
				this->yaw_stay_180();
			if (c_config::get()->i["rage_antiaim_yaw"] == 2)
				this->yaw_stay_jitter();
			if (c_config::get()->i["rage_antiaim_yaw"] == 3)
				this->yaw_stay_static();
		}
		else {
			if (c_config::get()->i["rage_antiaim_yaw_move"] == 1)
				this->yaw_move_180();
			if (c_config::get()->i["rage_antiaim_yaw_move"] == 2)
				this->yaw_move_jitter();
			if (c_config::get()->i["rage_antiaim_yaw_move"] == 3)
				this->yaw_move_static();
		}

		G::RealAngle.y = G::FakeAngle.y = G::UserCmd->viewangles.y;
	}

	G::RealAngle.x = G::FakeAngle.x = G::UserCmd->viewangles.x;
}

__forceinline void c_antiaims::fake_duck() {
	static int cnt = 0;
	static bool do_ = false;
	if (c_config::get()->b["rage_fakeduck"] && c_keyhandler::get()->auto_check("rage_fakeduck_key")) {
		G::SendPacket = false;
		G::FakelagShouldLag = false;

		if (cnt % 14 == 0)
			do_ = true;
		else if (cnt % 14 == 6)
			G::SendPacket = true;
		else if (cnt % 14 == 7)
			do_ = false;

		if (do_)
			G::UserCmd->buttons |= IN_DUCK;
		else
			G::UserCmd->buttons &= ~IN_DUCK;

		cnt++;
	}
	else {
		do_ = false;
		cnt = 0;
		G::FakelagShouldLag = true;
	}
}

__forceinline void c_antiaims::yaw_stay_180() {
	this->yaw((c_config::get()->b["rage_antiaim_freestand"] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_custom"] + Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_jitter_min"], c_config::get()->i["rage_antiaim_yaw_jitter_max"]));
}

__forceinline void c_antiaims::yaw_stay_jitter() {
	static bool isminnow = false;
	this->yaw((c_config::get()->b["rage_antiaim_freestand"] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_custom"] + (isminnow ? c_config::get()->i["rage_antiaim_yaw_jitter_min"] : c_config::get()->i["rage_antiaim_yaw_jitter_max"]));
	isminnow = !isminnow;
}

__forceinline void c_antiaims::yaw_stay_static() {
	this->yaw_static((c_config::get()->b["rage_antiaim_freestand"] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_custom"] + Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_jitter_min"], c_config::get()->i["rage_antiaim_yaw_jitter_max"]));
}

__forceinline void c_antiaims::yaw_move_180() {

	this->yaw((c_config::get()->b["rage_antiaim_freestand"] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_move_custom"] + Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_move_jitter_min"], c_config::get()->i["rage_antiaim_yaw_move_jitter_max"]));
}

__forceinline void c_antiaims::yaw_move_jitter() {
	static bool isminnow = false;
	this->yaw((c_config::get()->b["rage_antiaim_freestand"] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_move_custom"] + (isminnow ? c_config::get()->i["rage_antiaim_yaw_move_jitter_min"] : c_config::get()->i["rage_antiaim_yaw_move_jitter_max"]));
	isminnow = !isminnow;
}

__forceinline void c_antiaims::yaw_move_static() {
	this->yaw_static((c_config::get()->b["rage_antiaim_freestand"] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_move_custom"] + Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_move_jitter_min"], c_config::get()->i["rage_antiaim_yaw_move_jitter_max"]));
}

__forceinline void c_antiaims::pitch(float pitch) {
	G::UserCmd->viewangles.x = pitch;
}

__forceinline void c_antiaims::yaw(float yaw) {
	if (c_config::get()->b["rage_antiaim_freestand"])
		G::UserCmd->viewangles.y = yaw;
	else
		G::UserCmd->viewangles.y += yaw;
}

__forceinline void c_antiaims::yaw_static(float yaw) {
	G::UserCmd->viewangles.y = yaw;
}

__forceinline void c_antiaims::pitch_emotion() {
	if (c_config::get()->b["misc_antiuntrusted"])
		this->pitch(89);
	else
		this->pitch(-178);
}

__forceinline void c_antiaims::pitch_up() {
	if (c_config::get()->b["misc_antiuntrusted"])
		this->pitch(-89);
	else
		this->pitch(-178);
}

__forceinline void c_antiaims::pitch_minimal() {
	this->pitch(79);
}

__forceinline void c_antiaims::slowwalk() {
	if (!c_config::get()->b["rage_slowmotion"] || !c_keyhandler::get()->auto_check("rage_slowmotion_key"))
		return;

	CBaseWeapon * weapon_handle = G::LocalPlayer->GetActiveWeapon();

	if (!weapon_handle)
		return;

	float amount = 0.0034f * c_config::get()->i["rage_slowmotion_speed"];

	Vector velocity = G::UnpredictedVelocity;
	Vector direction;

	Utilities->Math_VectorAngles(velocity, direction);

	float speed = velocity.Length2D();

	direction.y = G::UserCmd->viewangles.y - direction.y;

	Vector forward;

	Utilities->Math_AngleVectors(direction, forward);

	Vector source = forward * -speed;

	if (speed > (weapon_handle->get_full_info()->max_speed * amount))
	{
		G::UserCmd->forwardmove = source.x;
		G::UserCmd->sidemove = source.y;

	}
}
// короче технология по макс дельте есть
__forceinline bool c_antiaims::desync() {
	static bool switch_;
	switch_ = !switch_;
	float yaw = 0;
	float desyncdelta = std::clamp(G::LocalPlayer->GetMaxDesyncDelta(), 0.f, float(c_config::get()->i["rage_antiaim_desync_limit"]));

	if (desync_autodir > 0)
		yaw = realAngles + (desync_autodir == 1 ? desyncdelta : -desyncdelta);
	else
		yaw = realAngles + (c_config::get()->i["rage_antiaim_desync"] == 1 ? desyncdelta : -desyncdelta);

	Utilities->Math_NormalizeYaw(yaw);

	G::FakeAngle.y = yaw;
	G::UserCmd->viewangles.y = yaw;

	return c_config::get()->i["rage_antiaim_desync"] == 1 ? switch_ : c_config::get()->i["rage_antiaim_desync"] == 2;
}

__forceinline float c_antiaims::freestand() {

	if (c_config::get()->m["rage_antiaim_freestand"][0] && G::LocalPlayer->GetVelocity().Length() <= 110.f || c_config::get()->m["rage_antiaim_freestand"][1] && G::LocalPlayer->GetVelocity().Length() > 110.f && G::LocalPlayer->GetFlags() & FL_ONGROUND || c_config::get()->m["rage_antiaim_freestand"][2] && !(G::LocalPlayer->GetFlags() & FL_ONGROUND)) {

		enum {
			back,
			right,
			left
		};

		Vector view_angles;
		Interfaces::Engine->GetViewAngles(&view_angles);

		static constexpr int damage_tolerance = 50;

		std::vector< CBaseEntity* > enemies;

		auto get_target = [&]() -> CBaseEntity * {
			CBaseEntity* target = nullptr;
			float best_fov = 360.f;

			for (int id = 1; id <= Interfaces::Globals->maxclients; id++) {
				auto e = (Interfaces::EntityList->GetClientEntity(id));

				if (!e->valid(true))
					continue;

				float fov = Cheat::Utilities->Math_GetFov(view_angles, Cheat::Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), e->GetOrigin()));

				if (fov < best_fov) {
					target = e;
					best_fov = fov;
				}

				enemies.push_back(e);
			}

			return target;
		};

		CBaseEntity* e = get_target();

		if (!e)
			return G::UserCmd->viewangles.y + 180;

		auto calculate_damage = [&](Vector point) -> int {
			int damage = 0;
			/*for (auto& enemy : enemies)
				damage += AutoWall->calculate_return_info(enemy->GetEyePosition(), point, enemy, G::LocalPlayer, 1).m_damage;*/

			return damage;
		};

		auto rotate_and_extend = [](Vector position, float yaw, float distance) -> Vector {
			Vector direction;
			Cheat::Utilities->Math_AngleVectors(Vector(0, yaw, 0), direction);

			return position + (direction * distance);
		};

		Vector
			head_position = G::LocalPlayer->GetEyePosition(),
			at_target = Cheat::Utilities->Math_CalcAngle(G::LocalPlayer->GetOrigin(), e->GetOrigin());

		float angles[3] = {
			at_target.y + 180,
			at_target.y + 180 - 10.f,
			at_target.y + 180 + 10.f,
		};

		Vector head_positions[3] = {
			rotate_and_extend(head_position, at_target.y + 180, 999),
			rotate_and_extend(head_position, at_target.y + 180 - 10.f, 999),
			rotate_and_extend(head_position, at_target.y + 180 + 10.f, 999)
		};

		int damages[3] = {
			calculate_damage(head_positions[back]),
			calculate_damage(head_positions[right]),
			calculate_damage(head_positions[left])
		};

		if (damages[right] == damages[left]) {
			side = false;
			auto trace_to_end = [](Vector start, Vector end) -> Vector {
				trace_t trace;
				CTraceWorldOnly filter;
				Ray_t ray;

				ray.Init(start, end);
				Interfaces::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

				return trace.end;
			};

			Vector
				trace_right_endpos = trace_to_end(head_position, head_positions[right]),
				trace_left_endpos = trace_to_end(head_position, head_positions[left]);

			Ray_t ray;
			trace_t trace;
			CTraceWorldOnly filter;

			ray.Init(trace_right_endpos, e->GetEyePosition());
			Interfaces::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
			float distance_1 = (trace.start - trace.end).Length();

			ray.Init(trace_left_endpos, e->GetEyePosition());
			Interfaces::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
			float distance_2 = (trace.start - trace.end).Length();

			if (fabs(distance_1 - distance_2) > 70.f) {
				if (distance_1 < distance_2) {
					side = true;
					desync_autodir = 1;
					G::CurrentFreestandMode = "DIST RIGHT";
					return angles[right] + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
				}
				else if (distance_2 < distance_1) {
					side = true;
					desync_autodir = 2;
					G::CurrentFreestandMode = "DIST LEFT";
					return angles[left] + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
				}
				else {
					desync_autodir = 0;
					G::CurrentFreestandMode = "DIST BACK";
					side = true;
					return angles[back] + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
				}
			}
			else {
				side = true;
				desync_autodir = 0;
				G::CurrentFreestandMode = "BACK";
				return angles[back] + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
			}
		}
		else {
			if (damages[right] < damages[left]) {
				side = true;
				G::CurrentFreestandMode = "DMG RIGHT";
				desync_autodir = 1;
				return angles[right] + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
			}
			else if (damages[right] > damages[left]) {
				side = false;
				G::CurrentFreestandMode = "DMG LEFT";
				desync_autodir = 2;
				return angles[left] + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
			}
			else {
				side = true;
				G::CurrentFreestandMode = "DMG BACK";
				desync_autodir = 0;
				return angles[back] + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
			}
		}
	}
	else {
		G::CurrentFreestandMode = "OFF";
		desync_autodir = 0;
		return G::UserCmd->viewangles.y + 180.f + c_config::get()->i["rage_antiaim_yaw_freestand_add"];
	}
}