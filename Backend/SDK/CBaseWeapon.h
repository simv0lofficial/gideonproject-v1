#pragma once
#include "../Interfaces/Interfaces.h"
#include "../Utility/Utilities.h"
#include "../Configuration/Settings.h"
#include "../Interfaces/NetVar.h"
#include "../Hashing/xorstr.h"
#include "vfunc.h"
#include "vector3d.h"
class CBaseEntity;
#include "GlobalDefenitions.h"
struct CSWeaponInfo
{
	char _0x0000[20];
	__int32 max_clip;           //0x0014
	char _0x0018[12];
	__int32 max_reserved_ammo;  //0x0024
	char _0x0028[96];
	char* hud_name;             //0x0088
	char* weapon_name;          //0x008C
	char _0x0090[60];
	__int32 type;               //0x00CC
	__int32 price;              //0x00D0
	__int32 reward;             //0x00D4
	char _0x00D8[20];
	BYTE full_auto;             //0x00EC
	char _0x00ED[3];
	__int32 damage;             //0x00F0
	float armor_ratio;          //0x00F4
	__int32 bullets;            //0x00F8
	float penetration;          //0x00FC
	char _0x0100[8];
	float range;                //0x0108
	float range_modifier;       //0x010C
	char _0x0110[16];
	BYTE silencer;              //0x0120
	char _0x0121[15];
	float max_speed;            //0x0130
	float max_speed_alt;        //0x0134
	char _0x0138[76];
	__int32 recoil_seed;        //0x0184
	char _0x0188[32];

	char* bullet_type() {
		return *(char**)((uintptr_t)this + 0x80);
	}
}; //Size: 0x1D0

enum ItemDefinitionIndex
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

enum WeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

using namespace Cheat;

class CBaseWeapon
{
	public:
		float GetNextPrimaryAttack()
		{
			if ( !this )
				return 0.f;

			return *reinterpret_cast <float*>( uintptr_t( this ) + offys.m_flnextprimaryattack );
		}

		float GetNextSecondaryAttack()
		{
			if ( !this )
				return 0.f;

			return *( float* )( ( DWORD )this + offys.m_flNextSecondaryAttack );
		}

		int GetAmmoInClip()
		{
			if ( !this )
				return 0;

			return *( int* )( ( uintptr_t )this + offys.m_iclip1 );
		}


		float GetLastShotTime()
		{
			if (!this)
				return 0.f;

			return *(float*)((DWORD)this + offys.m_fLastShotTime);
		}
	


		short GetItemDefenitionIndex()
		{
			if ( this != nullptr )
				return *reinterpret_cast <short*>( uintptr_t( this ) + offys.m_iitemdefinitionindex );

			return 0;
		}

		float GetPostponeFireReadyTime()
		{
			if ( !this )
				return 0.f;

			return *reinterpret_cast <float*>( uintptr_t( this ) + offys.m_flpostponefirereadytime );
		}

		float GetSpreadCone()
		{
			typedef float( __thiscall * Fn )( void* );

			if ( this )
				return vfunc < Fn >( this, 439 )( this );
		}

		float GetInaccuracy()
		{
			typedef float( __thiscall * Fn )( void* );

			if ( this )
				return vfunc < Fn >( this, 471 )( this );
		}

		HANDLE GetOwnerHandle()
		{
			if ( this )
				return *reinterpret_cast <HANDLE*>( ( DWORD )this + offys.m_howner );
		}

		Vector GetOrigin()
		{
			if ( this )
				return *( Vector* )( ( uintptr_t )this + 0x00000134 );
		}

		bool is_revolver()
		{
			if ( this )
			{
				int iWeaponID = this->GetItemDefenitionIndex();
				return ( iWeaponID == 64 );
			}
		}

