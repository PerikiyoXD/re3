#include "common.h"
#include "patcher.h"

#include "GameLogic.h"


int8 &CGameLogic::ActivePlayers = *(int8*)0x95CD5E;

//WRAPPER void CGameLogic::InitAtStartOfGame(void) { EAXJMP(0x4213F0); }
void CGameLogic::InitAtStartOfGame(void)
{
	CGameLogic::ActivePlayers = 1;
}

STARTPATCHES
InjectHook(0x4213F0, &CGameLogic::InitAtStartOfGame, PATCH_JUMP);
ENDPATCHES