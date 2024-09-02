#pragma once
//#include "../../Interfaces/Interfaces.h"
//#include "../../SDK/CBaseEntity.h"
//#include "../../Interfaces/NetVar.h"
//#include "../../Features/Skinchanger/items.h"
//#include "../../Security/fnv.hpp"
//
//RecvVarProxyFn oSequenceHook;
//
//static auto random_sequence(const int low, const int high) -> int
//{
//	return rand() % (high - low + 1) + low;
//}
//
//static auto get_new_animation(const fnv::hash model, const int sequence) -> int
//{
//	// Hashes for best performance.
//	switch (model)
//	{
//	case FNV("models/weapons/v_knife_butterfly.mdl"):
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
//		default:
//			return sequence + 1;
//		}
//	}
//	case FNV("models/weapons/v_knife_falchion_advanced.mdl"):
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_IDLE2:
//			return SEQUENCE_FALCHION_IDLE1;
//		case SEQUENCE_DEFAULT_HEAVY_MISS1:
//			return random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
//		case SEQUENCE_DEFAULT_DRAW:
//		case SEQUENCE_DEFAULT_IDLE1:
//			return sequence;
//		default:
//			return sequence - 1;
//		}
//	}
//	case FNV("models/weapons/v_knife_push.mdl"):
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_IDLE2:
//			return SEQUENCE_DAGGERS_IDLE1;
//		case SEQUENCE_DEFAULT_LIGHT_MISS1:
//		case SEQUENCE_DEFAULT_LIGHT_MISS2:
//			return random_sequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
//		case SEQUENCE_DEFAULT_HEAVY_MISS1:
//			return random_sequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
//		case SEQUENCE_DEFAULT_HEAVY_HIT1:
//		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return sequence + 3;
//		case SEQUENCE_DEFAULT_DRAW:
//		case SEQUENCE_DEFAULT_IDLE1:
//			return sequence;
//		default:
//			return sequence + 2;
//		}
//	}
//	case FNV("models/weapons/v_knife_survival_bowie.mdl"):
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//		case SEQUENCE_DEFAULT_IDLE1:
//			return sequence;
//		case SEQUENCE_DEFAULT_IDLE2:
//			return SEQUENCE_BOWIE_IDLE1;
//		default:
//			return sequence - 1;
//		}
//	}
//	case FNV("models/weapons/v_knife_ursus.mdl"):
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
//		default:
//			return sequence + 1;
//		}
//	}
//	case FNV("models/weapons/v_knife_stiletto.mdl"):
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return random_sequence(12, 13);
//		}
//	}
//	case FNV("models/weapons/v_knife_widowmaker.mdl"):
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return random_sequence(14, 15);
//		}
//	}
//	default:
//		return sequence;
//	}
//}
//
//namespace Cheat {
//	namespace Hooked {
//		void _cdecl SequenceHook(const CRecvProxyData* pData, void* pStruct, void* pOut) {
//			const auto data = const_cast<CRecvProxyData*>(pData);
//			const auto entity = static_cast<C_BaseViewModel*>(pStruct);
//
//			const auto local = static_cast<CBaseEntity*>(Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex()));
//
//			if (!local)
//				return;
//
//			if (!local->IsAlive())
//				return;
//
//			const auto owner = (CBaseEntity*)Interfaces::EntityList->GetClientEntityFromHandle2(entity->GetOwner());
//
//			if (owner != local)
//				return;
//
//			const auto view_model_weapon = (C_BaseAttributableItem*)Interfaces::EntityList->GetClientEntityFromHandle2(entity->GetWeapon());
//
//			if (!view_model_weapon)
//				return;
//
//			const auto weapon_info = game_data::get_weapon_info(view_model_weapon->GetItemDefinitionIndex());
//
//			if (!weapon_info)
//				return;
//
//			const auto override_model = weapon_info->model;
//
//			auto& sequence = data->m_Value.m_Int;
//			sequence = get_new_animation(fnv::hash_runtime(override_model), sequence);
//
//			oSequenceHook(pData, pStruct, pOut);
//		}
//	}
//}