// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QStringView>

#include "CharTools.h"
#include "TextCursor.h"

class CheckingTextCursor : public TextCursor
{
public:
    using Condition = CharTools::ShortCondition;

    CheckingTextCursor(QStringView              strView,
                       int                      pos,
                       Condition                leftChecker,
                       Condition                rightChecker,
                       std::optional<Direction> autoStartSearch = std::nullopt);

    bool check() const;

    bool search(Direction dir);

    static CheckingTextCursor rightWordBorder(QStringView              strView,
                                              int                      pos             = 0,
                                              std::optional<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor leftWordBorder(QStringView              strView,
                                             int                      pos             = 0,
                                             std::optional<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor leftBracketOnRight(QStringView              strView,
                                                 int                      pos             = 0,
                                                 std::optional<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor rightBracketOnLeft(QStringView              strView,
                                                 int                      pos             = 0,
                                                 std::optional<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor anyBracketOnRight(QStringView              strView,
                                                int                      pos             = 0,
                                                std::optional<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor anyBracketOnLeft(QStringView              strView,
                                               int                      pos             = 0,
                                               std::optional<Direction> autoStartSearch = std::nullopt);

private:
    const Condition mLeftChecker;
    const Condition mRightChecker;
};
