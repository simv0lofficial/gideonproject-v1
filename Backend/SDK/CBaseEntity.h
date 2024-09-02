#pragma once
#include "../Include/GlobalIncludes.h"
#include "vmatrix.h"
#include "qangle.h"
#include "AnimLayer.h"
#include "vfunc.h"
#include "../Interfaces/Interfaces.h"
#include "../Configuration/Settings.h"
#include "../Utility/Utilities.h"
#include "../Interfaces/NetVar.h"
#include "CBaseWeapon.h"
#include "EHandle.h"
#include <array>
#include "Collideable.h"
#include "GlobalDefenitions.h"
#include "CBaseAnimState.h"
#include "../Lua/CLua.h"

class CBaseAnimState;
class Collideable;
class ClientClass;

struct studiohdr_t;
struct model_t {};
typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8_t;

class matrix3x4_t;
class IClientUnknown;
enum ClientFrameStage_t : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

enum class ClassID 
{
	CAI_BaseNPC = 0,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBRC4Target,
	CBreachCharge,
	CBreachChargeProjectile,
	CBreakableProp,
	CBreakableSurface,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDangerZone,
	CDangerZoneController,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDrone,
	CDronegun,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvGasCanister,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFists,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CGrassBurn,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoMapRegion,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemCash,
	CItemDogtags,
	CKnife,
	CKnifeGG,
	CLightGlow,
	CMaterialModifyControl,
	CMelee,
	CMolotovGrenade,
	CMolotovProjectile,
	CMovieDisplay,
	CParadropChopper,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysicsPropMultiplayer,
	CPhysMagnet,
	CPhysPropAmmoBox,
	CPhysPropLootCrate,
	CPhysPropRadarJammer,
	CPhysPropWeaponUpgrade,
	CPlantedC4,
	CPlasma,
	CPlayerResource,
	CPointCamera,
	CPointCommentaryNode,
	CPointWorldText,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CProp_Hallucination,
	CPropCounter,
	CPropDoorRotating,
	CPropJeep,
	CPropVehicleDriveable,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRopeKeyframe,
	CSCAR17,
	CSceneEntity,
	CSensorGrenade,
	CSensorGrenadeProjectile,
	CShadowControl,
	CSlideshowDisplay,
	CSmokeGrenade,
	CSmokeGrenadeProjectile,
	CSmokeStack,
	CSpatialEntity,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CStatueProp,
	CSteamJet,
	CSun,
	CSunlightShadowControl,
	CSurvivalSpawnChopper,
	CTablet,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFireBullets,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlantBomb,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTERadioIcon,
	CTEShatterSurface,
	CTEShowLine,
	CTesla,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CTriggerPlayerMovement,
	CTriggerSoundOperator,
	CVGuiScreen,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAug,
	CWeaponAWP,
	CWeaponBaseItem,
	CWeaponBizon,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponCycler,
	CWeaponElite,
	CWeaponFamas,
	CWeaponFiveSeven,
	CWeaponG3SG1,
	CWeaponGalil,
	CWeaponGalilAR,
	CWeaponGlock,
	CWeaponHKP2000,
	CWeaponM249,
	CWeaponM3,
	CWeaponM4A1,
	CWeaponMAC10,
	CWeaponMag7,
	CWeaponMP5Navy,
	CWeaponMP7,
	CWeaponMP9,
	CWeaponNegev,
	CWeaponNOVA,
	CWeaponP228,
	CWeaponP250,
	CWeaponP90,
	CWeaponSawedoff,
	CWeaponSCAR20,
	CWeaponScout,
	CWeaponSG550,
	CWeaponSG552,
	CWeaponSG556,
	CWeaponSSG08,
	CWeaponTaser,
	CWeaponTec9,
	CWeaponTMP,
	CWeaponUMP45,
	CWeaponUSP,
	CWeaponXM1014,
	CWorld,
	CWorldVguiText,
	DustTrail,
	MovieExplosion,
	ParticleSmokeGrenade,
	RocketTrail,
	SmokeTrail,
	SporeExplosion,
	SporeTrail,
};

using namespace Cheat;

class CBaseAnimating
{
	public:
		std::array<float, 24>* m_flPoseParameter()
		{
			if ( !this )
				return new std::array<float, 24>();

			return ( std::array<float, 24>* )( ( uintptr_t )this + offys.m_poseparam );
		}

		model_t* GetModel()
		{
			void* pRenderable = reinterpret_cast<void*>( uintptr_t( this ) + 0x4 );
			typedef model_t* ( __thiscall * fnGetModel )( void* );

			if ( this )
				return vfunc<fnGetModel>( pRenderable, 8 )( pRenderable );
		}

