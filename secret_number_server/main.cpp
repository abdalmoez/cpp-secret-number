#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

#include <iostream>

#include "PlayerManager.h"
#include "Server.h"

void test1()
{
    PlayerManager pm;
    auto p0 = pm.addPlayer("132.15.158.12", 2352);
    auto p1 = pm.addPlayer("132.15.158.13", 2352);
    auto p2 = pm.addPlayer("132.15.158.14", 2352);
    std::cout<<*p0<<std::endl;
    std::cout<<*p1<<std::endl;
    std::cout<<*p2<<std::endl;
    pm.removePlayer(1);
    p1 = pm.addPlayer("132.15.158.15", 2352);
    std::cout<<*p1<<std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Secret number game server");
    parser.addHelpOption();

    QCommandLineOption limitOption(QStringList() << "l" << "limit",
             QCoreApplication::translate("main", "Max numbers of tries (-1 for unlimited)"),
             QCoreApplication::translate("main", "limit"), QLatin1String("-1"));
    parser.addOption(limitOption);
    QCommandLineOption boundsOption(QStringList() << "b" << "bounds",
             QCoreApplication::translate("main", "Bounds of the secret number x,y"),
             QCoreApplication::translate("main", "bounds"), QLatin1String("1,100"));
    parser.addOption(boundsOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "The port the server should use. You will need to Port Forward in order for players to join your server from outside your LAN. (default: 4242)"),
            QCoreApplication::translate("main", "port"), QLatin1String("4242"));
    parser.addOption(portOption);
    parser.process(a);
    int limit = parser.value(limitOption).toInt();
    int port  = parser.value(portOption).toInt();
    auto bounds_param = parser.value(boundsOption).split(",");

    if(bounds_param.size()!=2)
    {
        qFatal("Invalid bounds argument");
        return 1;
    }
    if(limit<0 && limit != -1)
    {
        qFatal("Invalid limit argument");
        return 1;
    }
    if(port <= 0)
    {
        qFatal("Invalid port argument");
        return 1;
    }

    GameInfo::s_minValue = bounds_param[0].toInt();
    GameInfo::s_maxValue = bounds_param[1].toInt();
    GameManager::s_maxTries = limit;

    Server server(port);
    QObject::connect(&server, &Server::closeApp, &a, &QCoreApplication::quit);
    server.start();
    return a.exec();
}
