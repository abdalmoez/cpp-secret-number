#include "PlayerManager.h"

PlayerInfo* PlayerManager::addPlayer(QString ip, uint32_t port)
{
    uint32_t playerid = getNextID();
    if(playerid == INVALID_PLAYER_ID)
    {
        // REFUSE CONNECTION
        return nullptr;
    }

    m_Players[playerid] = PlayerInfo(playerid, ip, port);

    return &m_Players[playerid];
}

void PlayerManager::removePlayer(uint32_t playerid)
{
    if(playerid != INVALID_PLAYER_ID)
    {
        m_Players.erase(playerid);
    }
}


uint32_t PlayerManager::getNextID()
{
    if(m_Players.size() >= MAX_PLAYERS)
    {
        return INVALID_PLAYER_ID;
    }

    uint32_t newid=0;

    for (auto it = m_Players.begin(); it != m_Players.end(); it++)
    {
        if(it->first > newid)
        {
            return newid;
        }
        newid++;
    }

    return newid;
}
