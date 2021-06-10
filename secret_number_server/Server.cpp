#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "MsgFactory.h"

Server::Server(uint16_t port)
         :m_Port(port)
         ,m_WebSocketServer(QStringLiteral("secret_number_server"), QWebSocketServer::NonSecureMode, this)
{}

Server::~Server()
{
    stop();
}

void Server::start()
{
    if (m_WebSocketServer.listen(QHostAddress::Any, m_Port))
    {
        qInfo() << "[2021/04/09 13:29:22] Server started on port " << m_Port;

        connect(&m_WebSocketServer, &QWebSocketServer::newConnection, this, &Server::onPlayerConnect);

        connect(&m_WebSocketServer, &QWebSocketServer::closed, this, &Server::stop);
    }
    else
    {
        qFatal("[2021/04/09 13:29:22] Failed to started server");
        emit closeApp();
    }
}

void Server::stop()
{
    qInfo("[2021/04/09 13:29:22] Stoping the server");
    m_WebSocketServer.close();
    m_Clients.clear();
    emit closeApp();
}

void Server::onPlayerConnect()
{
    QWebSocket *socket = m_WebSocketServer.nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &Server::onProcessMsg);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &Server::onProcessBinaryMsg);
    connect(socket, &QWebSocket::disconnected, this, &Server::onPlayerDisconnect);

    QString ip = socket->peerAddress().toString();
    QString name = socket->peerName();

    if(name=="")
    {
        name="auto";
    }

    PlayerInfo* player_ptr = m_PlayerManager.addPlayer(ip, socket->peerPort());

    if(player_ptr == nullptr)
    {
        socket->sendTextMessage(MsgFactory::createErrorMsg(RC_SERVER_IS_FULL));
        socket->close();
        return;
    }

    m_Clients[socket] = *player_ptr;
    m_Clients[socket].setName(name);
    socket->sendTextMessage(MsgFactory::createAcceptedConnectionMsg(m_Clients[socket].getId(), m_Clients[socket].getLastLogin()));


    qInfo() << "[2021/04/09 13:29:22] [join] " << name << " has joined the server (" << m_Clients[socket].getId() << ":" << ip << ")";


}

void Server::onProcessMsg(QString msg)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());

    qDebug() << "Message received:" << msg;

    if (client)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(msg.toUtf8());

        QJsonObject jsonObject = jsonResponse.object();

        if(!jsonObject.contains("type"))
        {
            client->sendTextMessage(MsgFactory::createErrorMsg(RC_INVALID_REQUEST));
            return;
        }

        if(!jsonObject["type"].isDouble())
        {
            client->sendTextMessage(MsgFactory::createErrorMsg(RC_INVALID_REQUEST));
            return;
        }

        switch((int16_t)jsonObject["type"].toDouble())
        {
            case REQUEST_NEW_GAME:
            {
                GameInfo* game = m_GameManager.createGame(m_Clients[client].getId());
                if(game == nullptr)
                {
                    client->sendTextMessage(MsgFactory::createErrorMsg(RC_CANNOT_CREATE_GAME));
                }
                else
                {
                    client->sendTextMessage(MsgFactory::createGameCreatedMsg(game->getGameId(), game->getPlayerId(), game->getStartTime()));
                }
                break;
            }
            case REQUEST_ANSWER:
            {
                if(!jsonObject.contains("playerid") || !jsonObject["playerid"].isDouble())
                {
                    client->sendTextMessage(MsgFactory::createErrorMsg(RC_INVALID_REQUEST));
                    return;
                }
                if(!jsonObject.contains("gameid") || !jsonObject["gameid"].isDouble())
                {
                    client->sendTextMessage(MsgFactory::createErrorMsg(RC_INVALID_REQUEST));
                    return;
                }
                if(!jsonObject.contains("answer") || !jsonObject["answer"].isDouble())
                {
                    client->sendTextMessage(MsgFactory::createErrorMsg(RC_INVALID_REQUEST));
                    return;
                }
                uint32_t playerid = (uint32_t)jsonObject["playerid"].toDouble();
                uint32_t gameid = (uint32_t)jsonObject["gameid"].toDouble();
                int32_t answer = (int32_t)jsonObject["answer"].toDouble();

                if(gameid == INVALID_GAME_ID)
                {
                    client->sendTextMessage(MsgFactory::createErrorMsg(RC_INVALID_REQUEST));
                    return;
                }

                if(m_Clients[client].getId() != playerid)
                {
                    client->sendTextMessage(MsgFactory::createErrorMsg(RC_FORBIDDEN_CONTENT));
                    return;
                }
                GameInfo* game = m_GameManager.getPlayerGame(playerid);
                if(game == nullptr || game->getGameId() != gameid)
                {
                    client->sendTextMessage(MsgFactory::createErrorMsg(RC_FORBIDDEN_CONTENT));
                    return;
                }

                game->incNbTries();

                if(game->isValidAnswer(answer))
                {
                    game->endGame(true);
                    // TODO: Rank
                    client->sendTextMessage(MsgFactory::createGameoverWinnerMsg(
                                                gameid,
                                                playerid,
                                                game->getStartTime(),
                                                game->getEndTime(),
                                                game->getTotalMs(),
                                                game->getNbTries(),
                                                game->getSecretNumber(),
                                                1));
                    m_GameManager.removeGame(gameid);

                }
                else if(game->getNbTries() < MAX_TRIES)
                {
                    if(game->getSecretNumber() < answer)
                    {
                        client->sendTextMessage(MsgFactory::createInvalidAnswerMsg(gameid, playerid, game->getNbTries(), -1));
                    }
                    else
                    {
                        client->sendTextMessage(MsgFactory::createInvalidAnswerMsg(gameid, playerid, game->getNbTries(), 1));
                    }

                }
                else
                {
                    // TODO: Rank
                    game->endGame(false);
                    client->sendTextMessage(MsgFactory::createGameoverLoserMsg(
                                                gameid,
                                                playerid,
                                                game->getStartTime(),
                                                game->getEndTime(),
                                                game->getTotalMs(),
                                                game->getNbTries(),
                                                game->getSecretNumber(),
                                                1));
                    m_GameManager.removeGame(gameid);
                }

                break;
            }
            case REQUEST_END_GAME:
            {
                //TODO: send end game resonse
                break;
            }
            case REQUEST_QUIT:
            {
                //TODO: create end game on player close app
                break;
            }
            default:
            {
                client->sendTextMessage(MsgFactory::createErrorMsg(RC_INVALID_REQUEST));
                return;
            }
        }


    }
}

void Server::onProcessBinaryMsg(QByteArray msg)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());

    qDebug() << "Binary Message received:" << msg;

    if (client)
    {
        client->sendBinaryMessage(msg);
    }

}

void Server::onPlayerDisconnect()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());

    if (client)
    {
        QString name = m_Clients[client].getName();
        uint32_t id  = m_Clients[client].getId();
        qInfo() << "[2021/04/09 13:35:13] [part] "<< name <<" has left the server (" << id << ":" << client->closeCode() <<")";

        m_PlayerManager.removePlayer(m_Clients[client].getId());
        m_Clients.erase(client);
        GameInfo * game = m_GameManager.getPlayerGame(m_Clients[client].getId());
        if(game)
        {
            m_GameManager.removeGame(game->getGameId());
        }
        client->deleteLater();
    }
}
