#include "simplelistenserver.h"

SimpleListenServer::SimpleListenServer(quint16 listenPort, QObject *parent) : QObject(parent)
{
    this->listenPort = listenPort;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
}

bool SimpleListenServer::startListen()
{
    return server->listen(QHostAddress::Any, listenPort);
}

void SimpleListenServer::newConnection()
{
    if (server->hasPendingConnections()) {
        QTcpSocket* newSocket = server->nextPendingConnection();
        connect(newSocket, SIGNAL(readyRead()), SLOT(newInputData()));
        this->inputSockets.append(newSocket);
        emit newConnectionFrom(getSocketDescription(newSocket));
    }
}

void SimpleListenServer::newInputData()
{
    auto* inputSocket = dynamic_cast<QTcpSocket*>(sender());
    if (inputSocket == nullptr)
        return;
    QHostAddress senderAddress = inputSocket->peerAddress();
    QByteArray data = inputSocket->readAll();
    emit newReceivedData(senderAddress, data);
}

QString SimpleListenServer::getSocketDescription(QTcpSocket *socket)
{
    if (socket != nullptr) {
        return socket->peerAddress().toString() + " " + socket->peerName();
    }
    return "";
}
