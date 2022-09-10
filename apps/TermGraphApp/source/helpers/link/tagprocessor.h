// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QString>
#include <QStringView>


class TagProcessor : public QObject
{
    Q_OBJECT

public:
    static TagProcessor& instance();

    TagProcessor(TagProcessor const&) = delete;
    void operator=(TagProcessor const&) = delete;

    Q_INVOKABLE static bool isValidCursor(const QString& str, int cursor);
    Q_INVOKABLE static bool isCursorOnLink(const QString& str, int cursor);
    Q_INVOKABLE static bool hasSoftLinks(const QString& linkedText);

    Q_INVOKABLE static QString addLink(QString str, int cursor);
    Q_INVOKABLE static QString expandLinkRight(QString str, int cursor);
    Q_INVOKABLE static QString removeLink(QString str, int cursor);
    Q_INVOKABLE static QString decorateTags(const QString& str);

private:
    explicit TagProcessor(QObject* parent = nullptr);
};
