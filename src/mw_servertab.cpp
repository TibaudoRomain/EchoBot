#include "mw_servertab.h"
#include "ui_MainWindow.h"

ServerTab::ServerTab(Ui::MainWindow *arg_ui, Server* arg_server, QObject *arg_parent) {
    this->ui = arg_ui;
    this->server = arg_server;
    connect(server, &Server::serverState, this, &ServerTab::on_serverState);
    connect(server, &Server::newConnection, this, &ServerTab::on_newConnection);
    connect(this, &ServerTab::portchanged, server, &Server::on_portchanged);
}

void ServerTab::on_serverState(bool state){

}

void ServerTab::on_newConnection(int howmany){
    ui->CLientNumber->display(howmany);
    ui->textBrowser->append("connection count changed");
}
