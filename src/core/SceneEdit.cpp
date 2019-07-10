#include "common.h"
#include "patcher.h"

#include "SceneEdit.h"

WRAPPER void CSceneEdit::Initialise() { EAXJMP(0x585170); }