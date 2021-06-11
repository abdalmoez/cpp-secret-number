#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H

#include <QString>
#include <QJsonObject>

enum HistoryRecordState
{
    HistoryRecordState_Win,
    HistoryRecordState_Lost,
    HistoryRecordState_Left
};

class HistoryRecord
{
public:
    HistoryRecord(
            QString              player_name,
            uint64_t             start,
            uint64_t             time_ms,
            uint64_t             end,
            QString              ip,
            uint16_t             port,
            uint32_t             nb_tries,
            uint32_t             playerid,
            int32_t              min,
            int32_t              max,
            HistoryRecordState   status)
        : m_playerName(player_name)
        , m_start(start)
        , m_timeMs(time_ms)
        , m_end(end)
        , m_ip(ip)
        , m_port(port)
        , m_nbTries(nb_tries)
        , m_playerId(playerid)
        , m_min(min)
        , m_max(max)
        , m_status(status)
    {}

    QString              getPlayerName () { return m_playerName; }
    uint64_t             getStart      () { return m_start;      }
    uint64_t             getTimeMs     () { return m_timeMs;     }
    uint64_t             getEnd        () { return m_end;        }
    QString              getIp         () { return m_ip;         }
    uint16_t             getPort       () { return m_port;       }
    uint32_t             getNbTries    () { return m_nbTries;    }
    uint32_t             getPlayerId   () { return m_playerId;   }
    uint32_t             getMin        () { return m_min;        }
    uint32_t             getMax        () { return m_max;        }
    HistoryRecordState   getStatus     () { return m_status;     }

    QJsonObject toJson();
private:
    QString              m_playerName;
    uint64_t             m_start;
    uint64_t             m_timeMs;
    uint64_t             m_end;
    QString              m_ip;
    uint16_t             m_port;
    uint32_t             m_nbTries;
    uint32_t             m_playerId;
    int32_t              m_min;
    int32_t              m_max;
    HistoryRecordState   m_status;
};

#endif // HISTORYRECORD_H
