// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QThread>
#include <QNetworkAccessManager>

class NetworkThread : public QThread
{
public:
    NetworkThread();
    ~NetworkThread();

    void run() final;

    QSharedPointer<QNetworkAccessManager> manager;
};