		void SetBoneMatrix( matrix3x4_t* boneMatrix )
		{
			const auto model = this->GetModel();

			if ( !model )
				return;

			matrix3x4_t* matrix = *( matrix3x4_t** )( ( DWORD )this + 9880 );
			studiohdr_t* hdr = Interfaces::ModelInfo->GetStudioModel( model );

			if ( !hdr )
				return;

			int size = hdr->numbones;

			if ( matrix && this )
			{
				for ( int i = 0; i < size; i++ )
					memcpy( matrix + i, boneMatrix + i, sizeof( matrix3x4_t ) );
			}
		}

		void GetDirectBoneMatrix( matrix3x4_t* boneMatrix )
		{
			const auto model = this->GetModel();

			if ( !model )
				return;

			matrix3x4_t* matrix = *( matrix3x4_t** )( ( DWORD )this + 9880 );
			studiohdr_t* hdr = Interfaces::ModelInfo->GetStudioModel( model );

			if ( !hdr )
				return;

			int size = hdr->numbones;

			if ( matrix && this )
			{
				for ( int i = 0; i < size; i++ )
					memcpy( boneMatrix + i, matrix + i, sizeof( matrix3x4_t ) );
			}
		}
};

enum
{
	OBS_MODE_NONE = 0,	// not in spectator mode
	OBS_MODE_DEATHCAM,	// special mode for death cam animation
	OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,		// view from a fixed camera position
	OBS_MODE_IN_EYE,	// follow a player in first person view
	OBS_MODE_CHASE,		// follow a player in third person view
	OBS_MODE_POI,		// PASSTIME point of interest - game objective, big fight, anything interesting; added in the middle of the enum due to tons of hard-coded xorstr_("<ROAMING") enum compares
	OBS_MODE_ROAMING,	// free roaming

	NUM_OBSERVER_MODES,
};

class CBaseViewModel : public IClientEntity
{
public:
	//[swap_lines]
	int		GetModelIndex();
	void	SetModelIndex(int nModelIndex);
	void	SetWeaponModel(const char* Filename, IClientEntity* Weapon);

	DWORD	GetOwner();
	DWORD	GetWeapon();
	//[/swap_lines]
};

class CBaseAttributableItem : public IClientEntity
{
public:
	//[swap_lines]
	int*			GetItemDefinitionIndex();
	int*			GetItemIDHigh();
	int*			GetAccountID();
	int*			GetEntityQuality();
	int*			GetOriginalOwnerXuidLow();
	int*			GetOriginalOwnerXuidHigh();
	int*			GetFallbackPaintKit();
	float*			GetFallbackWear();
	int*			GetFallbackSeed();
	int*			GetFallbackStatTrak();
	CBaseViewModel*	GetViewModel();
	//[/swap_lines]
};
#include "IHandleEntity.h"
class CBaseEntity : public IClientEntity, public IHandleEntity
{
	public:
		sol::object get_prop(sol::this_state s, const char* prop) {
			auto offs = NetVarManager->getOffs(this->GetClientClass()->m_pNetworkName, prop);
			int* v1 = (int*)(uintptr_t(this) + offs);
			if (v1)
				return sol::make_object(s, *v1);

			bool* v2 = (bool*)(uintptr_t(this) + offs);
			if (v2)
				return sol::make_object(s, *v2);

			float* v3 = (float*)(uintptr_t(this) + offs);
			if (v3)
				return sol::make_object(s, *v3);

			Vector* v4 = (Vector*)(uintptr_t(this) + offs);
			if (v4)
				return sol::make_object(s, *v4);

			return sol::nil;
		}

		void set_prop_int(const char* prop, int i) {
			auto offs = NetVarManager->getOffs(this->GetClientClass()->m_pNetworkName, prop);
			int* v1 = (int*)(uintptr_t(this) + offs);
			if (v1)
				*v1 = i;
		}

		void set_prop_bool(const char* prop, bool i) {
			auto offs = NetVarManager->getOffs(this->GetClientClass()->m_pNetworkName, prop);
			bool* v1 = (bool*)(uintptr_t(this) + offs);
			if (v1)
				*v1 = i;
		}

		void set_prop_float(const char* prop, float i) {
			auto offs = NetVarManager->getOffs(this->GetClientClass()->m_pNetworkName, prop);
			float* v1 = (float*)(uintptr_t(this) + offs);
			if (v1)
				*v1 = i;
		}

		void set_prop_vector(const char* prop, Vector i) {
			auto offs = NetVarManager->getOffs(this->GetClientClass()->m_pNetworkName, prop);
			Vector* v1 = (Vector*)(uintptr_t(this) + offs);
			if (v1)
				*v1 = i;
		}