		bool is_knife()
		{
			if ( this )
			{
				int iWeaponID = this->GetItemDefenitionIndex();
				return ( iWeaponID == 42 || iWeaponID == 59 ||
				        iWeaponID == 59 || iWeaponID == 41 ||
				        iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506 ||
				        iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509 ||
				        iWeaponID == 515 );
			}
		}

		bool IsPinPulled()
		{
			if ( !this || !this->is_grenade() )
				return false;

			return *( bool* )( ( DWORD )this + offys.m_bPinPulled );
		}

		float GetThrowTime()
		{
			if ( !this || !this->is_grenade() )
				return 0.f;

			return *( float* )( ( DWORD )this + offys.m_fThrowTime );
		}

		bool is_grenade()
		{
			if ( !this )
				return false;

			int WeaponId = this->GetItemDefenitionIndex();

			if ( !WeaponId )
				return false;

			if ( WeaponId == 43 || WeaponId == 44 || WeaponId == 45 || WeaponId == 46 || WeaponId == 47 || WeaponId == 48 )
				return true;

			return false;
		}
		int GetZoomLevel()
		{
			return *( PINT )( ( DWORD )this + offys.m_izoomLevel );
		}

		bool is_sniper()
		{
			if ( !this )
				return false;

			int WeaponId = this->GetItemDefenitionIndex();

			if ( !WeaponId )
				return false;

			if ( WeaponId == 9 || WeaponId == 11 || WeaponId == 38 || WeaponId == 40 )
				return true;

			return false;
		}
#define GUNICON(t, x) case t: \
				return #x;

