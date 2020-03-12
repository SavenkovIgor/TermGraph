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

#include "source/Managers/notificationmanager.h"

NotificationManager& NotificationManager::instance()
{
    static NotificationManager mgr;
    return mgr;
}

void NotificationManager::showInfo(const QString& info)
{
    instance().addNotify(Notify(NotifyType::Info, info));
}

void NotificationManager::showWarning(const QString& warning)
{
    instance().addNotify(Notify(NotifyType::Warning, warning));
}

void NotificationManager::showError(const QString& error)
{
    instance().addNotify(Notify(NotifyType::Error, error));
}

void NotificationManager::showDebug(const QString& debug)
{
    instance().addNotify(Notify(NotifyType::Debug, debug));
}

void NotificationManager::handleNotifyShow()
{
    auto notify = currentNotify();

    if (!notify)
        return;

    const auto& [type, message] = notify.value();

    if (type == NotifyType::Debug)
        return;

    auto notifyShowTime = predictMessageShowTime(message);
    hideNotifyTimer.start(notifyShowTime);
}

void NotificationManager::handleNotifyHide()
{
    // User can close notify drawer manually, while timer is still active
    hideNotifyTimer.stop();

    if (!notificationsQueue.isEmpty())
        notificationsQueue.dequeue();

    checkQueue();
}

void NotificationManager::handleUiInitialization()
{
    uiInitialized = true;
    checkQueue();
}

void NotificationManager::checkQueue()
{
    if (!uiInitialized)
        return;

    auto notify = currentNotify();

    if (notify)
        showNotify(notify.value());
}

NotificationManager::NotificationManager(QObject* parent)
    : QObject(parent)
{
    hideNotifyTimer.setSingleShot(true);
    connect(&hideNotifyTimer, &QTimer::timeout, this, &NotificationManager::hideNotify);
}

std::optional<NotificationManager::Notify> NotificationManager::currentNotify() const
{
    if (!notificationsQueue.isEmpty())
        return notificationsQueue.head();

    return std::nullopt;
}

void NotificationManager::addNotify(const NotificationManager::Notify& notify)
{
    notificationsQueue.enqueue(notify);
    checkQueue();
}

void NotificationManager::showNotify(const NotificationManager::Notify& notify)
{
    const auto& [type, message] = notify;

    switch (type) {
    case NotifyType::Info:
        emit showInfoQml(message);
        return;
    case NotifyType::Warning:
        emit showWarningQml(message);
        return;
    case NotifyType::Error:
        emit showErrorQml(message);
        return;
    case NotifyType::Debug:
        emit showDebugQml(message);
        return;
    }
}

int NotificationManager::predictMessageShowTime(const QString& message)
{
    auto wordsCount = message.simplified().split(" ").size();
    return std::max(wordsCount * 500, 1500);  // ~2 words in second
}
