#include "common.h"
#include "patcher.h"
#include "Phones.h"

CPhoneInfo &gPhoneInfo = *(CPhoneInfo*)0x732A20;

WRAPPER void PhonePutDownCB(CAnimBlendAssociation *assoc, void *arg) { EAXJMP(0x42F570); }
WRAPPER void PhonePickUpCB(CAnimBlendAssociation *assoc, void *arg) { EAXJMP(0x42F470); }
WRAPPER void CPhoneInfo::Initialise() { EAXJMP(0x42F640)}