		void DoExtraBonesProcessingHook(studiohdr_t* a, Vector* b, Quaternion* c, matrix3x4_t& d, uint8_t* e, void* f) {
			const auto state = this->GetAnimState();

			if (!state || !state->m_pBaseEntity) {
				DoExtraBonesProcessing(this, a, b, c, d, e, f);
				return;
			}

			const auto backup_onground = state->m_bOnGround;
			state->m_bOnGround = false;
			DoExtraBonesProcessing(this, a, b, c, d, e, f);
			state->m_bOnGround = backup_onground;
		}

		__forceinline void __vectorcall setup_bone_matrix(CBaseEntity *entity, matrix3x4_t *mat) {
			// must be set to retrive right matrix
			*entity->clientsideanim() = false;
			*reinterpret_cast<int*>(uintptr_t(entity) + 0x104) |= 0xF0;

			*reinterpret_cast<int*>(entity->GetAnimState() + 112) = *reinterpret_cast<int*>(uintptr_t(entity) + 0x104);

			// now we setup the matrix
			entity->SetupBones(mat, 126 /*yes.. you dont want these 2 index's. they are fucked up */, 
				BONE_USED_BY_HITBOX | 0x200, Interfaces::Globals->curtime + 3.5f 
			/* this is needed to be on the right time with the server time value of interpolation of player models on client*/);

			// strip flags
			*reinterpret_cast<int*>(uintptr_t(entity) + 0x104) &= ~(0xF0);

			// set new flags
			*reinterpret_cast<int*>(uintptr_t(entity) + 0x104) |= 0x98;

			// clear our bone cache so we dont look retarded when dead, should be easy to find. index is getabsangles + 0x2
			/*entity->ClearCSRagdollBoneCache();*/

			// correct our bone matrix ( values are from server loggings, can share method in private ). legs are now matching
			mat[3].Base()[2] *= 0.6431f;
			mat[1].Base()[1] /= 0.2117f;
			*entity->clientsideanim() = true;
		}

		void SetBoneMatrix(matrix3x4_t* mt) {
			memcpy((matrix3x4_t*)(uintptr_t(this) + 0x26A8), mt, sizeof(matrix3x4_t) * 126);
		}

		unsigned long *CBaseEntity::ulpMostRecentModelBoneCounter(void)
		{
			return (unsigned long*)((uintptr_t)this + 0x2918);
		}

		matrix3x4_t **CBaseEntity::ppCachedBoneData(void)
		{
			return (matrix3x4_t**)((uintptr_t)this + 0x290C);
		}

		int GetHealth()
		{
			if ( this != nullptr )
				return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_ihealth );

