//#include "Aimbot.h"
//#include "../../Include/GlobalVariables.hpp"
//#include "../../../Frontend/Menu/Menu.h"
//
//
//using namespace Features;
//using namespace Cheat;
//
//CLegitAimbot* Features::LegitAimbot = new CLegitAimbot();
//CTimer* Features::m_pShotDelay = new CTimer();
//
//byte tt_ct_best_hit_3[5] =
//{
//	HITBOX_HEAD,
//	HITBOX_NECK,
//	HITBOX_BODY,
//	HITBOX_THORAX,
//	HITBOX_CHEST
//};
//
//#define TT_CT_BEST_HIT_SIZE_1 ( sizeof( tt_ct_best_hit_3 ) / sizeof( *tt_ct_best_hit_3 ) )
//
//int weaponindex( CBaseWeapon* activeweapon )
//{
//	
//}
//
//bool CLegitAimbot::IsEnable()
//{
//	if ( !G::LocalPlayer || !G::UserCmd )
//		return false;
//
//	if ( !G::LocalPlayer->GetActiveWeapon()->GetAmmoInClip() || G::LocalPlayer->GetActiveWeapon()->IsReloading() || !G::LocalPlayer->IsAlive() || G::LocalPlayer->GetHealth() < 1 )
//		return false;
//
//	if ( G::LocalPlayer->GetActiveWeapon()->Type() == WEAPONTYPE_UNKNOWN || G::LocalPlayer->GetActiveWeapon()->Type() == WEAPONTYPE_C4 || G::LocalPlayer->GetActiveWeapon()->Type() == WEAPONTYPE_KNIFE )
//		return false;
//
//	return true;
//}
//
//int CLegitAimbot::GetPlayerFov( CBaseEntity* pPlayer )
//{
//	int iFov = 0;
//	int iFovVal = 0;
//
//	if (
//	    G::LocalPlayer->GetShotsFired() > 1 &&
//	    ( m_pCurrentWeapon->Type() == WEAPONTYPE_MACHINEGUN ||
//	        m_pCurrentWeapon->Type() == WEAPONTYPE_RIFLE ||
//	        m_pCurrentWeapon->Type() == WEAPONTYPE_SUBMACHINEGUN ||
//	        m_pCurrentWeapon->Type() == WEAPONTYPE_SHOTGUN )
//	)
//		iFovVal = weapon( m_pCurrentWeapon ).RCS_FOV;
//
//	else
//		iFovVal = weapon( m_pCurrentWeapon ).Aimbot_FOV;
//
//	int base_fov = pow( iFovVal + 30, 2 ) * 90;
//	int a = int( G::LocalPlayer->GetOrigin().DistTo( pPlayer->GetOrigin() ) );
//
//	if ( weapon( m_pCurrentWeapon ).Aimbot_FOVType <= 0 )
//		iFov = ( int )( base_fov / ( a * 90 ) );
//
//	else
//		iFov = ( int )( base_fov / ( 18000 ) );
//
//	return iFov;
//}
//
//typedef bool( *LineGoesThroughSmokeFn )( Vector, Vector );
//LineGoesThroughSmokeFn  m_fncLineGoesThroughSmoke = nullptr;
//
//bool LineGoesThroughSmoke( Vector start, Vector end )
//{
//	if ( !m_fncLineGoesThroughSmoke )
//	{
//		uintptr_t LineGoesThroughSmokePointer = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" );
//		m_fncLineGoesThroughSmoke = reinterpret_cast<LineGoesThroughSmokeFn>( LineGoesThroughSmokePointer );
//	}
//
//	return m_fncLineGoesThroughSmoke( start, end );
//}
//
//int CLegitAimbot::GetBestTarget()
//{
//	float ScreenDistanceBase = 1000.f;
//	int m_lBestTarget = -1;
//
//	for ( int PlayerIndex = 1; PlayerIndex < Interfaces::EntityList->GetHighestEntityIndex(); PlayerIndex++ )
//	{
//		CBaseEntity* pPlayer = Interfaces::EntityList->GetClientEntity( PlayerIndex );
//
//		if ( pPlayer && pPlayer->IsAlive() && pPlayer->GetHealth() > 0 && !pPlayer->IsDormant() && pPlayer->GetClientClass()->m_ClassID == 38)
//		{
//			if ( pPlayer == G::LocalPlayer )
//				continue;
//
//			if ( !Config->Legit_FriendlyFire && G::LocalPlayer->GetTeam() == pPlayer->GetTeam() )
//				continue;
//
//			if ( !Config->Legit_WallAttack && !pPlayer->IsVisible( G::LocalPlayer ) )
//				continue;
//
//			Vector vHitbox;
//			Vector vHitboxScreen;
//
//			if ( weapon( G::LocalPlayer->GetActiveWeapon() ).Aimbot_AutoHitbox )
//				vHitbox = pPlayer->GetHitboxPosition( HITBOX_BODY );
//
//			else
//				vHitbox = pPlayer->GetHitboxPosition( tt_ct_best_hit_3[weapon( G::LocalPlayer->GetActiveWeapon() ).Aimbot_Hitbox] );
//
//			if ( Cheat::Utilities->Game_WorldToScreen( vHitbox, vHitboxScreen ) )
//			{
//				Vector2D vPlayerScr = Vector2D( vHitboxScreen.x, vHitboxScreen.y );
//				float fDistanceScreen = Cheat::Utilities->Game_DistanceScreen( Vector2D( G::iScreenWidth / 2, G::iScreenHeight / 2 ), vPlayerScr );
//
//				if ( fDistanceScreen < ScreenDistanceBase )
//				{
//					ScreenDistanceBase = fDistanceScreen;
//					m_lBestTarget = PlayerIndex;
//				}
//			}
//		}
//	}
//
//	return m_lBestTarget;
//}
//
//auto AngleNormalize( Vector &vAngles )
//{
//	if ( vAngles.x > 89.0f && vAngles.x <= 180.0f )
//		vAngles.x = 89.0f;
//
//	if ( vAngles.x > 180.f )
//		vAngles.x -= 360.f;
//
//	if ( vAngles.x < -89.0f )
//		vAngles.x = -89.0f;
//
//	if ( vAngles.y > 180.f )
//		vAngles.y -= 360.f;
//
//	if ( vAngles.y < -180.f )
//		vAngles.y += 360.f;
//
//	vAngles.z = 0;
//}
//
//int CLegitAimbot::GetBestHitbox()
//{
//	float ScreenDistanceBase = 1000.f;
//	int m_lBestHitbox = -1;
//	CBaseEntity* pPlayer = Interfaces::EntityList->GetClientEntity( m_iBestTarget );
//
//	if ( pPlayer && pPlayer->IsAlive() && pPlayer->GetHealth() > 0 && !pPlayer->IsDormant() )
//	{
//		if ( m_lBestHitbox == -1 )
//		{
//			if ( weapon( m_pCurrentWeapon ).Aimbot_AutoHitbox )
//			{
//				for ( int bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++ )
//				{
//					Vector vHitBox = pPlayer->GetHitboxPosition( tt_ct_best_hit_3[bHitbox] );
//					Vector vHitBoxScreen;
//
//					if ( !Cheat::Utilities->Game_WorldToScreen( vHitBox, vHitBoxScreen ) )
//						continue;
//
//					Vector2D vHitboxSrc = Vector2D( vHitBoxScreen.x, vHitBoxScreen.y );
//					float fDistanceScreen = Cheat::Utilities->Game_DistanceScreen( Vector2D( G::iScreenWidth / 2, G::iScreenHeight / 2 ), vHitboxSrc );
//
//					if ( fDistanceScreen < ScreenDistanceBase )
//					{
//						ScreenDistanceBase = fDistanceScreen;
//						m_lBestHitbox = tt_ct_best_hit_3[bHitbox];
//					}
//				}
//			}
//			else
//				m_lBestHitbox = tt_ct_best_hit_3[weapon( m_pCurrentWeapon ).Aimbot_Hitbox];
//		}
//
//		Vector vHitBox = pPlayer->GetHitboxPosition( m_lBestHitbox );
//
//		if ( vHitBox.IsValid() && Cheat::Utilities->Game_WorldToScreen( vHitBox, m_vAimBestHitboxScreen ) )
//		{
//			m_vAimBestHitbox = vHitBox;
//			return m_lBestHitbox;
//		}
//
//		else
//		{
//			m_vAimBestHitbox.Zero();
//			m_vAimBestHitboxScreen.Zero();
//			return -1;
//		}
//	}
//
//	return m_lBestHitbox;
//}
//
//void CLegitAimbot::NoRecoil()
//{
//	if ( !m_vAimBestHitbox.IsValid() )
//		return;
//
//	Vector vAimAngle, vSmoothAimAngle;
//	Cheat::Utilities->Math_VectorAngles( m_vAimBestHitbox - G::LocalPlayer->GetEyePosition(), vAimAngle );
//	bool bEnableRcs = false;
//
//	if ( weapon( m_pCurrentWeapon ).RCS_Value > 0 )
//	{
//		if ( ( m_pCurrentWeapon->Type() == WEAPONTYPE_MACHINEGUN || m_pCurrentWeapon->Type() == WEAPONTYPE_RIFLE || m_pCurrentWeapon->Type() == WEAPONTYPE_SUBMACHINEGUN || m_pCurrentWeapon->Type() == WEAPONTYPE_SHOTGUN ) && G::LocalPlayer->GetShotsFired() > 1 )
//			bEnableRcs = true;
//
//		if ( m_pCurrentWeapon->Type() == WEAPONTYPE_PISTOL && G::LocalPlayer->GetShotsFired() <= 1 )
//			bEnableRcs = true;
//	}
//
//	if ( bEnableRcs )
//	{
//		float MulVar = weapon( m_pCurrentWeapon ).RCS_Value * 0.02f;
//
//		if ( weapon( m_pCurrentWeapon ).RCS_Clamp <= 0 )
//			vAimAngle -= G::LocalPlayer->GetPunchAngles() * MulVar;
//
//		else
//			if ( weapon( m_pCurrentWeapon ).RCS_Clamp == 1 )
//				vAimAngle -= ( G::LocalPlayer->GetViewPunchAngles() + G::LocalPlayer->GetPunchAngles() ) * MulVar;
//
//			else
//				if ( weapon( m_pCurrentWeapon ).RCS_Clamp >= 2 )
//					vAimAngle -= ( G::LocalPlayer->GetViewPunchAngles() + ( G::LocalPlayer->GetPunchAngles() * 2 * 0.5f ) ) * MulVar;
//	}
//
//	float fSmooth = 10.f;
//
//	if ( bEnableRcs && ( m_pCurrentWeapon->Type() == WEAPONTYPE_MACHINEGUN || m_pCurrentWeapon->Type() == WEAPONTYPE_RIFLE || m_pCurrentWeapon->Type() == WEAPONTYPE_SUBMACHINEGUN || m_pCurrentWeapon->Type() == WEAPONTYPE_SHOTGUN ) )
//		fSmooth = weapon( m_pCurrentWeapon ).RCS_Smooth;
//
//	else
//		fSmooth = weapon( m_pCurrentWeapon ).Aimbot_Smooth;
//
//	if ( fSmooth < 10.f )
//		fSmooth = 10.f;
//
//	fSmooth = fSmooth / 10.f;
//	AngleNormalize( vAimAngle );
//	auto SmoothAngles = []( Vector MyViewAngles, Vector AimAngles, Vector & OutAngles, float Smoothing )
//	{
//		OutAngles = AimAngles - MyViewAngles;
//		AngleNormalize( OutAngles );
//		OutAngles.x = OutAngles.x / Smoothing + MyViewAngles.x;
//		OutAngles.y = OutAngles.y / Smoothing + MyViewAngles.y;
//		AngleNormalize( OutAngles );
//	};
//	SmoothAngles( G::UserCmd->viewangles, vAimAngle, vSmoothAimAngle, fSmooth );
//	G::UserCmd->viewangles = vSmoothAimAngle;
//}
//
//void CLegitAimbot::StandAloneNoRecoil()
//{
//}
//
//void CLegitAimbot::AutoPistol()
//{
//	float tick = G::LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
//
//	if ( m_pCurrentWeapon->GetNextPrimaryAttack() < tick )
//		return;
//
//	if ( ( ItemDefinitionIndex )m_pCurrentWeapon->GetItemDefenitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER )
//		G::UserCmd->buttons &= ~IN_ATTACK2;
//
//	else
//		G::UserCmd->buttons &= ~IN_ATTACK;
//}
//
//inline bool CheckPlayerFov( Vector vScreen, int iFov )
//{
//	int	iFovX = ( int )( G::iScreenWidth / 2 ) - ( int )vScreen.x;
//	int	iFovY = ( int )( G::iScreenHeight / 2 ) - ( int )vScreen.y;
//
//	if ( iFovX < iFov && iFovX > -iFov && iFovY < iFov && iFovY > -iFov )
//		return true;
//
//	return false;
//}
//void CLegitAimbot::SetMenu()
//{
//	switch ( m_pCurrentWeapon->Type() )
//	{
//		case WEAPONTYPE_PISTOL:
//
//			switch ( ( ItemDefinitionIndex )m_pCurrentWeapon->GetItemDefenitionIndex() )
//			{
//				case ItemDefinitionIndex::WEAPON_DEAGLE:
//					Config->Legit_CurrentWeapon[1] = 1;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_ELITE:
//					Config->Legit_CurrentWeapon[1] = 2;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_FIVESEVEN:
//					Config->Legit_CurrentWeapon[1] = 3;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_GLOCK:
//					Config->Legit_CurrentWeapon[1] = 4;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_HKP2000:
//					Config->Legit_CurrentWeapon[1] = 5;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_P250:
//					Config->Legit_CurrentWeapon[1] = 6;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_USP_SILENCER:
//					Config->Legit_CurrentWeapon[1] = 7;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_CZ75A:
//					Config->Legit_CurrentWeapon[1] = 8;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_REVOLVER:
//					Config->Legit_CurrentWeapon[1] = 9;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_TEC9:
//					Config->Legit_CurrentWeapon[1] = 10;
//					break;
//			}
//
//			break;
//
//		case WEAPONTYPE_SUBMACHINEGUN:
//
//			switch ( ( ItemDefinitionIndex )m_pCurrentWeapon->GetItemDefenitionIndex() )
//			{
//				case ItemDefinitionIndex::WEAPON_MAC10:
//					Config->Legit_CurrentWeapon[3] = 1;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_P90:
//					Config->Legit_CurrentWeapon[3] = 2;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_UMP45:
//					Config->Legit_CurrentWeapon[3] = 3;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_BIZON:
//					Config->Legit_CurrentWeapon[3] = 4;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_MP7:
//					Config->Legit_CurrentWeapon[3] = 5;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_MP5SD:
//					Config->Legit_CurrentWeapon[3] = 6;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_MP9:
//					Config->Legit_CurrentWeapon[3] = 7;
//					break;
//			}
//
//			break;
//
//		case WEAPONTYPE_RIFLE:
//
//			switch ( ( ItemDefinitionIndex )m_pCurrentWeapon->GetItemDefenitionIndex() )
//			{
//				case ItemDefinitionIndex::WEAPON_AK47:
//					Config->Legit_CurrentWeapon[0] = 1;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_AUG:
//					Config->Legit_CurrentWeapon[0] = 2;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_FAMAS:
//					Config->Legit_CurrentWeapon[0] = 3;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_GALILAR:
//					Config->Legit_CurrentWeapon[0] = 4;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_M4A1:
//					Config->Legit_CurrentWeapon[0] = 5;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_M4A1_SILENCER:
//					Config->Legit_CurrentWeapon[0] = 6;
//					break;
//
//				case ItemDefinitionIndex::WEAPON_SG556:
//					Config->Legit_CurrentWeapon[0] = 7;
//					break;
//			}
//
//			break;
//
//		case WEAPONTYPE_SHOTGUN:
//			{
//
//
//				switch ( ( ItemDefinitionIndex )m_pCurrentWeapon->GetItemDefenitionIndex() )
//				{
//					case ItemDefinitionIndex::WEAPON_XM1014:
//						Config->Legit_CurrentWeapon[4] = 1;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_MAG7:
//						Config->Legit_CurrentWeapon[4] = 2;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_NEGEV:
//						Config->Legit_CurrentWeapon[4] = 3;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_SAWEDOFF:
//						Config->Legit_CurrentWeapon[4] = 4;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_NOVA:
//						Config->Legit_CurrentWeapon[4] = 5;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_M249:
//						Config->Legit_CurrentWeapon[4] = 6;
//						break;
//				}
//			}
//			break;
//
//		case WEAPONTYPE_MACHINEGUN:
//			{
//
//
//				switch ( ( ItemDefinitionIndex )m_pCurrentWeapon->GetItemDefenitionIndex() )
//				{
//					case ItemDefinitionIndex::WEAPON_XM1014:
//						Config->Legit_CurrentWeapon[4] = 1;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_MAG7:
//						Config->Legit_CurrentWeapon[4] = 2;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_NEGEV:
//						Config->Legit_CurrentWeapon[4] = 3;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_SAWEDOFF:
//						Config->Legit_CurrentWeapon[4] = 4;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_NOVA:
//						Config->Legit_CurrentWeapon[4] = 5;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_M249:
//						Config->Legit_CurrentWeapon[4] = 6;
//						break;
//				}
//			}
//			break;
//
//		case WEAPONTYPE_SNIPER_RIFLE:
//			{
//
//				switch ( ( ItemDefinitionIndex )m_pCurrentWeapon->GetItemDefenitionIndex() )
//				{
//					case ItemDefinitionIndex::WEAPON_G3SG1:
//						Config->Legit_CurrentWeapon[2] = 1;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_SCAR20:
//						Config->Legit_CurrentWeapon[2] = 2;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_AWP:
//						Config->Legit_CurrentWeapon[2] = 3;
//						break;
//
//					case ItemDefinitionIndex::WEAPON_SSG08:
//						Config->Legit_CurrentWeapon[2] = 4;
//						break;
//				}
//			}
//			break;
//	}
//}
//
//void CLegitAimbot::Do()
//{
//	m_bAimShot = true;
//	m_bTargetFov = false;
//
//	if ( !IsEnable() )
//		return;
//
//	m_pCurrentWeapon = G::LocalPlayer->GetActiveWeapon();
//	SetMenu();
//
//	if ( !weapon( m_pCurrentWeapon ).Aimbot_Active )
//		return;
//
//	m_iBestPreTarget = m_iBestTarget;
//	m_iBestTarget = GetBestTarget();
//
//	if ( weapon( m_pCurrentWeapon ).Aimbot_AutoPistol && m_pCurrentWeapon->Type() == WEAPONTYPE_PISTOL && G::UserCmd->buttons & IN_ATTACK )
//		AutoPistol();
//
//	if ( m_iBestTarget == -1 )
//		return;
//
//	if ( m_iBestPreTarget != m_iBestTarget && G::UserCmd->buttons & IN_ATTACK )
//		m_bTargetChange = true;
//
//	else
//		m_bTargetChange = false;
//
//	m_iBestHitbox = GetBestHitbox();
//
//	if ( m_iBestHitbox == -1 )
//		return;
//
//	CBaseEntity* pPreTargetPlayer = Interfaces::EntityList->GetClientEntity( m_iBestPreTarget );
//	CBaseEntity* pTargetPlayer = Interfaces::EntityList->GetClientEntity( m_iBestTarget );
//	int iPlayerFov = GetPlayerFov( pTargetPlayer );
//
//	if ( CheckPlayerFov( m_vAimBestHitboxScreen, iPlayerFov ) )
//		m_bTargetFov = true;
//
//	static bool bLockDelay = false;
//	static bool bLockAttack = false;
//
//	if ( !( G::UserCmd->buttons & IN_ATTACK ) )
//	{
//		m_bClamp = true;
//		bLockDelay = false;
//		bLockAttack = false;
//		m_pShotDelay->reset();
//	}
//
//	if ( weapon( m_pCurrentWeapon ).Aimbot_AutoPistol && m_pCurrentWeapon->Type() == WEAPONTYPE_PISTOL )
//	{
//		if ( m_bTargetFov && !G::UserCmd->buttons & IN_ATTACK )
//			m_bAutoPistolEn = true;
//
//		else
//			if ( !m_bTargetFov && !G::UserCmd->buttons & IN_ATTACK )
//				m_bAutoPistolEn = false;
//
//			else
//				if ( !m_bTargetFov && G::UserCmd->buttons & IN_ATTACK )
//					m_bAutoPistolEn = false;
//	}
//
//	if ( !weapon( m_pCurrentWeapon ).Aimbot_AutoPistol && m_pCurrentWeapon->Type() == WEAPONTYPE_PISTOL )
//	{
//		if ( G::LocalPlayer->GetShotsFired() == 1 && bLockDelay )
//			return;
//	}
//
//	AIM_ATTACK_TYPE AttackType = AA_SHOTGUN;
//
//	if ( m_pCurrentWeapon->Type() == WEAPONTYPE_SNIPER_RIFLE || m_pCurrentWeapon->Type() == WEAPONTYPE_PISTOL )
//		AttackType = AA_SNIPER_PISTOL;
//
//	static bool bAimAttack = false;
//	static bool bAimCheckAttack = false;
//
//	if ( AttackType == AA_SHOTGUN )
//	{
//		if ( G::UserCmd->buttons & IN_ATTACK && m_bTargetFov )
//			bAimCheckAttack = true;
//
//		else
//			bAimCheckAttack = false;
//	}
//
//	else
//		if ( AttackType == AA_SNIPER_PISTOL )
//		{
//			if ( !weapon( m_pCurrentWeapon ).Aimbot_AutoPistol )
//			{
//				if ( m_bTargetFov && !G::UserCmd->buttons & IN_ATTACK )
//					bAimAttack = true;
//
//				else
//					if ( !m_bTargetFov && G::UserCmd->buttons & IN_ATTACK )
//						bAimAttack = false;
//
//					else
//						if ( !m_bTargetFov && !G::UserCmd->buttons & IN_ATTACK )
//							bAimAttack = false;
//
//				if ( G::UserCmd->buttons & IN_ATTACK && bAimAttack && m_bTargetFov )
//					bAimCheckAttack = true;
//
//				else
//					bAimCheckAttack = false;
//			}
//
//			else
//			{
//				if ( G::UserCmd->buttons & IN_ATTACK && m_bTargetFov )
//					bAimCheckAttack = true;
//
//				else
//				{
//					bAimCheckAttack = false;
//					m_bClamp = true;
//				}
//			}
//		}
//
//	if ( weapon( m_pCurrentWeapon ).RCS_Clamp >= 1 && ( m_pCurrentWeapon->Type() == WEAPONTYPE_MACHINEGUN || m_pCurrentWeapon->Type() == WEAPONTYPE_RIFLE || m_pCurrentWeapon->Type() == WEAPONTYPE_SUBMACHINEGUN || m_pCurrentWeapon->Type() == WEAPONTYPE_SHOTGUN ) )
//	{
//		if ( !bLockAttack )
//		{
//			if ( m_bTargetFov && !G::UserCmd->buttons & IN_ATTACK )
//				bAimAttack = true;
//
//			else
//				if ( !m_bTargetFov && G::UserCmd->buttons & IN_ATTACK )
//					bAimAttack = false;
//
//				else
//					if ( !m_bTargetFov && !G::UserCmd->buttons & IN_ATTACK )
//						bAimAttack = false;
//		}
//
//		if ( G::UserCmd->buttons & IN_ATTACK && bAimAttack && m_bTargetFov && !bLockAttack )
//		{
//			bAimCheckAttack = true;
//			bLockAttack = true;
//		}
//
//		else
//			if ( G::UserCmd->buttons & IN_ATTACK && m_bTargetFov && bLockAttack )
//			{
//				bAimCheckAttack = true;
//				bLockAttack = true;
//			}
//
//			else
//				bAimCheckAttack = false;
//	}
//
//	if ( !( pTargetPlayer->GetFlags() & FL_ONGROUND ) && Config->Legit_JumpCheck )
//		return;
//
//	if ( bAimCheckAttack )
//	{
//		int AimDelay = weapon( m_pCurrentWeapon ).Aimbot_Delay;
//
//		if ( AimDelay )
//		{
//			if ( m_pShotDelay->delay( AimDelay ) )
//				m_bAimShot = false;
//		}
//
//		if ( m_bClamp && m_bAimShot )
//		{
//			if ( Config->Legit_SmokeCheck )
//			{
//				if ( LineGoesThroughSmoke( G::LocalPlayer->GetEyePosition(), m_vAimBestHitbox ) )
//					return;
//			}
//
//			NoRecoil();
//		}
//
//		if ( m_pCurrentWeapon->Type() == WEAPONTYPE_PISTOL && weapon( m_pCurrentWeapon ).Aimbot_AutoPistol && m_bAutoPistolEn )
//		{
//			if ( m_bAimShot )
//			{
//				G::UserCmd->buttons |= IN_ATTACK;
//				m_bAutoPistolEn = false;
//			}
//
//			else
//				G::UserCmd->buttons &= ~IN_ATTACK;
//		}
//	}
//
//	if ( G::SendPacket )
//		Interfaces::Engine->SetViewAngles( &G::UserCmd->viewangles );
//}
//
//#include "../../Features/Visuals/Drawing.h"
//
//void CLegitAimbot::Draw()
//{
//
//	if ( !IsEnable() || m_iBestTarget == -1 || m_iBestHitbox == -1 || !weapon( m_pCurrentWeapon ).Aimbot_Active )
//		return;
//
//	if ( m_vAimBestHitboxScreen.x <= 0.f || m_vAimBestHitboxScreen.y <= 0.f )
//		return;
//
//	CBaseEntity* pTargetPlayer = Interfaces::EntityList->GetClientEntity( m_iBestTarget );
//	CColor TargetFovColor = CColor(
//	        atoi( Interfaces::Var->FindVar( "cl_crosshaircolor_r" )->GetString() ),
//	        atoi( Interfaces::Var->FindVar( "cl_crosshaircolor_g" )->GetString() ),
//	        atoi( Interfaces::Var->FindVar( "cl_crosshaircolor_b" )->GetString() )
//	    );
//
//	if ( Config->Legit_DrawAimSpot )
//	{
//		studiohdr_t* pStudioModel = Interfaces::ModelInfo->GetStudioModel( pTargetPlayer->GetModel() );
//
//		if ( !pStudioModel )
//			return;
//
//		static matrix3x4_t pBoneToWorldOut[128];
//
//		if ( !pTargetPlayer->SetupBones( pBoneToWorldOut, 128, 256, Interfaces::Globals->curtime ) )
//			return;
//
//		mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet( 0 );
//
//		if ( !pHitboxSet )
//			return;
//
//		for ( int i = 0; i < pHitboxSet->numhitboxes; i++ )
//		{
//			mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox( i );
//
//			if ( !pHitbox || i != m_iBestHitbox )
//				continue;
//
//			Vector vMin, vMax;
//			Cheat::Utilities->Math_VectorTransform( pHitbox->bbmin, pBoneToWorldOut[pHitbox->bone], vMin );
//			Cheat::Utilities->Math_VectorTransform( pHitbox->bbmax, pBoneToWorldOut[pHitbox->bone], vMax );
//
//			if ( pHitbox->radius > -1 )
//				Interfaces::DebugOverlay->AddCapsuleOverlay( vMin, vMax, pHitbox->radius + 2.f, TargetFovColor.r(), TargetFovColor.g(), TargetFovColor.b(), 250, Config->Legit_EnginePrediction ? 0.1f : 0.f );
//		}
//	}
//
//	if ( Config->Legit_DrawFOV )
//	{
//		int iPlayerFov = GetPlayerFov( pTargetPlayer );
//
//		if ( weapon( m_pCurrentWeapon ).Aimbot_FOVType <= 0 )
//			Graphics::Drawing->DrawCircle( m_vAimBestHitboxScreen.x - iPlayerFov, m_vAimBestHitboxScreen.y - iPlayerFov,
//			    iPlayerFov, TargetFovColor );
//
//		else
//			Graphics::Drawing->DrawCircle( G::iScreenWidth / 2, G::iScreenHeight / 2, iPlayerFov, TargetFovColor );
//	}
//}