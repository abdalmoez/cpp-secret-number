#include "GameClient.h"
#include "MsgFactory.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "mainwindow.h"
#include <QApplication>


GameClient::GameClient(const QUrl &url, MainWindow* parent)
    : m_url(url)
    , m_playerId(INVALID_PLAYER_ID)
    , m_currentGameId(INVALID_GAME_ID)
    , m_parent(parent)
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
                m_webSocket.sendTextMessage(MsgFactory::createNewGameRequest(m_playerId, m_parent->getPlayerName()));
                return;
            }
            break;
        }
        case RC_SERVER_IS_FULL:
        {
            QMessageBox messageBox;
            messageBox.information(m_parent,"Server","The server is full try again later.");
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
            messageBox.critical(m_parent,"Alert","You have been kicked from the server!");
            messageBox.show();
            m_webSocket.close();
            break;
        }
        case RC_BANNED:
        {
            QMessageBox messageBox;
            messageBox.critical(m_parent,"Alert","You have been banned from the server!");
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
            if(jsonObject.contains("max") && jsonObject["max"].isDouble()
               && jsonObject.contains("min") && jsonObject["min"].isDouble())
            {
                int32_t max = jsonObject["max"].toDouble();
                int32_t min = jsonObject["min"].toDouble();

                m_parent->showGamePanel(max, min);

                if(m_currentGameId != INVALID_GAME_ID && m_playerId != INVALID_PLAYER_ID)
                {
                    Bot* bot = m_parent->getBot();

                    if(bot != nullptr)
                    {
                        bot->setup(min, max);
                        bot->learn();
                        int32_t output = bot->getOutput();
                        m_parent->setAnswerInputValue(output);
                        m_webSocket.sendTextMessage(MsgFactory::createAnswerMsg(m_playerId, m_currentGameId, output));
                    }
                }
            }

            break;
        }
        case RC_INVALID_ANSWER:
        {
            QString state = "";
            if(!jsonObject.contains("answer") || !jsonObject["answer"].isDouble())
            {
                qWarning() << "Invalid msg format "<<jsonObject["type"];
                return;
            }

            if(!jsonObject.contains("state") || !jsonObject["state"].isDouble())
            {
                qWarning() << "Invalid msg format "<<jsonObject["type"];
                return;
            }
            int32_t answer_state = ((int32_t)jsonObject["state"].toDouble());

            if(answer_state == -1)
            {
                state = "\nThe secret number is less than your value.";
            }
            else if(answer_state == 1)
            {
                state = "\nThe secret number is high than your value.";
            }


            Bot* bot = m_parent->getBot();

            if(bot != nullptr)
            {
                bot->onFail(jsonObject["answer"].toDouble(), answer_state);
                bot->learn();
                int32_t output = bot->getOutput();
                m_parent->setAnswerInputValue(output);
                m_webSocket.sendTextMessage(MsgFactory::createAnswerMsg(m_playerId, m_currentGameId, output));
            }
            else
            {
                QMessageBox messageBox;

                if(jsonObject.contains("total_tries") && jsonObject["total_tries"].isDouble())
                {
                    messageBox.warning(m_parent,"Server","Invalid answer! (total tries: " + QString::number(jsonObject["total_tries"].toDouble()) +")"+state);
                }
                else
                {
                    messageBox.warning(m_parent,"Server","Invalid answer!"+state);
                }
                messageBox.show();
            }


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

            {
                QMessageBox messageBox;
                messageBox.critical(m_parent,"GameOver", content);
                messageBox.show();
            }
            m_currentGameId = INVALID_GAME_ID;
            m_parent->onEndGame();
            break;
        }
        case RC_GAMEOVER_WINNER:
        {
            QString content = "================== You Win ==================\n";
            if(jsonObject.contains("rank") && jsonObject["rank"].isDouble() && ((uint32_t)jsonObject["rank"].toDouble())>0)
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
            {
                QMessageBox messageBox;
                messageBox.information(m_parent,"GameOver", content);
                messageBox.show();
            }
            m_currentGameId = INVALID_GAME_ID;
            m_parent->onEndGame();
            break;
        }
        default:
        {
            qWarning() << "Unkown msg type "<<jsonObject["type"];
            return;
        }
    }



}
