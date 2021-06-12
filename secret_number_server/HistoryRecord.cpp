#include "HistoryRecord.h"
#include <QVariant>


QJsonObject HistoryRecord::toJson() const
{
    QJsonObject recordObject;
    recordObject.insert("playerid"    , QJsonValue::fromVariant(m_playerId));
    recordObject.insert("playername" , QJsonValue::fromVariant(m_playerName));
    recordObject.insert("start"       , QJsonValue::fromVariant(m_start));
    recordObject.insert("time_ms"     , QJsonValue::fromVariant(m_timeMs));
    recordObject.insert("end"         , QJsonValue::fromVariant(m_end));
    recordObject.insert("ip"          , QJsonValue::fromVariant(m_ip));
    recordObject.insert("port"        , QJsonValue::fromVariant(m_port));
    recordObject.insert("nb_tries"    , QJsonValue::fromVariant(m_nbTries));
    recordObject.insert("min"         , QJsonValue::fromVariant(m_min));
    recordObject.insert("max"         , QJsonValue::fromVariant(m_max));
    recordObject.insert("status"      , QJsonValue::fromVariant(m_status));

    return recordObject;
}
