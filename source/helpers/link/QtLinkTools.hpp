// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QStringView>

class QtLinkTools : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(LinkTools)
    QML_SINGLETON

public:
    explicit QtLinkTools(QObject* parent = nullptr);

    QtLinkTools(QtLinkTools const&)    = delete;
    void operator=(QtLinkTools const&) = delete;

    Q_INVOKABLE static bool isValidCursor(const QString& str, int cursor);
    Q_INVOKABLE static bool isCursorOnLink(const QString& str, int cursor);
    Q_INVOKABLE static bool hasSoftLinks(const QString& linkedText);

    Q_INVOKABLE static QString add(QString str, int cursor);
    Q_INVOKABLE static QString expandRight(QString str, int cursor);
    Q_INVOKABLE static QString remove(QString str, int cursor);
    Q_INVOKABLE static QString decorate(const QString& str);
};
