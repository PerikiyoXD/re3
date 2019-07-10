#include "common.h"
#include "patcher.h"
#include "WaterLevel.h"

WRAPPER void CWaterLevel::Initialise(char* waterfile) { EAXJMP(0x554EA0); }
WRAPPER void CWaterLevel::RenderWater(void) { EAXJMP(0x5554E0); }
