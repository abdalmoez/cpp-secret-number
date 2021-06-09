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
    parser.setApplicationDescription("QtWebSockets example: echoserver");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for echoserver [default: 1234]."),
            QCoreApplication::translate("main", "port"), QLatin1String("1234"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    Server server(port);
    QObject::connect(&server, &Server::closeApp, &a, &QCoreApplication::quit);
    server.start();
    return a.exec();
}
