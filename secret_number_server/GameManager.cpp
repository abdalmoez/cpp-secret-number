#include "GameManager.h"

GameInfo* GameManager::createGame(uint32_t playerid)
{
    uint32_t gameid = getNextID();

    if(gameid == INVALID_GAME_ID)
    {
        // REFUSE CREATE GAME REQUEST
        return nullptr;
    }

    m_Games[gameid] = GameInfo(gameid, playerid);

    return &m_Games[gameid];
}

void GameManager::removeGame(uint32_t gameid)
{
    if(gameid != INVALID_GAME_ID)
    {
        m_Games.erase(gameid);
    }
}
GameInfo* GameManager::getPlayerGame(uint32_t playerid)
{
    if(m_Games.find(playerid) == m_Games.end())
    {
        return nullptr;
    }
    return &m_Games[playerid];
}

uint32_t GameManager::getNextID()
{
    if(m_Games.size() >= MAX_GAMES)
    {
        return INVALID_GAME_ID;
    }

    uint32_t newid=0;

    for (auto it = m_Games.begin(); it != m_Games.end(); it++)
    {
        if(it->first > newid)
        {
            return newid;
        }
        newid++;
    }

    return newid;
}
