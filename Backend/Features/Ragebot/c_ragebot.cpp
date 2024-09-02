#include "c_ragebot.h"
#include "c_playerlist.h"

void c_ragebot::clear_bonelist() {
	
}

void c_ragebot::clear_bones(int ent) {
	
}

void c_ragebot::create_move() {
	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayer->IsAlive())
		return;

	if (!G::LocalPlayer->GetActiveWeapon())
		return;

	if (!G::UserCmd)
		return;

	if (!(c_config::get()->b["rage_aimbot"] && c_keyhandler::get()->auto_check("rage_aimbot_key")))
		return;

	tempTarget = nullptr;

	if (shouldRevertViewangle) {
		G::UserCmd->viewangles = oldViewangle;
		shouldRevertViewangle = false;
	}

	if (target && c_config::get()->b["rage_aimlock"]) {
		if (!target->IsAlive() || target->IsDormant())
			target = nullptr;
	}

	if (!(target && c_config::get()->b["rage_aimlock"])) {
		int best_fov = INT_MAX;
		int best_health = INT_MAX;
		int best_dist = INT_MAX;
		int best_dmg = 0;

		auto aimpoint = hitpoint();
		for (int i = 0; i < 65; i++) {
			auto ent = Interfaces::EntityList->GetClientEntity(i);

			if (ent) {
				if (!ent->IsAlive() || ent->IsDormant() || !ent->IsPlayer())
					continue;

				if (ent == G::LocalPlayer)
					continue;
				
				if (ent->GetTeam() == G::LocalPlayer->GetTeam() && !c_config::get()->b["rage_friendlyfire"])
					continue;
				
				if (Utilities->Math_GetFov(G::UserCmd->viewangles, Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), ent->GetHitboxPosition(HITBOX_HEAD))) > std::clamp(c_config::get()->i["rage_fov"], 0, c_config::get()->b["rage_aimstep"] ? 27 : 180))
					continue;

				if (c_playerlist::get()->should_disable_aimbot(ent))
					continue;

				int curfov = INT_MAX;
				int curhealth = INT_MAX;
				int curdist = INT_MAX;
				int curdmg = INT_MAX;

				int oldfov = INT_MAX;
				int oldhealth = INT_MAX;
				int olddist = INT_MAX;
				int olddmg = 0;

				auto selection = c_config::get()->i["rage_selectionmode"];

				if (shouldForceDistanceSelection)
					selection = 2;

				switch (selection) {
				case 0:
					curfov = Utilities->Math_GetFov(G::UserCmd->viewangles, Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), ent->GetHitboxPosition(HITBOX_HEAD)));
					if (curfov < best_fov) {
						oldfov = best_fov;
						best_fov = curfov;
						tempTarget = ent;

						aimpoint = this->hitscan();
						if (aimpoint.pos == VECTOR_MAX) {
							tempTarget = nullptr;
							best_fov = oldfov;
						}
					}
					break;

				case 1:
					curhealth = ent->GetHealth();
					if (curhealth < best_health) {
						oldhealth = best_health;
						best_health = curhealth;
						tempTarget = ent;

						aimpoint = this->hitscan();
						if (aimpoint.pos == VECTOR_MAX) {
							tempTarget = nullptr;
							best_health = oldhealth;
						}
					}
					break;

				case 2:
					curdist = G::LocalPlayer->GetOrigin().DistTo(ent->GetOrigin());
					if (curdist < best_dist) {
						olddist = best_dist;
						best_dist = curdist;
						tempTarget = ent;

						aimpoint = this->hitscan();
						if (aimpoint.pos == VECTOR_MAX) {
							tempTarget = nullptr;
							best_dist = olddist;
						}
					}
					break;
				case 3:
					tempTarget = ent;
					aimpoint = this->hitscan(&curdmg);

					if (curdmg > best_dmg) {
						olddmg = best_dmg;
						best_dmg = curdmg;
						
						if (aimpoint.pos == VECTOR_MAX) {
							tempTarget = nullptr;
							best_dmg = olddmg;
						}
					}

					break;
				}
			}
		}

		if (shouldForceDistanceSelection)
			shouldForceDistanceSelection = false;

		if (tempTarget && aimpoint.pos != VECTOR_MAX) {
			bool didAimed = this->aim_at_point(aimpoint);
			if (c_config::get()->b["rage_aimlock"] && didAimed) {
				target = tempTarget;
				return;
			}
		}
		else {
			if (c_config::get()->i["rage_selectionmode"] == 3)
				shouldForceDistanceSelection = true;
		}
	}
	else {
		this->tempTarget = this->target;
		
		auto aimpoint = this->hitscan();
		if (aimpoint.pos != VECTOR_MAX)
			this->aim_at_point(aimpoint);

		return;
	}

	if (G::UserCmd->buttons & IN_ATTACK) {
		if (c_config::get()->b["rage_removerecoil"])
			G::UserCmd->viewangles -= G::LocalPlayer->GetPunchAngles() * 2.f;
	}
}

