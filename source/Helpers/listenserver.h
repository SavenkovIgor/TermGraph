#ifndef LISTENSERVER_H
#define LISTENSERVER_H

#include <QObject>
#include <QTcpServer>

class ListenServer : public QObject
{
    Q_OBJECT
public:
    explicit ListenServer(QObject *parent = nullptr);

signals:

public slots:
private:
    QTcpServer* server;
};

#endif // LISTENSERVER_H
