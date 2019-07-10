#include "common.h"
#include "patcher.h"

#include "Restart.h"

WRAPPER void CRestart::Initialise(void) { EAXJMP(0x435E20); }