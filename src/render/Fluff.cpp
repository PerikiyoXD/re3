#include "common.h"
#include "patcher.h"
#include "Fluff.h"

WRAPPER void CMovingThings::Init(void) { EAXJMP(0x4FE7C0); }
WRAPPER void CMovingThings::Render(void) { EAXJMP(0x4FF210); }
