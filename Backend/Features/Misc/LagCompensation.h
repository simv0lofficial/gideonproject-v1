#pragma once
#include "../../SDK/IEngine.h"
#include "../../SDK/CBaseEntity.h"

namespace Features
{
	struct lbyRecords
	{
		int tick_count;
		float lby;
		Vector headPosition;
	};

	struct backtrackData
	{
		float simtime;
		Vector hitboxPos;
	};

	class CLagCompensation
	{
		public:
			void Update( int tickCount );
			void CompensateLegit( CUserCmd* cmd, CBaseEntity* pLocal );

			lbyRecords records[64];

			backtrackData headPositions[64][12];
		private:
			void UpdateRecord( int i );
			bool IsTickValid( int tick );

			int latest_tick;
	};

	extern CLagCompensation* LagCompensation;
}