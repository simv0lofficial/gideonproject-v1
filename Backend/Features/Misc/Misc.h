#pragma once
#include "../../Include/GlobalIncludes.h"
#include "../../SDK/CBaseEntity.h"
#include "../../Configuration/Config.h"
#include "../../Configuration/Settings.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/player_info_t.h"
#include "../../Include/GlobalIncludes.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CUserCmd.h"
#include "../Legitbot/Aimbot.h"
#include "../../SDK/player_info_t.h"
#include "../../Utility/Utilities.h"

#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) /  Interfaces::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( dt )		( Interfaces::Globals->interval_per_tick * (float)(dt) )

struct PlayerListSett
{
	float yaw;
	float pitch;
	bool bodyaim;
	bool prioritize;
	bool apply;
	bool selected;
	player_info_t info;
	CBaseEntity* playerptr;
};
extern PlayerListSett playerlistarr[64];



namespace Features
{
	class CMisc
	{
		public:
			 void Run();
			 void SetClanTag( const char* tag, const char* name );
			 void FakeLag();
			 void SlideWalk();
		private:
			void BunnyHop();
			void KnifeBot();
	public:
			void ClanTagChanger();
	private:
			void ServerRankRevealAll();
			void AutoWeapons();

			Vector circleStrafeDbg;
			Vector d_col;
			Vector d_colL;
			Vector d_colR;
			Vector oldAngles;
			bool didSwitch = false;
			bool usingTick;
	};

	extern CMisc* Misc;
}

using namespace Features;

#include "../../Hooking/KeyHandler.h"

inline void CMisc::Run()
{
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	FakeLag();

	if (c_config::get()->b["misc_autojump"] && G::LocalPlayer)
		BunnyHop();

	if (c_config::get()->b["misc_revealranks"])
		ServerRankRevealAll();

	if (c_config::get()->b["misc_automaticwpns"] && G::LocalPlayer)
		AutoWeapons();

	if (c_config::get()->b["misc_knifebot"] && G::LocalPlayer)
		KnifeBot();

	if (c_config::get()->b["misc_infiniteduck"])
		G::UserCmd->buttons |= IN_BULLRUSH;

	if (c_config::get()->b["misc_lagexploit"] && c_keyhandler::get()->auto_check("misc_lagexploit_key"))
		Interfaces::Engine->ClientCmd_Unrestricted("rcon");
}

inline void CMisc::SetClanTag(const char* tag, const char* name)
{
	typedef void(__fastcall * ClanTagFn)(const char*, const char*);
	static ClanTagFn fn_ClanTag;
	if (!fn_ClanTag)
		fn_ClanTag = reinterpret_cast<ClanTagFn>(offys.dwSetClanTag);

	if (fn_ClanTag)
		fn_ClanTag(tag, name);
}

inline void CMisc::SlideWalk() {
	auto move_type = G::LocalPlayer->GetMoveType();

	if (move_type != 9) {
		if (!(G::UserCmd->buttons & IN_USE)) {
			if (G::UserCmd->forwardmove > 0)
			{
				G::UserCmd->buttons |= IN_BACK;
				G::UserCmd->buttons &= ~IN_FORWARD;
			}

			if (G::UserCmd->forwardmove < 0)
			{
				G::UserCmd->buttons |= IN_FORWARD;
				G::UserCmd->buttons &= ~IN_BACK;
			}

			if (G::UserCmd->sidemove < 0)
			{
				G::UserCmd->buttons |= IN_MOVERIGHT;
				G::UserCmd->buttons &= ~IN_MOVELEFT;
			}

			if (G::UserCmd->sidemove > 0)
			{
				G::UserCmd->buttons |= IN_MOVELEFT;
				G::UserCmd->buttons &= ~IN_MOVERIGHT;
			}
		}
	}
}

#define ct(a, x) case a: SetClanTag(x, x); break;

