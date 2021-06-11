#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "PlayerInfo.h"
#include <map>

const uint32_t INVALID_PLAYER_ID = 0xFFFFFFFF;


class PlayerManager
{
    public:
        PlayerInfo* addPlayer(QString ip, uint32_t port);
        void removePlayer(uint32_t playerid);


        static uint32_t s_maxPlayers;

    protected:
        uint32_t getNextID();

        std::map<uint32_t, PlayerInfo> m_Players;


};

#endif