		const char* GetGunIcon( short itemdef )
		{
			switch ( ( ItemDefinitionIndex )itemdef )
			{
					GUNICON(ItemDefinitionIndex::WEAPON_DEAGLE, DESERT EAGLE);
					GUNICON(ItemDefinitionIndex::WEAPON_ELITE, DUAL BERETTAS);
					GUNICON(ItemDefinitionIndex::WEAPON_FIVESEVEN, FIVE-SEVEN);
					GUNICON(ItemDefinitionIndex::WEAPON_GLOCK, GLOCK-18);
					GUNICON(ItemDefinitionIndex::WEAPON_AK47, AK-47);
					GUNICON(ItemDefinitionIndex::WEAPON_AUG, AUG);
					GUNICON(ItemDefinitionIndex::WEAPON_AWP, AWP);
					GUNICON(ItemDefinitionIndex::WEAPON_FAMAS, FAMAS);
					GUNICON(ItemDefinitionIndex::WEAPON_G3SG1, G3SG1);
					GUNICON(ItemDefinitionIndex::WEAPON_GALILAR, GALIL-AR);
					GUNICON(ItemDefinitionIndex::WEAPON_M249, M249);
					GUNICON(ItemDefinitionIndex::WEAPON_M4A1, M4A4);
					GUNICON(ItemDefinitionIndex::WEAPON_MAC10, MAC-10);
					GUNICON(ItemDefinitionIndex::WEAPON_P90, P90);
					GUNICON(ItemDefinitionIndex::WEAPON_UMP45, UMP-45);
					GUNICON(ItemDefinitionIndex::WEAPON_XM1014, XM1014);
					GUNICON(ItemDefinitionIndex::WEAPON_BIZON, PP-BIZON);
					GUNICON(ItemDefinitionIndex::WEAPON_MAG7, MAG-7);
					GUNICON(ItemDefinitionIndex::WEAPON_NEGEV, NEGEV);
					GUNICON(ItemDefinitionIndex::WEAPON_SAWEDOFF, SAWED-OFF);
					GUNICON(ItemDefinitionIndex::WEAPON_TEC9, TEC-9);
					GUNICON(ItemDefinitionIndex::WEAPON_TASER, ZEUS);
					GUNICON(ItemDefinitionIndex::WEAPON_HKP2000, P2000);
					GUNICON(ItemDefinitionIndex::WEAPON_MP7, MP7);
					GUNICON(ItemDefinitionIndex::WEAPON_MP9, MP9);
					GUNICON(ItemDefinitionIndex::WEAPON_NOVA, NOVA);
					GUNICON(ItemDefinitionIndex::WEAPON_P250, P250);
					GUNICON(ItemDefinitionIndex::WEAPON_SCAR20, SCAR-20);
					GUNICON(ItemDefinitionIndex::WEAPON_SG556, SG-556);
					GUNICON(ItemDefinitionIndex::WEAPON_SSG08, SSG-08);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_FLASHBANG, FLASHBANG);
					GUNICON(ItemDefinitionIndex::WEAPON_HEGRENADE, HIGH EXPLOSIVE GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_SMOKEGRENADE, SMOKE GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_MOLOTOV, MOLOTOV);
					GUNICON(ItemDefinitionIndex::WEAPON_DECOY, DECOY);
					GUNICON(ItemDefinitionIndex::WEAPON_INCGRENADE, INCENDIARY GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_C4, BOMB);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_T, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_M4A1_SILENCER, M4A1-S);
					GUNICON(ItemDefinitionIndex::WEAPON_USP_SILENCER, USP-S);
					GUNICON(ItemDefinitionIndex::WEAPON_CZ75A, CZ75-AUTO);
					GUNICON(ItemDefinitionIndex::WEAPON_REVOLVER, R8 REVOLVER);
					GUNICON(ItemDefinitionIndex::WEAPON_BAYONET, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_FLIP, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_GUT, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_FALCHION, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_PUSH, KNIFE);
					GUNICON( ItemDefinitionIndex::WEAPON_MP5SD, MP5-SD);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFEGG, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_HEALTHSHOT, HEALTHSHOT);
					GUNICON(ItemDefinitionIndex::WEAPON_HAMMER, HAMMER);
					GUNICON(ItemDefinitionIndex::WEAPON_FISTS, FISTS);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_STILETTO, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_AXE, AXE);
					GUNICON(ItemDefinitionIndex::WEAPON_TABLET, TABLET);
					GUNICON(ItemDefinitionIndex::WEAPON_BREACHCHARGE, BREACH CHARGE);
					GUNICON(ItemDefinitionIndex::WEAPON_TAGRENADE, TA GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_DIVERSION, DIVERSION GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_URSUS, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_SPANNER, SPANNER);

				default:
					return "UNKNOWN";
			}
		}

		int GetLoadedAmmo()
		{
			if ( this )
				return *( int* )( ( DWORD )this + 0x3204 );
		}

		ClientClass* GetClientClass()
		{
			void* Networkable = ( void* )( this + 0x8 );
			typedef ClientClass*( __thiscall * OriginalFn )( void* );

			if ( this )
				return vfunc<OriginalFn>( Networkable, 2 )( Networkable );
		}

		void UpdateAccuracyPenalty()
		{
			typedef void( __thiscall * Fn )( void* );

			if ( this )
				return vfunc<Fn>( this, 472 )( this );
		}

		bool IsReloading()
		{
			if(this)
				return *( bool* )((uint32_t)this +*(uint32_t*)offys.dwReloading);
		}


		CSWeaponInfo* get_full_info()
		{
			if ( this && this != NULL && this != nullptr )
			{
				typedef CSWeaponInfo *( __thiscall * fn )( void* );
				return vfunc<fn>( this, 448 )( this );
			}
		}

		int Type()
		{
			if ( !this )
				return WEAPONTYPE_UNKNOWN;

			auto id = this->GetItemDefenitionIndex();

			switch ( ( ItemDefinitionIndex )id )
			{
				case ItemDefinitionIndex::WEAPON_DEAGLE:
				case ItemDefinitionIndex::WEAPON_ELITE:
				case ItemDefinitionIndex::WEAPON_FIVESEVEN:
				case ItemDefinitionIndex::WEAPON_HKP2000:
				case ItemDefinitionIndex::WEAPON_USP_SILENCER:
				case ItemDefinitionIndex::WEAPON_CZ75A:
				case ItemDefinitionIndex::WEAPON_TEC9:
				case ItemDefinitionIndex::WEAPON_REVOLVER:
				case ItemDefinitionIndex::WEAPON_GLOCK:
				case ItemDefinitionIndex::WEAPON_P250:
					return WEAPONTYPE_PISTOL;
					break;

				case ItemDefinitionIndex::WEAPON_AK47:
				case ItemDefinitionIndex::WEAPON_M4A1:
				case ItemDefinitionIndex::WEAPON_M4A1_SILENCER:
				case ItemDefinitionIndex::WEAPON_GALILAR:
				case ItemDefinitionIndex::WEAPON_AUG:
				case ItemDefinitionIndex::WEAPON_FAMAS:
				case ItemDefinitionIndex::WEAPON_SG556:
					return WEAPONTYPE_RIFLE;
					break;

				case ItemDefinitionIndex::WEAPON_P90:
				case ItemDefinitionIndex::WEAPON_BIZON:
				case ItemDefinitionIndex::WEAPON_MP7:
				case ItemDefinitionIndex::WEAPON_MP9:
				case ItemDefinitionIndex::WEAPON_MAC10:
				case ItemDefinitionIndex::WEAPON_UMP45:
				case ItemDefinitionIndex::WEAPON_MP5SD:
					return WEAPONTYPE_SUBMACHINEGUN;
					break;

				case ItemDefinitionIndex::WEAPON_AWP:
				case ItemDefinitionIndex::WEAPON_G3SG1:
				case ItemDefinitionIndex::WEAPON_SCAR20:
				case ItemDefinitionIndex::WEAPON_SSG08:
					return WEAPONTYPE_SNIPER_RIFLE;
					break;

				case ItemDefinitionIndex::WEAPON_NEGEV:
				case ItemDefinitionIndex::WEAPON_M249:
					return WEAPONTYPE_SHOTGUN;
					break;

				case ItemDefinitionIndex::WEAPON_MAG7:
				case ItemDefinitionIndex::WEAPON_SAWEDOFF:
				case ItemDefinitionIndex::WEAPON_NOVA:
				case ItemDefinitionIndex::WEAPON_XM1014:
					return WEAPONTYPE_SHOTGUN;
					break;

				case ItemDefinitionIndex::WEAPON_KNIFE:
				case ItemDefinitionIndex::WEAPON_BAYONET:
				case ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
				case ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
				case ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
				case ItemDefinitionIndex::WEAPON_KNIFE_GUT:
				case ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
				case ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
				case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
				case ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
				case ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
				case ItemDefinitionIndex::WEAPON_KNIFE_T:
					return WEAPONTYPE_KNIFE;
					break;

				default:
					return WEAPONTYPE_UNKNOWN;
			}
		}

};



class CCSBomb
{
	public:
		// Is bomb still ticking down
		bool IsBombTicking()
		{
			if ( this )
				return *( bool* )( ( DWORD )this + offys.m_bBombTicking );
		}

		// Where is bomb planted
		int GetBombSite()
		{
			if ( this )
				return *( int* )( ( DWORD )this + offys.m_nBombSite );
		}

		// When bomb will be exploded
		float GetBlowTime()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flC4Blow );
		}

		// Length of bomb timer
		float GetTimerLength()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flTimerLength );
		}

		// Length of bomb defuse timer
		float GetDefuseLength()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flDefuseLength );
		}

		// When bomb will be defused
		float GetDefuseTime()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flDefuseCountDown );
		}

		// Was bomb defused
		bool IsBombDefused()
		{
			if ( this )
				return *( bool* )( ( DWORD )this + offys.m_bBombDefused );
		}

		ULONG GetBombDefuser()
		{
			if ( !this )
				return NULL;

			return *( PULONG )( ( DWORD )this + offys.m_hBombDefuser );
		}
};