inline void CMisc::ClanTagChanger()
{
	static int oldcurtime = 0;

	if (oldcurtime != (int)(Interfaces::Globals->curtime * 2.5f) % 29) {
		switch ((int)(Interfaces::Globals->curtime * 2.5f) % 29)
		{
			ct(0, "gideonproject");
			ct(1, "gideonproject");
			ct(2, "gideonproject");
			ct(3, "gideonproject");
			ct(4, "ideonproject  ");
			ct(5, "deonproject    ");
			ct(6, "eonproject      ");
			ct(7, "onproject        ");
			ct(8, "nproject          ");
			ct(9, "project            ");
			ct(10, "roject              ");
			ct(11, "oject                ");
			ct(12, "ject                  ");
			ct(13, "ect                    ");
			ct(14, "ct                      ");
			ct(15, "t                        ");
			ct(16, "                          ");
			ct(17, "                        g");
			ct(18, "                      gi");
			ct(19, "                    gid");
			ct(20, "                  gide");
			ct(21, "                gideo");
			ct(22, "              gideon");
			ct(23, "            gideonp");
			ct(24, "          gideonpr");
			ct(25, "        gideonpro");
			ct(26, "      gideonproj");
			ct(27, "    gideonproje");
			ct(28, "  gideonprojec");
		}

		oldcurtime = (int)(Interfaces::Globals->curtime * 2.5f) % 29;
	}
}

inline void CMisc::ServerRankRevealAll()
{
	if (!G::UserCmd->buttons& IN_SCORE)
		return;

	using MsgFunc_ServerRankRevealAllFn = bool(__cdecl*)(float*);
	static MsgFunc_ServerRankRevealAllFn ServerRankRevealAll = reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(offys.dwServerRankRevealAll);
	float fArray[3] = { 0.f };
	ServerRankRevealAll(fArray);
}

inline void CMisc::AutoWeapons()
{
	if (G::LocalPlayer->GetActiveWeapon()->GetAmmoInClip() <= 0 && !G::LocalPlayer->GetActiveWeapon()->is_knife() && G::UserCmd->buttons & IN_ATTACK)
	{
		auto type = G::LocalPlayer->GetActiveWeapon()->Type();

		if ((type == (int)WeaponType::WEAPONTYPE_MACHINEGUN || type == (int)WeaponType::WEAPONTYPE_RIFLE || type == (int)WeaponType::WEAPONTYPE_SHOTGUN ||
			type == (int)WeaponType::WEAPONTYPE_SNIPER_RIFLE || type == (int)WeaponType::WEAPONTYPE_SUBMACHINEGUN) && !didSwitch) {
			Interfaces::Engine->ClientCmd_Unrestricted("slot2");
			didSwitch = true;
		}
	}
	else
		didSwitch = false;
}

inline void CMisc::KnifeBot()
{
	if (G::LocalPlayer->GetActiveWeapon()->is_knife())
	{
		for (int PlayerIndex = 0; PlayerIndex < 64; PlayerIndex++)
		{
			CBaseEntity* pPlayer = Interfaces::EntityList->GetClientEntity(PlayerIndex);

			if (pPlayer)
			{
				if (pPlayer->GetTeam() == G::LocalPlayer->GetTeam())
					continue;

				Vector vHead = pPlayer->GetHitboxPosition(HITBOX_HEAD);
				Vector vOrigin = pPlayer->GetAbsOrigin();
				Vector out1;
				Vector out2;

				if (Cheat::Utilities->Game_WorldToScreen(vOrigin, out1) && Cheat::Utilities->Game_WorldToScreen(vHead, out2))
				{
					bool bOriginScreen = (out1.x > 0 && out1.y > 0);
					bool bHitBoxScreen = (vHead.x > 0 && vHead.y > 0);

					if (bOriginScreen || bHitBoxScreen)
					{
						int MinDistance = 75;
						int a = int(G::LocalPlayer->GetOrigin().DistTo(pPlayer->GetOrigin()));
						int DistanceToPlayer = a;

						if (DistanceToPlayer > MinDistance)
							continue;

						if (DistanceToPlayer > 64)
							G::UserCmd->buttons |= IN_ATTACK;

						else
							G::UserCmd->buttons |= IN_ATTACK2;
					}
				}
			}
		}
	}
}

