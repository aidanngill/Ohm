#pragma once

#include <iostream>

#include "./Math/Vector.h"

class INetChannel;

class CClockDriftManager {
public:
    float               m_clock_offsets[17];
    uint32_t            m_cur_clock_offset;
    uint32_t            m_server_tick;
    uint32_t            m_client_tick;
};

class CClientState {
public:
    void force_full_update() {
        *reinterpret_cast<int*>(uintptr_t(this) + 0x174) = -1;
    };

    char                pad_0000[156];
    INetChannel*        m_net_channel;
    uint32_t            m_challenge_nr;
    char                pad_00A4[100];
    uint32_t            m_signon_state;
    char                pad_010C[8];
    float               m_next_cmd_time;
    uint32_t            m_server_count;
    uint32_t            m_current_sequence;
    char                pad_0120[8];
    CClockDriftManager  m_clock_drift_mgr;
    uint32_t            m_delta_tick;
    bool                m_paused;
    char                pad_017D[3];
    uint32_t            m_view_entity;
    uint32_t            m_player_slot;
    char                m_level_name[260];
    char                m_level_name_short[80];
    char                m_group_name[80];
    char                pad_032C[92];
    uint32_t            m_max_clients;
    char                pad_0314[18820];
    float               m_last_server_tick_time;
    bool                m_insimulation;
    char                pad_4C9D[3];
    uint32_t            m_oldtickcount;
    float               m_tick_remainder;
    float               m_frame_time;
    uint32_t            m_last_outgoing_command;
    uint32_t            m_choked_commands;
    uint32_t            m_last_command_ack;
    uint32_t            m_command_ack;
    uint32_t            m_sound_sequence;
    char                pad_4CC0[80];
    Vector              viewangles;
};