#include "common.h"
#include "patcher.h"
#include "Rubbish.h"

WRAPPER void CRubbish::Init(void) { EAXJMP(0x511940); }
WRAPPER void CRubbish::Render(void) { EAXJMP(0x512190); }