inline bool GetWorldIntersection(Vector start, Vector direction, float range, CBaseEntity * skip, trace_t & trace = trace_t())
{
	Ray_t ray;
	CTraceFilter filter;
	Vector forward;
	Cheat::Utilities->Math_AngleVectors(direction, forward);
	forward.NormalizeInPlace();
	Vector end = start + forward * range;
	ray.Init(start, end);
	filter.pSkip1 = skip;
	Interfaces::Trace->TraceRay(ray, MASK_SOLID, (ITraceFilter*)& filter, &trace);

	if (trace.m_pEnt&& trace.m_pEnt->GetTeam() == G::LocalPlayer->GetTeam())
		return false;

	return trace.fraction != 1.0f;
}

__forceinline void selfinterpolate() {
	const auto var_map = reinterpret_cast<uintptr_t>(G::LocalPlayer) + 36;

	for (auto index = 0; index < *reinterpret_cast<int*>(var_map + 20); index++)
		* reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(var_map) + index * 12) = 0;
}

inline void CMisc::FakeLag()
{
	if (!G::LocalPlayer || !G::LocalPlayer->IsAlive() || G::LocalPlayer->GetHealth() <= 0 || !G::FakelagShouldLag)
		return;

	auto nc = Interfaces::Engine->GetNetChannel();
	if (!nc)
		return;

	if (Interfaces::Engine->IsVoiceRecording()) {
		G::SendPacket = !G::SendPacket;
		return;
	}

	if (c_config::get()->i["rage_fakelag"] && (c_config::get()->m["rage_fakelag_conds"][0] || c_config::get()->m["rage_fakelag_conds"][1] || c_config::get()->m["rage_fakelag_conds"][2]) && c_config::get()->i["rage_fakelag_limit"]) {
		auto shouldLag = false;
		auto max = c_config::get()->i["rage_fakelag_limit"];

		if (c_config::get()->m["rage_fakelag_conds"][0] && G::LocalPlayer->GetVelocity().Length2D() <= 0.1f && G::LocalPlayer->GetFlags() & FL_ONGROUND)
			shouldLag = true;

		if (c_config::get()->m["rage_fakelag_conds"][1] && G::LocalPlayer->GetVelocity().Length2D() > 10.f) {
			shouldLag = true;

			if (c_config::get()->i["rage_fakelag"] == 2)
				max = int(float(max) / 360.f * G::LocalPlayer->GetVelocity().Length2D());
		}
		if (c_config::get()->m["rage_fakelag_conds"][2] && !(G::LocalPlayer->GetFlags() & FL_ONGROUND)) {
			shouldLag = true;

			if (c_config::get()->i["rage_fakelag"] == 2)
				max = int(float(max) / 450.f * G::LocalPlayer->GetVelocity().Length2D());
		}

		if (shouldLag) {
			auto min = float(max) - float(max) / 100.f * float(c_config::get()->i["rage_fakelag_variance"]);
			auto limit = min + (rand() % static_cast<int>(max - min + 1));

			if (nc->m_nChokedPackets < limit) {
				G::SendPacket = false;
				return;
			}
		}

		G::SendPacket = !G::SendPacket;
		return;
	} else
		G::SendPacket = !G::SendPacket;
}

const double M_U_DEG = 360.0 / 65536;
const double M_U_RAD = M_PI / 32768;

#define PI_2 M_PI/2
#define PI_4 M_PI/4

class AutoStrafer
{
		double anglemod_deg( double a )
		{
			return M_U_DEG * ( ( int )( a / M_U_DEG ) & 0xffff );
		}

		double anglemod_rad( double a )
		{
			return M_U_RAD * ( ( int )( a / M_U_RAD ) & 0xffff );
		}

		double point2line_distsq( const double pos[2], const double line_origin[2], const double line_dir[2] )
		{
			double tmp[2] = { line_origin[0] - pos[0], line_origin[1] - pos[1] };
			double dotprod = line_dir[0] * tmp[0] + line_dir[1] * tmp[1];
			tmp[0] -= line_dir[0] * dotprod;
			tmp[1] -= line_dir[1] * dotprod;
			return tmp[0] * tmp[0] + tmp[1] * tmp[1];
		}

		double strafe_theta_opt( double speed, double L, double tauMA )
		{
			double tmp = L - tauMA;

			if ( tmp <= 0 )
				return PI_2;

			if ( tmp < speed )
				return std::acos( tmp / speed );

			return 0;
		}

