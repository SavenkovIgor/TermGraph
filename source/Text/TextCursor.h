// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QStringView>
#include <QtGlobal>

#include <source/CommonTools/HandyTypes.h>

class TextCursor
{
public:
    TextCursor(QStringView strView, int pos = 0);

    // Tools for creation safety
    static bool isValidCursor(QStringView view, int cursor);

    int  pos() const;
    bool atStart() const;
    bool atEnd() const;
    bool atBorder() const;

    // WARNING! Can return null QChar!
    QChar left() const;
    QChar right() const;
    QChar getSymbol(Direction dir) const;

    bool canMove(Direction dir) const;
    bool move(Direction dir);

    TextCursor& operator++(int);
    TextCursor& operator--(int);

private:
    int mPos = 0;

    const QStringView mString;
};
