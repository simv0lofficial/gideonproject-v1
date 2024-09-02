#pragma once
//#include "../../Include/GlobalIncludes.h"
//
//#include "../../SDK/CUserCmd.h"
//#include "../../SDK/CBaseEntity.h"
//#include "../../SDK/GlobalDefenitions.h"
//
//#include "../../Utility/Timer.h"
//
//#include "../../Configuration/Config.h"
//
//enum AIM_ATTACK_TYPE
//{
//	AA_SHOTGUN,
//	AA_SNIPER_PISTOL
//};
//
//namespace Features
//{
//	class CLegitAimbot
//	{
//		public:
//			void Do();
//			void Draw();
//			void StandAloneNoRecoil();
//
//
//			int GetPlayerFov( CBaseEntity* pPlayer );
//			int GetBestTarget();
//			int GetBestHitbox();
//			void NoRecoil();
//
//			void AutoPistol();
//			bool IsEnable();
//			void SetMenu();
//			int		m_iBestPreTarget;
//
//			int		m_iBestTarget;
//			int		m_iBestHitbox;
//
//			bool	m_bClamp;
//			bool	m_bAimShot;
//			bool	m_bTargetFov;
//			bool	m_bTargetChange;
//			bool	m_bAutoPistolEn;
//
//			CBaseWeapon* m_pCurrentWeapon;
//			Vector	m_vAimBestHitbox;
//			Vector  m_vAimBestHitboxScreen;
//	};
//
//	extern CLegitAimbot* LegitAimbot;
//	extern CTimer* m_pShotDelay;
//}