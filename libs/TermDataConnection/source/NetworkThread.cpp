// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/TermDataConnection/NetworkThread.h"


NetworkThread::NetworkThread()
    : QThread()
{
    start();
}

NetworkThread::~NetworkThread() { exit(); }

void NetworkThread::run()
{
    manager.reset(new QNetworkAccessManager());
    exec();
}

void NetworkThread::get(QUrl url, std::function<void (QNetworkReply *)> callback) const
{
    QMetaObject::invokeMethod(manager.get(), [this, url, callback](){
        auto* reply = manager->get(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            callback(reply);
            reply->deleteLater();
        });
    });
}

void NetworkThread::post(QUrl url, QByteArray data, ReplyCallback callback) const
{
    QMetaObject::invokeMethod(manager.get(), [this, url, data, callback](){
        auto* reply = manager->post(QNetworkRequest(url), data);

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            callback(reply);
            reply->deleteLater();
        });
    });
}

void NetworkThread::put(QUrl url, QByteArray data, ReplyCallback callback) const
{
    QMetaObject::invokeMethod(manager.get(), [this, url, data, callback](){
        auto* reply = manager->put(QNetworkRequest(url), data);

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            callback(reply);
            reply->deleteLater();
        });
    });
}

void NetworkThread::deleteResource(QUrl url, ReplyCallback callback) const
{
    QMetaObject::invokeMethod(manager.get(), [this, url, callback](){
        auto* reply = manager->deleteResource(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            callback(reply);
            reply->deleteLater();
        });
    });
}
