#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
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

void MainWindow::showGamePanel(int32_t max_value, int32_t min_value)
{
    ui->answerInput->setMaximum(max_value);
    ui->answerInput->setMinimum(min_value);
    ui->answerInput->setValue(min_value);
    ui->answerLabel->setText("Enter the secret value. It's between "+QString::number(min_value) +" and "+ QString::number(max_value)+".");
    ui->loginFrame->setVisible(false);
    ui->answerFrame->setVisible(true);
}

void MainWindow::showLoginPanel()
{
    ui->loginFrame->setVisible(true);
    ui->answerFrame->setVisible(false);

}

void MainWindow::lookedUp(const QHostInfo &host)
{
    if (host.error() != QHostInfo::NoError)
    {

        QMessageBox messageBox;
        messageBox.critical(0,"Error","Cannot reach host!\n Reason:"+host.errorString());
        messageBox.show();
        return;
    }

    QString urlstr = "ws://" + host.hostName() + ":" + ui->inputPort->text();
    QUrl url(urlstr);
    client = new GameClient(url, this);

    qInfo() << "Connecting to "<<urlstr;

    QObject::connect(client, &GameClient::closeApp, this, &MainWindow::close);
}

void MainWindow::on_pushButton_clicked()
{
    if(client != nullptr)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Client already connected to the server!");
        messageBox.show();
        return;
    }

    QHostInfo::lookupHost(ui->inputHost->text(), this, SLOT(lookedUp(QHostInfo)));
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

