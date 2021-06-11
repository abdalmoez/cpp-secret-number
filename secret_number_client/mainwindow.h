#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostInfo>

#include "GameClient.h"
#include "BotAlex.h"
#include "BotSimone.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showGamePanel(int32_t max_value, int32_t min_value);
    void showLoginPanel();
    QString getPlayerName();
    Bot* getBot();
    void setAnswerInputValue(int32_t value);

private slots:
    void on_pushButton_clicked();

    void on_sendAnswerBtn_clicked();
    void lookedUp(const QHostInfo &host);

    void on_botState_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    GameClient * client;
    BotAlex     m_BotAlex;
    BotSimone   m_BotSimone;

};
#endif // MAINWINDOW_H
