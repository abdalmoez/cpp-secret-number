#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <iostream>
#include <QString>
#include <chrono>
#include <QRandomGenerator>


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
            m_SecretNumber = s_minValue + QRandomGenerator::global()->generate() % (s_maxValue - s_minValue);
            m_StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        uint32_t     getGameId       () const { return m_GameId;       }
        uint32_t     getPlayerId     () const { return m_PlayerId;     }
        int32_t      getSecretNumber () const { return m_SecretNumber; }
        int32_t      getNbTries      () const { return m_NbTries;      }
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
            if(m_EndTime==0 && m_NbTries != std::numeric_limits<int32_t>::max())
            {
                m_NbTries++;
            }
        }


        static int32_t s_minValue;
        static int32_t s_maxValue;
    private:
        uint32_t     m_GameId;
        uint32_t     m_PlayerId;
        int32_t      m_SecretNumber;
        int32_t      m_NbTries;
        uint64_t     m_StartTime;
        uint64_t     m_EndTime;
        bool         m_IsResolved;
};

std::ostream& operator<<(std::ostream& out, const GameInfo& game);

#endif
