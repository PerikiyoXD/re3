#include "common.h"
#include "patcher.h"
#include "WaterCannon.h"

WRAPPER void CWaterCannons::Init(void) { EAXJMP(0x522440); }
WRAPPER void CWaterCannons::Render(void) { EAXJMP(0x522550); }