bool c_ragebot::aim_at_point(hitpoint point) {
	auto idx = tempTarget->GetIndex();
	auto wep = G::LocalPlayer->GetActiveWeapon();

	auto unpredicted = point;
	if (oldPoint[idx].pos != Vector(0, 0, 0))
		point.pos -= (oldPoint[idx].pos - point.pos);

	oldPoint[idx] = unpredicted;

	auto aimangle = Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), point.pos);
	Vector localview;
	Interfaces::Engine->GetViewAngles(&localview);

	if (Utilities->Math_GetFov(localview, aimangle) > std::clamp(c_config::get()->i["rage_fov"], 0, c_config::get()->b["rage_aimstep"] ? 27 : 180))
		return false;

	if (c_config::get()->b["rage_removerecoil"])
		aimangle -= G::LocalPlayer->GetPunchAngles() * 2.f;

	auto shoot_state = this->is_able_to_shoot();
	auto hc_state = this->hitchance(aimangle);
	if (c_config::get()->b["rage_quickstop"] && !wep->is_knife() && wep->GetItemDefenitionIndex() != WEAPON_TASER && !hc_state && shoot_state)
		this->quick_stop();

	if (c_config::get()->b["rage_autofire"] && hc_state && shoot_state)
		G::UserCmd->buttons |= IN_ATTACK;

	if (c_config::get()->b["rage_autoscope"] && G::LocalPlayer->GetActiveWeapon()->is_sniper() && !G::LocalPlayer->IsScoped())
		G::UserCmd->buttons |= IN_ATTACK2;

	if (G::UserCmd->buttons & IN_ATTACK) {
		if (c_config::get()->b["rage_silentaim"]) {
			oldViewangle = G::UserCmd->viewangles;
			shouldRevertViewangle = true;
		}
		else
			Interfaces::Engine->SetViewAngles(&aimangle);

		if (point.tick != G::UserCmd->tick_count) // вот тут чек на всякий случай имхо пофиксит хп
			G::UserCmd->tick_count = point.tick;

		G::UserCmd->viewangles = aimangle;
		G::SendPacket = true;
	}

	return true;
}

bool c_ragebot::hitchance(Vector aimangle) {
	auto hc = float(c_config::get()->i["rage_hitchance_value"]);
	if (!c_config::get()->b["rage_hitchance"])
		hc = 1.f;

	auto wep = G::LocalPlayer->GetActiveWeapon();
	if (!wep)
		return false;

	if (wep->GetItemDefenitionIndex() == WEAPON_TASER)
		hc = 85.f;

	Vector fw, rw, uw;
	Utilities->Math_AngleVectors(aimangle, fw, rw, uw);

	int hits = 0;
	int total = int(256.f * (hc / 100.f));

	wep->UpdateAccuracyPenalty();

	float cone = wep->GetSpreadCone();
	float inacc = wep->GetInaccuracy();

	Vector src = G::LocalPlayer->GetEyePosition();

	for (int i = 0; i < 256; i++) {
		float a = Utilities->Game_RandomFloat(0.f, 1.f);
		float b = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
		float c = Utilities->Game_RandomFloat(0.f, 1.f);
		float d = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
		float inaccuracy = a * inacc;
		float spread = c * cone;

		if (wep->GetItemDefenitionIndex() == WEAPON_REVOLVER) {
			if (G::UserCmd->buttons & IN_ATTACK2) {
				a = 1.f - a * a;
				c = 1.f - c * c;
			}
		}

		Vector spread_view((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0);
		Vector direction;

		direction.x = fw.x + (spread_view.x * rw.x) + (spread_view.y * uw.x);
		direction.y = fw.y + (spread_view.x * rw.y) + (spread_view.y * uw.y);
		direction.z = fw.z + (spread_view.x * rw.z) + (spread_view.y * uw.z);
		direction.Normalized();

		Vector viewangles_spread;
		Vector view_forward;

		Utilities->Math_VectorAngles(direction, uw, viewangles_spread);
		Utilities->Math_Normalize(viewangles_spread);
		Utilities->Math_AngleVectors(viewangles_spread, view_forward);

		view_forward.NormalizeInPlace();
		view_forward = src + (view_forward * wep->get_full_info()->range);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, view_forward);
		Interfaces::Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, tempTarget, &tr);

		if (tr.m_pEnt == tempTarget)
			hits++;

		if ((float(hits) / 256.f) * 100.f >= hc)
			return true;

		if ((256 - i + hits) < total)
			return false;
	}

	return false;
}

