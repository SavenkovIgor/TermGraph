#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    server = new SimpleListenServer(AppConfig::NetworkSettings::listenPort, this);
    server->startListen();
    connect(server,
            SIGNAL(newReceivedData(QHostAddress, QByteArray)),
            SLOT(newInputData(QHostAddress, QByteArray)));

    connect(server, SIGNAL(newConnectionFrom(QString)), SLOT(sendConnectionInfo(QString)));

    outputSocket = new QTcpSocket(this);
    connect(outputSocket,
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            SLOT(outputConnectionStateChange(QAbstractSocket::SocketState)));
}

void NetworkManager::connectToHost()
{
    outputSocket->connectToHost(receiverIp,
                                AppConfig::NetworkSettings::listenPort,
                                QIODevice::WriteOnly);
}

void NetworkManager::disconnectFromHost()
{
    outputSocket->disconnectFromHost();
}

void NetworkManager::setReceiverHostIp(QString ip)
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
    for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
            && address != QHostAddress(QHostAddress::LocalHost)) {
            return address.toString();
        }
    }
    return "";
}

void NetworkManager::newInputData(QHostAddress fromHost, QByteArray data)
{
    Q_UNUSED(fromHost);

    static QByteArray byteBuffer = "";
    byteBuffer.append(data);

    QJsonDocument doc = QJsonDocument::fromJson(byteBuffer);

    // Подаем на импорт только валидные json
    if (!doc.isNull()) {
        emit newSyncGroup(QString(byteBuffer));
        byteBuffer.clear();
    }
}

bool NetworkManager::sendGroup(QJsonDocument doc)
{
    if (!isValidHostAddress(receiverIp))
        return false;

    if (outputSocket->state() == QAbstractSocket::ConnectedState) {
        outputSocket->write(doc.toJson());
        outputSocket->flush();
        return true;
    } else {
        return false;
    }
}

void NetworkManager::outputConnectionStateChange(QAbstractSocket::SocketState state)
{
    Q_UNUSED(state)
    emit newOutputConnectionState();
}

void NetworkManager::sendConnectionInfo(QString info)
{
    emit showInfo("Подключение от: " + info);
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
