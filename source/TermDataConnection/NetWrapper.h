// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <functional>

#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetWrapper
{
public:
    using ReplyCallback = std::function<void(QNetworkReply* reply)>;

    NetWrapper()  = default;
    ~NetWrapper() = default;

    void get(QUrl url, ReplyCallback callback) const;
    void post(QUrl url, QByteArray data, ReplyCallback callback) const;
    void put(QUrl url, QByteArray data, ReplyCallback callback) const;
    void deleteResource(QUrl url, ReplyCallback callback) const;

private:
    QScopedPointer<QNetworkAccessManager> manager{new QNetworkAccessManager()};
};
