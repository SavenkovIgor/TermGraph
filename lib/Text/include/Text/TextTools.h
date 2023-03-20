// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSizeF>
#include <QString>
#include <QStringList>

class TextTools
{
public:
    static bool                    isTermWithDefinition(const QString& termDef);
    static QPair<QString, QString> splitTermAndDefinition(const QString& termDef);
    static QString                 joinTermDef(const QString& term, const QString& definition);

    static int wordCount(const QString& str);

    static QSizeF preferredTextSize(const QString& text, qreal whProportion = 3);
};
