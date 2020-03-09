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
    void showDebugQml(QString debugInfo);

private:
    explicit NotificationManager(QObject* parent = nullptr);
};
