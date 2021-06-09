#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QHostAddress>
#include <QMessageBox>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString ipstr = ui->inputIp->text();
    QHostAddress ip(ipstr);

    if(ip.isNull())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid ip!");
        messageBox.show();
        return;
    }

    if(client != nullptr)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Client already connected to the server!");
        messageBox.show();
        return;
    }

    QString urlstr = "ws://" + ipstr + ":" + ui->inputPort->text();
    QUrl url(urlstr);
    client = new GameClient(url);

    qInfo() << "Connecting to "<<urlstr;

    QObject::connect(client, &GameClient::closeApp, this, &MainWindow::close);
}