		double strafe_theta_const( double speed, double nofric_speed, double L, double tauMA )
		{
			double sqdiff = nofric_speed * nofric_speed - speed * speed;
			double tmp = sqdiff / tauMA;

			if ( tmp + tauMA < 2 * L && 2 * speed >= std::fabs( tmp - tauMA ) )
				return std::acos( ( tmp - tauMA ) / ( 2 * speed ) );

			tmp = std::sqrt( L * L - sqdiff );

			if ( tauMA - L > tmp && speed >= tmp )
				return std::acos( -tmp / speed );

			return strafe_theta_opt( speed, L, tauMA );
		}

		void strafe_fme_vec( double vel[2], const double avec[2], double L, double tauMA )
		{
			double tmp = L - vel[0] * avec[0] - vel[1] * avec[1];

			if ( tmp < 0 )
				return;

			if ( tauMA < tmp )
				tmp = tauMA;

			vel[0] += avec[0] * tmp;
			vel[1] += avec[1] * tmp;
		}

		void strafe_fric( double vel[2], double E, double ktau )
		{
			double speed = std::hypot( vel[0], vel[1] );

			if ( speed >= E )
			{
				vel[0] *= 1 - ktau;
				vel[1] *= 1 - ktau;
				return;
			}

			double tmp = E * ktau;

			if ( speed > tmp )
			{
				tmp /= speed;
				vel[0] -= tmp * vel[0];
				vel[1] -= tmp * vel[1];
				return;
			}

			vel[0] = 0;
			vel[1] = 0;
		}

		double strafe_fric_spd( double spd, double E, double ktau )
		{
			if ( spd >= E )
				return spd * ( 1 - ktau );

			double tmp = E * ktau;

			if ( spd > tmp )
				return spd - tmp;

			return 0;
		}

		void strafe_side( double &yaw, int &Sdir, int &Fdir, double vel[2], double theta, double L, double tauMA, int dir )
		{
			double phi;

			if ( theta >= PI_2 * 0.75 )
			{
				Sdir = dir;
				Fdir = 0;
				phi = std::copysign( PI_2, dir );
			}

			else
				if ( PI_2 * 0.25 <= theta && theta <= PI_2 * 0.75 )
				{
					Sdir = dir;
					Fdir = 1;
					phi = std::copysign( PI_4, dir );
				}

				else
				{
					Sdir = 0;
					Fdir = 1;
					phi = 0;
				}

			if ( std::fabs( vel[0] ) > 0.1 || std::fabs( vel[1] ) > 0.1 )
				yaw = std::atan2( vel[1], vel[0] );

			yaw += phi - std::copysign( theta, dir );
			double yawcand[2] =
			{
				anglemod_rad( yaw ), anglemod_rad( yaw + std::copysign( M_U_RAD, yaw ) )
			};
			double avec[2] = { std::cos( yawcand[0] - phi ), std::sin( yawcand[0] - phi ) };
			double tmpvel[2] = { vel[0], vel[1] };
			strafe_fme_vec( vel, avec, L, tauMA );
			avec[0] = std::cos( yawcand[1] - phi );
			avec[1] = std::sin( yawcand[1] - phi );
			strafe_fme_vec( tmpvel, avec, L, tauMA );

			if ( tmpvel[0] * tmpvel[0] + tmpvel[1] * tmpvel[1] > vel[0] * vel[0] + vel[1] * vel[1] )
			{
				vel[0] = tmpvel[0];
				vel[1] = tmpvel[1];
				yaw = yawcand[1];
			}

			else
				yaw = yawcand[0];
		}

	public:
		void strafe_side_opt( double &yaw, int &Sdir, int &Fdir, double* vel, double L, double tauMA, int dir )
		{
			double speed = std::hypot( vel[0], vel[1] );
			double theta = strafe_theta_opt( speed, L, tauMA );
			strafe_side( yaw, Sdir, Fdir, vel, theta, L, tauMA, dir );
		}

