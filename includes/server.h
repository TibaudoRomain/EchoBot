#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

#include "msg_Structure.h"



class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject* parent);

private slots:
    void client_disconnected();
    void read_data();
    void new_client();

public slots:
    void on_msg_clicked();

    void on_portchanged(int port);
    void on_results(uint8_t angleA, uint8_t angleB, uint8_t angleC, uint8_t angleD, uint8_t angleE);


signals:
    void message_received(message msg);
    void newConnection(int howmany);
    void serverState(bool state);


private:
    quint32 SK = 1856942345;
    QByteArray SecurityKey;

    QByteArray msgBuffer;
    message* messageData;


    void processMessage(QByteArray message);

    QTcpServer* TCPServ;
    QList<QTcpSocket*> clients;
    qint8 m_port;
};

#endif // SERVER_H
