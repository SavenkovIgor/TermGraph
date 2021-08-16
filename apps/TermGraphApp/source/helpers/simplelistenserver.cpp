/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

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
