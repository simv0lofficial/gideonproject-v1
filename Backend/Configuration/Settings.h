#pragma once
#include "../DLL/Client/ActiveSocket.h"
#include "../Include/GlobalIncludes.h"
#include "../Hashing/xorstr.h"
namespace Cheat
{
	class CSettings
	{
		public:
			bool PanoramaMode	= false;
			bool ShowConsole	= false;
			bool UnloadCheat	= false;
			bool OnlineMode		= true;
			bool UIDevMode		= false;

			CActiveSocket SocketConnection;

			int VersionMajor = 2;
			int VersionMinor = 0;
			int VersionBuild = 1337;
			int ExpireTime	 = 0;
			int ProductID	 = 2; /* 2 */

			const char* GameName = ("Counter-Strike: Global Offensive");

			std::string ClientDLL = ("client_panorama.dll");
			std::string UserName = ("unk");

			void LoadDefaults();
	};

	extern CSettings* Settings;
}