		void strafe_side_const( double &yaw, int &Sdir, int &Fdir, double vel[2], double nofricspd, double L, double tauMA, int dir )
		{
			double speed = std::hypot( vel[0], vel[1] );
			double theta = strafe_theta_const( speed, nofricspd, L, tauMA );
			strafe_side( yaw, Sdir, Fdir, vel, theta, L, tauMA, dir );
		}

		void strafe_line_opt( double &yaw, int &Sdir, int &Fdir, double vel[2], double pos[2], double L, double tau, double MA, double line_origin[2], double line_dir[2] )
		{
			double tauMA = tau * MA;
			double speed = std::hypot( vel[0], vel[1] );
			double theta = strafe_theta_opt( speed, L, tauMA );
			double ct = std::cos( theta );
			double tmp = L - speed * ct;

			if ( tmp < 0 )
			{
				strafe_side( yaw, Sdir, Fdir, vel, theta, L, tauMA, 1 );
				return;
			}

			if ( tauMA < tmp )
				tmp = tauMA;

			tmp /= speed;
			double st = std::sin( theta );
			double newpos_right[2], newpos_left[2];
			double avec[2];
			avec[0] = ( vel[0] * ct + vel[1] * st ) * tmp;
			avec[1] = ( -vel[0] * st + vel[1] * ct ) * tmp;
			newpos_right[0] = pos[0] + tau * ( vel[0] + avec[0] );
			newpos_right[1] = pos[1] + tau * ( vel[1] + avec[1] );
			avec[0] = ( vel[0] * ct - vel[1] * st ) * tmp;
			avec[1] = ( vel[0] * st + vel[1] * ct ) * tmp;
			newpos_left[0] = pos[0] + tau * ( vel[0] + avec[0] );
			newpos_left[1] = pos[1] + tau * ( vel[1] + avec[1] );
			bool rightgt = point2line_distsq( newpos_right, line_origin, line_dir ) <
			    point2line_distsq( newpos_left, line_origin, line_dir );
			strafe_side( yaw, Sdir, Fdir, vel, theta, L, tauMA, rightgt ? 1 : -1 );
		}

		void strafe_back( double &yaw, int &Sdir, int &Fdir, double vel[2], double tauMA )
		{
			Sdir = 0;
			Fdir = -1;
			yaw = std::atan2( vel[1], vel[0] );
			float frac = yaw / M_U_RAD;
			frac -= std::trunc( frac );

			if ( frac > 0.5 )
				yaw += M_U_RAD;

			else
				if ( frac < -0.5 )
					yaw -= M_U_RAD;

			yaw = anglemod_rad( yaw );
			double avec[2] = { std::cos( yaw ), std::sin( yaw ) };
			vel[0] -= tauMA * avec[0];
			vel[1] -= tauMA * avec[1];
		}

		double strafe_opt_spd( double spd, double L, double tauMA )
		{
			double tmp = L - tauMA;

			if ( tmp < 0 )
				return std::sqrt( spd * spd + L * L );

			if ( tmp < spd )
				return std::sqrt( spd * spd + tauMA * ( L + tmp ) );

			return spd + tauMA;
		}

		void MovementFix( CUserCmd* cmd, Vector &realvec )
		{
			Vector vMove( cmd->forwardmove, cmd->sidemove, cmd->upmove );
			Vector vMove2;
			float flSpeed = sqrt( vMove.x * vMove.x + vMove.y * vMove.y ), flYaw;
			Cheat::Utilities->Math_VectorAngles( vMove, vMove2 );
			flYaw = DEG2RAD( cmd->viewangles.y - realvec.y + vMove2.y );
			cmd->forwardmove = cos( flYaw ) * flSpeed;
			cmd->sidemove = sin( flYaw ) * flSpeed;

			if ( 90 < abs( cmd->viewangles.x ) )
				cmd->forwardmove *= -1;
		}

