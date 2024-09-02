#pragma once
#include "../../singleton.h"
#include "../../Configuration/Config.h"
#include "../../SDK/CBaseEntity.h"

class c_resolver : public singleton<c_resolver>
{
public:
	void run();
private:
	float anti_freestand(CBaseEntity* e);
	bool ajusting_balance(CBaseEntity* e);

	bool did_store_yaw[65];
	float stored_yaws[65];
};