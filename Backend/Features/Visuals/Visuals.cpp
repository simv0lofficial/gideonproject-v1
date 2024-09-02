#include "Visuals.h"
#include "Drawing.h"

#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/Collideable.h"
#include "../../SDK/player_info_t.h"
#include "../../SDK/CBaseWeapon.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../Ragebot/c_playerlist.h"

using namespace Features;
using namespace Graphics;

CVisuals* Features::Visuals = new CVisuals();

bool IsOnScreen(Vector origin, Vector &screen);
Vector get_hitbox_pos(CBaseEntity* entity, int hitbox_id);

bool GetBox( CBaseEntity* m_entity, EntityBoxInfo_t &box, bool dynamic )
{
	if ( !m_entity )
		return false;

	const VMatrix &trans = m_entity->GetCollisionBoundTrans();
	Collideable* collision = m_entity->GetCollision();

	if ( !collision )
		return false;

	Vector min, max;
	min = collision->VecMins();
	max = collision->VecMaxs();

	if ( dynamic )
	{
		Vector points[] = { Vector( min.x, min.y, min.z ),
		        Vector( min.x, max.y, min.z ),
		        Vector( max.x, max.y, min.z ),
		        Vector( max.x, min.y, min.z ),
		        Vector( max.x, max.y, max.z ),
		        Vector( min.x, max.y, max.z ),
		        Vector( min.x, min.y, max.z ),
		        Vector( max.x, min.y, max.z )
		    };
		auto vector_transform = []( const Vector in1, const VMatrix & in2 )
		{
			auto dot_product = []( const Vector & v1, const float * v2 )
			{
				float ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				return ret;
			};
			auto out = Vector();
			out[0] = dot_product( in1, in2[0] ) + in2[0][3];
			out[1] = dot_product( in1, in2[1] ) + in2[1][3];
			out[2] = dot_product( in1, in2[2] ) + in2[2][3];
			return out;
		};
		Vector pointsTransformed[8];

		for ( int i = 0; i < 8; i++ )
			pointsTransformed[i] = vector_transform( points[i], trans );

		Vector pos = m_entity->GetAbsOrigin();
		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;

		if ( !Utilities->Game_WorldToScreen( pointsTransformed[3], flb ) || !Utilities->Game_WorldToScreen( pointsTransformed[5], brt )
		    || !Utilities->Game_WorldToScreen( pointsTransformed[0], blb ) || !Utilities->Game_WorldToScreen( pointsTransformed[4], frt )
		    || !Utilities->Game_WorldToScreen( pointsTransformed[2], frb ) || !Utilities->Game_WorldToScreen( pointsTransformed[1], brb )
		    || !Utilities->Game_WorldToScreen( pointsTransformed[6], blt ) || !Utilities->Game_WorldToScreen( pointsTransformed[7], flt ) )
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for ( int i = 1; i < 8; i++ )
		{
			if ( left > arr[i].x )
				left = arr[i].x;

			if ( top < arr[i].y )
				top = arr[i].y;

			if ( right < arr[i].x )
				right = arr[i].x;

			if ( bottom > arr[i].y )
				bottom = arr[i].y;
		}

		Vector BotCenter = Vector( right - ( ( right - left ) / 2 ), top, 0 );
		Vector TopCenter = Vector( right - ( ( right - left ) / 2 ), bottom, 0 );
		box.x = left;
		box.y = TopCenter.y;
		box.w = right - left;
		box.h = BotCenter.y - TopCenter.y;
	}

	else
	{
		Vector pos, pos3D, top, top3D;
		pos3D = m_entity->GetAbsOrigin();
		top3D = pos3D + Vector( 0, 0, max.z );

		if ( Utilities->Game_WorldToScreen( pos3D, pos ) && Utilities->Game_WorldToScreen( top3D, top ) )
		{
			int height = ( pos.y - top.y );
			int width = height / 2;
			box.x = ( pos.x - width / 2 );
			box.y = top.y;
			box.w = width;
			box.h = height;
		}
	}

	box.pEnt = m_entity;
	return true;
}

void CVisuals::Run()
{
	if ( !Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame() || !G::LocalPlayer )
		return;

	if ( c_config::get()->b["vis_hitmarker"] && hitmarkerAlpha != 0 )
	{
		int w, h = 0;
		Interfaces::Engine->GetScreenSize( w, h );
		int cx, cy = 0;
		cx = w / 2;
		cy = h / 2;
		Graphics::Drawing->Line( cx - 8, cy - 8, cx - 3, cy - 3, CColor( 255, 255, 255, hitmarkerAlpha ) );
		Graphics::Drawing->Line( cx - 8, cy + 8, cx - 3, cy + 3, CColor( 255, 255, 255, hitmarkerAlpha ) );
		Graphics::Drawing->Line( cx + 8, cy - 8, cx + 3, cy - 3, CColor( 255, 255, 255, hitmarkerAlpha ) );
		Graphics::Drawing->Line( cx + 8, cy + 8, cx + 3, cy + 3, CColor( 255, 255, 255, hitmarkerAlpha ) );
		auto fade = ( ( 1.f / 0.8f ) * Interfaces::Globals->frametime ) * 255;

		if ( hitmarkerAlpha > 0 )
			hitmarkerAlpha -= fade;

		if ( hitmarkerAlpha < 0 )
			hitmarkerAlpha = 0;
	}

	DrawPlayers();
	DrawIndicators();
}

