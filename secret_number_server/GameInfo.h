#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <iostream>
#include <QString>
#include <chrono>
#include <random>


const int32_t MIN_VALUE = 1;
const int32_t MAX_VALUE = 100;

class GameInfo
{
    public:
        GameInfo()
            : m_GameId(0)
            , m_PlayerId(0)
            , m_SecretNumber(0)
            , m_NbTries(0)
            , m_StartTime(0)
            , m_EndTime(0)
            , m_IsResolved(false)
        {}

        GameInfo(uint32_t gameid, uint32_t playerid)
            : m_GameId(gameid)
            , m_PlayerId(playerid)
            , m_NbTries(0)
            , m_EndTime(0)
            , m_IsResolved(false)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distribution(MIN_VALUE, MAX_VALUE);


            m_SecretNumber = distribution(gen);
            m_StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        uint32_t     getGameId       () const { return m_GameId;       }
        uint32_t     getPlayerId     () const { return m_PlayerId;     }
        int32_t      getSecretNumber () const { return m_SecretNumber; }
        uint32_t     getNbTries      () const { return m_NbTries;      }
        uint64_t     getStartTime    () const { return m_StartTime;    }
        uint64_t     getEndTime      () const { return m_EndTime;      }
        uint64_t     isGameResolved  () const { return m_IsResolved;   }

        bool         isGameEnded     () const { return (m_EndTime != 0); }

        uint64_t     getTotalMs      () const
        {
            if(m_EndTime == 0)
            {
                return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_StartTime;
            }

            return (m_EndTime - m_StartTime);
        }

        bool isValidAnswer(int32_t answer)
        {
            return (answer == m_SecretNumber);
        }

        void endGame(bool resolved = false)
        {
            if(m_EndTime == 0)
            {
                m_EndTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                m_IsResolved = resolved;
            }
        }

        void incNbTries()
        {
            if(m_EndTime==0)
            {
                m_NbTries++;
            }
        }


    private:
        uint32_t     m_GameId;
        uint32_t     m_PlayerId;
        int32_t      m_SecretNumber;
        uint32_t     m_NbTries;
        uint64_t     m_StartTime;
        uint64_t     m_EndTime;
        bool         m_IsResolved;
};

std::ostream& operator<<(std::ostream& out, const GameInfo& game);

#endif
