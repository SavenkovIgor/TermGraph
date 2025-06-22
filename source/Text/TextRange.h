// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include <QStringView>

#include "TextCursor.h"

class TextRange
{
public:
    using List = std::vector<TextRange>;

    TextRange(QStringView view, int left, int right);

    const TextCursor& left() const;
    const TextCursor& right() const;

    int  size() const;
    bool isEmpty() const;

    // Returns string without range & cut position
    std::pair<QString, int> cutted() const;

    QStringView rangeView() const;

    static TextRange selectWord(QStringView str, int startPos);

protected: // Members
    const QStringView mString;
    const TextCursor  mLeftCursor;
    const TextCursor  mRightCursor;
};
