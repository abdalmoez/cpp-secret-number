#include "MsgFactory.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QDebug>

QString MsgFactory::createGameCreatedMsg(uint32_t gameid, uint32_t playerid, uint64_t start_time, int32_t max, int32_t min)
{
    QJsonObject recordObject;
    recordObject.insert("type"      , RC_GAME_CREATED);
    recordObject.insert("gameid"    , QJsonValue::fromVariant(gameid));
    recordObject.insert("playerid"  , QJsonValue::fromVariant(playerid));
    recordObject.insert("start_time", QJsonValue::fromVariant(start_time));
    recordObject.insert("max"       , QJsonValue::fromVariant(max));
    recordObject.insert("min"       , QJsonValue::fromVariant(min));
    QJsonDocument doc(recordObject);

    return doc.toJson();
}

QString MsgFactory::createInvalidAnswerMsg(uint32_t gameid, uint32_t playerid,int32_t answer, int32_t total_tries, int32_t state)
{
    QJsonObject recordObject;
    recordObject.insert("type"       , RC_INVALID_ANSWER);
    recordObject.insert("gameid"     , QJsonValue::fromVariant(gameid));
    recordObject.insert("answer"     , QJsonValue::fromVariant(answer));
    recordObject.insert("playerid"   , QJsonValue::fromVariant(playerid));
    recordObject.insert("total_tries", QJsonValue::fromVariant(total_tries));
    recordObject.insert("state"      , QJsonValue::fromVariant(state));
    QJsonDocument doc(recordObject);

    return doc.toJson();
}

QString MsgFactory::createGameoverLoserMsg(uint32_t gameid,
                                           uint32_t playerid,
                                           uint64_t start_time,
                                           uint64_t end_time,
                                           uint64_t total_time,
                                           int32_t  total_tries,
                                           int32_t  secret_number,
                                           uint64_t rank,
                                           std::vector<HistoryRecord> top_players)
{

    QJsonObject recordObject;
    recordObject.insert("type"          , RC_GAMEOVER_LOSER);
    recordObject.insert("gameid"        , QJsonValue::fromVariant(gameid));
    recordObject.insert("playerid"      , QJsonValue::fromVariant(playerid));
    recordObject.insert("start_time"    , QJsonValue::fromVariant(start_time));
    recordObject.insert("end_time"      , QJsonValue::fromVariant(end_time));
    recordObject.insert("total_time"    , QJsonValue::fromVariant(total_time));
    recordObject.insert("total_tries"   , QJsonValue::fromVariant(total_tries));
    recordObject.insert("secret_number" , QJsonValue::fromVariant(secret_number));
    recordObject.insert("rank"          , QJsonValue::fromVariant(rank));

    QJsonArray top_array;
    int rank_index = 0;
    for(auto it=top_players.begin();it!=top_players.end();it++)
    {
        rank_index++;
        QJsonObject o;
        o.insert("rank"         , QJsonValue::fromVariant(rank_index));
        o.insert("playername"   , QJsonValue::fromVariant(it->getPlayerName()));
        o.insert("total_time"   , QJsonValue::fromVariant(it->getTimeMs()));
        o.insert("total_tries"  , QJsonValue::fromVariant(it->getNbTries()));
        top_array.append(o);
    }
    if(top_array.size()>0)
    {
        recordObject.insert("top_players", top_array);
    }
    QJsonDocument doc(recordObject);

    return doc.toJson();
}
QString MsgFactory::createGameoverWinnerMsg(uint32_t gameid,
                                            uint32_t playerid,
                                            uint64_t start_time,
                                            uint64_t end_time,
                                            uint64_t total_time,
                                            int32_t  total_tries,
                                            int32_t  secret_number,
                                            uint64_t rank,
                                            std::vector<HistoryRecord>& top_players)
{
    QJsonObject recordObject;
    recordObject.insert("type"          , RC_GAMEOVER_WINNER);
    recordObject.insert("gameid"        , QJsonValue::fromVariant(gameid));
    recordObject.insert("playerid"      , QJsonValue::fromVariant(playerid));
    recordObject.insert("start_time"    , QJsonValue::fromVariant(start_time));
    recordObject.insert("end_time"      , QJsonValue::fromVariant(end_time));
    recordObject.insert("total_time"    , QJsonValue::fromVariant(total_time));
    recordObject.insert("total_tries"   , QJsonValue::fromVariant(total_tries));
    recordObject.insert("secret_number" , QJsonValue::fromVariant(secret_number));
    recordObject.insert("rank"          , QJsonValue::fromVariant(rank));

    QJsonArray top_array;
    int rank_index = 0;

    for(auto it=top_players.begin();it!=top_players.end();it++)
    {
        qDebug() << "top_players:"<<it->toJson();
        rank_index++;
        QJsonObject o;
        o.insert("rank", rank_index);
        o.insert("rank"         , QJsonValue::fromVariant(rank_index));
        o.insert("playername"   , QJsonValue::fromVariant(it->getPlayerName()));
        o.insert("total_time"   , QJsonValue::fromVariant(it->getTimeMs()));
        o.insert("total_tries"  , QJsonValue::fromVariant(it->getNbTries()));
        qDebug() << o;
        top_array.append(o);
    }
    if(top_array.size()>0)
    {
        recordObject.insert("top_players", top_array);
    }
    QJsonDocument doc(recordObject);

    return doc.toJson();

}


QString MsgFactory::createErrorMsg(uint16_t rc)
{
    QJsonObject recordObject;
    recordObject.insert("type"       , QJsonValue::fromVariant(rc));
    QJsonDocument doc(recordObject);

    return doc.toJson();

}

QString MsgFactory::createAcceptedConnectionMsg(uint32_t playerid, uint64_t logintime)
{
    QJsonObject recordObject;
    recordObject.insert("type"       , RC_ACCEPTED_CONNECTION);
    recordObject.insert("playerid"   , QJsonValue::fromVariant(playerid));
    recordObject.insert("logintime"  , QJsonValue::fromVariant(logintime));
    QJsonDocument doc(recordObject);

    return doc.toJson();

}
