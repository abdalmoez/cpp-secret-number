#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

const uint32_t INVALID_PLAYER_ID = 0xFFFFFFFF;
const uint32_t INVALID_GAME_ID   = 0xFFFFFFFF;

class GameClient: public QObject
{
    Q_OBJECT
public:
    GameClient(const QUrl &url);
    bool sendMsg(QString msg);

    uint32_t getPlayerId()
    {
        return m_playerId;
    }
    uint32_t getGameId()
    {
        return m_currentGameId;
    }
Q_SIGNALS:
    void closeApp();

private Q_SLOTS:
    void onConnected();
    void onMsgReceived(QString msg);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    uint32_t m_playerId;
    uint32_t m_currentGameId;
};

#endif // GAMECLIENT_H
