#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtNetwork>

#include "source/Helpers/simplelistenserver.h"
#include "source/Helpers/appconfig.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    bool sendGroup(QJsonDocument doc);

    Q_PROPERTY(bool isConnected READ hasConnection NOTIFY newOutputConnectionState)
    Q_PROPERTY(QString connectionState READ getOutputSocketState NOTIFY newOutputConnectionState)

signals:
    void newSyncGroup(QString groupJsonRaw);

    void newOutputConnectionState();

    // Notify
    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:

    void connectToHost();
    void disconnectFromHost();
    void setReceiverHostIp(QString ip);
    QString getReceiverIp();
    bool hasConnection();

    QString getFirstLocalIpString();

private slots:
    void newInputData(QHostAddress fromHost, QByteArray data);
    void outputConnectionStateChange(QAbstractSocket::SocketState state);

    void sendConnectionInfo(QString info);

private:
    SimpleListenServer *server;

//    QTcpServer* server;
    QTcpSocket* inputSocket;
    QTcpSocket* outputSocket;

    QString receiverIp = QStringLiteral("192.168.1.100");

    QString getOutputSocketState();

    static bool isValidHostAddress(const QString& ip);
    static QString getSocketStateDescription(QAbstractSocket::SocketState state);
};

#endif  // NETWORKMANAGER_H
