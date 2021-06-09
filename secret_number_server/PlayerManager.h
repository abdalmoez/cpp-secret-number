#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "PlayerInfo.h"
#include <map>


class PlayerManager
{
    public:
        PlayerInfo* addPlayer(QString ip, uint32_t port);
        void removePlayer(uint32_t playerid);

        const uint32_t MAX_PLAYERS = 1000;

        const uint32_t INVALID_PLAYER_ID = 0xFFFFFFFF;

    protected:
        uint32_t getNextID();


        std::map<uint32_t, PlayerInfo> m_Players;


};

#endif
