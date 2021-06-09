#include "GameClient.h"
#include "MsgFactory.h"
#include <QJsonDocument>
#include <QJsonObject>

GameClient::GameClient(const QUrl &url)
    : m_url(url)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &GameClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &GameClient::closeApp);
    m_webSocket.open(QUrl(url));
}

void GameClient::onConnected()
{
    qInfo() << "Connection established with the server";

    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &GameClient::onMsgReceived);
}

void GameClient::onMsgReceived(QString msg)
{
    qDebug() << "Message received:" << msg;

    QJsonDocument jsonResponse = QJsonDocument::fromJson(msg.toUtf8());

    QJsonObject jsonObject = jsonResponse.object();

    if(!jsonObject.contains("type"))
    {
        qWarning() << "Msg doesn't not contain type field";
        return;
    }

    if(!jsonObject["type"].isDouble())
    {
        qWarning() << "Invalid received msg type "<<jsonObject["type"];
        return;
    }

    switch((int16_t)jsonObject["type"].toDouble())
    {
        case RC_ACCEPTED_CONNECTION:
        {
            if(jsonObject.contains("playerid") && jsonObject["playerid"].isDouble())
            {
                qInfo() << "Accepted Connection with the server";
                m_webSocket.sendTextMessage(MsgFactory::createNewGameRequest((uint32_t)jsonObject["playerid"].toDouble()));
                return;
            }
            break;
        }
        case RC_SERVER_IS_FULL:
        {
            m_webSocket.close();
            break;
        }
        case RC_INVALID_REQUEST:
        {

            break;
        }
        case RC_KICKED:
        {
            m_webSocket.close();
            break;
        }
        case RC_BANNED:
        {
            m_webSocket.close();
            break;
        }
        case RC_TOP_PLAYERS:
        {

            break;
        }
        case RC_GAME_CREATED:
        {
            //TODO:
            m_webSocket.close();
            break;
        }
        case RC_INVALID_ANSWER:
        {

            break;
        }
        case RC_GAMEOVER_LOSER:
        {

            break;
        }
        case RC_GAMEOVER_WINNER:
        {
            break;
        }
        default:
        {
            qWarning() << "Unkown msg type "<<jsonObject["type"];
            return;
        }
    }



}
