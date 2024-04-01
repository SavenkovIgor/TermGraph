// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/managers/networkmanager.h"

#include "source/commonTools/NetworkTools.h"

#ifdef Q_OS_WASM
#include <emscripten/val.h>
#endif

NetworkManager::NetworkManager(NotifyInterface& notifier, QObject* parent)
    : QObject(parent)
    , notifier(notifier)
{
    server = new SimpleListenServer(NetworkTools::localSyncPort, this);

    connect(server, &SimpleListenServer::newReceivedData, this, &NetworkManager::newInputData);
    connect(server, &SimpleListenServer::newConnectionFrom, this, &NetworkManager::sendConnectionInfo);

    outputSocket = new QTcpSocket(this);
    connect(outputSocket, &QTcpSocket::stateChanged, this, &NetworkManager::outputConnectionStateChange);

    assert(!server->isListening()); // Minimal safety!
}

void NetworkManager::connectToHost()
{
    outputSocket->connectToHost(receiverIp, NetworkTools::localSyncPort, QIODevice::WriteOnly);
}

void NetworkManager::disconnectFromHost() { outputSocket->disconnectFromHost(); }

void NetworkManager::setReceiverHostIp(const QString& ip)
{
    if (!isValidHostAddress(ip)) {
        return;
    }

    receiverIp = ip;
}

QString NetworkManager::getReceiverIp() { return receiverIp; }

bool NetworkManager::hasConnection() { return outputSocket->state() == QAbstractSocket::ConnectedState; }

QString NetworkManager::getFirstLocalIpString()
{
#ifndef Q_OS_WASM
    for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
            return address.toString();
        }
    }
#endif
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
    if (!isValidHostAddress(receiverIp)) {
        return false;
    }

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

void NetworkManager::sendConnectionInfo(const QString& info) { notifier.showInfo("Подключение от: " + info); }

QString NetworkManager::getOutputSocketState()
{
    if (outputSocket == nullptr) {
        return "";
    }

    return getSocketStateDescription(outputSocket->state());
}

bool NetworkManager::isValidHostAddress(const QString& ip) { return !(QHostAddress(ip).isNull()); }

QString NetworkManager::getSocketStateDescription(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState: return "Нет соединения";
    case QAbstractSocket::HostLookupState: return "Поиск хоста";
    case QAbstractSocket::ConnectingState: return "Установка соединения";
    case QAbstractSocket::ConnectedState: return "Соединение установлено";
    case QAbstractSocket::BoundState: return "";
    case QAbstractSocket::ListeningState: return "";
    case QAbstractSocket::ClosingState: return "Соединение закрывается";
    }

    return "";
}

bool NetworkManager::isServerEnabled() const { return server->isListening(); }

void NetworkManager::setServerEnabled(bool enabled)
{
    if (server->isListening()) {
        if (!enabled) {
            server->stopListen();
            emit serverStateChanged();
        }
    } else {
        if (enabled) {
            if (server->startListen()) {
                emit serverStateChanged();
            }
        }
    }
}

QString NetworkManager::serverState() const { return isServerEnabled() ? "Ожидание соединения" : "Выключен"; }

QUrl NetworkManager::currentUrl() const
{
#ifdef Q_OS_WASM
    auto location  = emscripten::val::global("location");
    auto urlString = QString::fromStdString(location["href"].as<std::string>());
    return QUrl(urlString);
#endif

    return QUrl();
}
