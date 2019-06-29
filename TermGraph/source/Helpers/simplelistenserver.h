#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

/// Simple server, that can work with only one connection
class SimpleListenServer : public QObject
{
    Q_OBJECT
public:
    explicit SimpleListenServer(quint16 listenPort, QObject *parent = nullptr);

signals:
    void newReceivedData(QHostAddress fromHost, QByteArray data);
    void newConnectionFrom(QString hostDescription);

public slots:
    bool startListen();

private slots:
    void newConnection();
    void newInputData();

private:
    quint16 listenPort;
    QTcpServer* server;
    QList<QTcpSocket*> inputSockets;

    static QString getSocketDescription(QTcpSocket* socket);
};
