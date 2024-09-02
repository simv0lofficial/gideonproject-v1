#include "../Include/GlobalIncludes.h"
#include "../Utility/Utilities.h"
#include "../Configuration/Settings.h"
#include "../Interfaces/Interfaces.h"
#include "../Configuration/Config.h"
#include "../Features/EventListener.h"
#include "../Security/Security.h"
#include "../Features/Visuals/EventLogger.h"
#include "../DLL/HWID/HWID.h"
#include "../lazyimports.h"

#include "Engine.h"
#include <fstream>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>

#include "Client/main.h"
#include "../../Other/fonts.h"

#include "../Security/base64.h"
#include "../Security/md5.h"
#include "../Hashing/xorstr.h"

#include "../Lua/CLua.h"
#include "../Features/skinchanger/cskin.h"

using namespace std::chrono_literals;

void __stdcall MainThread(LPVOID lpParam);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved);

void quit(int) {
	Cheat::Settings->SocketConnection.Close();
	PostQuitMessage(0);
}

std::string ssprintf(const char* text, ...) {
	char bf[4096];

	va_list ap;
	va_start(ap, text);
	vsprintf(bf, text, ap);
	va_end(ap);

	return std::string(bf);
}

__forceinline std::vector<std::string> PseudoCode(size_t length = 0) {

	std::vector<std::string> a;
	a.push_back("abcdefghijklmnopqrstuvwxyz");

	HMODULE hMods[1024];
	int procID = 0;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (_stricmp(entry.szExeFile, xorstr_("x32dbg.exe")) == 0)
				procID = entry.th32ProcessID;
		}
	}

	CloseHandle(snapshot);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	DWORD cbNeeded;
	HMODULE hModArr[1024] = { 0 };

	if (K32EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
	{
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			if (K32GetModuleFileNameExA(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{

				if (strstr(szModName, xorstr_("kernel.32")))
					;
			}
		}
	}

	float sinus = sin(423);
	sinus *= 2;
	pow(sinus, 44);
	return a;
}

void __stdcall ProtectionThread(LPVOID lpParam) {
	while (1) {
		PseudoCode();
		if (FindWindowA(0, xorstr_("IDA v7.0.170914")) || FindWindowA(0, xorstr_("x64dbg")) || FindWindowA(0, xorstr_("Scylla x64 v0.9.8")) || FindWindowA(0, xorstr_("IAT Autosearch"))) {
			LI_FN(TerminateProcess).get()(LI_FN(GetCurrentProcess).get()(), 0);
			PseudoCode();
			quit(0);
			PseudoCode();
			BYTE* trash = (BYTE*)("0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBE");
			LI_FN(WriteProcessMemory).get()(GetCurrentProcess(), MainThread, trash, 1024, 0);
			LI_FN(WriteProcessMemory).get()(GetCurrentProcess(), DllMain, trash, 1024, 0);
			LI_FN(WriteProcessMemory).get()(GetCurrentProcess(), GetAsyncKeyState, trash, 1024, 0);
		}
		PseudoCode();
	}
}

__forceinline void antibot() {

}

__forceinline void shitasscode() {
	
}

__forceinline void retard() {
	
}

void SubVerifyThread() {

		static auto split = [](std::string str, const char* del) -> std::vector<std::string>
		{
			char* pTempStr = _strdup(str.c_str());
			char* pWord = strtok(pTempStr, del);
			std::vector<std::string> dest;

			while (pWord != NULL)
			{
				dest.push_back(pWord);
				pWord = strtok(NULL, del);
			}

			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			antibot();
			antibot();
			antibot();


			free(pTempStr);

			return dest;
		};

		if (Cheat::Settings->OnlineMode) {
			CActiveSocket& conn = Cheat::Settings->SocketConnection;
			Client* client = nullptr;
			client = new Client(conn);

			char szUsername[64] = {};
			char szPassword[64] = {};
			std::string username;
			std::string password;

			LI_FN(GetPrivateProfileStringA).get()(xorstr_("Loader"), xorstr_("username"), xorstr_(""), szUsername, 64, xorstr_("C:/gideonproject/ldr/data.ini"));
			LI_FN(GetPrivateProfileStringA).get()(xorstr_("Loader"), xorstr_("password"), xorstr_(""), szPassword, 64, xorstr_("C:/gideonproject/ldr/data.ini"));

			username = szUsername;
			password = szPassword;

			bool isEmail = false;
			for (int i = 0; i < username.length(); i++) {
				if (username.at(i) == '\0')
					break;

				if (username.at(i) == '@') {
					isEmail = true;
					break;
				}

				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				antibot();
				antibot();
				antibot();
				
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();

			}
			std::string hwid = c_hwid::get()->get_hwid();
			if (!client->WriteProtectedString(ssprintf(xorstr_("=b;=%s;=%s;=%s;=%i"), username.c_str(), password.c_str(), hwid.c_str(), isEmail).c_str())) {
				if (!conn.Close())
					quit(0);

				quit(0);

				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				shitasscode();
				retard();
				shitasscode();
				retard();
				shitasscode();
				antibot();
				antibot();
				antibot();
				antibot();
			}

			std::string login_reply = client->ReadProtectedString();

			if (login_reply != xorstr_("welcom") && login_reply != xorstr_("welcome"))
				quit(0);

			while (true) {
			if (!client->WriteProtectedString(ssprintf(xorstr_("=c;=%i"), GetTickCount()).c_str())) {
				if (!conn.Close())
					quit(0);

				quit(0);
			}

			std::string subs_reply = client->ReadProtectedString();
			if (subs_reply == xorstr_("failed"))
				quit(0);

			subs_reply = base64_decode(subs_reply);
			if (subs_reply == xorstr_(""))
				quit(0);

			std::vector<std::string> subs = split(subs_reply, xorstr_("|"));

			bool bfound = false;
			for (auto subInfo : subs) {
				auto sub = split(subInfo, xorstr_(":"));

				if (atoi(sub[0].c_str()) == Cheat::Settings->ProductID) {
					bfound = true;

					antibot();
					shitasscode();
					retard();
					shitasscode();
					retard();
					shitasscode();
					antibot();
					shitasscode();
					retard();
					shitasscode();
					retard();
					shitasscode();
					antibot();
					shitasscode();
					retard();
					shitasscode();
					retard();
					shitasscode();
					antibot();
					shitasscode();
					retard();
					shitasscode();
					retard();
					shitasscode();
					antibot();
					antibot();
					antibot();
					antibot();

					Cheat::Settings->ExpireTime = atoi(sub[4].c_str());
				}
			}

			if (!bfound)
				quit(0);

			Cheat::Settings->UserName = username;

			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			shitasscode();
			retard();
			shitasscode();
			retard();
			shitasscode();
			antibot();
			antibot();
			antibot();
			antibot();


			std::this_thread::sleep_for(1.5min);
		}


		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		antibot();
		antibot();
		antibot();

	}
}

void __stdcall MainThread(LPVOID lpParam)
{
	Cheat::Settings->LoadDefaults();

	std::string cmdln = LI_FN(GetCommandLineA).get()();
	if (cmdln.find("-d3d9ex") != std::string::npos) {
		LI_FN(MessageBoxA).get()(0, "Cannot run gideonproject with -d3d9ex parameter. Please, remove -d3d9ex parameter from CS:GO launch options and try again.", "Fatal error", MB_APPLMODAL | MB_OK | MB_ICONERROR);
		quit(0);
	}

	static auto split = [](std::string str, const char* del) -> std::vector<std::string>
	{
		char* pTempStr = _strdup(str.c_str());
		char* pWord = strtok(pTempStr, del);
		std::vector<std::string> dest;

		while (pWord != NULL)
		{
			dest.push_back(pWord);
			pWord = strtok(NULL, del);
		}

		free(pTempStr);

		return dest;
	};

	if (Cheat::Settings->OnlineMode) {
		CActiveSocket& conn = Cheat::Settings->SocketConnection;
		Client* client = nullptr;

		if (!conn.Initialize())
			quit(0);

		if (!conn.Open(xorstr_("194.58.90.227"), 1337))
			quit(0);

		client = new Client(conn);

		char szUsername[64] = {};
		char szPassword[64] = {};
		std::string username;
		std::string password;

		LI_FN(GetPrivateProfileStringA).get()(xorstr_("Loader"), xorstr_("username"), xorstr_(""), szUsername, 64, xorstr_("C:/gideonproject/ldr/data.ini"));
		LI_FN(GetPrivateProfileStringA).get()(xorstr_("Loader"), xorstr_("password"), xorstr_(""), szPassword, 64, xorstr_("C:/gideonproject/ldr/data.ini"));

		username = szUsername;
		password = szPassword;

		bool isEmail = false;
		for (int i = 0; i < username.length(); i++) {
			if (username.at(i) == '\0')
				break;

			if (username.at(i) == '@') {
				isEmail = true;
				break;
			}
		}
		std::string hwid = c_hwid::get()->get_hwid();

		if (!client->WriteProtectedString(ssprintf(xorstr_("=b;=%s;=%s;=%s;=%i"), username.c_str(), password.c_str(), hwid.c_str(), isEmail).c_str())) {
			if (!conn.Close())
				quit(0);

			quit(0);
		}



		std::string login_reply = client->ReadProtectedString();

		if (login_reply != xorstr_("welcom") && login_reply != xorstr_("welcome"))
			quit(0);

		if (!client->WriteProtectedString(ssprintf(xorstr_("=c;=%i"), GetTickCount()).c_str())) {
			if (!conn.Close())
				quit(0);

			quit(0);
		}

		std::string subs_reply = client->ReadProtectedString();
		if (subs_reply == xorstr_("failed"))
			quit(0);

		subs_reply = base64_decode(subs_reply);
		if (subs_reply == xorstr_(""))
			quit(0);

		std::vector<std::string> subs = split(subs_reply, xorstr_("|"));

		bool bfound = false;
		for (auto subInfo : subs) {
			auto sub = split(subInfo, xorstr_(":"));

			if (atoi(sub[0].c_str()) == Cheat::Settings->ProductID) {
				bfound = true;
				Cheat::Settings->ExpireTime = atoi(sub[4].c_str());
			}
		}

		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		shitasscode();
		retard();
		shitasscode();
		retard();
		shitasscode();
		antibot();
		antibot();
		antibot();
		antibot();

		if (!bfound)
			quit(0);

		Cheat::Settings->UserName = username;
		conn.Close();

		std::thread th(SubVerifyThread);
		th.detach();
	}


	AddFontResourceA(xorstr_("C:/windows/fonts/gideonproject-wpn.ttf"));
	AddFontResourceA(xorstr_("C:/windows/fonts/Smallest Pixel-7.ttf"));

	if (Cheat::Settings->ShowConsole)
		Cheat::Utilities->Console_Create(xorstr_("GideonProject BETA"));
	//LoadLibraryA(xorstr_("mscorlib.dll"));

	if (!LI_FN(GetModuleHandleA).get()(xorstr_("serverbrowser.dll")))
	{
		Cheat::Utilities->Console_Log(xorstr_("Waiting for CS:GO..."));

		while (!LI_FN(GetModuleHandleA).get()(xorstr_("serverbrowser.dll")))
			Sleep(1000);
	}

	Cheat::Utilities->Console_Log(xorstr_("GideonProject %i.%i build %i"),
		Cheat::Settings->VersionMajor,
		Cheat::Settings->VersionMinor,
		Cheat::Settings->VersionBuild);

	if (Cheat::Settings->UIDevMode)
		Cheat::Utilities->Console_Log(xorstr_("UI development mode! Only ImGUI was loaded..."));

	if (LI_FN(GetModuleHandleA).get()(xorstr_("client_panorama.dll")))
		Cheat::Settings->PanoramaMode = true;

	if (!Cheat::Settings->UIDevMode)
	{
		Interfaces::InterfaceLoader->Initialize();
	}

	Cheat::Initialize();

	c_lua::get()->initialize();

	Interfaces::Engine->ClientCmd_Unrestricted(xorstr_("clear"));
	LI_FN(Sleep).get()(500);
	Cheat::Utilities->Game_Msg((std::string(xorstr_("Welcome ")) + Cheat::Settings->UserName).c_str());

	while (!Cheat::Settings->UnloadCheat)
		LI_FN(Sleep).get()(1000);

	Cheat::Utilities->Console_Close();
	Cheat::Unload();
	LI_FN(FreeLibraryAndExitThread).get()((HMODULE)lpParam, LI_FN(GetCurrentThreadId).get()());
	DllMain((HMODULE)lpParam, DLL_PROCESS_DETACH, NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved)
{
	if (dwCallReason == DLL_PROCESS_ATTACH) {
		LI_FN(CreateThread).get()(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
	}
	return TRUE;
}
