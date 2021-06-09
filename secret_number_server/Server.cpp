#include "Server.h"

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

        connect(&m_WebSocketServer, &QWebSocketServer::closed, this, &Server::closeApp);
    }
    else
    {
        qFatal("[2021/04/09 13:29:22] Failed to started server");
        emit closeApp();
    }
}

void Server::stop()
{
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

    m_Clients[socket] = *m_PlayerManager.addPlayer(ip, socket->peerPort());

    m_Clients[socket].setName(name);


    qInfo() << "[2021/04/09 13:29:22] [join] " << name << " has joined the server (" << m_Clients[socket].getId() << ":" << ip << ")";

}

void Server::onProcessMsg(QString msg)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());

    qDebug() << "Message received:" << msg;

    if (client)
    {
        client->sendTextMessage(msg);
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
        client->deleteLater();
    }
}
