#include "Engine.h"

#include "../Configuration/Settings.h"
#include "../Utility/Utilities.h"
#include "../Features/EventListener.h"
#include "../Interfaces/NetVar.h"
#include "../Hashing/xorstr.h"
#include "../Hooking/Functions/EndScene.hpp"
#include "../Hooking/Functions/Reset.hpp"
#include "../Hooking/Functions/WndProc.hpp"
#include "../Hooking/Functions/LockCursor.hpp"
#include "../Hooking/Functions/CreateMove.hpp"
#include "../Hooking/Functions/FrameStageNotify.hpp"
#include "../Hooking/Functions/PaintTreverse.hpp"
#include "../Hooking/Functions/DrawModelExecute.hpp"
#include "../Hooking/Functions/DoPostScreenSpaceEffects.hpp"
#include "../Hooking/Functions/SceneEnd.hpp"
#include "../Hooking/Functions/OverrideView.hpp"
#include "../Hooking/Functions/EmitSound.hpp"
#include "../Hooking/Functions/SendDataGram.hpp"
#include "../Hooking/Functions/Sequence.h"

using namespace Cheat;
using namespace Cheat::Hooked;

void SetupOffsets()
{
	Cheat::Utilities->Console_Log( xorstr_("Initializing net vars") );
	Cheat::NetVarManager = new Cheat::CNetVarManager();
	Cheat::NetVarManager->init();
	offys.m_account = nv(xorstr_("CCSPlayer"), xorstr_("m_iAccount") );
	offys.m_angeyeangles = nv(xorstr_("CCSPlayer"), xorstr_("m_angEyeAngles[0]") );
	offys.m_bhashelmet = nv(xorstr_("CCSPlayer"), xorstr_("m_bHasHelmet") );
	offys.m_flnextattack = nv(xorstr_("CBaseCombatCharacter"), xorstr_("m_flNextAttack") );
	offys.m_flnextprimaryattack = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_flNextPrimaryAttack") );
	offys.m_flpostponefirereadytime = nv(xorstr_("CWeaponCSBase"), xorstr_("m_flPostponeFireReadyTime") );
	offys.m_gungameimmunity = nv(xorstr_("CCSPlayer"), xorstr_("m_bGunGameImmunity") );
	offys.m_hactiveweapon = nv(xorstr_("CBaseCombatCharacter"), xorstr_("m_hActiveWeapon") );
	offys.m_iarmor = nv(xorstr_("CCSPlayer"), xorstr_("m_ArmorValue") );
	offys.m_iclip1 = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_iClip1") );
	offys.m_ihealth = nv(xorstr_("CBasePlayer"), xorstr_("m_iHealth") );
	offys.m_iitemdefinitionindex = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_iItemDefinitionIndex") );
	offys.m_isscoped = nv(xorstr_("CCSPlayer"), xorstr_("m_bIsScoped") );
	offys.m_lifeState = nv(xorstr_("CBasePlayer"), xorstr_("m_lifeState") );
	offys.m_hitboxset = nv(xorstr_("CBasePlayer"), xorstr_("m_nHitboxSet") );
	offys.m_lowerbodyyawtarget = nv(xorstr_("CCSPlayer"), xorstr_("m_flLowerBodyYawTarget") );
	offys.m_observermode = nv(xorstr_("CBasePlayer"), xorstr_("m_iObserverMode") );
	offys.m_poseparam = nv(xorstr_("CBaseAnimating"), xorstr_("m_flPoseParameter") );
	offys.m_simtime = nv(xorstr_("CBaseEntity"), xorstr_("m_flSimulationTime"));
	offys.m_tickbase = nv(xorstr_("CBasePlayer"), xorstr_("m_nTickBase") );
	offys.m_vecorigin = nv(xorstr_("CBaseEntity"), xorstr_("m_vecOrigin") );
	offys.m_vecvelocity = nv( xorstr_("CBasePlayer"), xorstr_("m_vecVelocity[0]") );
	offys.m_ragdollpos = nv(xorstr_("DT_Ragdoll"), xorstr_("m_ragPos"));
	offys.m_vecviewoff = nv( xorstr_("CBasePlayer"), xorstr_("m_vecViewOffset[0]") );
	offys.m_howner = nv( xorstr_("CBaseCombatWeapon"), xorstr_("m_hOwner") );
	offys.m_iping = nv( xorstr_("CCSPlayerResource"), xorstr_("m_iPing") );
	offys.m_bclientsideanim = nv( xorstr_("CBaseAnimating"), xorstr_("m_bClientSideAnimation") );
	offys.m_hObserverTarget = nv( xorstr_("CCSPlayer"), xorstr_("m_hObserverTarget") );
	offys.m_viewPunchAngle = nv( xorstr_("CBasePlayer"), xorstr_("m_viewPunchAngle") );
	offys.m_flashalpha = nv( xorstr_("CCSPlayer"), xorstr_("m_flFlashDuration") );
	offys.m_fFlags = nv( xorstr_("DT_BasePlayer"), xorstr_("m_fFlags") );
	offys.m_aimpunchangle = nv( xorstr_("CBasePlayer"), xorstr_("m_aimPunchAngle") );
	offys.m_fduckamount = nv( xorstr_("CCSPlayer"), xorstr_("m_flDuckAmount") );
	offys.m_fduckspeed = nv( xorstr_("CCSPlayer"), xorstr_("m_flDuckSpeed") );
	offys.m_bBombTicking = nv( xorstr_("CPlantedC4"), xorstr_("m_bBombTicking") );
	offys.m_nBombSite = nv( xorstr_("CPlantedC4"), xorstr_("m_nBombSite") );
	offys.m_flC4Blow = nv( xorstr_("CPlantedC4"), xorstr_("m_flC4Blow") );
	offys.m_flTimerLength = nv( xorstr_("CPlantedC4"), xorstr_("m_flTimerLength") );
	offys.m_flDefuseLength = nv( xorstr_("CPlantedC4"), xorstr_("m_flDefuseLength") );
	offys.m_flDefuseCountDown = nv( xorstr_("CPlantedC4"), xorstr_("m_flDefuseCountDown") );
	offys.m_bBombDefused = nv( xorstr_("CPlantedC4"), xorstr_("m_bBombDefused") );
	offys.m_hBombDefuser = nv( xorstr_("CPlantedC4"), xorstr_("m_hBombDefuser") );
	offys.m_iShotsFired = nv( xorstr_("CCSPlayer"), xorstr_("m_iShotsFired") );
	offys.m_izoomLevel = nv( xorstr_("DT_WeaponAWP"), xorstr_("m_zoomLevel") );
	offys.m_bHasHeavyArmor = nv( xorstr_("DT_CSPlayer"), xorstr_("m_bHasHeavyArmor") );
	offys.m_bPinPulled = nv( xorstr_("DT_BaseCSGrenade"), xorstr_("m_bPinPulled") );
	offys.m_fThrowTime = nv( xorstr_("DT_BaseCSGrenade"), xorstr_("m_fThrowTime") );
	offys.dwReloading = Cheat::Utilities->Memory_PatternScan(Cheat::Settings->ClientDLL, xorstr_("C6 87 ? ? ? ? ? 8B 06 8B CE FF 90")) + 2;
	offys.m_flNextSecondaryAttack = nv( xorstr_("DT_LocalActiveWeaponData"), xorstr_("m_flNextSecondaryAttack") );
	offys.dwPlayerResource = *( DWORD* )( Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") ) + 2 );
	offys.dwGetSequenceActivity = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 53 8B 5D 08 56 8B F1 83") );
	offys.dwSetAbsOrigin = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 83 E4 F8 51 53 56 57 8B F1") );
	offys.dwSetAbsAngles = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8") );
	offys.dwLoadFromBuffer = Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, ( xorstr_("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04") ) );
	offys.dwInitKeyValues = Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, ( xorstr_("8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03 C1 F8 08 66 89 46 12 8B C6") ) ) - 0x45;
	offys.dwSmokeCount = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("A3 ? ? ? ? 57 8B CB") ) + 1;
	offys.dwLoadSky = Utilities->Memory_PatternScan( xorstr_("engine.dll"), xorstr_("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45") );
	offys.dwCrosshair = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("83 F8 05 75 17") );
	offys.dwSetClanTag = Cheat::Utilities->Memory_PatternScan( xorstr_("engine.dll"), xorstr_("53 56 57 8B DA 8B F9 FF 15") );
	offys.dwServerRankRevealAll = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 8B 0D ? ? ? ? 85 C9 75 28 A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 04 85 C0 74 0B 8B C8 E8 ? ? ? ? 8B C8 EB 02 33 C9 89 0D ? ? ? ? 8B 45 08") );
	
	Cheat::Utilities->Console_Log( xorstr_("Initialization finished") );
}

