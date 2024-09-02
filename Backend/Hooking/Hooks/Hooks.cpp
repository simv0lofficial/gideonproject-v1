#include "Hooks.h"

Cheat::Hooking::CHooks* Cheat::Hooking::Hooks = new Cheat::Hooking::CHooks();

using namespace Cheat::Hooking;

void CHooks::AddHookMgr( std::string name, std::unique_ptr<VTableHook> mgr )
{
	m_vecTableHooks.insert( std::make_pair( name, std::move( mgr ) ) );
}

void CHooks::ReleaseHookMgr( std::string name )
{
	m_vecTableHooks[name]->~VTableHook();
	m_vecTableHooks.erase( name );
}

