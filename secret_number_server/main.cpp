#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

#include <iostream>
#include <QFile>

#include "PlayerManager.h"
#include "Server.h"

bool verbose=false;
void fileLog(QString str)
{
    QFile f(qApp->applicationDirPath()+"/server.log");

    if (f.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&f);
        out << str;
    }
    else
    {
        fprintf(stderr, "Cannot open log file");
    }
}
void serverLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString time = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString content="";
    QByteArray localMsg = msg.toLocal8Bit();

    switch (type)
    {
        case QtDebugMsg:
            content = QString("[%1] [Debug] %2\n").arg(time, msg);
            if(verbose)
            {
                fprintf(stdout, content.toUtf8().data());
                fflush(stdout);
                fileLog(content);
            }
            break;
        case QtInfoMsg:
            content = QString("[%1] [Info] %2\n").arg(time, msg);
            if(verbose)
            {
                fprintf(stdout, content.toUtf8().data());
                fflush(stdout);
            }
            fileLog(content);
            break;
        case QtWarningMsg:
            content = QString("[%1] [Warning] %2\n").arg(time, msg);
            fileLog(content);
            fprintf(stderr, content.toUtf8().data());
            fflush(stderr);
            break;
        case QtCriticalMsg:
            content = QString("[%1] [Critical] %2\n").arg(time, msg);
            fileLog(content);
            fprintf(stderr, content.toUtf8().data());
            fflush(stderr);
            break;
        case QtFatalMsg:
            content = QString("[%1] [Fatal] %2\n").arg(time, msg);
            fileLog(content);
            fprintf(stderr, content.toUtf8().data());
            fflush(stderr);
            abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(serverLog);
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
    QCommandLineOption verboseOption(QStringList() << "v" << "verbose",
             QCoreApplication::translate("main", "Verbose mode. Prints out more information."));
    parser.addOption(verboseOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "The port the server should use. You will need to Port Forward in order for players to join your server from outside your LAN. (default: 4242)"),
            QCoreApplication::translate("main", "port"), QLatin1String("4242"));
    parser.addOption(portOption);
    parser.process(a);
    int limit = parser.value(limitOption).toInt();
    int port  = parser.value(portOption).toInt();
    auto bounds_param = parser.value(boundsOption).split(",");

    verbose = parser.isSet(verboseOption);

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