		static void BhopMovement( CUserCmd* pCmd, Vector &OrigAngles )
		{
			static AutoStrafer strafer;
			static Vector LastOrigAng = OrigAngles.Normalize();
			auto airaccel = Interfaces::Var->FindVar( ( "sv_airaccelerate" ) )->GetFloat();
			auto maxspeed = Interfaces::Var->FindVar( ( "sv_maxspeed" ) )->GetFloat();
			double yawrad = Cheat::Utilities->Math_NormalizeYawInPlace( OrigAngles.y ) * M_PI / 180.0f;
			Vector velocity = G::LocalPlayer->GetVelocity();
			float speed = velocity.Length();
			double tau = Interfaces::Globals->interval_per_tick, MA = ( maxspeed * airaccel ) / tau;
			int Sdir = 0, Fdir = 0;
			double vel[3] = { velocity[0], velocity[1], velocity[2] };
			double pos[2] = { 0, 0 };
			double dir[2] = { std::cos( OrigAngles[1] * M_PI / 180 ), std::sin( OrigAngles[1] * M_PI / 180 ) };
			strafer.strafe_line_opt( yawrad, Sdir, Fdir, vel, pos, 30.0, tau, MA, pos, dir );
			OrigAngles.y = Cheat::Utilities->Math_NormalizeYawInPlace( ( float )yawrad * 180.0f / M_PI );
			pCmd->sidemove = Sdir * 450.0f;
			pCmd->forwardmove = Fdir * 450.0f;
			strafer.MovementFix( pCmd, OrigAngles );
			Cheat::Utilities->Math_NormalizeYaw( LastOrigAng.y );
			LastOrigAng = OrigAngles.NormalizeAngles();
		}
};

namespace CircleStrafe
{
	extern float RightMovement;
	extern bool IsActive;
	extern float StrafeAngle;
	extern Vector CurrentAngles;
	extern Vector LastAngles;

	extern float GetTraceFractionWorldProps( Vector start, Vector end );
	extern void Rotate( CUserCmd* pCmd, float rotation );
	extern void Start( CUserCmd* pCmd );
	extern void Strafe( CUserCmd* pCmd );
};


namespace BoxCircleStrafe
{
	extern float strafeYaw;
	extern bool started;
	extern Vector lastAngles1;
	extern Vector boxCenter;
	extern float boxRadius;

	extern void TraceWorld( Vector start, Vector end, trace_t* trace );
	extern void Start( CUserCmd* pCmd );
	extern void Stop();
	extern void Strafe( CUserCmd* pCmd );
	extern bool GetBoxCenter( Vector origin, Vector forward, Vector left, float maxSearch, Vector &center, float &radius );
	extern bool OutOfBox( Vector origin );
};

