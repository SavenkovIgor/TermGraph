// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>
#include <QStringList>
#include <QSizeF>


class TextTools
{
public:
    static bool    isTermWithDefinition(const QString& def);
    static QString getTerm(const QString& def);
    static QString getDefinition(const QString& def);

    static int splitterIndex(const QString& str);

    static int wordCount(const QString& str);

    static QSizeF preferredTextSize(const QString& text, qreal whProportion = 3);

private:
    static const QStringList mSplitters;
};
