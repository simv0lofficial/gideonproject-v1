#pragma once
#include "../../singleton.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/AnimLayer.h"
#include "../../SDK/CBaseAnimState.h"
#include "../../SDK/handle.h"
#include <array>
struct AnimationInfo
{
	AnimationInfo()
	{
		m_flSpawnTime = 0.f;
		m_ulEntHandle = nullptr;
		m_playerAnimState = nullptr;
	}

	std::array<float_t, 24> m_flPoseParameters;
	CAnimationLayer m_AnimationLayer[15];

	float_t m_flSpawnTime;
	CBaseHandle *m_ulEntHandle;

	CBaseAnimState *m_playerAnimState;
};

class Animation : public singleton<Animation>
{
public:
	void UpdatePlayerAnimations(int32_t idx);

	AnimationInfo &GetPlayerAnimationInfo(int32_t idx);

private:

	AnimationInfo arr_infos[65];
};