#include "Settings.h"
#include "../../Other/version.h"

Cheat::CSettings* Cheat::Settings = new Cheat::CSettings();
using namespace Cheat;

void CSettings::LoadDefaults()
{
	#if _DEBUG
	this->ShowConsole = true;
	this->OnlineMode = false;
	#endif
	#if GP_BETABUILD
	this->ProductID = 3;
	#endif
	this->PanoramaMode = false;
	this->UnloadCheat = false;
	this->UIDevMode = false;
}
