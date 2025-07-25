// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QStringView>

#include "TextCursor.h"

export module Text.CheckingTextCursor;

import Text.CharTools;

export class CheckingTextCursor : public TextCursor
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

CheckingTextCursor::CheckingTextCursor(QStringView                   strView,
                                       int                           pos,
                                       CheckingTextCursor::Condition leftChecker,
                                       CheckingTextCursor::Condition rightChecker,
                                       std::optional<Direction>      autoStartSearch)
    : TextCursor(strView, pos)
    , mLeftChecker(std::move(leftChecker))
    , mRightChecker(std::move(rightChecker))
{
    if (autoStartSearch.has_value()) {
        search(autoStartSearch.value());
    }
}

bool CheckingTextCursor::check() const { return mLeftChecker(left()) && mRightChecker(right()); }

bool CheckingTextCursor::search(Direction dir)
{
    do {
        if (check()) {
            return true;
        }

        move(dir);
    } while (canMove(dir));

    return check();
}

CheckingTextCursor CheckingTextCursor::rightWordBorder(QStringView              strView,
                                                       int                      pos,
                                                       std::optional<Direction> autoStartSearch)
{
    return {strView, pos, CharTools::isLetterOrNumber, CharTools::notLetterOrNumber, autoStartSearch};
}

CheckingTextCursor CheckingTextCursor::leftWordBorder(QStringView              strView,
                                                      int                      pos,
                                                      std::optional<Direction> autoStartSearch)
{
    return {strView, pos, CharTools::notLetterOrNumber, CharTools::isLetterOrNumber, autoStartSearch};
}

CheckingTextCursor CheckingTextCursor::leftBracketOnRight(QStringView              strView,
                                                          int                      pos,
                                                          std::optional<Direction> autoStartSearch)
{
    return {strView, pos, CharTools::any, CharTools::isLeftBracket, autoStartSearch};
}

CheckingTextCursor CheckingTextCursor::rightBracketOnLeft(QStringView              strView,
                                                          int                      pos,
                                                          std::optional<Direction> autoStartSearch)
{
    return {strView, pos, CharTools::isRightBracket, CharTools::any, autoStartSearch};
}

CheckingTextCursor CheckingTextCursor::anyBracketOnRight(QStringView              strView,
                                                         int                      pos,
                                                         std::optional<Direction> autoStartSearch)
{
    return {strView, pos, CharTools::any, CharTools::isBracket, autoStartSearch};
}

CheckingTextCursor CheckingTextCursor::anyBracketOnLeft(QStringView              strView,
                                                        int                      pos,
                                                        std::optional<Direction> autoStartSearch)
{
    return {strView, pos, CharTools::isBracket, CharTools::any, autoStartSearch};
}