			return 0;
		}

		int GetShotsFired()
		{
			if ( this )
				return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_iShotsFired );

			return 0;
		}
		float_t m_flSpawnTime()
		{
			return *(float_t*)((uintptr_t)this + 0xA350);
		}

		float* flDuckSpeed()
		{
			return ( float* )( ( DWORD )this + offys.m_fduckspeed );
		}
		float* flDuckAmount()
		{
			return ( float* )( ( DWORD )this + offys.m_fduckamount );
		}
		Collideable* GetCollideable1()
		{
			if ( this )
				return ( Collideable* )( ( DWORD )this + 0x31C );

			return nullptr;
		}

		CBaseViewModel* GetViewModel()
		{
			// DT_BasePlayer -> m_hViewModel
			return (CBaseViewModel*)Interfaces::EntityList->GetClientEntityFromHandle((PVOID)*(PDWORD)((DWORD)this + NetVarManager->getOffs("CBasePlayer", "m_hViewModel")));
		}

		Vector WorldSpaceCenter()
		{
			Vector vecOrigin = GetOrigin();
			Vector min = this->GetCollideable1()->VecMins() + vecOrigin;
			Vector max = this->GetCollideable1()->VecMaxs() + vecOrigin;
			Vector size = max - min;
			size /= 2.f;
			size += min;
			return size;
		}

		bool HasHeavyArmor()
		{
			if ( !this )
				return false;

			return *( bool* )( ( DWORD )this + offys.m_bHasHeavyArmor );
		}

		UINT* GetWeapons()
		{
			// DT_BasePlayer -> m_hMyWeapons
			return (UINT*)((DWORD)this + NetVarManager->getOffs("CBasePlayer", "m_hMyWeapons"));
		}

		UINT* GetWearables()
		{
			return (UINT*)((DWORD)this + NetVarManager->getOffs("CBasePlayer", "m_hMyWearables"));
		}

		int GetFlags()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + 0x104);
		}

		int* GetFlagsptr()
		{
			if (!this)
				return 0;

			return reinterpret_cast<int*>(uintptr_t(this) + 0x104);
		}
		CUserCmd*& m_pCurrentCommand() {
			static auto current_command = *(uint32_t*)(Cheat::Utilities->Memory_PatternScan(xorstr_("client_panorama.dll"), xorstr_("89 BE ? ? ? ? E8 ? ? ? ? 85 FF")) + 2);

			return *(CUserCmd **)((uintptr_t)this + current_command);
		}
		bool IsPlayer()
		{
			if ( this )
				return vfunc<bool( __thiscall* )( void* )>( this, 153 )( this );

			return false;
		}

		void SetAngle2(Vector wantedang)
		{
			typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
			static SetAngleFn SetAngle = (SetAngleFn)((DWORD)Cheat::Utilities->Memory_PatternScan(xorstr_("client_panorama.dll"), xorstr_("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1")));
			SetAngle(this, wantedang);
		}

		int GetPing()
		{
			if ( !this )
				return 0;

			return *( int* )( ( DWORD )offys.dwPlayerResource + offys.m_iping + this->GetIndex() * 4 );
		}

		bool IsReloadingEnt()
		{
			if (this) {
				for(int i = 0; i < 15; i++) {
					auto animlayer = this->GetAnimOverlay(i);
					auto sequence = this->GetSequenceActivity(animlayer.m_nSequence);
					if (sequence == 967 && animlayer.m_flWeight != 0.0f)
						return true;
				}
			}
			return false;
		}

		void SetFlags( int flags )
		{
			if ( !this )
				return;

			*reinterpret_cast<int*>( uintptr_t( this ) + 0x100 ) = flags;
		}

		float GetMaxDesyncDelta() {
			auto animstate = this->GetAnimState();

			float duckammount = animstate->m_fDuckAmount;
			float speedfraction = max(0, min(animstate->m_flFeetSpeedForwardsOrSideWays, 1));

			float speedfactor = max(0, min(1, animstate->m_flFeetSpeedUnknownForwardOrSideways));

			float unk1 = ((animstate->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * speedfraction;
			float unk2 = unk1 + 1.f;
			float unk3;

			if (duckammount > 0)
				unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

			return (animstate->get_offset<float>(0x334) * unk2) - 2.f;
		}

		int GetTeam()
		{
			if ( this != nullptr )
				return *reinterpret_cast<int*>( uintptr_t( this ) + 0xF4 );

			return 0;
		}
		bool CBaseEntity::IsVisible( CBaseEntity* pLocalEntity )
		{
			if ( !G::LocalPlayer || !this || !G::LocalPlayer->IsAlive() || G::LocalPlayer->GetHealth() <= 0 )
				return false;

#define	  PlayerVisibleMask				(CONTENTS_HITBOX | CONTENTS_MONSTER | CONTENTS_DEBRIS | CONTENTS_MOVEABLE | CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_GRATE)
			Vector vSrcOrigin = pLocalEntity->GetEyePosition();

			if ( vSrcOrigin.IsZero() )
				return false;

			BYTE bHitBoxCheckVisible[6] =
			{
				HITBOX_HEAD,
				HITBOX_BODY,
				HITBOX_RIGHT_FOOT,
				HITBOX_LEFT_FOOT,
				HITBOX_RIGHT_HAND,
				HITBOX_LEFT_HAND,
			};
			CTraceFilter filter;
			filter.pSkip1 = pLocalEntity;

			for ( int nHit = 0; nHit < 6; nHit++ )
			{
				Vector vHitBox = GetHitboxPosition( bHitBoxCheckVisible[nHit] );

				if ( vHitBox.IsZero() )
					continue;

				trace_t tr;
				Ray_t ray;
				ray.Init( vSrcOrigin, vHitBox );
				Interfaces::Trace->TraceRay( ray, PlayerVisibleMask, &filter, &tr );

				if ( tr.m_pEnt == this && !tr.allsolid )
					return true;
			}

			return false;
		}
		int GetObserverMode()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_observermode );
		}

		void SetObserverMode( int mode )
		{
			if ( !this )
				return;

			*reinterpret_cast<int*>( uintptr_t( this ) + offys.m_observermode ) = mode;
		}

		CBaseEntity* GetObserverTarget()
		{
			if ( !this )
				return nullptr;

			ULONG ent = *( PULONG )( ( DWORD )this + offys.m_hObserverTarget );
			return ( CBaseEntity* )( Interfaces::EntityList->GetClientEntityFromHandle2( ent ) );
		}

		bool IsScoped()
		{
			if ( !this )
				return false;

			return *reinterpret_cast<bool*>( uintptr_t( this ) + offys.m_isscoped );
		}

		Vector GetVelocity()
		{
			if ( !this )
				return Vector();

			return *reinterpret_cast<Vector*>( uintptr_t( this ) + offys.m_vecvelocity );
		}
		Vector& get_ragdoll_pos()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + offys.m_ragdollpos);
		}
		void SetVelocity( Vector velocity )
		{
			if ( !this )
				return;

			*reinterpret_cast<Vector*>( uintptr_t( this ) + offys.m_vecvelocity ) = velocity;
		}

		int GetMoney()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_account );
		}

		int GetLifeState()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_lifeState );
		}
		static void UpdateAnimationState(CBaseAnimState *state, Vector angle)
		{
			if (!state)
				return;

			static auto UpdateAnimState = Cheat::Utilities->Memory_PatternScan(xorstr_("client_panorama.dll"), xorstr_("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));
			if (!UpdateAnimState)
				return;

			__asm
			{
				mov ecx, state

				movss xmm1, dword ptr[angle + 4]
				movss xmm2, dword ptr[angle]

				call UpdateAnimState
			}
		}

		static void ResetAnimationState(CBaseAnimState *state)
		{
			if (!state)
				return;

			using ResetAnimState_t = void(__thiscall*)(CBaseAnimState*);
			static auto ResetAnimState = (ResetAnimState_t)Cheat::Utilities->Memory_PatternScan(xorstr_("client_panorama.dll"), xorstr_("56 6A 01 68 ? ? ? ? 8B F1"));
			if (!ResetAnimState)
				return;

			ResetAnimState(state);
		}

		void CreateAnimationState(CBaseAnimState *state)
		{
			using CreateAnimState_t = void(__thiscall*)(CBaseAnimState*, CBaseEntity*);
			static auto CreateAnimState = (CreateAnimState_t)Cheat::Utilities->Memory_PatternScan(xorstr_("client_panorama.dll"), xorstr_("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
			if (!CreateAnimState)
				return;

			CreateAnimState(state, this);
		}
		float GetLowerBodyYaw()
		{
			if ( !this )
				return 0.f;

			return *reinterpret_cast<float*>( uintptr_t( this ) + offys.m_lowerbodyyawtarget );
		}

		void SetLowerBodyYaw(float yaw) {
			if (!this)
				return;

			*reinterpret_cast<float*>(uintptr_t(this) + offys.m_lowerbodyyawtarget) = yaw;
		}

		Vector& GetOrigin()
		{
			if ( this )
				return *reinterpret_cast<Vector*>( uintptr_t( this ) + offys.m_vecorigin );

			return Vector( 0, 0, 0 );
		}

		bool IsAlive()
		{
			if ( this != nullptr )
				return this->GetLifeState() == 0;

			return false;
		}

		bool* clientsideanim()
		{
			return ( bool* )( ( DWORD )this + offys.m_bclientsideanim );
		}

		bool valid( bool check_team = true, bool check_dormant = true )
		{
			if ( !this )
				return false;

			if ( IsDormant() && check_dormant )
				return false;

			if ( !IsAlive() || GetHealth() <= 0 )
				return false;

			if ( !G::LocalPlayer )
				return false;

			if ( check_team )
			{
				if ( G::LocalPlayer->GetTeam() == GetTeam() )
					return false;
			}

			return true;
		}
		bool IsWeapon()
		{
			if ( !this )
				return false;

			return vfunc<bool( __thiscall* )( CBaseEntity* )>( this, 161 )( this );
		}
		int HitBoxSet()
		{
			return *( int* )( DWORD( this ) + offys.m_hitboxset );
		}
		Vector &GetAbsOrigin()
		{
			if ( !this )
				return Vector( 0, 0, 0 );

			typedef Vector& ( __thiscall * OriginalFn )( void* );
			return ( ( OriginalFn )vfunc<OriginalFn>( this, 10 ) )( this );
		}

		int GetTickBase()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_tickbase );
		}
		int* GetTickBaseptr()
		{
			if (!this)
				return 0;

			return reinterpret_cast<int*>(uintptr_t(this) + offys.m_tickbase);
		}
		bool body_visible(CBaseEntity* other) {
			return
				point_visible(other->hitbox_position(HITBOX_CHEST)) ||
				point_visible(other->hitbox_position(HITBOX_THORAX)) ||
				point_visible(other->hitbox_position(HITBOX_BODY)) ||
				point_visible(other->hitbox_position(HITBOX_LEFT_FOOT)) ||
				point_visible(other->hitbox_position(HITBOX_RIGHT_FOOT));
		}
		bool point_visible( Vector endpos )
		{
			trace_t tr;
			Ray_t ray;
			CTraceFilter filter;
			filter.pSkip1 = this;
			ray.Init( GetEyePosition(), endpos );
			Interfaces::Trace->TraceRay( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );
			return tr.fraction > 0.97f;
		}
		Vector hitbox_position( int hitbox_id, matrix3x4_t matrix[128] )
		{
			studiohdr_t* hdr = Interfaces::ModelInfo->GetStudioModel( this->GetModel() );

			if ( hdr )
			{
				mstudiobbox_t* hitbox = hdr->pHitboxSet( this->HitBoxSet() )->GetHitbox( hitbox_id );

				if ( hitbox )
				{
					Vector
					min = Vector{ },
					max = Vector{ };
					Cheat::Utilities->Math_VectorTransform( hitbox->bbmin, matrix[hitbox->bone], min );
					Cheat::Utilities->Math_VectorTransform( hitbox->bbmax, matrix[hitbox->bone], max );
					return ( min + max ) / 2.0f;
				}
			}

			return Vector{ };
		}
		Vector hitbox_position( int hitbox_id )
		{
			matrix3x4_t matrix[128];

			if ( this->SetupBones( matrix, 128, 0x100, 0.f ) )
			{
				studiohdr_t* hdr = Interfaces::ModelInfo->GetStudioModel( this->GetModel() );

				if ( hdr )
				{
					mstudiobbox_t* hitbox = hdr->pHitboxSet( this->HitBoxSet() )->GetHitbox( hitbox_id );

					if ( hitbox )
					{
						Vector
						min = Vector{ },
						max = Vector{ };
						Cheat::Utilities->Math_VectorTransform( hitbox->bbmin, matrix[hitbox->bone], min );
						Cheat::Utilities->Math_VectorTransform( hitbox->bbmax, matrix[hitbox->bone], max );
						return ( min + max ) / 2.0f;
					}
				}
			}

			return Vector{ };
		}
		bool IsDormant()
		{
			if ( !this )
				return true;

			return *reinterpret_cast<bool*>( uintptr_t( this ) + 0xE9 );
		}

		Collideable* GetCollision()
		{
			if ( this )
				return reinterpret_cast<Collideable*>( ( DWORD )this + 0x31C );
		}

		VMatrix &GetCollisionBoundTrans()
		{
			if ( this )
				return *reinterpret_cast<VMatrix*>( ( DWORD )this + 0x0444);
		}

		CBaseAnimState* GetAnimState()
		{
			if ( this )
				return *reinterpret_cast<CBaseAnimState**>( reinterpret_cast<void*>( uintptr_t( this ) + 0x3900 ) );
		}
		void set_animation_state( CBaseAnimState* state )
		{
			*reinterpret_cast<CBaseAnimState**>( uintptr_t( this ) + 0x3900) = state;
		}

		void GetRenderBounds( Vector &mins, Vector &maxs )
		{
			if ( this )
			{
				void* pRenderable = ( void* )( this + 0x4 );
				typedef void( __thiscall * Fn )( void*, Vector &, Vector & );
				vfunc<Fn>( pRenderable, 17 )( pRenderable, mins, maxs );
			}
		}

		void DoExtraBonesProcessing(CBaseEntity* a, studiohdr_t* b, Vector* c, Quaternion* d, matrix3x4_t& e, uint8_t* f, void* g) {
			typedef void(__thiscall* fnDoExtraBonesProcessing)(CBaseEntity*, studiohdr_t*, Vector*, Quaternion*, matrix3x4_t&, uint8_t*, void*);
			vfunc<fnDoExtraBonesProcessing>(this, 192)(a, b, c, d, e, f, g);
		}

		std::tuple<Vector, Vector> get_render_bounds() {
			Vector mins, maxs;
			GetRenderBounds(mins, maxs);
			return std::make_tuple(mins, maxs);
		}

		int GetIndex()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + 0x64 );
		}

		int GetMoveType()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + 0x25C );
		}

		CAnimationLayer &GetAnimOverlay( int Index )
		{
			if ( this )
				return ( *( CAnimationLayer** )( ( DWORD )this + 0x2980) )[Index];
		}
		CAnimationLayer *GetAnimOverlays()
		{
			return *(CAnimationLayer**)((DWORD)this + 0x2980);
		}
		int GetNumAnimOverlays()
		{
			return *(int*)((DWORD)this + 0x298C);
		}
		void SetAnimOverlay( int Index, CAnimationLayer layer )
		{
			if ( this )
				( *( CAnimationLayer** )( ( DWORD )this + 0x2980) )[Index] = layer;
		}

		int GetSequenceActivity( int sequence )
		{
			const auto model = GetModel();

			if ( !model )
				return -1;

			const auto hdr = Interfaces::ModelInfo->GetStudioModel( model );

			if ( !hdr )
				return -1;

			static auto GetSequenceActivity = reinterpret_cast<int( __fastcall* )( void*, studiohdr_t*, int )>( offys.dwGetSequenceActivity );

			if ( this )
				return GetSequenceActivity( this, hdr, sequence );
		}

		Vector GetEyeAngles()
		{
			if ( !this )
				return Vector();

			return *reinterpret_cast<Vector*>( uintptr_t( this ) + offys.m_angeyeangles );
		}

		Vector* GetEyeAnglesPtr()
		{
			if ( !this )
				return new Vector();

			return ( Vector* )( ( uintptr_t )this + offys.m_angeyeangles );
		}

		void SetEyeAngles( Vector angles )
		{
			if ( !this )
				return;

			*reinterpret_cast<Vector*>( uintptr_t( this ) + offys.m_angeyeangles ) = angles;
		}

		float& GetSimTime()
		{
			float v = 0.f;
			if ( !this )
				return v;

			return *reinterpret_cast<float*>( uintptr_t( this ) + offys.m_simtime );
		}

		float GetOldSimTime()
		{
			if (!this)
				return 0.f;

			return *reinterpret_cast<float*>(uintptr_t(this) + offys.m_simtime + 0x4);
		}
		float* GetSimTime2()
		{
			if (!this)
				return nullptr;

			return reinterpret_cast<float*>(uintptr_t(this) + offys.m_simtime);
		}
		Vector GetViewOffset()
		{
			if ( !this )
				return Vector();

			return *reinterpret_cast<Vector*>( uintptr_t( this ) + offys.m_vecviewoff );
		}

		matrix3x4_t GetBoneMatrix( int BoneID )
		{
			matrix3x4_t mtrx[128];
			if(this->SetupBones(mtrx,128, BONE_USED_BY_HITBOX,0))
				return mtrx[BoneID];
			return matrix3x4_t();
		}

		Vector GetBonePosition( int i )
		{
			matrix3x4_t matrix[128];

			if ( this->SetupBones( matrix, 128, BONE_USED_BY_HITBOX, GetTickCount64() ) && this )
				return Vector( matrix[i][0][3], matrix[i][1][3], matrix[i][2][3] );

			return Vector( 0, 0, 0 );
		}

		mstudiobbox_t* GetHitBox( int nHitbox )
		{
			if ( nHitbox < 0 || nHitbox >= HITBOX_MAX )
				return nullptr;

			mstudiohitboxset_t* pHitboxSet = nullptr;
			mstudiobbox_t* pHitboxBox = nullptr;
			pHitboxSet = GetHitBoxSet();

			if ( !pHitboxSet )
				return pHitboxBox;

			pHitboxBox = pHitboxSet->GetHitbox( nHitbox );

			if ( !pHitboxBox )
				return nullptr;

			return pHitboxBox;
		}

		studiohdr_t* GetStudioModel()
		{
			const model_t* model = nullptr;
			model = GetModel();

			if ( !model )
				return nullptr;

			studiohdr_t* pStudioModel = Interfaces::ModelInfo->GetStudioModel( model );

			if ( !pStudioModel )
				return nullptr;

			return pStudioModel;
		}

		mstudiohitboxset_t* GetHitBoxSet()
		{
			studiohdr_t* pStudioModel = nullptr;
			mstudiohitboxset_t* pHitboxSet = nullptr;
			pStudioModel = GetStudioModel();

			if ( !pStudioModel )
				return pHitboxSet;

			pHitboxSet = pStudioModel->pHitboxSet( 0 );

			if ( !pHitboxSet )
				return nullptr;

			return pHitboxSet;
		}

		Vector GetHitboxPosition(int nHitbox, matrix3x4_t bones[126]) {
			Vector vRet, vMin, vMax;
			vRet = Vector(0, 0, 0);
			mstudiobbox_t* pHitboxBox = GetHitBox(nHitbox);

			if (!pHitboxBox || IsDormant() || !IsAlive())
				return vRet;

			if (!pHitboxBox->bone)
				return vRet;

			Utilities->Math_VectorTransform(pHitboxBox->bbmin, bones[pHitboxBox->bone], vMin);
			Utilities->Math_VectorTransform(pHitboxBox->bbmax, bones[pHitboxBox->bone], vMax);
			vRet = (vMin + vMax) * 0.5f;
			return vRet;
		}

		Vector GetHitboxPosition( int nHitbox )
		{
			matrix3x4_t MatrixArray[128];
			Vector vRet, vMin, vMax;
			vRet = Vector( 0, 0, 0 );
			mstudiobbox_t* pHitboxBox = GetHitBox( nHitbox );

			if ( !pHitboxBox || IsDormant() || !IsAlive() )
				return vRet;

			if ( !SetupBones( MatrixArray, 128, BONE_USED_BY_HITBOX, 0/*Interfaces::GlobalVars()->curtime*/ ) )
				return vRet;

			if ( !pHitboxBox->bone )
				return vRet;

			Utilities->Math_VectorTransform( pHitboxBox->bbmin, MatrixArray[pHitboxBox->bone], vMin );
			Utilities->Math_VectorTransform( pHitboxBox->bbmax, MatrixArray[pHitboxBox->bone], vMax );
			vRet = ( vMin + vMax ) * 0.5f;
			return vRet;
		}

		Vector GetEyePosition()
		{
			return GetOrigin() + GetViewOffset();
		}

		Vector GetPunchAngles()
		{
			if ( !this )
				return Vector();

			return *reinterpret_cast<Vector*>( uintptr_t( this ) + offys.m_aimpunchangle );
		}

		Vector* GetViewPunchPtr()
		{
			if ( this )
				return reinterpret_cast<Vector*>( ( DWORD )this + offys.m_viewPunchAngle);
		}

		float* FlashAlpha()
		{
			return ( float* )( ( DWORD )this + offys.m_flashalpha );
		}
		Vector* GetPunchAnglePtr()
		{
			if ( this )
				return reinterpret_cast<Vector*>( ( DWORD )this + offys.m_aimpunchangle);
		}

		Vector GetViewPunchAngles()
		{
			if ( !this )
				return Vector();

			return *reinterpret_cast<Vector*>( ( DWORD )this + offys.m_viewPunchAngle );
		}

		bool GetImmunity()
		{
			if ( !this )
				return true;

			return *reinterpret_cast<bool*>( uintptr_t( this ) + 0x3928);
		}



		void UpdateClientSideAnimation()
		{
			typedef void( __thiscall * original )( void* );

			if ( this )
				vfunc<original>( this, 219 )( this );
		}

		float GetSpread()
		{
			typedef float( __thiscall * original )( void* );

			if ( this )
				return vfunc<original>( this, 437 )( this );
		}

		float GetInaccuracy()
		{
			typedef float( __thiscall * oInaccuracy )( PVOID );

			if ( this )
				return vfunc<oInaccuracy>( this, 468 )( this );
		}

		void SetAbsOrigin( Vector ArgOrigin )
		{
			using Fn = void( __thiscall* )( CBaseEntity*, const Vector & origin );
			static Fn func;

			if ( !func )
				func = ( Fn )( offys.dwSetAbsOrigin );

			if ( this )
				func( this, ArgOrigin );
		}

		Vector &GetAbsAngles()
		{
			typedef Vector& ( __thiscall * OriginalFn )( void* );

			if ( this )
				return ( ( OriginalFn )vfunc<OriginalFn>( this, 11 ) )( this );
		}

		void InvalidateBoneCache()
		{
			typedef void (__thiscall * OriginalFn)(void*);
			static auto func = (OriginalFn)Utilities->Memory_PatternScan("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

			if (this && func)
				func(this);
		}

		void SetAbsAngles( Vector angles )
		{
			using Fn = void( __thiscall* )( CBaseEntity*, const Vector & angles );
			static Fn func;

			if ( !func )
				func = ( Fn )( offys.dwSetAbsAngles );

			if ( this )
				func( this, angles );
		}

		float* GetPoseParamaters()
		{
			if ( !this )
				return new float[256];

			return reinterpret_cast<float*>( uintptr_t( this ) + offys.m_poseparam );
		}

		std::array<float, 24> &m_flPoseParameter()
		{
			static int _m_flPoseParameter = nv(xorstr_("CBaseAnimating"), xorstr_("m_flPoseParameter"));
			return *(std::array<float, 24>*)((uintptr_t)this + _m_flPoseParameter);
		}

		Vector GetRenderAngles1()
		{
			if ( this )
				return *( Vector* )( ( DWORD )this + 0x128 );
		}

		void SetRenderAngles( Vector angles )
		{
			if ( this )
				*( Vector* )( ( DWORD )this + 0x128 ) = angles;
		}




		float GetNextAttack()
		{
			if ( !this )
				return 0.f;

			return *reinterpret_cast<float*>( uint32_t( this ) + offys.m_flnextattack );
		}

		CBaseWeapon* GetActiveWeapon()
		{
			if ( this != nullptr )
			{
				ULONG WeaponUlong = *( PULONG )( ( DWORD )this + offys.m_hactiveweapon );
				return ( CBaseWeapon* )( Interfaces::EntityList->GetClientEntityFromHandle2( WeaponUlong ) );
			}

			return nullptr;
		}

		int GetActiveWeaponIndex()
		{
			if ( !this )
				return -1;

			return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_hactiveweapon ) & 0xFFF;
		}

		int GetArmor()
		{
			if ( !this )
				return 0;

			return *reinterpret_cast<int*>( uintptr_t( this ) + offys.m_iarmor );
		}

		bool HasHelmet()
		{
			if ( !this )
				return false;

			return *reinterpret_cast<bool*>( uintptr_t( this ) + offys.m_bhashelmet );
		}

		char* GetArmorName()
		{
			if ( this )
			{
				if ( GetArmor() > 0 )
				{
					if ( HasHelmet() )
						return ( char* )("HK");

					else
						return ( char* )("K");
				}

				else
					return ( char* )(" ");
			}
		}

		bool HasC4()
		{
			if ( this )
			{
				int iBombIndex = *( int* )( *( DWORD* )( 0x2ED2ABC ) + 0x165c );

				if ( iBombIndex == this->GetIndex() )
					return true;

				else
					return false;
			}
		}
};