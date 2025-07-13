// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QList>
#include <QString>
#include <QStringView>

class LinkTools
{
public:
    static bool isValidCursor(const QString& str, int cursor);
    static bool isCursorOnLink(const QString& str, int cursor);
    static bool hasSoftLinks(const QString& linkedText);

    static QString add(QString str, int cursor);
    static QString expandRight(QString str, int cursor);
    static QString remove(QString str, int cursor);
    static QString decorate(const QString& str);

    static bool               linkAndTermSimilarWordDistance(const QString& link, const QString& term);
    static int                levDistance(QStringView src, QStringView dst, int limit = 100000);
    static std::optional<int> linkAndTermDistance(const QString& link, const QString& term, int maxLimit);
};
