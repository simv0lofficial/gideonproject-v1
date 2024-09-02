#pragma once
#include "../../singleton.h"
#include "../../Configuration/Config.h"
#include "../../Hooking/KeyHandler.h"

#include "../../SDK/CBaseEntity.h"

#include "CAutoWall.h"
#include "c_backtrack.h"

#define VECTOR_MAX Vector(FLT_MAX, FLT_MAX, FLT_MAX)

struct hitpoint {
	Vector pos = VECTOR_MAX;
	int tick = 0;
	record bt;
};

class c_ragebot : public singleton<c_ragebot> {
public:
	void create_move();
	void clear_bonelist();
	void clear_bones(int ent);

private:
	bool aim_at_point(hitpoint point);
	bool hitchance(Vector point);
	bool is_able_to_shoot();

	hitpoint hitscan(int* estimated_damage = nullptr);
	void multipoints(int hitbox, matrix3x4_t bones[126], std::vector<hitpoint>& points, int tick, record bt);

	void quick_stop();

	CBaseEntity* target = nullptr;
	CBaseEntity* tempTarget = nullptr;

	Vector oldViewangle = Vector(0, 0, 0);
	bool shouldRevertViewangle = false;
	bool shouldForceDistanceSelection = false;

	hitpoint oldPoint[65];
};