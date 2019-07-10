#include "common.h"
#include "patcher.h"
#include "CarGen.h"

WRAPPER void CTheCarGenerators::Init() { EAXJMP(0x543020); }
WRAPPER void CTheCarGenerators::Process() { EAXJMP(0x542F40); }