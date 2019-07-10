#include "common.h"
#include "patcher.h"

#include "Pools.h"

#define DEF_CPOOLS_INITIALISE 0

CCPtrNodePool *&CPools::ms_pPtrNodePool = *(CCPtrNodePool**)0x943044;
CEntryInfoNodePool *&CPools::ms_pEntryInfoNodePool = *(CEntryInfoNodePool**)0x941448;
CPedPool *&CPools::ms_pPedPool = *(CPedPool**)0x8F2C60;
CVehiclePool *&CPools::ms_pVehiclePool = *(CVehiclePool**)0x9430DC;
CBuildingPool *&CPools::ms_pBuildingPool = *(CBuildingPool**)0x8F2C04;
CTreadablePool *&CPools::ms_pTreadablePool = *(CTreadablePool**)0x8F2568;
CObjectPool *&CPools::ms_pObjectPool = *(CObjectPool**)0x880E28;
CDummyPool *&CPools::ms_pDummyPool = *(CDummyPool**)0x8F2C18;
//void*& CPools::ms_pAudioScriptObjectPool = *(void**)0x8F1B6C;

#if !DEF_CPOOLS_INITIALISE
WRAPPER void CPools::Initialise(void) { EAXJMP(0x4A1770); }
#else
void CPools::Initialise(void) {
	// TODO: unused right now
	ms_pPtrNodePool = new CCPtrNodePool(NUMPTRNODES);
	ms_pEntryInfoNodePool = new CEntryInfoNodePool(NUMENTRYINFOS);
	ms_pPedPool = new CPedPool(NUMPEDS);
	ms_pVehiclePool = new CVehiclePool(NUMVEHICLES);
	ms_pBuildingPool = new CBuildingPool(NUMBUILDINGS);
	ms_pTreadablePool = new CTreadablePool(NUMTREADABLES);
	ms_pObjectPool = new CObjectPool(NUMOBJECTS);
	ms_pDummyPool = new CDummyPool(NUMDUMMIES);
}
#endif

STARTPATCHES
#if DEF_CPOOLS_INITIALISE
	InjectHook(0x4A1770, &CPools::Initialise, PATCH_JUMP);
#endif
ENDPATCHES

