#include "common.h"
#include "patcher.h"
#include "Skidmarks.h"

WRAPPER void CSkidmarks::Init(void) { EAXJMP(0x517D70); }
WRAPPER void CSkidmarks::Render(void) { EAXJMP(0x5182E0); }
WRAPPER void CSkidmarks::Clear(void) { EAXJMP(0x518130); }
