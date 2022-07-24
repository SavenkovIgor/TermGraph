// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <functional>

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class NetworkThread : public QThread
{
public:
    using ReplyCallback = std::function<void(QNetworkReply* reply)>;

    NetworkThread();
    ~NetworkThread();

    void run() final;

    void get(QUrl url, ReplyCallback callback) const;
    void post(QUrl url, QByteArray data, ReplyCallback callback) const;
    void put(QUrl url, QByteArray data, ReplyCallback callback) const;
    void deleteResource(QUrl url, ReplyCallback callback) const;

    QSharedPointer<QNetworkAccessManager> manager;
};
