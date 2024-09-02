#pragma once
#include <deque>

#include "../../singleton.h"
#include "../../Interfaces/Interfaces.h"
#include "../../Configuration/Config.h"

#include "../../SDK/CBaseEntity.h"
#include "../../SDK/INetChannelInfo.h"

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / Interfaces::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t ) ( Interfaces::Globals->interval_per_tick * ( t ) )

struct record {
	float time;
	float simtime;

	Vector pos;
	Vector abs;

	matrix3x4_t bones[126];

	void set_data(CBaseEntity* ent);
};

struct track {
	CBaseEntity* entity;

	void add_record();

	std::deque<record> get_valid_records();
	std::deque<record> data;
};

class c_backtrack : public singleton<c_backtrack> {
public:
	void frame_stage_notify(int stage);
	float get_lerp_time();

	track players[65];
};