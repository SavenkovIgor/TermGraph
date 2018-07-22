#include "listenserver.h"

ListenServer::ListenServer(quint16 listenPort, QObject *parent) : QObject(parent)
{
    this->listenPort = listenPort;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
}

bool ListenServer::startListen()
{
    return server->listen(QHostAddress::Any, listenPort);
}

void ListenServer::newConnection()
{
    if (server->hasPendingConnections()) {
        QTcpSocket* newSocket = server->nextPendingConnection();
        connect(newSocket, SIGNAL(readyRead()), SLOT(newInputData()));
        this->inputSockets.append(newSocket);
    }
}

void ListenServer::newInputData()
{
    QTcpSocket *inputSocket = dynamic_cast<QTcpSocket*>(sender());
    if (inputSocket == nullptr)
        return;
    QHostAddress senderAddress = inputSocket->peerAddress();
    QByteArray data = inputSocket->readAll();
    newReceivedData(senderAddress, data);
}
