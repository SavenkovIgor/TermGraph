#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    server = new QTcpServer();
    inputSocket = nullptr;
    outputSocket = nullptr;

    startListen();
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
}

bool NetworkManager::connectAndSendGroup(QJsonDocument doc)
{
    QHostAddress hAddress(receiverIp);
    if(hAddress.isNull())
        return false;

    jsonSendBuffer = doc;

    if(outputSocket != nullptr) {
        delete outputSocket;
    }

    outputSocket = new QTcpSocket();
    connect(outputSocket, SIGNAL(connected()), SLOT(sendGroup()));
    outputSocket->connectToHost(hAddress, NetworkSettings::listenPort, QIODevice::WriteOnly);
    return true;
}

bool NetworkManager::startListen()
{
    return server->listen(QHostAddress::Any, NetworkSettings::listenPort);
}

void NetworkManager::setReceiverHostIp(QString ip)
{
    receiverIp = ip;
}

QString NetworkManager::getFirstLocalIpString()
{
    for(const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
            return address.toString();
        }
    }
    return "";
}

void NetworkManager::newConnection()
{
    if(server->hasPendingConnections()) {
        if(inputSocket != nullptr) {
            inputSocket->close();
        }
        inputSocket = server->nextPendingConnection();
        connect(inputSocket,SIGNAL(readyRead()),SLOT(newInputData()));
    }
}

void NetworkManager::newInputData()
{
    QString jsonString(inputSocket->readAll());
    newSyncGroup(jsonString);
}

void NetworkManager::sendGroup()
{
    if(outputSocket == nullptr) {
        return;
    }

    outputSocket->write(jsonSendBuffer.toJson());
    outputSocket->flush();
    groupSended();
}
