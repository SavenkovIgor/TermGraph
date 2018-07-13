#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "../glb.h"

#include <QObject>
#include <QtNetwork>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    bool connectAndSendGroup(QString hostIp, QJsonDocument doc);
signals:
    void newSyncGroup(QString groupJsonRaw);

    void groupSended();
public slots:
    bool startListen();


private slots:
    void newConnection();
    void newInputData();

    void sendGroup();
private:
    QTcpServer* server;
    QTcpSocket* inputSocket;
    QTcpSocket* outputSocket;

    QJsonDocument jsonSendBuffer;
};

#endif // NETWORKMANAGER_H
