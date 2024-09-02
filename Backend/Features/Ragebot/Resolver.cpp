#include "Resolver.h"

void c_resolver::run() {
	if (!c_config::get()->b["rage_resolver"])
	return;

	for (int i = 0; i < 64; i++) {
		auto e = Interfaces::EntityList->GetClientEntity(i);
		if (e) {
			if (e->IsAlive() && e->GetHealth() > 0 && !e->IsDormant()) {
				if (e->GetVelocity().Length() > 110.f && e->GetFlags() & FL_ONGROUND) {
					e->GetEyeAnglesPtr()->y = e->GetLowerBodyYaw();
				}
				else if (e->GetFlags() & FL_ONGROUND) {

					if (G::MissedShots[i] == 0) {
						if (this->ajusting_balance(e)) {
							stored_yaws[i] = e->GetLowerBodyYaw();
							did_store_yaw[i] = true;
						}

						if (did_store_yaw[i])
							e->GetEyeAnglesPtr()->y = stored_yaws[i];
					}
					else if (G::MissedShots[i] == 2)
						e->GetEyeAnglesPtr()->y = stored_yaws[i];
					else if (G::MissedShots[i] > 2) {
						auto f = stored_yaws[i];
						if (f > 0)
							e->GetEyeAnglesPtr()->y = f + (G::ResolverAdd[i] / 2); // тут 
						else if (f < 0) {
							e->GetEyeAnglesPtr()->y = f - (G::ResolverAdd[i] / 2); // и тут 
						}
						else {
							switch (G::MissedShots[i] % 2) {
							case 0:
								e->GetEyeAnglesPtr()->y = f - (G::ResolverAdd[i] / 2);
								break;
							case 1:
								e->GetEyeAnglesPtr()->y = f + (G::ResolverAdd[i] / 2);
								break;
							}
						}
					}

				}
			}
			else {
				G::MissedShots[i] = 0;
				G::ResolverAdd[i] = 0;

				did_store_yaw[i] = false;
			}
		}
		else {
			G::MissedShots[i] = 0;
			G::ResolverAdd[i] = 0;

			did_store_yaw[i] = false;
		}
	}
}


bool c_resolver::ajusting_balance(CBaseEntity* e) {

	for (int i = 0; i < e->GetNumAnimOverlays(); i++)
	{
		const int activity = e->GetSequenceActivity(e->GetAnimOverlay(i).m_nSequence);
		if (activity == 979)
			return true;
	}
	return false;
}