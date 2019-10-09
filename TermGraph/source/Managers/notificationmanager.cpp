#include "notificationmanager.h"

#include <QDebug>

NotificationManager::NotificationManager(QObject* parent)
    : QObject(parent)
{}

NotificationManager& NotificationManager::instance()
{
    static NotificationManager mgr;
    return mgr;
}

void NotificationManager::showInfo(const QString& info)
{
    emit instance().showInfoQml(info);
}

void NotificationManager::showWarning(const QString& warning)
{
    emit instance().showWarningQml(warning);
}

void NotificationManager::showError(const QString& error)
{
    emit instance().showErrorQml(error);
}
