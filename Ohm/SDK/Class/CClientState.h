#pragma once

#include <iostream>

#include "../Math/Vector.h"

class INetChannel;

class CClockDriftManager {
public:
	float               clockOffsets[17];
	uint32_t            currentClockOffset;
	uint32_t            serverTick;
	uint32_t            clientTick;
};

class CClientState {
public:
	void forceFullUpdate() {
		*reinterpret_cast<int*>(uintptr_t(this) + 0x174) = -1;
	};

	char				pad_0000[156];
	INetChannel*        netChannel;
	uint32_t            challengeNumber;
	char                pad_0001[100];
	uint32_t            signOnState;
	char                pad_0002[8];
	float               nextCmdTime;
	uint32_t            serverCount;
	uint32_t            currentSequence;
	char                pad_0003[8];
	CClockDriftManager  clockDriftManager;
	uint32_t            deltaTick;
	bool                isPaused;
	char                pad_0004[3];
	uint32_t            viewEntity;
	uint32_t            playerSlot;
	char                levelName[260];
	char                levelNameShort[80];
	char                groupName[80];
	char                pad_0005[92];
	uint32_t            maxClients;
	char                pad_0006[18820];
	float               lastServerTickTime;
	bool                isInSimulation;
	char                pad_0007[3];
	uint32_t            oldTickCount;
	float               tickRemainder;
	float               frameTime;
	uint32_t            lastOutgoingCommand;
	uint32_t            chokedCommands;
	uint32_t            lastCommandAck;
	uint32_t            commandAck;
	uint32_t            soundSequence;
	char                pad_0008[80];
	Vector              viewAngles;
};