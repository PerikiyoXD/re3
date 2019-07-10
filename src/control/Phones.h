#pragma once

#include "AnimBlendAssociation.h"

class CPhoneInfo
{
public:
	void Initialise();
};

extern CPhoneInfo &gPhoneInfo;

void PhonePutDownCB(CAnimBlendAssociation *assoc, void *arg);
void PhonePickUpCB(CAnimBlendAssociation *assoc, void *arg);