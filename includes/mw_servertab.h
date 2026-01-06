#ifndef MW_SERVERTAB_H
#define MW_SERVERTAB_H

#include <QObject>
#include "server.h"

namespace Ui {
class MainWindow;
}

class ServerTab : public QObject
{
    Q_OBJECT
public:
    explicit ServerTab(Ui::MainWindow *arg_ui, Server* arg_server, QObject *arg_parent = nullptr);

signals:
    void portchanged(int port);
private slots:
    void on_serverState(bool state);
    void on_newConnection(int howmany);

private:
    Ui::MainWindow* ui;
    Server* server;
};

#endif // MW_SERVERTAB_H
