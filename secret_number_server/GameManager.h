#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameInfo.h"
#include <map>

const uint32_t INVALID_GAME_ID = 0xFFFFFFFF;
const uint32_t MAX_GAMES = 1000;
const uint32_t MAX_TRIES = 1000;

class GameManager
{
    public:
        GameInfo* createGame(uint32_t playerid);
        void removeGame(uint32_t gameid);
        GameInfo* getPlayerGame(uint32_t playerid);


    protected:
        uint32_t getNextID();

        std::map<uint32_t, GameInfo> m_Games;
};

#endif // GAMEMANAGER_H
