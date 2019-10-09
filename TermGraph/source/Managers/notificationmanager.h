#pragma once

#include <QObject>

class NotificationManager : public QObject
{
    Q_OBJECT
public:
    static NotificationManager& instance();

    NotificationManager(NotificationManager const&) = delete;
    void operator=(NotificationManager const&) = delete;

    Q_INVOKABLE static void showInfo(const QString& info);
    Q_INVOKABLE static void showWarning(const QString& warning);
    Q_INVOKABLE static void showError(const QString& error);

signals:
    void showInfoQml(QString info);
    void showWarningQml(QString warning);
    void showErrorQml(QString error);

private:
    explicit NotificationManager(QObject* parent = nullptr);
};
