#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    server = new QTcpServer();
    connect(server,SIGNAL(newConnection()),SLOT(newConnection()));
}

bool NetworkManager::startListen()
{
    return server->listen(QHostAddress::AnyIPv4, NetworkSettings::listenPort);
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

}
