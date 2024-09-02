#include "LagCompensation.h"
#include "../../Configuration/Config.h"
#include "../../SDK/INetChannelInfo.h"
#include "../../SDK/player_info_t.h"
#include "../Legitbot/Aimbot.h"

using namespace Features;
using namespace Cheat;
CLagCompensation* Features::LagCompensation = new CLagCompensation();

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define ConfigGun(t, x) case t: \
	return x;


inline float distance_point_to_line( Vector Point, Vector LineOrigin, Vector Dir )
{
	auto PointDir = Point - LineOrigin;
	auto TempOffset = PointDir.Dot( Dir ) / ( Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z );

	if ( TempOffset < 0.000001f )
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + ( Dir * TempOffset );
	return ( Point - PerpendicularPoint ).Length();
}

void CLagCompensation::Update( int tick_count )
{
	if ( !c_config::get()->b["legit_lagcomp"] /*|| !Features::LegitAimbot->IsEnable() || !weapon( G::LocalPlayer->GetActiveWeapon() ).Aimbot_Active*/ )
		return;

	latest_tick = tick_count;

	for ( int i = 0; i < 64; i++ )
		UpdateRecord( i );
}

bool CLagCompensation::IsTickValid( int tick )
{
	int delta = latest_tick - tick;
	float deltaTime = delta * Interfaces::Globals->interval_per_tick;
	return ( fabs( deltaTime ) <= 0.2f );
}

void CLagCompensation::UpdateRecord( int i )
{
	CBaseEntity* pEntity = Interfaces::EntityList->GetClientEntity( i );

	if ( pEntity && pEntity->IsAlive() && !pEntity->IsDormant() )
	{
		float lby = pEntity->GetLowerBodyYaw();

		if ( lby != records[i].lby )
		{
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].headPosition = pEntity->GetHitboxPosition( HITBOX_HEAD );
		}
	}

	else
		records[i].tick_count = 0;
}

void CLagCompensation::CompensateLegit( CUserCmd* cmd, CBaseEntity* pLocal )
{
	if ( c_config::get()->b["legit_lagcomp"] /*&& Features::LegitAimbot->IsEnable() && weapon( G::LocalPlayer->GetActiveWeapon() ).Aimbot_Active*/ )
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		player_info_t info;

		if ( !pLocal->IsAlive() )
			return;

		for ( int i = 0; i < Interfaces::Engine->GetMaxClients(); i++ )
		{
			auto entity = Interfaces::EntityList->GetClientEntity( i );

			if ( !entity || !pLocal )
				continue;

			if ( entity == pLocal )
				continue;

			if ( !Interfaces::Engine->GetPlayerInfo( i, &info ) )
				continue;

			if ( entity->IsDormant() )
				continue;

			if ( entity->GetTeam() == pLocal->GetTeam() && !c_config::get()->b["legit_friendlyfire"])
				continue;

			if ( entity->IsAlive() )
			{
				float simtime = entity->GetSimTime();
				Vector hitboxPos = entity->GetHitboxPosition( HITBOX_HEAD );
				headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				/*bestFov = Features::LegitAimbot->GetPlayerFov( entity );
				bestTargetIndex = Features::LegitAimbot->GetBestTarget();*/
			}
		}

		float bestTargetSimTime;

		if ( bestTargetIndex != -1 )
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir;
			Utilities->Math_AngleVectors( cmd->viewangles + ( pLocal->GetPunchAngles() * 2.f ), ViewDir );

			for ( int t = 0; t < 12; ++t )
			{
				float tempFOVDistance = distance_point_to_line( headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir );

				if ( headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1 )
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}

			if ( cmd->buttons & IN_ATTACK )
				cmd->tick_count = TIME_TO_TICKS( bestTargetSimTime );
		}
	}
}