// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <functional>
#include <memory>

#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetWrapper
{
public:
    using ReplyCallback = std::function<void(QNetworkReply* reply)>;

    NetWrapper();
    ~NetWrapper() = default;

    void get(QUrl url, ReplyCallback callback) const;
    void post(QUrl url, QByteArray data, ReplyCallback callback) const;
    void put(QUrl url, QByteArray data, ReplyCallback callback) const;
    void deleteResource(QUrl url, ReplyCallback callback) const;

private:
    std::unique_ptr<QNetworkAccessManager> manager = nullptr;
};
