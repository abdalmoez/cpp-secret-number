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
    HistoryRecord()
        : m_playerName("")
        , m_start(0)
        , m_timeMs(0)
        , m_end(0)
        , m_ip("")
        , m_port(0)
        , m_nbTries(0)
        , m_playerId(0)
        , m_min(0)
        , m_max(0)
        , m_status(HistoryRecordState_Win)
    {}

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

    QString              getPlayerName () const { return m_playerName; }
    uint64_t             getStart      () const { return m_start;      }
    uint64_t             getTimeMs     () const { return m_timeMs;     }
    uint64_t             getEnd        () const { return m_end;        }
    QString              getIp         () const { return m_ip;         }
    uint16_t             getPort       () const { return m_port;       }
    uint32_t             getNbTries    () const { return m_nbTries;    }
    uint32_t             getPlayerId   () const { return m_playerId;   }
    uint32_t             getMin        () const { return m_min;        }
    uint32_t             getMax        () const { return m_max;        }
    HistoryRecordState   getStatus     () const { return m_status;     }

    QJsonObject toJson() const;
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
