#include "common.h"
#include "patcher.h"

#include "main.h"
#include "CdStream.h"
#include "GameLogic.h"
#include "IniFile.h"
#include "PedStats.h"
#include "Pools.h"
#include "TxdStore.h"
#include "References.h"
#include "Camera.h"
#include "PathFind.h"
#include "Weather.h"
#include "ZoneCull.h"
#include "Collision.h"
#include "Zones.h"
#include "Messages.h"
#include "Record.h"
#include "CutsceneMgr.h"
#include "FileLoader.h"
#include "Pickups.h"
#include "CarCtrl.h"
#include "World.h"
#include "FileMgr.h"
#include "Particle.h"
#include "WaterLevel.h"
#include "Pad.h"
#include "Script.h"
#include "Clock.h"
#include "Heli.h"
#include "Plane.h"
#include "Rubbish.h"
#include "Coronas.h"
#include "Shadows.h"
#include "WeaponEffects.h"
#include "Replay.h"
#include "User.h"
#include "Restart.h"
#include "ModelIndices.h"
#include "CarGen.h"
#include "Streaming.h"
#include "NodeName.h"
#include "Credits.h"
#include "Train.h"
#include "Garages.h"
#include "Bridge.h"
#include "WaterCannon.h"
#include "SpecialFX.h"
#include "Fluff.h"
#include "Gangs.h"
#include "Stats.h"
#include "Darkel.h"
#include "Glass.h"
#include "Clouds.h"
#include "Antennas.h"
#include "Skidmarks.h"
#include "Population.h"
#include "TrafficLights.h"
#include "Renderer.h"
#include "EventList.h"
#include "Phones.h"
#include "RoadBlock.h"
#include "SceneEdit.h"
#include "RouteNode.h"
#include "Cranes.h"

#include "Game.h"

eLevelName & CGame::currLevel = *(eLevelName*)0x941514;
bool& CGame::bDemoMode = *(bool*)0x5F4DD0;
bool& CGame::nastyGame = *(bool*)0x5F4DD4;
bool& CGame::frenchGame = *(bool*)0x95CDCB;
bool& CGame::germanGame = *(bool*)0x95CD1E;
bool& CGame::noProstitutes = *(bool*)0x95CDCF;
bool& CGame::playingIntro = *(bool*)0x95CDC2;
char* CGame::aDatFile = (char*)0x773A48;
int& gameTxdSlot = *(int*)0x628D88;

//WRAPPER void CGame::Initialise(const char* datFile) { EAXJMP(0x48BED0); }