inline void CMisc::BunnyHop()
{
	if (!c_config::get()->b["misc_autojump"] && !G::LocalPlayer)
		return;

	d_col = Vector(0, 0, 0);
	d_colL = Vector(0, 0, 0);
	d_colR = Vector(0, 0, 0);
	usingTick = false;

	if (!c_config::get()->b["misc_autojump"])
		return;

	Vector velocity = G::LocalPlayer->GetVelocity();
	Vector origin = G::LocalPlayer->GetOrigin();
	Vector viewAngles = G::UserCmd->viewangles;
	Vector velocityAngles;
	Cheat::Utilities->Math_VectorAngles(velocity, velocityAngles);
	velocityAngles.NormalizeAngles();
	static bool bLastJumped = false;
	static bool circleStarted = false;
	static float circle = 0;
	static float startCircleYaw;
	static float dodge = 0;
	static bool swap = false;
	static bool circleKeyPressed = false;
	bool circleKey = GetAsyncKeyState(c_config::get()->i["misc_zhop_key"]) & 1;
	static bool cStrafeStart = false;
	static bool duckSwitch = false;

	if (circleKey)
		circleKeyPressed = !circleKeyPressed;

	float speed = velocity.Length2D();
	bool inGround = (G::LocalPlayer->GetFlags() & FL_ONGROUND);

	if (G::UserCmd->buttons & IN_JUMP || (circleStarted))
	{
		usingTick = true;

		if (speed > 0)
		{
			if (inGround)
			{
				if (c_config::get()->b["misc_autojump"])
					G::UserCmd->buttons |= IN_JUMP;

				duckSwitch = false;
			}

			if (velocity.z >= -5.0f && velocity.z <= 0 && c_config::get()->b["misc_airduck"])
				duckSwitch = true;

			if (duckSwitch)
				G::UserCmd->buttons |= IN_DUCK;

			if (c_config::get()->b["misc_airduck"] && speed > 250)
				G::UserCmd->buttons |= IN_DUCK;

			if (c_config::get()->b["misc_autojump"] && !inGround)
				G::UserCmd->buttons &= ~IN_JUMP;

			if (c_config::get()->i["misc_airstrafe"] > 0)
			{
				swap = G::UserCmd->command_number % 2 == 0;

				if (c_config::get()->i["misc_airstrafe"] == 2)
				{
					if (!inGround)
					{
						if (circleKeyPressed || (!circleKeyPressed && cStrafeStart && CircleStrafe::IsActive))
						{
							circleStarted = true;

							if (!cStrafeStart)
							{
								CircleStrafe::Start(G::UserCmd);
								cStrafeStart = true;
							}

							if (!CircleStrafe::IsActive)
							{
								CircleStrafe::IsActive = true;
								CircleStrafe::Start(G::UserCmd);
							}

							CircleStrafe::Strafe(G::UserCmd);
						}
						else
						{
							if (GetAsyncKeyState(VK_CHAR_A) || GetAsyncKeyState(VK_CHAR_S) || GetAsyncKeyState(VK_CHAR_D) || GetAsyncKeyState(VK_CHAR_W)) {
								if (GetAsyncKeyState(VK_CHAR_A) || GetAsyncKeyState(VK_CHAR_D)) {
									if (GetAsyncKeyState(VK_CHAR_A))
										G::UserCmd->sidemove = -(10000.f) / (G::LocalPlayer->GetVelocity().Length2D() + 1);
									if (GetAsyncKeyState(VK_CHAR_D))
										G::UserCmd->sidemove = (10000.f) / (G::LocalPlayer->GetVelocity().Length2D() + 1);

									G::UserCmd->forwardmove = (G::UserCmd->command_number % 2) == 0 ? -450.f : 450.f;
								}

								if (GetAsyncKeyState(VK_CHAR_S) || GetAsyncKeyState(VK_CHAR_W)) {
									if (GetAsyncKeyState(VK_CHAR_W))
										G::UserCmd->forwardmove = (10000.f) / (G::LocalPlayer->GetVelocity().Length2D() + 1);
									if (GetAsyncKeyState(VK_CHAR_S))
										G::UserCmd->forwardmove = (10000.f) / (G::LocalPlayer->GetVelocity().Length2D() + 1) * -1;

									if (!GetAsyncKeyState(VK_CHAR_A) && !GetAsyncKeyState(VK_CHAR_D))
										G::UserCmd->sidemove = (G::UserCmd->command_number % 2) == 0 ? -450.f : 450.f;
								}
							}
							else {
								if (G::UserCmd->mousedx > 1 || G::UserCmd->mousedx < -1) {
									G::UserCmd->sidemove = G::UserCmd->mousedx < 0.f ? -450.f : 450.f;
								}
								else {
									G::UserCmd->forwardmove = (10000.f) / (G::LocalPlayer->GetVelocity().Length2D() + 1);
									G::UserCmd->sidemove = (G::UserCmd->command_number % 2) == 0 ? -450.f : 450.f;
								}
							}
						}
					}
				}

				else
				{
					if (G::UserCmd->mousedx < 0)
						G::UserCmd->sidemove = -450.0f;
					else if (G::UserCmd->mousedx > 0)
						G::UserCmd->sidemove = +450.0f;
				}
			}
		}

		else
		{
			bool freeSpace = true != GetWorldIntersection(G::LocalPlayer->GetOrigin() + Vector(0, 0, 5), Vector(0, viewAngles.y, 0), 50.0f, G::LocalPlayer);

			if (freeSpace)
				G::UserCmd->forwardmove = 450.0f;
		}
	}

	else
	{
		circleStarted = false;
		circleKeyPressed = false;
		CircleStrafe::LastAngles = Vector(0, 0, 0);
		CircleStrafe::IsActive = false;
		CircleStrafe::StrafeAngle = 0;
		BoxCircleStrafe::Stop();
	}

	circleStrafeDbg = Vector(0, circle, 0);
	oldAngles = G::UserCmd->viewangles;
}