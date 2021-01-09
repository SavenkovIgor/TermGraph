/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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
#include <QPair>
#include <QQueue>
#include <QTimer>

#include "source/Helpers/handytypes.h"

class NotificationManager : public QObject
{
    Q_OBJECT

    enum class NotifyType { Info = 0, Warning, Error, Debug };

    using Notify = QPair<NotifyType, QString>;

public:
    static NotificationManager& instance();

    NotificationManager(NotificationManager const&) = delete;
    void operator=(NotificationManager const&) = delete;

    Q_INVOKABLE static void showInfo(const QString& info);
    Q_INVOKABLE static void showWarning(const QString& warning);
    Q_INVOKABLE static void showError(const QString& error);
    Q_INVOKABLE static void showDebug(const QString& debug);

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
    opt<Notify>    currentNotify() const;

    void addNotify(const Notify& notify);
    void showNotify(const Notify& notify);

    static int predictMessageShowTime(const QString& message);
};
