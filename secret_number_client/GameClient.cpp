#include "GameClient.h"

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
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void GameClient::onMsgReceived(QString msg)
{
    qDebug() << "Message received:" << msg;
    m_webSocket.close();
}
