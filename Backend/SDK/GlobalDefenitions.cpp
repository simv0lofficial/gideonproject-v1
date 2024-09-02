#include "../Include/GlobalIncludes.h"

#include "GlobalDefenitions.h"
#include "CBaseEntity.h"
#include "CUserCmd.h"

class CUserCmd;


CBaseEntity* c_globals::LocalPlayer = nullptr;
int c_globals::LocalPlayerIndex = 0;
CUserCmd* G::UserCmd = nullptr;
CUserCmd* G::UserCmd_Backtracking = nullptr;
bool G::SendPacket = true;
Vector G::RealAngle = Vector(0, 0, 0);
int G::iScreenWidth = 0, G::iScreenHeight = 0, G::choked = 1;
bool G::ResetCheatState = false;
bool G::CanShootSomeone = false;
bool G::DisableFakelags = false;
float G::matrix[4][4];
float G::DesyncDelta = 0.f;
Vector G::FakeAngle = Vector(0, 0, 0);

bool G::RagebotIsShootingAR = false;
bool G::RagebotIsShooting = false;
CBaseEntity* G::RagebotTarget = nullptr;
CBaseEntity* G::RagebotPreTarget = nullptr;
bool G::MissingDueToSpread = false;
bool G::RageBotHurting = false;
float G::ResolverAdd[64] = {0};
int  G::MissedShots[64] = {0};
bool  G::Shot[65];
bool G::Hit[65];
int            G::MissedShots1[65];
Vector G::RagebotShootPosition = Vector(0, 0, 0);
Vector G::FakePosition = Vector(0, 0, 0);
Vector G::UnpredictedVelocity = Vector(0, 0, 0);
matrix3x4_t G::RagebotMatrix[128];

float G::VisualLastInaccuracy = 0;
float G::VisualLastCone = 0;
bool G::VisualLastKnife = 0;
bool G::VisualLastNade = 0;
float G::VisualLastRange = 0;
std::string G::CurrentFreestandMode = "OFF";
bool G::PK_RoundStarted = false;
bool G::PK_Reset = false;
bool G::FakelagShouldLag = true;
bool G::IsInThirdPerson = false;
float G::ViewZ = 0.f;

Vector G::DisplayRealAngle = Vector();
Vector G::DisplayFakeAngle = Vector();