#pragma once

class CRestart
{
private:
	static int8& ActivePlayers;
public:
	//static void AddHospitalRestartPoint(CVector pos, float headings);
	//static void	AddPoliceRestartPoint(CVector pos, float headings);
	//static void	CancelOverrideRestart(void);
	//static void FindClosestHospitalRestartPoint(CVector const&, CVector*, float*);
	//static void	FindClosestPoliceRestartPoint(CVector const&, CVector*, float*);
	static void	Initialise(void);
	//static void	LoadAllRestartPoints(uchar*, uint);
	//static void OverrideNextRestart(CVector const&, float);
	//static void SaveAllRestartPoints(uchar*, uint*);

};

