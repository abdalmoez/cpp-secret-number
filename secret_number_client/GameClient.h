#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

class GameClient: public QObject
{
    Q_OBJECT
public:
    GameClient(const QUrl &url);

Q_SIGNALS:
    void closeApp();

private Q_SLOTS:
    void onConnected();
    void onMsgReceived(QString msg);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
};

#endif // GAMECLIENT_H
