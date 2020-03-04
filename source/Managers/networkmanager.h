/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QObject>
#include <QtNetwork>

#include "source/Helpers/appconfig.h"
#include "source/Helpers/simplelistenserver.h"
#include "source/Managers/notificationmanager.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject* parent = nullptr);

    bool sendGroup(const QJsonDocument& doc);

    Q_PROPERTY(bool isConnected READ hasConnection NOTIFY newOutputConnectionState)
    Q_PROPERTY(QString connectionState READ getOutputSocketState NOTIFY newOutputConnectionState)
    Q_PROPERTY(bool synchronization READ isServerEnabled WRITE setServerEnabled NOTIFY serverStateChanged)
    Q_PROPERTY(QString synchronizationState READ serverState NOTIFY serverStateChanged)

signals:
    void newSyncGroup(QString groupJsonRaw);
    void newOutputConnectionState();

    void serverStateChanged();

public slots:
    void    connectToHost();
    void    disconnectFromHost();
    void    setReceiverHostIp(const QString& ip);
    QString getReceiverIp();
    bool    hasConnection();

    QString getFirstLocalIpString();

private slots:
    void newInputData(QHostAddress fromHost, QByteArray data);
    void outputConnectionStateChange(QAbstractSocket::SocketState state);

    void sendConnectionInfo(const QString& info);

private:
    SimpleListenServer* server;

    //    QTcpServer* server;
    QTcpSocket* inputSocket;
    QTcpSocket* outputSocket;

    QString receiverIp = QStringLiteral("192.168.1.100");

    QString getOutputSocketState();

    static bool    isValidHostAddress(const QString& ip);
    static QString getSocketStateDescription(QAbstractSocket::SocketState state);

    bool    isServerEnabled() const;
    void    setServerEnabled(bool enabled);
    QString serverState() const;
};
