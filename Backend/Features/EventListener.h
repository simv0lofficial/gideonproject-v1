#pragma once
#include "../SDK/GameEvents.h"
#include "../SDK/CBaseEntity.h"
#include "../Include/GlobalIncludes.h"
#include "../SDK/GlobalDefenitions.h"
#include "../SDK/Cbeam.h"

#define EVENT_HOOK( x )

class CGameEvents : public IGameEventListener
{
	public:
		void InitializeEventListeners();
		void FireGameEvent( IGameEvent* game_event );
		void Register();
		int GetEventDebugID();
};

extern CGameEvents g_pGameEvents;

class CMyBulletTracer
{
	public:
		void CollectTracers( IGameEvent* pEvent );
		void DrawTracers();
		void ClearTracers();
	private:
		class CMyBulletTracer_info
		{
			public:
				CMyBulletTracer_info( Vector src, Vector dst, float time, CColor colorLine, CColor ColorBox )
				{
					this->src = src;
					this->dst = dst;
					this->time = time;
					this->color1 = colorLine;
					this->color2 = ColorBox;
				}

				Vector src, dst;
				float time;
				CColor color1;
				CColor color2;
				Beam_t* beamInfo;
		};

		std::vector<CMyBulletTracer_info> BulletTracerData;
};

class CSoundEsp {
public:
	void CollectSteps(IGameEvent* pEvent);
	void DrawSteps();
	void ClearSteps();
	void AddStepManually(int ent, Vector origin);
private:
	class CSoundEsp_info {
	public:
		CSoundEsp_info(Vector origin, float time, CColor color) {
			this->origin = origin;
			this->time = time;
			this->color = color;
		}

		Vector origin;
		float time;
		CColor color;
		Beam_t* beamInfo;
	};

	std::vector<CSoundEsp_info> StepsData;
};

extern CSoundEsp* SoundEsp;
extern CMyBulletTracer* BulletTracer;
