// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/TermDataConnection/NetWrapper.h"

void NetWrapper::get(QUrl url, ReplyCallback callback) const
{
    auto* reply = manager->get(QNetworkRequest(url));

    QObject::connect(reply, &QNetworkReply::finished, [callback, reply] {
        callback(reply);
        reply->deleteLater();
    });
}

void NetWrapper::post(QUrl url, QByteArray data, ReplyCallback callback) const
{
    auto* reply = manager->post(QNetworkRequest(url), data);

    QObject::connect(reply, &QNetworkReply::finished, [callback, reply] {
        callback(reply);
        reply->deleteLater();
    });
}

void NetWrapper::put(QUrl url, QByteArray data, ReplyCallback callback) const
{
    auto* reply = manager->put(QNetworkRequest(url), data);

    QObject::connect(reply, &QNetworkReply::finished, [callback, reply] {
        callback(reply);
        reply->deleteLater();
    });
}

void NetWrapper::deleteResource(QUrl url, ReplyCallback callback) const
{
    auto* reply = manager->deleteResource(QNetworkRequest(url));

    QObject::connect(reply, &QNetworkReply::finished, [callback, reply] {
        callback(reply);
        reply->deleteLater();
    });
}
