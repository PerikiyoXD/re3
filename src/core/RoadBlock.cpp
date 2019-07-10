#include "common.h"
#include "patcher.h"
#include "RoadBlock.h"

WRAPPER void CRoadBlocks::Init() { EAXJMP(0x436F50); }