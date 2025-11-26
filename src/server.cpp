#include "server.h"

Server::Server(QObject* parent) {
    TCPServ = new QTcpServer(this);
    TCPServ->listen(QHostAddress::Any, 6666);
    connect(TCPServ, &QTcpServer::newConnection, this, &Server::new_client);
    SecurityKey.append(SK);
}


void Server::client_disconnected(){
    QTcpSocket *disconnected_clt= static_cast<QTcpSocket*>(sender());
    clients.removeOne(disconnected_clt);
    disconnected_clt->disconnect();
    disconnected_clt->deleteLater();
    emit newConnection(clients.length());
}

void Server::read_data(){
    QTcpSocket* data_sender = reinterpret_cast<QTcpSocket*>(sender());
    msgBuffer.append(data_sender->readAll());
    if(msgBuffer.length()>sizeof(message)){
        while(msgBuffer.first(8)!=SecurityKey){
            msgBuffer.removeFirst();
        }
        messageData = (struct message*)msgBuffer.data();
        msgBuffer.slice(sizeof(message));
    }
}

void Server::processMessage(QByteArray message){

}

void Server::new_client(){
    while (TCPServ->hasPendingConnections()){
        clients.append(TCPServ->nextPendingConnection());
    }
    emit newConnection(clients.length());
}

void Server::on_results(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E){
    message tosend;
    tosend.securityKey = SK;
    tosend.angleA = A;
    tosend.angleB = B;
    tosend.angleC = C;
    tosend.angleD = D;
    tosend.angleE = E;
    QByteArray message(reinterpret_cast<const char*>(&tosend), sizeof(message));
    for(QTcpSocket* client : std::as_const(clients)){
        qint64 bytes_sent = client->write(message);
        if (bytes_sent == -1) {
        qDebug() << "Erreur d'envoi à un client.";
        }
    }
}

void Server::on_msg_clicked(){
    message tosend;
    tosend.securityKey = SK;
    tosend.angleA = 1;
    tosend.angleB = 22;
    tosend.angleC = 99;
    tosend.angleD = 4;
    tosend.angleE = 5;
    QByteArray message(reinterpret_cast<const char*>(&tosend), sizeof(message));
    for(QTcpSocket* client : std::as_const(clients)){
        qint64 bytes_sent = client->write(message);
        if (bytes_sent == -1) {
            qDebug() << "Erreur d'envoi à un client.";
        }
    }
}

void Server::on_portchanged(int port){

}

