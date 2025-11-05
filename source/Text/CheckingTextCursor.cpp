// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QStringView>

export module Text.CheckingTextCursor;

import Text.CharTools;
import Text.TextCursor;

using namespace std;

export class CheckingTextCursor : public TextCursor
{
public:
    using Condition = CharTools::ShortCondition;

    CheckingTextCursor(QStringView         strView,
                       int                 pos,
                       Condition           leftChecker,
                       Condition           rightChecker,
                       optional<Direction> autoStartSearch = nullopt)
        : TextCursor(strView, pos)
        , mLeftChecker(std::move(leftChecker))
        , mRightChecker(std::move(rightChecker))
    {
        if (autoStartSearch.has_value()) {
            search(autoStartSearch.value());
        }
    }

    bool check() const { return mLeftChecker(left()) && mRightChecker(right()); }

    bool search(Direction dir)
    {
        do {
            if (check()) {
                return true;
            }

            move(dir);
        } while (canMove(dir));

        return check();
    }

    static CheckingTextCursor rightWordBorder(QStringView         strView,
                                              int                 pos             = 0,
                                              optional<Direction> autoStartSearch = nullopt)
    {
        return {strView, pos, CharTools::isLetterOrNumber, CharTools::notLetterOrNumber, autoStartSearch};
    }

    static CheckingTextCursor leftWordBorder(QStringView         strView,
                                             int                 pos             = 0,
                                             optional<Direction> autoStartSearch = nullopt)
    {
        return {strView, pos, CharTools::notLetterOrNumber, CharTools::isLetterOrNumber, autoStartSearch};
    }

    static CheckingTextCursor leftBracketOnRight(QStringView         strView,
                                                 int                 pos             = 0,
                                                 optional<Direction> autoStartSearch = nullopt)
    {
        return {strView, pos, CharTools::any, CharTools::isLeftBracket, autoStartSearch};
    }

    static CheckingTextCursor rightBracketOnLeft(QStringView         strView,
                                                 int                 pos             = 0,
                                                 optional<Direction> autoStartSearch = nullopt)
    {
        return {strView, pos, CharTools::isRightBracket, CharTools::any, autoStartSearch};
    }

    static CheckingTextCursor anyBracketOnRight(QStringView         strView,
                                                int                 pos             = 0,
                                                optional<Direction> autoStartSearch = nullopt)
    {
        return {strView, pos, CharTools::any, CharTools::isBracket, autoStartSearch};
    }

    static CheckingTextCursor anyBracketOnLeft(QStringView         strView,
                                               int                 pos             = 0,
                                               optional<Direction> autoStartSearch = nullopt)
    {
        return {strView, pos, CharTools::isBracket, CharTools::any, autoStartSearch};
    }

private:
    const Condition mLeftChecker;
    const Condition mRightChecker;
};
