#pragma once

#include "../Utility/VirtualFunc.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h#L194

typedef struct player_info_s
{
	char			pad_0000[0x10];
	char			name[128];
	int				userID;
	char			guid[33];
	unsigned long	friendID;
	char			friendName[128];
	bool			isFakePlayer;
	bool			isHLTV;
	unsigned int	customFiles[4];
	unsigned char	filesDownloaded;
	char			pad_0001[0x200];
} player_info_t;

class IVEngineClient {
public:
	bool GetPlayerInfo(int ent_num, player_info_t* pinfo) {
		typedef bool(__thiscall* OriginalFn)(void*, int, player_info_t*);
		return GetVFunc<OriginalFn>(this, 8)(this, ent_num, pinfo);
	}
	int GetLocalPlayer() {
		typedef int(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 12)(this);
	}
	void GetViewAngles(Vector& angles) {
		typedef void(__thiscall* OriginalFn)(void*, Vector&);
		return GetVFunc<OriginalFn>(this, 18)(this, std::ref(angles));
	}
	void SetViewAngles(const Vector& angles) {
		typedef void(__thiscall* OriginalFn)(void*, const Vector&);
		return GetVFunc<OriginalFn>(this, 19)(this, std::cref(angles));
	}
	int GetMaxClients() {
		typedef int(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 20)(this);
	}
	bool IsInGame() {
		typedef bool(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 26)(this);
	}
	bool IsConnected() {
		typedef bool(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 27)(this);
	}
	bool IsDrawingLoadingImage() {
		typedef bool(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 28)(this);
	}
	const char* GetLevelName() {
		typedef const char* (__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 52)(this);
	}
};