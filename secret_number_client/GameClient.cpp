#include "GameClient.h"
#include "MsgFactory.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

GameClient::GameClient(const QUrl &url)
    : m_url(url)
    , m_playerId(INVALID_PLAYER_ID)
    , m_currentGameId(INVALID_GAME_ID)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &GameClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &GameClient::closeApp);
    m_webSocket.open(QUrl(url));
}

bool GameClient::sendMsg(QString msg)
{
    if(!m_webSocket.isValid())
    {
        return false;
    }

    return (m_webSocket.sendTextMessage(msg) > 0);
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
                m_playerId = (uint32_t)jsonObject["playerid"].toDouble();
                m_webSocket.sendTextMessage(MsgFactory::createNewGameRequest(m_playerId));
                return;
            }
            break;
        }
        case RC_SERVER_IS_FULL:
        {
            QMessageBox messageBox;
            messageBox.information(0,"Server","The server is full try again later.");
            messageBox.show();
            m_webSocket.close();
            break;
        }
        case RC_INVALID_REQUEST:
        {

            break;
        }
        case RC_KICKED:
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Alert","You have been kicked from the server!");
            messageBox.show();
            m_webSocket.close();
            break;
        }
        case RC_BANNED:
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Alert","You have been banned from the server!");
            messageBox.show();
            m_webSocket.close();
            break;
        }
        case RC_TOP_PLAYERS:
        {

            break;
        }
        case RC_GAME_CREATED:
        {
            if(jsonObject.contains("gameid") && jsonObject["gameid"].isDouble())
            {
                m_currentGameId = (uint32_t)jsonObject["gameid"].toDouble();
            }
            else
            {
                m_currentGameId = INVALID_GAME_ID;
            }
            break;
        }
        case RC_INVALID_ANSWER:
        {
            QMessageBox messageBox;
            if(jsonObject.contains("total_tries") && jsonObject["total_tries"].isDouble())
            {
                messageBox.information(0,"Server","Invalid answer! (total tries: " + QString::number(jsonObject["total_tries"].toDouble()) +")");
            }
            else
            {
                messageBox.information(0,"Server","Invalid answer!");
            }
            messageBox.show();


            break;
        }
        case RC_GAMEOVER_LOSER:
        {
            QMessageBox messageBox;
            messageBox.critical(0,"GameOver","You lose!");
            messageBox.show();
            m_currentGameId = INVALID_GAME_ID;
            break;
        }
        case RC_GAMEOVER_WINNER:
        {
            QMessageBox messageBox;
            messageBox.information(0,"GameOver","You Win!");
            messageBox.show();
            m_currentGameId = INVALID_GAME_ID;
            break;
        }
        default:
        {
            qWarning() << "Unkown msg type "<<jsonObject["type"];
            return;
        }
    }



}
