#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtNetwork>

#include "../Helpers/simplelistenserver.h"
#include "../Helpers/appconfig.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    bool sendGroup(QJsonDocument doc);

signals:
    void newSyncGroup(QString groupJsonRaw);

    void newOutputConnectionState(QString state);

public slots:

    void connectToHost();
    void disconnectFromHost();
    void setReceiverHostIp(QString ip);
    QString getReceiverIp();
    bool isConnected();

    QString getFirstLocalIpString();

private slots:
    void newInputData(QHostAddress fromHost, QByteArray data);
    void outputConnectionStateChange(QAbstractSocket::SocketState state);

private:
    SimpleListenServer *server;

//    QTcpServer* server;
    QTcpSocket* inputSocket;
    QTcpSocket* outputSocket;

    QString receiverIp = "192.168.0.80";

    bool isValidHostAddress(QString ip);
    QString getDescriptionForSocketState(QAbstractSocket::SocketState state);
};

#endif  // NETWORKMANAGER_H