void CVisuals::RunHitmarker()
{
	hitmarkerAlpha = 255;
}

void CVisuals::RunSpectators()
{
	std::vector<std::string> specs;
	int w, h = 0;

	if ( Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && G::LocalPlayer && G::LocalPlayer->IsAlive() && c_config::get()->b["vis_spectators"] )
	{
		Interfaces::Engine->GetScreenSize( w, h );

		for ( int i = 0; i < 64; i++ )
		{
			CBaseEntity* pEnt = Interfaces::EntityList->GetClientEntity( i );

			if ( !pEnt || !pEnt->IsPlayer() || pEnt == G::LocalPlayer || pEnt->GetHealth() > 0 || pEnt->IsDormant() || pEnt->GetObserverTarget() != G::LocalPlayer )
				continue;

			player_info_t pInfo;
			Interfaces::Engine->GetPlayerInfo( i, &pInfo );
			specs.push_back( pInfo.name );
		}

		for ( int i = 0; i < specs.size(); i++ )
		{
			int xOffset = Drawing->TextSize( specs.at( i ).c_str(), Drawing->fonts.esp_main ).right + 5;
			int y = i * 14;
			Drawing->Text( w - xOffset + 2, y + 6, specs.at( i ).c_str(), Drawing->fonts.esp_main, CColor( 255, 255, 255 ) );
		}
	}
}

void CVisuals::DrawPlayers()
{
	for ( int i = 1; i < Interfaces::EntityList->GetHighestEntityIndex(); i++ )
	{
		EntityBoxInfo_t &box = infos[i - 1];
		auto ent = Interfaces::EntityList->GetClientEntity( i );

		if ( !ent )
			continue;

		if (c_playerlist::get()->should_disable_visuals(ent))
			continue;

		if ( ent->GetClientClass()->m_ClassID == ( int )ClassID::CCSPlayer )
		{
			box.w = 0;
			box.h = 0;
			box.x = 0;
			box.y = 0;

			Vector dist = G::LocalPlayer->GetOrigin() - ent->GetOrigin();
			float flDist = sqrtf( dist.Length() );

			if ( (flDist < 10 && c_config::get()->b["vis_hideespdistance"]) || ( c_config::get()->i["vis_maximalespdistance"] != 500 && flDist > c_config::get()->i["vis_maximalespdistance"] ) )
				continue;

			if ( ent->GetTeam() == G::LocalPlayer->GetTeam() && !c_config::get()->b["vis_teammates"] && ent != G::LocalPlayer )
				continue;

			if ( ent == G::LocalPlayer )
				continue;

			DrawArrow( ent );

			Vector screenPos;
			if ( GetBox( ent, box, c_config::get()->i["vis_boundingbox"] == 2 ? true : false ))
			{
				if ( box.x == G::iScreenWidth / 2 && box.y == G::iScreenHeight / 2 && box.x + box.w > G::iScreenWidth - 1 && box.y + box.h > G::iScreenHeight - 1 )
					continue;

				if (box.x <= 0 || box.y <= 0)
					continue;

				if ( box.hp == -1 )
					box.hp = std::clamp( ent->GetHealth(), 0, 100 );

				else
				{
					int hp = std::clamp( ent->GetHealth(), 0, 100 );

					if ( hp != box.hp )
					{
						if ( box.hp > hp )
						{
							if ( box.hpDiffTime == 0.f )
								box.hpDifference = box.hp - hp;

							else
								box.hpDifference += box.hp - hp;

							box.hpDiffTime = Interfaces::Globals->curtime;
						}

						else
						{
							box.hpDifference = 0;
							box.hpDiffTime = 0.f;
						}

						box.hp = hp;
					}

					if ( Interfaces::Globals->curtime - box.hpDiffTime > 0.6f && box.hpDifference != 0 )
					{
						float diffFactor = ( ( 1.f / 0.5f ) * Interfaces::Globals->frametime ) * ( box.hpDifference * 2 );
						box.hpDifference -= diffFactor;
						box.hpDifference = std::clamp( box.hpDifference, 0, 100 );

						if ( box.hpDifference == 0 )
							box.hpDiffTime = 0.f;
					}
				}

				if ( ent->IsDormant() || !ent->IsAlive() )
				{
					if ( box.alpha_anim > 0 )
					{
						auto fadeNormal = ( ( 1.f / 0.25f ) * Interfaces::Globals->frametime ) * 255;

						if ( ent->IsDormant() && c_config::get()->b["vis_dormant"] && ent->IsAlive() )
						{
							if ( box.dormancy == 0 )
								box.dormancy = Interfaces::Globals->curtime;

							if ( Interfaces::Globals->curtime - box.dormancy > 5.f )
								box.alpha_anim -= fadeNormal;
						}

						else
							box.alpha_anim -= fadeNormal;
					}

					else
					{
						box.dormancy = Interfaces::Globals->curtime;
						box.alpha_anim = 0;
					}
				}

				else
					box.alpha_anim = 1275;

				int alpha_anim_unc = box.alpha_anim / 5;
				bool isTeammate = false;

				if ( box.pEnt->GetTeam() == G::LocalPlayer->GetTeam() )
					isTeammate = true;

				int alpha_toc = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_boundingbox_color"]).a();

				box.alpha = std::clamp( alpha_anim_unc, 0, alpha_toc );

				if ( c_config::get()->i["vis_boundingbox"] > 0 )
					DrawBoundingBox( box );

				if ( c_config::get()->b["vis_healthbar"] )
					DrawHealthBar( box );

				if ( c_config::get()->b["vis_name"] )
					DrawName( box );

				if ( c_config::get()->b["vis_armor"] || c_config::get()->b["vis_latency"] || c_config::get()->b["vis_flags"] || c_config::get()->b["vis_money"] || c_config::get()->b["vis_distance"])
					DrawSideInfo( box );

				if ( c_config::get()->b["vis_ammo"] || c_config::get()->b["vis_weapons"] )
					DrawBottomInfo( box );
			}
		}

		else
		{
			if ( ent->GetClientClass()->m_ClassID == ( int )ClassID::CPlantedC4 && c_config::get()->b["vis_bomb"] )
			{
				DrawBombInterface( ent );
				continue;
			}

			if ( !ent->IsWeapon() )
				continue;

			auto mdl = Interfaces::ModelInfo->GetModelName( ent->GetModel() );

			if ( !strstr( mdl, "w_" ) )
				continue;

			if ( GetBox( ent, box, true ) )
			{
				box.alpha = 255;

				if ( c_config::get()->m["vis_droppedwpns"][0] )
					DrawWeaponName( box );

				if (c_config::get()->m["vis_droppedwpns"][2])
					DrawBoundingBox( box );

				if (c_config::get()->m["vis_droppedwpns"][3])
					DrawWeaponAmmo( box );
			}
		}
	}
}

