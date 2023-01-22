// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QPair>
#include <QQueue>
#include <QTimer>
#include <QQmlEngine>

#include <CommonTools/HandyTypes.h>

#include "source/managers/notifyinterface.h"

class NotificationManager : public QObject, public NotifyInterface
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    enum class NotifyType { Info = 0, Warning, Error, Debug };

    using Notify = QPair<NotifyType, QString>;

public:
    static NotificationManager& instance();
    static NotificationManager* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    NotificationManager(NotificationManager const&) = delete;
    void operator=(NotificationManager const&) = delete;

    void showInfo(const QString& info) final;
    void showWarning(const QString& warning) final;
    void showError(const QString& error) final;
    void showDebug(const QString& debug) final;

    Q_INVOKABLE void handleNotifyShow();
    Q_INVOKABLE void handleNotifyHide();
    Q_INVOKABLE void handleUiInitialization();

signals:
    void showInfoQml(QString info);
    void showWarningQml(QString warning);
    void showErrorQml(QString error);
    void showDebugQml(QString debugInfo);
    void hideNotify();

private slots:
    void checkQueue();

private:
    explicit NotificationManager(QObject* parent = nullptr);

    bool uiInitialized = false;

    QTimer hideNotifyTimer = QTimer(this);

    QQueue<Notify> notificationsQueue;
    Opt<Notify>    currentNotify() const;

    void addNotify(const Notify& notify);
    void showNotify(const Notify& notify);

    static int predictMessageShowTime(const QString& message);
};