bool c_ragebot::is_able_to_shoot() {
	if (!G::LocalPlayer->GetActiveWeapon())
		return false;

	return !(G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() > G::LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick);
}

void c_ragebot::multipoints(int hitbox, matrix3x4_t bones[126], std::vector<hitpoint>& points, int tick, record bt) {
	auto cfg = c_config::get();
	auto mdl = Interfaces::ModelInfo->GetStudioModel(tempTarget->GetModel());
	auto set = mdl->pHitboxSet(tempTarget->HitBoxSet());
	auto hbx = set->GetHitbox(hitbox);

	if (!hbx)
		return;

	Vector mins, maxs;
	Utilities->Math_VectorTransform(hbx->bbmin, bones[hbx->bone], mins);
	Utilities->Math_VectorTransform(hbx->bbmax, bones[hbx->bone], maxs);

	Vector center = (mins + maxs) * 0.5f;
	Vector angle = Utilities->Math_CalcAngle(center, G::LocalPlayer->GetEyePosition());

	Vector forward;
	Utilities->Math_AngleVectors(angle, forward);

	Vector right = forward.Cross(Vector(0, 0, 1));
	Vector left = Vector(-right.x, -right.y, right.z);
	Vector top = Vector(0, 0, 1);
	Vector bottom = Vector(0, 0, -1);

	float adjusted_radius = 0.f;
	switch (hitbox) {
	case HITBOX_HEAD:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius, tick, bt });
			points.push_back({ center + right * adjusted_radius, tick, bt });
			points.push_back({ center + left * adjusted_radius, tick, bt });
		}
		break;

	case HITBOX_NECK:
		adjusted_radius = hbx->radius * float(cfg->i["rage_neckselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + right * adjusted_radius, tick, bt });
			points.push_back({ center + left * adjusted_radius, tick, bt });
		}
		break;

	case HITBOX_CHEST:
		adjusted_radius = hbx->radius * float(cfg->i["rage_chestselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + right * adjusted_radius, tick, bt });
			points.push_back({ center + left * adjusted_radius, tick, bt });
			points.push_back({ center + bottom * adjusted_radius, tick, bt });
		}
		break;

	case HITBOX_UPPER_CHEST:
		adjusted_radius = hbx->radius * float(cfg->i["rage_chestselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius, tick, bt });
			points.push_back({ center + right * adjusted_radius, tick, bt });
			points.push_back({ center + left * adjusted_radius, tick, bt });
		}
		break;
		
	case HITBOX_BODY:
		adjusted_radius = hbx->radius * float(cfg->i["rage_pelvisselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius, tick, bt });
			points.push_back({ center + right * adjusted_radius, tick, bt });
			points.push_back({ center + left * adjusted_radius, tick, bt });
		}
		break;

	case HITBOX_PELVIS:
		adjusted_radius = hbx->radius * float(cfg->i["rage_pelvisselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + right * adjusted_radius, tick, bt });
			points.push_back({ center + left * adjusted_radius, tick, bt });
			points.push_back({ center + bottom * adjusted_radius, tick, bt });
		}
		break;

	case HITBOX_LEFT_FOREARM:
	case HITBOX_RIGHT_FOREARM:
		adjusted_radius = hbx->radius * float(cfg->i["rage_armsselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius, tick, bt });
			points.push_back({ center + bottom * adjusted_radius, tick, bt });
		}
		break;

	case HITBOX_LEFT_CALF:
	case HITBOX_RIGHT_CALF:
		adjusted_radius = hbx->radius * float(cfg->i["rage_legsselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius, tick, bt });
			points.push_back({ center + bottom * adjusted_radius, tick, bt });
		}
		break;
	}
}

hitpoint c_ragebot::hitscan(int* estimated_damage) {
	static DWORD addr = (DWORD)Cheat::Utilities->Memory_PatternScan("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
	unsigned long ModelBoneCounter = **(unsigned long**)(addr + 10);

	std::vector<hitpoint> points = {};

	tempTarget->InvalidateBoneCache();

	auto cfg = c_config::get();
	auto wep = G::LocalPlayer->GetActiveWeapon();
	auto wepdmg = wep->get_full_info()->damage;
	auto wepidx = wep->GetItemDefenitionIndex();
	auto enemyhp = tempTarget->GetHealth();
	auto ent = tempTarget->GetIndex();

	auto force_baim = false;
	if (cfg->b["rage_baimonlowhp"] && enemyhp <= wepdmg)
		force_baim = true;
	if (cfg->b["rage_awpbodyaim"] && wepidx == WEAPON_AWP)
		force_baim = true;
	if (c_keyhandler::get()->auto_check("rage_forcebaim_key"))
		force_baim = true;
	if (c_playerlist::get()->should_prefer_body(tempTarget))
		force_baim = true;
	if (wepidx == WEAPON_TASER || wep->is_knife())
		force_baim = true;
	if (cfg->m["rage_points"][3]) {
		switch (cfg->i["rage_baimpreference"]) {
		case 1:
			if (tempTarget->GetVelocity().Length2D() > 1)
				force_baim = true;
			break;

		case 2:
			if ((tempTarget->GetVelocity().Length2D() > 1 && tempTarget->GetVelocity().Length2D() < 110) || !(tempTarget->GetFlags() & FL_ONGROUND))
				force_baim = true;
			break;
		}
	}

	matrix3x4_t bones[126];
	tempTarget->setup_bone_matrix(tempTarget, bones);

	std::deque<record> records;

	if (cfg->i["rage_fakelagfix"] > 0) {
		auto ticks = c_backtrack::get()->players[ent].get_valid_records();

		int tmp = 0;
		if (ticks.size() > 0) {
			switch (cfg->i["rage_fakelagfix"])
			{
			case 1:
				records.push_back(ticks.back());
				break;

			case 2:
				records.push_back(ticks.at(ticks.size() / 2));
				records.push_back(ticks.back());

				break;

			case 3:
				tmp = ticks.size() / 4;

				for (int i = 0; i < tmp; i++)
					records.push_back(ticks.at(tmp * i));

				break;
			}
		}
	}

	auto lerp = c_backtrack::get()->get_lerp_time();
	if (cfg->m["rage_points"][0] && !force_baim) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_HEAD), G::UserCmd->tick_count });
		this->multipoints(HITBOX_HEAD, bones, points, G::UserCmd->tick_count, record());
	}

	if (cfg->m["rage_points"][1] && !force_baim) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_NECK), G::UserCmd->tick_count });

		this->multipoints(HITBOX_NECK, bones, points, G::UserCmd->tick_count, record());
	}

	if (cfg->m["rage_points"][2] && !force_baim) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_CHEST), G::UserCmd->tick_count });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_UPPER_CHEST), G::UserCmd->tick_count });

		this->multipoints(HITBOX_CHEST, bones, points, G::UserCmd->tick_count, record());
		this->multipoints(HITBOX_UPPER_CHEST, bones, points, G::UserCmd->tick_count, record());
	}

	if (cfg->m["rage_points"][3] || force_baim) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_BODY), G::UserCmd->tick_count });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_PELVIS), G::UserCmd->tick_count });

		this->multipoints(HITBOX_BODY, bones, points, G::UserCmd->tick_count, record());
		this->multipoints(HITBOX_PELVIS, bones, points, G::UserCmd->tick_count, record());
	}

	if (cfg->m["rage_points"][4] && !force_baim) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_FOREARM), G::UserCmd->tick_count });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_FOREARM), G::UserCmd->tick_count });

		this->multipoints(HITBOX_LEFT_FOREARM, bones, points, G::UserCmd->tick_count, record());
		this->multipoints(HITBOX_RIGHT_FOREARM, bones, points, G::UserCmd->tick_count, record());
	}

	if (cfg->m["rage_points"][5] && !force_baim) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_CALF), G::UserCmd->tick_count });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_CALF), G::UserCmd->tick_count });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_THIGH), G::UserCmd->tick_count });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_THIGH), G::UserCmd->tick_count });

		this->multipoints(HITBOX_LEFT_CALF, bones, points, G::UserCmd->tick_count, record());
		this->multipoints(HITBOX_RIGHT_CALF, bones, points, G::UserCmd->tick_count, record());
		this->multipoints(HITBOX_LEFT_THIGH, bones, points, G::UserCmd->tick_count, record());
		this->multipoints(HITBOX_RIGHT_THIGH, bones, points, G::UserCmd->tick_count, record());
	}

	auto head_selected = cfg->m["rage_points"][0];
	auto body_selected = cfg->m["rage_points"][3];
	for (auto rd : records) {
		if (head_selected && !force_baim)
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_HEAD, rd.bones), TIME_TO_TICKS(rd.simtime + lerp), rd });
	
		if (body_selected || force_baim) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_BODY, rd.bones), TIME_TO_TICKS(rd.simtime + lerp), rd });
			this->multipoints(HITBOX_BODY, rd.bones, points, TIME_TO_TICKS(rd.simtime + lerp), rd);
		}
	}

	auto wallbang_min = -1;
	if (cfg->b["rage_automaticpenetration"]) {
		wallbang_min = cfg->i["rage_automaticpenetration_damage"];
		if (wallbang_min == 0) {
			if (wepdmg > enemyhp)
				wallbang_min = enemyhp + 5;
			else
				wallbang_min = (wepdmg / 2) + 5;
		}
	}

	auto damage_min = cfg->i["rage_minimaldamage"];
	if (damage_min == 0) {
		if (wepdmg > enemyhp)
			damage_min = enemyhp + 5;
		else
			damage_min = (wepdmg / 2) + 5;
	}
	else if (damage_min > 100)
		damage_min = enemyhp + (damage_min - 100);

	if (wepidx == WEAPON_TASER)
		damage_min = enemyhp + 26;

	auto best_damage = 0;
	hitpoint best_point;

	for (auto point : points) {
		auto isvisible = G::LocalPlayer->point_visible(point.pos);
		auto mindamage = 0;
		if (wallbang_min > -1 && !isvisible)
			mindamage = wallbang_min;
		else if (isvisible)
			mindamage = damage_min;

		if (mindamage == 0)
			continue;

		unsigned long ulMostRecentModelBoneCounter = *tempTarget->ulpMostRecentModelBoneCounter();
		matrix3x4_t *pCachedBoneData = *tempTarget->ppCachedBoneData();

		auto is_bt = point.tick != G::UserCmd->tick_count;

		record old;
		if (is_bt) {
			tempTarget->InvalidateBoneCache();

			*tempTarget->ulpMostRecentModelBoneCounter() = 126;
			*tempTarget->ppCachedBoneData() = point.bt.bones;	
		}

		auto aw_dmg = c_autowall::get()->get_point_damage(point.pos, tempTarget);
		if (aw_dmg >= best_damage) {
			best_damage = aw_dmg;
			best_point = point;
		}

		if (is_bt) {
			tempTarget->InvalidateBoneCache();

			*tempTarget->ulpMostRecentModelBoneCounter() = ulMostRecentModelBoneCounter;
			*tempTarget->ppCachedBoneData() = pCachedBoneData;
		}
	}

	if (estimated_damage)
		*estimated_damage = best_damage;

	return best_point;
}

void c_ragebot::quick_stop() {
	if (G::LocalPlayer->GetVelocity().Length() > 3.1f && (G::LocalPlayer->GetFlags() & FL_ONGROUND) || c_keyhandler::get()->auto_check("rage_quickstop_key")) {
		if (!G::LocalPlayer->GetActiveWeapon())
			return;
		if (G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() - Interfaces::Globals->curtime > 0.f)
			return;
		if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
			return;

		Vector velocity = G::UnpredictedVelocity;
		Vector direction;
		Utilities->Math_VectorAngles(velocity, direction);
		float speed = velocity.Length();

		direction.y = G::UserCmd->viewangles.y - direction.y;

		Vector negated_direction = direction.Forward() * -speed;

		G::UserCmd->forwardmove = negated_direction.x;
		G::UserCmd->sidemove = negated_direction.y;
		G::UserCmd->upmove = negated_direction.z;
	}
}