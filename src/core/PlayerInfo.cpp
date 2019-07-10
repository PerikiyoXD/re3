#include "common.h"
#include "patcher.h"
#include "PlayerInfo.h"

WRAPPER void CPlayerInfo::Clear() { EAXJMP(0x49FC10); }
WRAPPER void CPlayerInfo::MakePlayerSafe(bool) { EAXJMP(0x4A1400); }
WRAPPER void CPlayerInfo::LoadPlayerSkin() { EAXJMP(0x4A1700); }