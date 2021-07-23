#pragma once

#include "../Utility/VirtualFunc.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h#L194

typedef struct player_info_s
{
    char			pad_0x00[0x10];
    char            name[128];
    int             user_id;
    char            guid[33];
    unsigned long   friend_id;
    char            friend_name[128];
    bool            is_fake_player;
    bool            is_hltv;
    unsigned int    custom_files[4];
    unsigned char   files_downloaded;
    char			pad_big[0x200];
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
    int GetMaxClients() {
        typedef int(__thiscall* OriginalFn)(void*);
        return GetVFunc<OriginalFn>(this, 20)(this);
    }
    bool IsInGame() {
        typedef bool(__thiscall* OriginalFn)(void*);
        return GetVFunc<OriginalFn>(this, 26)(this);
    }
};