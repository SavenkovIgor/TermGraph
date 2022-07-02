// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/managers/notificationmanager.h"

NotificationManager& NotificationManager::instance()
{
    static NotificationManager mgr;
    return mgr;
}

void NotificationManager::showInfo(const QString& info) { addNotify({NotifyType::Info, info}); }

void NotificationManager::showWarning(const QString& warning) { addNotify({NotifyType::Warning, warning}); }

void NotificationManager::showError(const QString& error) { addNotify({NotifyType::Error, error}); }

void NotificationManager::showDebug(const QString& debug) { addNotify({NotifyType::Debug, debug}); }

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

Opt<NotificationManager::Notify> NotificationManager::currentNotify() const
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
    case NotifyType::Info: emit showInfoQml(message); return;
    case NotifyType::Warning: emit showWarningQml(message); return;
    case NotifyType::Error: emit showErrorQml(message); return;
    case NotifyType::Debug: emit showDebugQml(message); return;
    }
}

int NotificationManager::predictMessageShowTime(const QString& message)
{
    auto wordsCount = static_cast<int>(message.simplified().split(" ").size());
    return std::max(wordsCount * 500, 1500); // ~2 words in second
}
