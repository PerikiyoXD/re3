#pragma once

enum {
	RECORDSTATE_0,
	RECORDSTATE_1,
	RECORDSTATE_2,
};

class CRecordDataForGame
{
public:
	static uint16 &RecordingState;
	static void Init();
};

class CRecordDataForChase
{
public:
	static uint8 &Status;
	static void Init();
};
