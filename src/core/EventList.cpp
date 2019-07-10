#include "common.h"
#include "patcher.h"
#include "EventList.h"


WRAPPER void CEventList::Initialise(void) { EAXJMP(0x475B60); }