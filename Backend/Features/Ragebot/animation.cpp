#include "animation.h"
#include "../../SDK/CBaseEntity.h"

void Animation::UpdatePlayerAnimations(int32_t idx)
{
	CBaseEntity *player = Interfaces::EntityList->GetClientEntity(idx);
	if (!player || !player->IsAlive())
		return;

	auto &info = arr_infos[idx];

	bool
		allocate = (info.m_playerAnimState == nullptr),
		change = (!allocate) && (const_cast<CBaseHandle*>(&player->GetRefEHandle()) != info.m_ulEntHandle),
		reset = (!allocate && !change) && (player->m_flSpawnTime() != info.m_flSpawnTime);

	// player changed, free old animation state.
	if (change)
		Interfaces::MemAlloc->Free(info.m_playerAnimState);

	// need to reset? (on respawn)
	if (reset)
	{
		// reset animation state.
		CBaseEntity::ResetAnimationState(info.m_playerAnimState);

		// note new spawn time.
		info.m_flSpawnTime = player->m_flSpawnTime();
	}

	// need to allocate or create new due to player change.
	if (allocate || change)
	{
		CBaseAnimState *state = (CBaseAnimState*)Interfaces::MemAlloc->Alloc(sizeof(CBaseAnimState));

		if (state != nullptr)
			player->CreateAnimationState(state);

		// used to detect if we need to recreate / reset.
		info.m_ulEntHandle = const_cast<CBaseHandle*>(&player->GetRefEHandle());
		info.m_flSpawnTime = player->m_flSpawnTime();

		// note anim state for future use.
		info.m_playerAnimState = state;
	}

	if (!info.m_playerAnimState)
		return;

	std::array<float_t, 24> backup_poses = player->m_flPoseParameter();

	CAnimationLayer backup_layers[15];
	std::memcpy(backup_layers, player->GetAnimOverlays(), (sizeof(CAnimationLayer) * player->GetNumAnimOverlays()));

	// fixing legs and few other things missing here
	CBaseEntity::UpdateAnimationState(info.m_playerAnimState, player->GetEyeAngles());

	info.m_flPoseParameters = player->m_flPoseParameter();
	std::memcpy(info.m_AnimationLayer, player->GetAnimOverlays(), (sizeof(CAnimationLayer) * player->GetNumAnimOverlays()));

	player->m_flPoseParameter() = backup_poses;
	std::memcpy(player->GetAnimOverlays(), backup_layers, (sizeof(CAnimationLayer) * player->GetNumAnimOverlays()));
}

AnimationInfo &Animation::GetPlayerAnimationInfo(int32_t idx)
{
	return arr_infos[idx];
}