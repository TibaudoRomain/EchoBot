#include "mw_servertab.h"
#include "ui_MainWindow.h"

ServerTab::ServerTab(Ui::MainWindow *ui, Server* server, QObject *parent) {
    connect(server, &Server::serverState, this, &ServerTab::on_serverState);
    connect(server, &Server::newConnection, this, &ServerTab::on_newConnection);
    connect(this, &ServerTab::portchanged, server, &Server::on_portchanged);
    this->ui = ui;
    this->server = server;

}

void ServerTab::on_serverState(bool state){

}

void ServerTab::on_newConnection(int howmany){
    ui->CLientNumber->display(howmany);
    ui->textBrowser->append("connection count changed");
}
