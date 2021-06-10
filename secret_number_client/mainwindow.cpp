#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QHostAddress>
#include <QMessageBox>
#include <sstream>
#include "MsgFactory.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(nullptr)
{
    ui->setupUi(this);
    ui->answerFrame->setVisible(false);
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

    ui->loginFrame->setVisible(false);
    ui->answerFrame->setVisible(true);
}


void MainWindow::on_sendAnswerBtn_clicked()
{
    if(client == nullptr)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You are not connected to any server!");
        messageBox.show();
        return;
    }

    if(client->getPlayerId() != INVALID_PLAYER_ID)
    {
        client->sendMsg(MsgFactory::createAnswerMsg(client->getPlayerId(), client->getGameId(), ui->answerInput->value()));
    }

}

