// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>
#include <QStringList>

class TextUtils
{
public:
    static bool    isTermWithDefinition(const QString& def);
    static QString getTerm(const QString& def);
    static QString getDefinition(const QString& def);

    static QString insertNewLineNearMiddle(const QString& str);

    static int splitterIndex(const QString& str);

    static int wordCount(const QString& str);

private:
    static const QStringList mSplitters;
};
