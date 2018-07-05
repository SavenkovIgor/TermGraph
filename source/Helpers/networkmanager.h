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

signals:
    void newSyncGroup(QString groupJsonRaw);

public slots:
    bool startListen();

private slots:
    void newConnection();
    void newInputData();
private:
    QTcpServer *server;
    QTcpSocket *inputSocket;
};

#endif // NETWORKMANAGER_H
