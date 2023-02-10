// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QtNetwork>

#include "source/helpers/simplelistenserver.h"
#include "source/managers/notifyinterface.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(NotifyInterface& notifier, QObject* parent = nullptr);

    // Properties
    Q_PROPERTY(bool isConnected READ hasConnection NOTIFY newOutputConnectionState FINAL)
    Q_PROPERTY(QString connectionState READ getOutputSocketState NOTIFY newOutputConnectionState FINAL)
    Q_PROPERTY(bool synchronization READ isServerEnabled WRITE setServerEnabled NOTIFY serverStateChanged FINAL)
    Q_PROPERTY(QString synchronizationState READ serverState NOTIFY serverStateChanged FINAL)

    // Invokables
    Q_INVOKABLE void    connectToHost();
    Q_INVOKABLE void    disconnectFromHost();
    Q_INVOKABLE QString getFirstLocalIpString();
    Q_INVOKABLE QString getReceiverIp();
    Q_INVOKABLE void    setReceiverHostIp(const QString& ip);

signals:
    void newSyncGroup(QString groupJsonRaw);
    void newOutputConnectionState();

    void serverStateChanged();

public slots:
    bool sendGroup(const QJsonDocument& doc);

private slots:
    void newInputData(QHostAddress fromHost, QByteArray data);
    void outputConnectionStateChange(QAbstractSocket::SocketState state);

    void sendConnectionInfo(const QString& info);

private: // Methods
    QString getOutputSocketState();

    static bool    isValidHostAddress(const QString& ip);
    static QString getSocketStateDescription(QAbstractSocket::SocketState state);

    bool    isServerEnabled() const;
    void    setServerEnabled(bool enabled);
    QString serverState() const;

    bool hasConnection();

private: // Members
    SimpleListenServer* server;

    //    QTcpServer* server;
    QTcpSocket* inputSocket;
    QTcpSocket* outputSocket;

    QString receiverIp = QStringLiteral("192.168.1.100");

    NotifyInterface& notifier;
};
