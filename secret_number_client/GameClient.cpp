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
            QString state = "";
            if(jsonObject.contains("state") && jsonObject["state"].isDouble())
            {
                if(((int32_t)jsonObject["state"].toDouble()) == -1)
                {
                    state = "\nThe secret number is less than your value.";
                }
                else if(((int32_t)jsonObject["state"].toDouble()) == 1)
                {
                    state = "\nThe secret number is high than your value.";
                }
            }

            if(jsonObject.contains("total_tries") && jsonObject["total_tries"].isDouble())
            {
                messageBox.information(0,"Server","Invalid answer! (total tries: " + QString::number(jsonObject["total_tries"].toDouble()) +")"+state);
            }
            else
            {
                messageBox.information(0,"Server","Invalid answer!"+state);
            }
            messageBox.show();


            break;
        }
        case RC_GAMEOVER_LOSER:
        {
            QString content = "================== You Lose ==================\n";
            if(jsonObject.contains("secret_number") && jsonObject["secret_number"].isDouble())
            {
                content += "\n Secret number: " + QString::number((int32_t)jsonObject["secret_number"].toDouble());
            }
            if(jsonObject.contains("total_time") && jsonObject["total_time"].isDouble())
            {
                uint64_t total_ms = (uint64_t)jsonObject["total_time"].toDouble();
                content += "\n Time: " + QString::number(total_ms/1000)+"."+QString::number(total_ms%1000)+" ms";
            }
            if(jsonObject.contains("total_tries") && jsonObject["total_tries"].isDouble())
            {
                content += "\n Total tries: " + QString::number((uint32_t)jsonObject["total_tries"].toDouble());
            }

            QMessageBox messageBox;
            messageBox.critical(0,"GameOver", content);
            messageBox.show();
            m_currentGameId = INVALID_GAME_ID;
            break;
        }
        case RC_GAMEOVER_WINNER:
        {
            QString content = "================== You Win ==================\n";
            if(jsonObject.contains("rank") && jsonObject["rank"].isDouble())
            {
                content += "\n Your rank: #" + QString::number((uint32_t)jsonObject["rank"].toDouble());
            }
            if(jsonObject.contains("secret_number") && jsonObject["secret_number"].isDouble())
            {
                content += "\n Secret number: " + QString::number((int32_t)jsonObject["secret_number"].toDouble());
            }
            if(jsonObject.contains("total_time") && jsonObject["total_time"].isDouble())
            {
                uint64_t total_ms = (uint64_t)jsonObject["total_time"].toDouble();
                content += "\n Time: " + QString::number(total_ms/1000)+"."+QString::number(total_ms%1000)+" ms";
            }
            if(jsonObject.contains("total_tries") && jsonObject["total_tries"].isDouble())
            {
                content += "\n Total tries: " + QString::number((uint32_t)jsonObject["total_tries"].toDouble());
            }
            QMessageBox messageBox;
            messageBox.information(0,"GameOver", content);
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
