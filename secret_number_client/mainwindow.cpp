#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QHostAddress>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    //TODO: Connect to server
}

