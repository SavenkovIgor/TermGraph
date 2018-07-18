#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    server = new QTcpServer();
    inputSocket = nullptr;
    outputSocket = new QTcpSocket();
    connect(
                outputSocket,
                SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                SLOT(outputConnectionStateChange(QAbstractSocket::SocketState)));

    startListen();
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
}

bool NetworkManager::startListen()
{
    return server->listen(QHostAddress::Any, NetworkSettings::listenPort);
}

bool NetworkManager::connectToHost()
{
    outputSocket->connectToHost(receiverIp, NetworkSettings::listenPort, QIODevice::WriteOnly);
}

void NetworkManager::disconnectFromHost()
{
    outputSocket->disconnectFromHost();
}

void NetworkManager::setReceiverHostIp(QString ip)
{
    if(!isValidHostAddress(ip))
        return;
    receiverIp = ip;
}

QString NetworkManager::getReceiverIp()
{
    return receiverIp;
}

bool NetworkManager::isConnected()
{
    return outputSocket->state() == QAbstractSocket::ConnectedState;
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
    static QByteArray byteBuffer = "";

    byteBuffer.append(inputSocket->readAll());

    QJsonDocument doc = QJsonDocument::fromJson(byteBuffer);

    //Подаем на импорт только валидные json
    if(!doc.isNull()) {
        newSyncGroup(QString(byteBuffer));
        byteBuffer.clear();
    }
}

bool NetworkManager::sendGroup(QJsonDocument doc)
{
    if(!isValidHostAddress(receiverIp))
        return false;

    if(outputSocket->state() == QAbstractSocket::ConnectedState) {
        outputSocket->write(doc.toJson());
        outputSocket->flush();
        return true;
    } else {
        return false;
    }
}

void NetworkManager::outputConnectionStateChange(QAbstractSocket::SocketState state)
{
    QString description = getDescriptionForSocketState(state);
    newOutputConnectionState(description);
}

bool NetworkManager::isValidHostAddress(QString ip)
{
    return !(QHostAddress(ip).isNull());
}

QString NetworkManager::getDescriptionForSocketState(QAbstractSocket::SocketState state)
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
