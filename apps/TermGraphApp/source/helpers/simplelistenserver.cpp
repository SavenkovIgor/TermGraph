// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/simplelistenserver.h"

SimpleListenServer::SimpleListenServer(quint16 listenPort, QObject* parent)
    : QObject(parent)
{
    this->listenPort = listenPort;
    server           = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &SimpleListenServer::newConnection);
}

bool SimpleListenServer::isListening() { return server->isListening(); }

bool SimpleListenServer::startListen() { return server->listen(QHostAddress::Any, listenPort); }

void SimpleListenServer::stopListen() { server->close(); }

void SimpleListenServer::newConnection()
{
    if (server->hasPendingConnections()) {
        QTcpSocket* newSocket = server->nextPendingConnection();
        connect(newSocket, &QTcpSocket::readyRead, this, &SimpleListenServer::newInputData);
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
    QByteArray   data          = inputSocket->readAll();
    emit         newReceivedData(senderAddress, data);
}

QString SimpleListenServer::getSocketDescription(QTcpSocket* socket)
{
    if (socket != nullptr) {
        return socket->peerAddress().toString() + " " + socket->peerName();
    }
    return "";
}