Vector get_hitbox_pos( CBaseEntity* entity, int hitbox_id )
{
	auto getHitbox = []( CBaseEntity * entity, int hitboxIndex ) -> mstudiobbox_t*
	{
		if ( entity->IsDormant() || entity->GetHealth() <= 0 )
			return NULL;

		const auto pModel = entity->GetModel();

		if ( !pModel )
			return NULL;

		auto pStudioHdr = Interfaces::ModelInfo->GetStudioModel( pModel );

		if ( !pStudioHdr )
			return NULL;

		auto pSet = pStudioHdr->pHitboxSet( 0 );

		if ( !pSet )
			return NULL;

		if ( hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0 )
			return NULL;

		return pSet->GetHitbox( hitboxIndex );
	};
	auto hitbox = getHitbox( entity, hitbox_id );

	if ( !hitbox )
		return Vector( 0, 0, 0 );

	matrix3x4_t matrix[128];
	if (entity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		Vector bbmin, bbmax;
		Utilities->Math_VectorTransform(hitbox->bbmin, matrix[hitbox->bone], bbmin);
		Utilities->Math_VectorTransform(hitbox->bbmax, matrix[hitbox->bone], bbmax);
		return (bbmin + bbmax) / 2;
	}
}

bool IsOnScreen( Vector origin, Vector &screen )
{
	if ( !Utilities->Game_WorldToScreen( origin, screen ) )
		return false;

	int iScreenWidth, iScreenHeight;
	Interfaces::Engine->GetScreenSize( iScreenWidth, iScreenHeight );
	bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
	return xOk && yOk;
}

