#include "common.h"
#include "patcher.h"

#include "Cranes.h"

WRAPPER void CCranes::InitCranes() { EAXJMP(0x543360); }