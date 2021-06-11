#include "HistoryStorage.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

std::vector<HistoryRecord> HistoryStorage::m_records;

void HistoryStorage::save()
{
    QJsonArray recordsArray;

    for(auto it=m_records.begin();it!=m_records.end();it++)
    {
        recordsArray.append(it->toJson());
    }
    QJsonDocument  json;
    json.setArray(recordsArray);
    QString data = json.toJson();

    QFile f(qApp->applicationDirPath()+"/history.json");

    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream out(&f);
        out << data;
    }
}

void HistoryStorage::load()
{
    m_records.clear();
    QFile f(qApp->applicationDirPath()+"/history.json");

    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }
    QTextStream in(&f);

    QJsonDocument jsonResponse = QJsonDocument::fromJson(in.readAll().toUtf8());

    QJsonArray rec = jsonResponse.array();

    for(auto it=rec.begin();it!=rec.end();it++)
    {
        auto v = it->toObject();
        if(!v.contains("playerid")    || !v["playerid"].isDouble()    ||
           !v.contains("playername")  || !v["playername"].isString()  ||
           !v.contains("start")       || !v["start"].isDouble()       ||
           !v.contains("time_ms")     || !v["time_ms"].isDouble()     ||
           !v.contains("end")         || !v["end"].isDouble()         ||
           !v.contains("ip")          || !v["ip"].isString()          ||
           !v.contains("port")        || !v["port"].isDouble()        ||
           !v.contains("nb_tries")    || !v["nb_tries"].isDouble()    ||
           !v.contains("min")         || !v["min"].isDouble()         ||
           !v.contains("max")         || !v["max"].isDouble()         ||
           !v.contains("status")      || !v["status"].isDouble())
        {
            continue;
        }
        QString              player_name = v["playername"].toString();
        uint64_t             start       = (uint64_t)v["start"].toDouble();
        uint64_t             time_ms     = (uint64_t)v["time_ms"].toDouble();
        uint64_t             end         = (uint64_t)v["end"].toDouble();
        QString              ip          = v["ip"].toString();
        uint16_t             port        = (uint16_t)v["port"].toDouble();
        uint32_t             nb_tries    = (uint32_t)v["nb_tries"].toDouble();
        uint32_t             playerid    = (uint32_t)v["playerid"].toDouble();
        int32_t              min         = (int32_t)v["min"].toDouble();
        int32_t              max         = (int32_t)v["max"].toDouble();
        HistoryRecordState   status      = (HistoryRecordState)v["status"].toDouble();
        m_records.push_back(HistoryRecord(player_name,
                                          start,
                                          time_ms,
                                          end,
                                          ip,
                                          port,
                                          nb_tries,
                                          playerid,
                                          min,
                                          max,
                                          status));
    }
}
