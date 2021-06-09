#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QDebug>

#include "PlayerManager.h"
#include "GameManager.h"


class Server: public QObject
{
    Q_OBJECT

public:
    Server(uint16_t port);
    ~Server();

    void start();
    void stop();

Q_SIGNALS:
    void closeApp();

private Q_SLOTS:
    void onPlayerConnect();
    void onProcessMsg(QString msg);
    void onProcessBinaryMsg(QByteArray message);
    void onPlayerDisconnect();

private:
    uint16_t           m_Port;
    QWebSocketServer   m_WebSocketServer;
    std::map<QWebSocket *, PlayerInfo> m_Clients;
    PlayerManager                      m_PlayerManager;
    GameManager                        m_GameManager;
};

#endif // SERVER_H
