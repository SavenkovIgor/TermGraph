// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QStringView>

class LinkTools : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit LinkTools(QObject* parent = nullptr);

    LinkTools(LinkTools const&)      = delete;
    void operator=(LinkTools const&) = delete;

    Q_INVOKABLE static bool isValidCursor(const QString& str, int cursor);
    Q_INVOKABLE static bool isCursorOnLink(const QString& str, int cursor);
    Q_INVOKABLE static bool hasSoftLinks(const QString& linkedText);

    Q_INVOKABLE static QString add(QString str, int cursor);
    Q_INVOKABLE static QString expandRight(QString str, int cursor);
    Q_INVOKABLE static QString remove(QString str, int cursor);
    Q_INVOKABLE static QString decorate(const QString& str);

    static bool               linkAndTermSimilarWordDistance(const QString& link, const QString& term);
    static int                levDistance(QStringView src, QStringView dst, int limit = 100000);
    static std::optional<int> linkAndTermDistance(const QString& link, const QString& term, int maxLimit);

private:
};
