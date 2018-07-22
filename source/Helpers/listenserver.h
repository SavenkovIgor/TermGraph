#ifndef LISTENSERVER_H
#define LISTENSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

/// Simple server, that can work with only one connection
class ListenServer : public QObject
{
    Q_OBJECT
public:
    explicit ListenServer(quint16 listenPort, QObject *parent = nullptr);
signals:

public slots:
    bool startListen();
    void newReceivedData(QHostAddress fromHost, QByteArray data);

private slots:
    void newConnection();
    void newInputData();

private:
    QTcpServer* server;
    QList<QTcpSocket*> inputSockets;
    quint16 listenPort;
};

#endif // LISTENSERVER_H
