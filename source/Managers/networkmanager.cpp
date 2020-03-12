/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include "source/Managers/networkmanager.h"

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
{
    server = new SimpleListenServer(AppSettings::Network::listenPort, this);

    connect(server, &SimpleListenServer::newReceivedData, this, &NetworkManager::newInputData);
    connect(server, &SimpleListenServer::newConnectionFrom, this, &NetworkManager::sendConnectionInfo);

    outputSocket = new QTcpSocket(this);
    connect(outputSocket, &QTcpSocket::stateChanged, this, &NetworkManager::outputConnectionStateChange);

    assert(!server->isListening());  // Minimal safety!
}

void NetworkManager::connectToHost()
{
    outputSocket->connectToHost(receiverIp, AppSettings::Network::listenPort, QIODevice::WriteOnly);
}

void NetworkManager::disconnectFromHost()
{
    outputSocket->disconnectFromHost();
}

void NetworkManager::setReceiverHostIp(const QString& ip)
{
    if (!isValidHostAddress(ip))
        return;

    receiverIp = ip;
}

QString NetworkManager::getReceiverIp()
{
    return receiverIp;
}

bool NetworkManager::hasConnection()
{
    return outputSocket->state() == QAbstractSocket::ConnectedState;
}

QString NetworkManager::getFirstLocalIpString()
{
    for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
            return address.toString();
        }
    }
    return "";
}

void NetworkManager::newInputData([[maybe_unused]] QHostAddress fromHost, QByteArray data)
{
    static QByteArray byteBuffer = "";
    byteBuffer.append(data);

    QJsonDocument doc = QJsonDocument::fromJson(byteBuffer);

    // Подаем на импорт только валидные json
    if (!doc.isNull()) {
        emit newSyncGroup(QString(byteBuffer));
        byteBuffer.clear();
    }
}

bool NetworkManager::sendGroup(const QJsonDocument& doc)
{
    if (!isValidHostAddress(receiverIp))
        return false;

    if (outputSocket->state() == QAbstractSocket::ConnectedState) {
        outputSocket->write(doc.toJson());
        outputSocket->flush();
        return true;
    }

    return false;
}

void NetworkManager::outputConnectionStateChange([[maybe_unused]] QAbstractSocket::SocketState state)
{
    emit newOutputConnectionState();
}

void NetworkManager::sendConnectionInfo(const QString& info)
{
    NotificationManager::showInfo("Подключение от: " + info);
}

QString NetworkManager::getOutputSocketState()
{
    if (outputSocket == nullptr)
        return "";

    return getSocketStateDescription(outputSocket->state());
}

bool NetworkManager::isValidHostAddress(const QString& ip)
{
    return !(QHostAddress(ip).isNull());
}

QString NetworkManager::getSocketStateDescription(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        return "Нет соединения";
    case QAbstractSocket::HostLookupState:
        return "Поиск хоста";
    case QAbstractSocket::ConnectingState:
        return "Установка соединения";
    case QAbstractSocket::ConnectedState:
        return "Соединение установлено";
    case QAbstractSocket::BoundState:
        return "";
    case QAbstractSocket::ListeningState:
        return "";
    case QAbstractSocket::ClosingState:
        return "Соединение закрывается";
    }

    return "";
}

bool NetworkManager::isServerEnabled() const
{
    return server->isListening();
}

void NetworkManager::setServerEnabled(bool enabled)
{
    if (server->isListening()) {
        if (!enabled) {
            server->stopListen();
            emit serverStateChanged();
        }
    } else {
        if (enabled) {
            if (server->startListen())
                emit serverStateChanged();
        }
    }
}

QString NetworkManager::serverState() const
{
    return isServerEnabled() ? "Ожидание соединения" : "Выключен";
}