bool Cheat::Initialize()
{
	if ( Settings->OnlineMode )
		; // TODO: add loader verification

	if ( !Settings->UIDevMode )
	{
		SetupOffsets();
		MaterialHelper = new CMaterialHelper();
		Utilities->Console_Log( xorstr_("Preparing hooking managers (1/1)") );
	}

	std::unique_ptr<Hooking::VTableHook> DirectXManager = std::make_unique<Hooking::VTableHook>();
	DirectXManager->Setup( **reinterpret_cast<void*** >( ( Utilities->Memory_PatternScan( xorstr_("shaderapidx9.dll"), xorstr_("A1 ? ? ? ? 6A 00 53") ) + 1 ) ) );
	std::unique_ptr<Hooking::VTableHook> SurfaceManager = std::make_unique<Hooking::VTableHook>();
	std::unique_ptr<Hooking::VTableHook> PanelManager = std::make_unique<Hooking::VTableHook>();
	std::unique_ptr<Hooking::VTableHook> ClientManager = std::make_unique<Hooking::VTableHook>();
	std::unique_ptr<Hooking::VTableHook> ClientModeManager = std::make_unique<Hooking::VTableHook>();
	std::unique_ptr<Hooking::VTableHook> ModelRenderManager = std::make_unique<Hooking::VTableHook>();
	std::unique_ptr<Hooking::VTableHook> RenderViewManager = std::make_unique<Hooking::VTableHook>();
	std::unique_ptr<Hooking::VTableHook> EngineSoundManager = std::make_unique<Hooking::VTableHook>();

	if ( !Settings->UIDevMode )
	{
		SurfaceManager->Setup( Interfaces::Surface );
		PanelManager->Setup( Interfaces::GuiPanel );
		ClientManager->Setup( Interfaces::Client );
		ClientModeManager->Setup( Interfaces::ClientMode );
		ModelRenderManager->Setup( Interfaces::ModelRender );
		RenderViewManager->Setup( Interfaces::RenderView );
		EngineSoundManager->Setup( Interfaces::EngineSound );
	}

	if ( !Settings->UIDevMode )
	{
		Utilities->Console_Log( xorstr_("Initialize GameEventListeners (1/1)") );
		g_pGameEvents.InitializeEventListeners();
		Utilities->Console_Log( xorstr_("Hooking VPanel (1/1)") );
		PanelManager->Hook( 41, PaintTraverseHook );
		oPaintTraverse = PanelManager->GetOriginal<fnPaintTraverse>( 41 );
		Utilities->Console_Log( xorstr_("Hooking RenderView (1/1)") );
		RenderViewManager->Hook( 9, SceneEndHook );
		oSceneEnd = RenderViewManager->GetOriginal<fnSceneEnd>( 9 );
		Utilities->Console_Log( xorstr_("Hooking ModelRender (1/1)") );
		ModelRenderManager->Hook( 21, DrawModelExecuteHook );
		oDrawModelExecute = ModelRenderManager->GetOriginal<fnDrawModelExecute>( 21 );
		Utilities->Console_Log( xorstr_("Hooking Surface (1/1)") );
		SurfaceManager->Hook( 67, LockCursorHook );
		oLockCursor = SurfaceManager->GetOriginal<fnLockCursor>( 67 );
	}

	Utilities->Console_Log( xorstr_("Hooking DirectX 9 (1/2)") );
	DirectXManager->Hook( 17, PresentHook );
	oPresent = DirectXManager->GetOriginal<fnPresent>( 17 );
	Utilities->Console_Log( xorstr_("Hooking DirectX 9 (2/2)") );
	DirectXManager->Hook( 16, ResetHook );
	oReset = DirectXManager->GetOriginal<fnReset>( 16 );

	if ( !Settings->UIDevMode )
	{
		Utilities->Console_Log( xorstr_("Hooking Client (1/1)") );
		ClientManager->Hook( 37, FrameStageNotifyHook );
		oFrameStageNotify = ClientManager->GetOriginal<fnFrameStageNotify>( 37 );
		Utilities->Console_Log( xorstr_("Hooking ClientMode (1/3)") );
		ClientModeManager->Hook( 24, CreateMoveHook );
		oCreateMove = ClientModeManager->GetOriginal<fnCreateMove>( 24 );
		Utilities->Console_Log( xorstr_("Hooking ClientMode (2/3)") );
		ClientModeManager->Hook( 44, DoPostScreenSpaceEffectsHook );
		oDoPostScreenSpaceEffects = ClientModeManager->GetOriginal<fnDoPostScreenSpaceEffects>( 44 );
		Utilities->Console_Log( xorstr_("Hooking ClientMode (3/3)") );
		ClientModeManager->Hook( 18, OverrideViewHook );
		oOverrideView = ClientModeManager->GetOriginal<fnOverrideView>( 18 );
	}

	/*Utilities->Console_Log(xorstr_("Hooking NetVars (1/1)"));
	NetVarManager->hookProp("CBaseViewModel", "m_nSequence", SequenceHook, oSequenceHook);*/

	Utilities->Console_Log(xorstr_("Hooking EngineSound (1/1)"));
	EngineSoundManager->Hook(5, emit_sound);
	oEmitSound = EngineSoundManager->GetOriginal<fnEmitSound>(5);

	Utilities->Console_Log( xorstr_("Hooking WndProc") );
	HWND hWnd = FindWindow( xorstr_("Valve001"), 0 );
	oWndProc = ( WNDPROC )SetWindowLongPtr( hWnd, GWL_WNDPROC, ( LONG )&WndProcHook );
	Utilities->Console_Log( xorstr_("Saving hooking managers (1/8)") );
	Hooking::Hooks->AddHookMgr( xorstr_("d3dx9"), std::move( DirectXManager ) );

	if ( !Settings->UIDevMode )
	{
		Utilities->Console_Log( xorstr_("Saving hooking managers (2/8)") );
		Hooking::Hooks->AddHookMgr( xorstr_("VGUI_Surface"), std::move( SurfaceManager ) );
		Utilities->Console_Log( xorstr_("Saving hooking managers (3/8)") );
		Hooking::Hooks->AddHookMgr( xorstr_("ClientMode"), std::move( ClientModeManager ) );
		Utilities->Console_Log( xorstr_("Saving hooking managers (4/8)") );
		Hooking::Hooks->AddHookMgr( xorstr_("Client"), std::move( ClientManager ) );
		Utilities->Console_Log( xorstr_("Saving hooking managers (5/8)") );
		Hooking::Hooks->AddHookMgr( xorstr_("VGuiPanel"), std::move( PanelManager ) );
		Utilities->Console_Log( xorstr_("Saving hooking managers (6/8)") );
		Hooking::Hooks->AddHookMgr( xorstr_("ModelRender"), std::move( ModelRenderManager ) );
		Utilities->Console_Log( xorstr_("Saving hooking managers (7/8)") );
		Hooking::Hooks->AddHookMgr( xorstr_("RenderView"), std::move( RenderViewManager ) );
		Utilities->Console_Log( xorstr_("Saving hooking managers (8/8)") );
		Hooking::Hooks->AddHookMgr( xorstr_("EngineSound"), std::move( EngineSoundManager ) );
	}

	return true;
}

void Cheat::Unload()
{
	HWND hWnd = FindWindow( xorstr_("Valve001"), 0 );
	SetWindowLongPtr( hWnd, GWL_WNDPROC, ( LONG )oWndProc );
	Hooking::Hooks->ReleaseHookMgr( xorstr_("d3dx9") );

	if ( !Settings->UIDevMode )
	{
		Hooking::Hooks->ReleaseHookMgr( xorstr_("VGUI_Surface") );
		Hooking::Hooks->ReleaseHookMgr( xorstr_("ClientMode") );
		Hooking::Hooks->ReleaseHookMgr( xorstr_("Client") );
		Hooking::Hooks->ReleaseHookMgr( xorstr_("VGuiPanel") );
		Hooking::Hooks->ReleaseHookMgr( xorstr_("ModelRender") );
		Hooking::Hooks->ReleaseHookMgr( xorstr_("RenderView") );
		Hooking::Hooks->ReleaseHookMgr( xorstr_("EngineSound") );
	}
}
