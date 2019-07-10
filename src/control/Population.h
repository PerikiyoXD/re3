#pragma once

class CPed;
class CVehicle;

struct PedGroup
{
	int32 models[8];
};

class CPopulation
{
public:
	static PedGroup *ms_pPedGroups;	//[31]
	static bool &ms_bGivePedsWeapons;
	static int32 &m_AllRandomPedsThisType;
	static float &PedDensityMultiplier;

	static void Initialise();
	static void UpdatePedCount(uint32, bool);
	static void DealWithZoneChange(eLevelName oldLevel, eLevelName newLevel, bool);
	static CPed *AddPedInCar(CVehicle *vehicle);
};