void CGame::Initialise(const char* datFile)
{
	ResetLoadingScreenBar();
	strcpy(CGame::aDatFile, datFile);
	CPools::Initialise();
	CIniFile::LoadIniFile();
	CGame::currLevel = LEVEL_INDUSTRIAL;
	LoadingScreen("Loading the Game", "Loading generic textures", GetRandomSplashScreen());
	gameTxdSlot = CTxdStore::AddTxdSlot("generic");
	CTxdStore::Create(gameTxdSlot);
	CTxdStore::AddRef(gameTxdSlot);
	LoadingScreen("Loading the Game", "Loading particles", 0);
	int particleTxd = CTxdStore::AddTxdSlot("particle");
	CTxdStore::LoadTxd(particleTxd, "MODELS/PARTICLE.TXD");
	CTxdStore::AddRef(particleTxd);
	CTxdStore::SetCurrentTxd(gameTxdSlot);
	LoadingScreen("Loading the Game", "Setup game variables", 0);
	CGameLogic::InitAtStartOfGame();
	CReferences::Init();
	TheCamera.Init();
	TheCamera.SetRwCamera(Scene.camera);
	//CDebug::DebugInitTextBuffer();
	ThePaths.Init(); //ESP 0019FB0C
	ThePaths.AllocatePathFindInfoMem(4500);
	CWeather::Init(); //ESP = 0019FB10
	CCullZones::Init();
	CCollision::Init();
	CTheZones::Init();
	CUserDisplay::Init();
	CMessages::Init();
	CMessages::ClearAllMessagesDisplayedByGame();
	CRecordDataForGame::Init();
	CRestart::Initialise();
	CWorld::Initialise();
	CParticle::Initialise();
	CAnimManager::Initialise();
	CCutsceneMgr::Initialise();
	CCarCtrl::Init();
	InitModelIndices();
	CModelInfo::Initialise();
	CPickups::Init();
	CTheCarGenerators::Init();
	CdStreamAddImage("MODELS\\GTA3.IMG");
	CFileLoader::LoadLevel("DATA\\DEFAULT.DAT");
	CFileLoader::LoadLevel(datFile);
	CWorld::AddParticles();
	CVehicleModelInfo::LoadVehicleColours();
	CVehicleModelInfo::LoadEnvironmentMaps();
	CTheZones::PostZoneCreation();
	LoadingScreen("Loading the Game", "Setup paths", GetRandomSplashScreen());
	ThePaths.PreparePathData();
	for (int i = 0; i < NUMPLAYERS; i++)
	{
		CWorld::Players[i].Clear();
	}
	CWorld::Players->LoadPlayerSkin();
	//TestModelIndices();
	LoadingScreen("Loading the Game", "Setup water", 0);
	CWaterLevel::Initialise("DATA\\WATER.DAT");
	//byte_8F6499 = 0;
	//TheConsole = 0;
	CDraw::SetFOV(120.0);
	//CDraw::ms_fLODDistance = 0x43FA0000;
	LoadingScreen("Loading the Game", "Setup streaming", 0);
	int txdImg = CFileMgr::OpenFile("MODELS\\TXD.IMG", "r");
	if (txdImg) CFileMgr::CloseFile(txdImg);
	if (!CheckVideoCardCaps() && txdImg)
	{
		CdStreamAddImage("MODELS\\TXD.IMG");
		CStreaming::Init();
	}
	else
	{
		CStreaming::Init();
		if (CreateTxdImageForVideoCard())
		{
			CStreaming::Shutdown();
			CdStreamAddImage("MODELS\\TXD.IMG");
			CStreaming::Init();
		}
	}
	CStreaming::LoadInitialVehicles();
	CStreaming::LoadInitialPeds();
	CStreaming::RequestBigBuildings(LEVEL_NONE);
	CStreaming::LoadAllRequestedModels(0);
	debug("Streaming uses %dK of its memory", CStreaming::ms_memoryUsed / 1024);
	LoadingScreen("Loading the Game", "Load animations", GetRandomSplashScreen());
	CAnimManager::LoadAnimFiles();
	CPed::Initialise();
	CRouteNode::Initialise(); //004EE470
	CEventList::Initialise();
	LoadingScreen("Loading the Game", "Find big buildings", 0);
	CRenderer::Init();
	LoadingScreen("Loading the Game", "Setup game variables", 0);
	CRadar::Initialise();
	CRadar::LoadTextures();
	CWeapon::InitialiseWeapons();
	LoadingScreen("Loading the Game", "Setup traffic lights", 0);
	CTrafficLights::ScanForLightsOnMap();
	CRoadBlocks::Init();
	LoadingScreen("Loading the Game", "Setup game variables", 0);
	CPopulation::Initialise();
	CWorld::PlayerInFocus = 0;
	CCoronas::Init();
	CShadows::Init();
	CWeaponEffects::Init();
	CSkidmarks::Init();
	CAntennas::Init();
	CGlass::Init();
	gPhoneInfo.Initialise();
	CSceneEdit::Initialise();
	LoadingScreen("Loading the Game", "Load scripts", 0);
	CTheScripts::Init();
	CGangs::Initialise();
	LoadingScreen("Loading the Game", "Setup game variables", 0);	  //ok
	CClock::Initialise(1000);										  //ok
	CHeli::InitHelis();												  //ok
	CCranes::InitCranes();											  //ok
	CMovingThings::Init();											  //ok
	CDarkel::Init();												  //ok
	CStats::Init();													  //ok
	CPacManPickups::Init();											  //ok
	CRubbish::Init();												  //ok
	CClouds::Init();												  //ok
	CSpecialFX::Init();												  //ok
	CWaterCannons::Init();											  //ok
	CBridge::Init();												  //ok
	CGarages::Initialise(); 										  //ok
	LoadingScreen("Loading the Game", "Position dynamic objects", 0); //ok
	CWorld::RepositionCertainDynamicObjects();						  //ok
	LoadingScreen("Loading the Game", "Initialise vehicle paths", 0); //ok
	CCullZones::ResolveVisibilities();								  //ok
	CTrain::InitTrains();											  //ok
	CPlane::InitPlanes();											  //ok
	CCredits::Init();												  //ok
	CRecordDataForChase::Init();									  //ok
	CReplay::Init();												  //ok
	LoadingScreen("Loading the Game", "Start script", 0);			  //ok
	CTheScripts::StartTestScript();
	CTheScripts::Process();
	TheCamera.Process();
	LoadingScreen("Loading the Game", "Load scene", 0);
	CModelInfo::RemoveColModelsFromOtherLevels(CGame::currLevel);
	CCollision::ms_collisionInMemory = CGame::currLevel;
	CPad::GetPad(0)->Clear(1);
	CPad::GetPad(1)->Clear(1);
}

WRAPPER bool CreateTxdImageForVideoCard(void) { EAXJMP(0x592C70); }
WRAPPER bool CheckVideoCardCaps(void) { EAXJMP(0x592740); }
WRAPPER void CGame::Process(void) { EAXJMP(0x48C850); }
WRAPPER bool CGame::InitialiseOnceBeforeRW(void) { EAXJMP(0x48BB80); }
WRAPPER bool CGame::InitialiseRenderWare(void) { EAXJMP(0x48BBA0); }
WRAPPER void CGame::ShutdownRenderWare(void) { EAXJMP(0x48BCB0); }

void CGame::FinalShutdown(void)
{
	CTxdStore::Shutdown();
	CPedStats::Shutdown();
	CdStreamShutdown();
}

WRAPPER void CGame::ShutDown(void) { EAXJMP(0x48C3A0); }
WRAPPER void CGame::ShutDownForRestart(void) { EAXJMP(0x48C6B0); }
WRAPPER void CGame::InitialiseWhenRestarting(void) { EAXJMP(0x48C740); }
WRAPPER bool CGame::InitialiseOnceAfterRW(void) { EAXJMP(0x48BD50); }

STARTPATCHES
InjectHook(0x48BED0, &CGame::Initialise, PATCH_JUMP);
InjectHook(0x48BEC0, &CGame::FinalShutdown, PATCH_JUMP);
ENDPATCHES

