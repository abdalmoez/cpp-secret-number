#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <iostream>
#include <QString>
#include <chrono>

class PlayerInfo
{
    public:
        PlayerInfo()
            :m_Id(0)
            ,m_Ip("")
            ,m_Port(0)
            ,m_Name("")
            ,m_TotalTries(0)
            ,m_TotalSolvedGames(0)
            ,m_LastLogin(0)
        {}

        PlayerInfo(int32_t id, QString ip, uint32_t port, QString name="")
            :m_Id(id)
            ,m_Ip(ip)
            ,m_Port(port)
            ,m_Name(name)
            ,m_TotalTries(0)
            ,m_TotalSolvedGames(0)
        {
            m_LastLogin = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

    public:

        void         setName      (QString value) { m_Name = value;            }

        uint32_t     getId               () const { return m_Id;               }
        QString      getIp               () const { return m_Ip;               }
        uint32_t     getPort             () const { return m_Port;             }
        QString      getName             () const { return m_Name;             }


        uint32_t     getTotalTries       () const { return m_TotalTries;       }
        uint32_t     getTotalSolvedGames () const { return m_TotalSolvedGames; }
        uint64_t     getLastLogin        () const { return m_LastLogin;        }

    protected:
        uint32_t     m_Id;
        QString      m_Ip;
        uint32_t     m_Port;
        QString      m_Name;
        uint32_t     m_TotalTries;
        uint32_t     m_TotalSolvedGames;
        uint64_t  m_LastLogin;
};

std::ostream& operator<<(std::ostream& out, const PlayerInfo& player);

#endif
