#pragma once
#include "../../Include/GlobalIncludes.h"
#include "../VTableHooking.h"

namespace Cheat
{
	namespace Hooking
	{
		class CHooks
		{
			public:

				void AddHookMgr( std::string name, std::unique_ptr<Hooking::VTableHook> mgr );
				void ReleaseHookMgr( std::string name );

			private:
				std::map<std::string, std::unique_ptr<Hooking::VTableHook>> m_vecTableHooks;
		};

		extern CHooks* Hooks;
	}
}

