#include "MsgFactory.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

QString MsgFactory::createNewGameRequest(uint32_t playerid)
{
    QJsonObject recordObject;
    recordObject.insert("type"      , REQUEST_NEW_GAME);
    recordObject.insert("playerid"  , QJsonValue::fromVariant(playerid));
    QJsonDocument doc(recordObject);

    return doc.toJson();
}

QString MsgFactory::createAnswerMsg(uint32_t playerid, uint32_t gameid, int32_t answer)
{
    QJsonObject recordObject;
    recordObject.insert("type"      , REQUEST_ANSWER);
    recordObject.insert("playerid"  , QJsonValue::fromVariant(playerid));
    recordObject.insert("gameid"    , QJsonValue::fromVariant(gameid));
    recordObject.insert("answer"    , QJsonValue::fromVariant(answer));
    QJsonDocument doc(recordObject);

    return doc.toJson();
}

QString MsgFactory::createTopScoresRequest(uint32_t playerid)
{
    QJsonObject recordObject;
    recordObject.insert("type"      , REQUEST_TOP_SCORES);
    recordObject.insert("playerid"  , QJsonValue::fromVariant(playerid));
    QJsonDocument doc(recordObject);

    return doc.toJson();

}
