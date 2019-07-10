#include "common.h"
#include "patcher.h"
#include "Record.h"

uint16 &CRecordDataForGame::RecordingState = *(uint16*)0x95CC24;

uint8 &CRecordDataForChase::Status = *(uint8*)0x95CDCE;

WRAPPER void CRecordDataForGame::Init(void) { EAXJMP(0x4340F0); }
WRAPPER void CRecordDataForChase::Init(void) { EAXJMP(0x434780); }