void CVisuals::DrawArrow( CBaseEntity* pEnt )
{
	if ( !c_config::get()->b["vis_outoffovarrows"] )
		return;

	if ( !G::LocalPlayer || pEnt->IsDormant() || pEnt->IsWeapon() || !pEnt->IsPlayer() || pEnt->GetClientClass()->m_ClassID != ( int )ClassID::CCSPlayer || pEnt == G::LocalPlayer || !pEnt->IsAlive() || pEnt->GetHealth() <= 0 )
		return;

	int alpha = std::clamp( floor( sin( Interfaces::Globals->realtime * 2 ) * 127 + 128 ), 0.f, 0.5f * 255.f );
	Vector screenPos, client_viewangles2, client_viewangles;
	int screen_width = 0, screen_height = 0;
	float radius = 50.f + c_config::get()->i["vis_outoffovarrows_dist"];

	if ( IsOnScreen( pEnt->GetHitboxPosition(HITBOX_HEAD), screenPos ) )
		return;

	Interfaces::Engine->GetViewAngles( &client_viewangles );
	Interfaces::Engine->GetScreenSize( screen_width, screen_height );

	// υσι

	const auto scr = Vector2D( screen_width / 2.f, screen_height / 2.f );
	const auto rot = DEG2RAD( client_viewangles.y - Utilities->Math_CalcAngle( G::LocalPlayer->GetEyePosition(), get_hitbox_pos( pEnt, HITBOX_HEAD ) ).y - 90);
	auto pos = scr + Vector2D( radius * cos( rot ), radius * sin( rot ) );
	auto line = pos - scr;
	Vector2D arrowBase = pos - ( line * ( c_config::get()->i["vis_outoffovarrows_size"] / ( 2 * ( tanf( 45 ) / 2 ) * line.length() ) ) );
	Vector2D normal = Vector2D( -line.y, line.x );
	Vector2D left = arrowBase + normal * ( c_config::get()->i["vis_outoffovarrows_size"] / ( 2 * line.length() ) );
	Vector2D right = arrowBase + normal * ( -c_config::get()->i["vis_outoffovarrows_size"] / ( 2 * line.length() ) );
	std::vector<Vertex_t> vertices;
	vertices.push_back( Vertex_t( Vector2D( left.x, left.y ) ) );
	vertices.push_back( Vertex_t( Vector2D( right.x, right.y ) ) );
	vertices.push_back( Vertex_t( pos ) );
	Graphics::Drawing->Polygon( ( int )3, vertices.data(), c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_outoffovarrows_color"], alpha / 255.f) );
}

int fl2bigint( float fl )
{
	char timerfl[32];
	sprintf( timerfl, "%.2f", fl );
	std::string timerfls = timerfl;
	timerfls.erase( std::remove_if( timerfls.begin(), timerfls.end(), []( char c )
	{
		return c == ',' || c == '.';
	} ) );
	return atoi( timerfls.c_str() );
}

void CVisuals::DrawBombInterface( CBaseEntity* pEnt )
{
	auto bomb = ( CCSBomb* )pEnt;

	if ( !bomb || !bomb->IsBombTicking() || bomb->IsBombDefused() )
		return;

	float blow = bomb->GetBlowTime();
	float remain = blow - ( Interfaces::Globals->interval_per_tick * G::LocalPlayer->GetTickBase() );

	if ( remain < 0 )
		remain = 0;

	float def = bomb->GetDefuseTime();
	float defrem = def - ( Interfaces::Globals->interval_per_tick * G::LocalPlayer->GetTickBase() );

	if ( defrem < 0 )
		defrem = 0;

	int w, h = 0;
	Interfaces::Engine->GetScreenSize( w, h );
	char buffer[32];
	sprintf( buffer, "%s - %.0fs", bomb->GetBombSite() ? "B" : "A", remain );
	char defbuffer[32];
	sprintf( defbuffer, "Defuse: %.0fs", defrem );
	int remainanim, timeranim, defremanim, deftimeanim = 0;
	remainanim = fl2bigint( remain );
	timeranim = fl2bigint( bomb->GetTimerLength() );
	defremanim = fl2bigint( defrem );
	deftimeanim = fl2bigint( bomb->GetDefuseLength() );
	float a = 450.7f;
	float b = 75.68f;
	float c = 789.2f;
	float d = ( ( G::LocalPlayer->GetAbsOrigin().DistTo( pEnt->GetOrigin() ) - b ) / c );
	float damage = a * expf( -d * d );
	auto GetArmourHealth = []( float flDamage, int ArmorValue )
	{
		float flCurDamage = flDamage;

		if ( flCurDamage == 0.0f || ArmorValue == 0 )
			return flCurDamage;

		float flArmorRatio = 0.5f;
		float flArmorBonus = 0.5f;
		float flNew = flCurDamage * flArmorRatio;
		float flArmor = ( flCurDamage - flNew ) * flArmorBonus;

		if ( flArmor > ArmorValue )
		{
			flArmor = ArmorValue * ( 1.0f / flArmorBonus );
			flNew = flCurDamage - flArmor;
		}

		return flNew;
	};
	float fdamage = max( ( int )ceilf( GetArmourHealth( damage, G::LocalPlayer->GetArmor() ) ), 0 );
	bool someoneDef = Interfaces::EntityList->GetClientEntityFromHandle2( bomb->GetBombDefuser() ) != nullptr;
	int height = h - ( ( int )remainanim * h / timeranim );
	int defheight = 0;

	if ( someoneDef )
		defheight = h - ( ( int )defremanim * h / deftimeanim );

	Graphics::Drawing->Rect( 0, 0, 8, h, CColor( 0, 0, 0, 150 ) );
	Graphics::Drawing->Rect( 0, height, 8, h - height, CColor( remain > 10 ? CColor( 100, 255, 100 ) : CColor( 255, 100, 100 ) ) );

	if ( someoneDef )
	{
		Graphics::Drawing->Rect( 8, 0, 8, h, CColor( 0, 0, 0, 150 ) );
		Graphics::Drawing->Rect( 8, defheight, 8, h - defheight, CColor( 100, 100, 255 ) );
	}

	int textH = height;
	int textStrH = Graphics::Drawing->TextSize( buffer, Graphics::Drawing->fonts.esp_indicator ).bottom;

	if ( textH + textStrH > h )
		textH = h - textStrH;

	else
		textH = textH - textStrH;

	Graphics::Drawing->Text( 1, textH, buffer, Graphics::Drawing->fonts.esp_indicator, CColor( 255, 255, 255 ) );

	if ( someoneDef )
		Graphics::Drawing->Text( 1, 5, defbuffer, Graphics::Drawing->fonts.esp_indicator, CColor( 255, 255, 255 ) );

	if ( fdamage > G::LocalPlayer->GetHealth() )
		Graphics::Drawing->Text( 1, ( someoneDef ? textStrH : 0 ) + 5, "Fatal", Graphics::Drawing->fonts.esp_indicator, CColor( 255, 100, 100 ) );
}
#include "../../Hooking/KeyHandler.h"
#include "../c_fps.h"

void CVisuals::DrawIndicators()
{
	std::vector<Indicator_t> indicators;

	for (auto i : custom_inds)
		indicators.push_back(i);

	static auto percent_col = [](int per) -> CColor {
		int red = per < 50 ? 255 : floorf(255 - (per * 2 - 100) * 255.f / 100.f);
		int green = per > 50 ? 255 : floorf((per * 2) * 255.f / 100.f);

		return CColor(red, green, 0);
	};

	if (c_keyhandler::get()->auto_check("rage_forcebaim_key"))
		indicators.push_back({ "BAIM", percent_col(100) });

	if (c_config::get()->b["misc_lowfpswarning"])
		indicators.push_back({ "FPS", percent_col((float(std::clamp(c_fps::get()->get_fps(), 64, 200) - 64)) / 200.f * 100.f) });

	if (c_keyhandler::get()->auto_check("rage_fakeduck_key") && c_config::get()->b["rage_fakeduck"])
		indicators.push_back({ "HEAD", percent_col(100.f - (G::LocalPlayer->GetEyePosition().z - G::LocalPlayer->GetAbsOrigin().z - 46.f) / 18.f * 100.f) });
	
	if (c_config::get()->i["rage_fakelag"] > 0 && G::LocalPlayer && G::LocalPlayer->GetVelocity().Length2D() > 110 && G::LocalPlayer->IsAlive()) {
		static auto is_breaking_lagcomp = false;
		if (!Interfaces::ClientState->m_nChokedCommands)
		{
			static auto last_origin = G::LocalPlayer->GetOrigin();
			if (G::LocalPlayer->GetOrigin() != last_origin)
			{
				is_breaking_lagcomp = (last_origin.Length2DSqr() - G::LocalPlayer->GetOrigin().Length2DSqr()) > 4096;
				last_origin = G::LocalPlayer->GetOrigin();
			}
		}

		indicators.push_back({ "LC", is_breaking_lagcomp ? percent_col(100) : percent_col(0) });
	}

	if (c_config::get()->i["rage_antiaim_desync"] > 0 && G::LocalPlayer->IsAlive())
		indicators.push_back({ "FAKE", percent_col(std::clamp(fabs(G::RealAngle.y - G::FakeAngle.y), 0.f, 56.f) / 56.f * 100) });

	/*if (Config->desyncbeta)
		indicators.push_back({ "LBY", abs(G::RealAngle.y - G::LocalPlayer->GetLowerBodyYaw()) > 30 ? CColor(0, 255, 0) : CColor(255, 0, 0) });*/

	int yOffset = 0;
	int screenW, screenH = 0;
	Interfaces::Engine->GetScreenSize( screenW, screenH );
	auto size = Graphics::Drawing->TextSize( "A", Graphics::Drawing->fonts.esp_indicator );

	for ( auto i : indicators )
	{
		Graphics::Drawing->Text( 10, screenH - ( 60 + size.bottom ) - yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_indicator, i.color );
		yOffset += size.bottom;
	}

	custom_inds.clear();
}

void CVisuals::DrawWeaponName( EntityBoxInfo_t inf )
{
	auto name = ( ( CBaseWeapon* )inf.pEnt )->GetGunIcon( ( ( CBaseWeapon* )inf.pEnt )->GetItemDefenitionIndex() );
	int textWidth = Graphics::Drawing->TextSize( name, Graphics::Drawing->fonts.esp_icons).right;
	int x = inf.x;
	Graphics::Drawing->Text( inf.x + ( inf.w / 2 ) - ( textWidth / 2 ), inf.y - 13, name, Graphics::Drawing->fonts.esp_icons, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_droppedwpns_color"]));
}

void CVisuals::DrawWeaponAmmo( EntityBoxInfo_t inf )
{
	if ( inf.pEnt->GetClientClass()->m_ClassID == ( int )ClassID::CC4 )
		return;

	std::vector<BottomInfo_t> botInfos;
	auto weapon = ( CBaseWeapon* )inf.pEnt;

	if ( c_config::get()->m["vis_droppedwpns"][3] && weapon )
	{
		int maxAmmo = weapon->get_full_info()->max_clip;
		int curAmmo = weapon->GetAmmoInClip();
		botInfos.push_back( { false, "", ( double )maxAmmo, ( double )curAmmo, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_ammo_color"], Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, 1.f ) ) } );
	}

	int yOffset = 0;
	int textHeight = Graphics::Drawing->TextSize( "A", Graphics::Drawing->fonts.esp_main ).bottom + 1;

	for ( auto i : botInfos )
	{
		if ( i.isString )
		{
			auto w = Graphics::Drawing->TextSize( i.str.c_str(), Graphics::Drawing->fonts.esp_main ).right;

			if ( w > inf.w )
				continue;

			Graphics::Drawing->Text( inf.x + inf.w / 2 - w / 2, inf.y + inf.h + 2 + yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_main, i.color );
			yOffset += textHeight;
		}

		else
		{
			Graphics::Drawing->Rect( inf.x - 1, inf.y + inf.h + 2 + yOffset, inf.w + 2, 4, CColor( 0, 0, 0, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) );
			Graphics::Drawing->Rect( inf.x, inf.y + inf.h + 2 + yOffset + 1, ceil( ( double )inf.w * ( i.percentage / i.maxValue ) ), 2, i.color );
			yOffset += 5;
		}
	}
}

void CVisuals::DrawBoundingBox( EntityBoxInfo_t inf )
{
	CColor col;

	if ( !inf.pEnt->IsWeapon() )
	{
		if ( inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"] )
			col = CColor( 150, 150, 150, inf.alpha );

		else
			col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_boundingbox_color"], Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, 0.5f) );
	}

	else
		col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_droppedwpns_color"]);

	Graphics::Drawing->Outline( inf.x - 1, inf.y - 1, inf.w + 2, inf.h + 2, CColor( 0, 0, 0, std::clamp( inf.alpha, 0, 150 ) ) );
	Graphics::Drawing->Outline( inf.x, inf.y, inf.w, inf.h, col );
	Graphics::Drawing->Outline( inf.x + 1, inf.y + 1, inf.w - 2, inf.h - 2, CColor(0, 0, 0, std::clamp( inf.alpha, 0, 150 ) ) );
}

void CVisuals::DrawHealthBar( EntityBoxInfo_t inf )
{
	CColor col, col2;

	if ( inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"] )
	{
		col = CColor( 150, 150, 150, inf.alpha );
		col2 = CColor( 150, 150, 150, inf.alpha );
	}

	else
	{
		col = CColor( 128, 244, 66, ( int )Utilities->Util_Clamp( ( float )( inf.alpha ), 0.f, 200.f ) );
		col2 = CColor( 244, 100, 66, ( int )Utilities->Util_Clamp( ( float )( inf.alpha ), 0.f, 200.f ) );
	}

	Graphics::Drawing->Rect( inf.x - 6, inf.y - 1, 4, inf.h + 2, CColor( 0, 0, 0, ( int )Utilities->Util_Clamp( ( float )( inf.alpha ), 0.f, 150.f ) ) );
	int height = inf.h - ( inf.hp * inf.h / 100 );
	Graphics::Drawing->Rect( inf.x - 5, height + inf.y, 2, inf.h - height, col );

	if ( inf.hpDifference != 0 )
		Graphics::Drawing->Rect( inf.x - 5, inf.y + height - ( inf.hpDifference * inf.h / 100 ), 2, inf.hpDifference * inf.h / 100, col2 );

	if (inf.hp <= 90 && inf.hp > 0)
		Graphics::Drawing->Text(inf.x - 8, inf.y + height - 8, std::to_string(inf.hp).c_str(), Graphics::Drawing->fonts.esp_side, CColor(255, 255, 255, (int)Utilities->Util_Clamp((float)(inf.alpha), 0.f, 150.f)));
}

void CVisuals::DrawName( EntityBoxInfo_t inf )
{
	player_info_t info;
	Interfaces::Engine->GetPlayerInfo( inf.pEnt->GetIndex(), &info );
	int textWidth = Graphics::Drawing->TextSize( info.name, Graphics::Drawing->fonts.esp_main).right;
	int x = inf.x;
	x -= ( textWidth - ( inf.w - 8 ) ) / 2 - 2;
	CColor col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_name_color"], Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, 1.f ) );

	if ( inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"] )
		col = CColor( 150, 150, 150, inf.alpha );

	Graphics::Drawing->Text( inf.x + ( inf.w / 2 ) - ( textWidth / 2 ), inf.y - 13, info.name, Graphics::Drawing->fonts.esp_main, col );
}

void CVisuals::DrawSideInfo( EntityBoxInfo_t inf )
{
	if (inf.w <= 0)
		return;

	std::vector<SideInfo_t> sizeInfos;
	Vector dist = G::LocalPlayer->GetOrigin() - inf.pEnt->GetOrigin();
	float flDist = sqrtf( dist.Length() );

	if ( flDist < 1.f )
		flDist = 1.f;

	if ( c_config::get()->b["vis_armor"] && inf.pEnt->GetArmor() > 0 && flDist < 32.f )
		sizeInfos.push_back( { inf.pEnt->GetArmorName(), CColor( 255, 255, 255, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) } );

	if ( c_config::get()->b["vis_latency"] && flDist < 48.f )
		sizeInfos.push_back( { std::to_string( inf.pEnt->GetPing() ) + "MS", CColor( 244, 149, 66, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) } );

	if (c_config::get()->b["vis_flags"] )
	{
		if ( inf.pEnt->IsScoped() && flDist < 32.f)
			sizeInfos.push_back( { "SCOPED", CColor( 65, 169, 244, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) } );

		if (inf.pEnt->IsReloadingEnt())
			sizeInfos.push_back({ "RELOAD", CColor(65, 169, 244, Utilities->Util_Clamp(inf.alpha, 0, 150)) });

		static int crouched_ticks[65] = { 0 };
		static int stored_tick = 0;
		if (*inf.pEnt->flDuckAmount() <= 0.9f && *inf.pEnt->flDuckAmount() > 0.1f && inf.pEnt->GetFlags() & 1 && *inf.pEnt->flDuckSpeed() == 8) {
			if (stored_tick != Interfaces::Globals->tickcount) {
				crouched_ticks[inf.pEnt->GetIndex()]++;
				stored_tick = Interfaces::Globals->tickcount;
			}

			if (crouched_ticks[inf.pEnt->GetIndex()] >= 5)
				sizeInfos.push_back({ "FAKEDUCK", CColor(244, 65, 65, Utilities->Util_Clamp(inf.alpha, 0, 150)) });
		}
		else
			crouched_ticks[inf.pEnt->GetIndex()] = 0;
	}

	if ( c_config::get()->b["vis_money"] && flDist < 32.f )
		sizeInfos.push_back( { std::to_string( inf.pEnt->GetMoney() ) + "$", CColor( 65, 244, 79, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) } );

	if ( c_config::get()->b["vis_distance"] && flDist < 32.f )
		sizeInfos.push_back( { std::to_string( ( int )flDist ) + "FT", CColor( 255, 255, 255, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) } );

	auto weapon = inf.pEnt->GetActiveWeapon();
	auto icon = weapon->GetGunIcon( weapon->GetItemDefenitionIndex() );
	int yOffset = 0;
	int textHeight = 8;
	bool entDormant = inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"];

	for ( auto i : sizeInfos )
	{
		Graphics::Drawing->Text( inf.x + inf.w + 4, inf.y + yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_side, entDormant ? CColor( 150, 150, 150, inf.alpha ) : i.color );
		yOffset += textHeight;
	}
}

void CVisuals::DrawBottomInfo( EntityBoxInfo_t inf )
{
	std::vector<BottomInfo_t> botInfos;
	auto weapon = inf.pEnt->GetActiveWeapon();

	if ( c_config::get()->b["vis_ammo"] && weapon )
	{
		int maxAmmo = weapon->get_full_info()->max_clip;
		int curAmmo = weapon->GetAmmoInClip();
		botInfos.push_back( { false, "", ( double )maxAmmo, ( double )curAmmo, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_ammo_color"], Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, 1.f ) * 255.f ) } );
	}

	/*if ( Config->Vis_LBYTimer )
		botInfos.push_back( { false, "", 100, 100, im2pt( Config->Vis_LBYTimerCol, Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, Config->Vis_LBYTimerCol[3] ) ) } );*/
	auto icon = weapon->GetGunIcon( weapon->GetItemDefenitionIndex() );

	if ( c_config::get()->b["vis_weapons"] && weapon )
		botInfos.push_back( { true, weapon->GetGunIcon( weapon->GetItemDefenitionIndex() ), 0, 0, CColor( 197, 197, 197, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) } );

	int yOffset = 0;
	int textHeight = Graphics::Drawing->TextSize( "A", Graphics::Drawing->fonts.esp_icons ).bottom / 2;
	bool entDormant = inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"];

	for ( auto i : botInfos )
	{
		if ( i.isString )
		{
			auto w = Graphics::Drawing->TextSize( i.str.c_str(), Graphics::Drawing->fonts.esp_icons).right;

			Graphics::Drawing->Text( inf.x + inf.w / 2 - w / 2, inf.y + inf.h + 2 + yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_icons, entDormant ? CColor( 150, 150, 150, inf.alpha ) : i.color );
			yOffset += textHeight;
		}

		else
		{
			Graphics::Drawing->Rect( inf.x - 1, inf.y + inf.h + 2 + yOffset, inf.w + 2, 4, CColor( 0, 0, 0, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) );
			Graphics::Drawing->Rect( inf.x, inf.y + inf.h + 2 + yOffset + 1, ceil( ( double )inf.w * ( i.percentage / i.maxValue ) ), 2, entDormant ? CColor( 150, 150, 150, inf.alpha ) : i.color );
		
			yOffset += 6;
		}
	}
}


void CVisuals::remove_smoke()
{
	static auto smoke_count = *reinterpret_cast<uint32_t**>( offys.dwSmokeCount );
	static std::vector<const char*> smoke_materials =
	{
		"effects/overlaysmoke",
		"particle/beam_smoke_01",
		"particle/particle_smokegrenade",
		"particle/particle_smokegrenade1",
		"particle/particle_smokegrenade2",
		"particle/particle_smokegrenade3",
		"particle/particle_smokegrenade_sc",
		"particle/smoke1/smoke1",
		"particle/smoke1/smoke1_ash",
		"particle/smoke1/smoke1_nearcull",
		"particle/smoke1/smoke1_nearcull2",
		"particle/smoke1/smoke1_snow",
		"particle/smokesprites_0001",
		"particle/smokestack",
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev4_emods_nocull",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull"
	};

	for ( auto material_name : smoke_materials )
	{
		IMaterial* material = Interfaces::MaterialSystem->FindMaterial( material_name, "Other textures" );
		material->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, true );
	}

	*( int* )smoke_count = 0;
}
enum ACT
{
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP,
};
void grenade_prediction::Tick(int buttons)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	bool in_attack = buttons & IN_ATTACK;
	bool in_attack2 = buttons & IN_ATTACK2;

	//g_pICvar->ConsoleColorPrintf(SDK::Color(255, 255, 255, 255), "OView");

	act = (in_attack && in_attack2) ? ACT_LOB :
		(in_attack2) ? ACT_DROP :
		(in_attack) ? ACT_THROW :
		ACT_NONE;
}
void grenade_prediction::View(CViewSetup* setup)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;

	if (G::LocalPlayer && G::LocalPlayer->IsAlive())
	{
		CBaseWeapon* weapon = G::LocalPlayer->GetActiveWeapon();

		if (weapon && weapon->is_grenade() && act != ACT_NONE)
		{
			type = weapon->GetItemDefenitionIndex();
			Simulate(setup);
		}
		else
		{
			type = 0;
		}
	}
}

void grenade_prediction::Paint()
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;

	if ((type) && path.size() > 1)
	{
		float Red, Green, Blue;
		Red = 150;
		Green = 150;
		Blue = 255;
		Vector nadeStart, nadeEnd;

		CColor lineColor(Red, Green, Blue);
		Vector prev = path[0];

		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (Cheat::Utilities->Game_WorldToScreen(prev, nadeStart) && Cheat::Utilities->Game_WorldToScreen(*it, nadeEnd))
			{
				Interfaces::Surface->SetDrawColor(lineColor);
				Interfaces::Surface->DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y);
			}

			prev = *it;
		}

		if (Cheat::Utilities->Game_WorldToScreen(prev, nadeEnd)) {
			Graphics::Drawing->DrawCircle(nadeEnd.x, nadeEnd.y, 3, CColor(0, 0, 0, 255));
			Graphics::Drawing->DrawCircle(nadeEnd.x, nadeEnd.y, 2, CColor(150, 150, 150, 255));
		}
	}
}
static const constexpr auto PIRAD = 0.01745329251f;
void angle_vectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	sp = static_cast<float>(sin(double(angles.x) * PIRAD));
	cp = static_cast<float>(cos(double(angles.x) * PIRAD));
	sy = static_cast<float>(sin(double(angles.y) * PIRAD));
	cy = static_cast<float>(cos(double(angles.y) * PIRAD));
	sr = static_cast<float>(sin(double(angles.z) * PIRAD));
	cr = static_cast<float>(cos(double(angles.z) * PIRAD));

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}
void grenade_prediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;

	Vector angThrow = viewangles;
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	// Gets ThrowVelocity from weapon files
	// Clamped to [15,750]
	float flVel = 750.0f * 0.9f;

	// Do magic on member of grenade object [esi+9E4h]
	// m1=1  m1+m2=0.5  m2=0
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	// Clamped to [0,1]
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	angle_vectors2(angThrow, &vForward, &vRight, &vUp); //angThrow.ToVector(vForward, vRight, vUp);

	vecSrc = G::LocalPlayer->GetEyePosition();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f; //vecDest.MultAdd(vForward, 22.0f);

	TraceHull(vecSrc, vecDest, tr);

	// After the hull trace it moves 6 units back along vForward
	// vecSrc = tr.endpos - vForward * 6
	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.end;
	vecSrc -= vecBack;

	// Finally calculate velocity
	vecThrow = G::LocalPlayer->GetVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel; //	vecThrow.MultAdd(vForward, flVel);
}

void grenade_prediction::Simulate(CViewSetup* setup)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Vector vecSrc, vecThrow;
	Vector angles; Interfaces::Engine->GetViewAngles(&angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = Interfaces::Globals->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;


	path.clear();
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int grenade_prediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{

	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	// Set new position
	vecSrc = tr.end;

	return result;
}


bool grenade_prediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval)
{
	switch (type)
	{
	case WEAPON_SMOKEGRENADE:
	case WEAPON_DECOY:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick%det_tick_mod);
		}
		return false;

	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;
		// OR we've been flying for too long

	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		return static_cast<float>(tick)*interval > 1.5f && !(tick%static_cast<int>(0.2f / interval));

	default:
		assert(false);
		return false;
	}
}

void grenade_prediction::TraceHull(Vector& src, Vector& end, trace_t& tr)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Ray_t ray;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	CTraceWorldOnly filter;
	//filter.SetIgnoreClass("BaseCSGrenadeProjectile");
	//filter.bShouldHitPlayers = false;

	Interfaces::Trace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void grenade_prediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Vector basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void grenade_prediction::PushEntity(Vector& src, const Vector& move, trace_t& tr)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void grenade_prediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr < flMinSpeedSqr)
	{
		//vecAbsVelocity.Zero();
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	// Stop if on ground
	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
		PushEntity(tr.end, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int grenade_prediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}