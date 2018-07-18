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

    bool connectAndSendGroup(QJsonDocument doc);
signals:
    void newSyncGroup(QString groupJsonRaw);

    void groupSended();
public slots:
    bool startListen();

    void setReceiverHostIp(QString ip);
    QString getReceiverIp();

    QString getFirstLocalIpString();

private slots:
    void newConnection();
    void newInputData();

    void sendGroup();
private:
    QTcpServer* server;
    QTcpSocket* inputSocket;
    QTcpSocket* outputSocket;

    QJsonDocument jsonSendBuffer;

    QString receiverIp = "192.168.0.80";

    QString getDescriptionForSocketState(QAbstractSocket::SocketState state);
};

#endif // NETWORKMANAGER_H
