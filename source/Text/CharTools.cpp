// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <functional>

#include <QChar>

export module Text.CharTools;

using namespace std;

export class CharTools
{
public:
    using ShortCondition = function<bool(const QChar)>;

    constexpr static auto leftBracket  = '{';
    constexpr static auto rightBracket = '}';
    constexpr static auto linkSplitter = '|';

    static inline bool any([[maybe_unused]] const QChar ch) { return true; }
    static inline bool isBracket(const QChar ch) { return ch == leftBracket || ch == rightBracket; }
    static inline bool isLeftBracket(const QChar ch) { return ch == leftBracket; }
    static inline bool isRightBracket(const QChar ch) { return ch == rightBracket; }
    static inline bool isLetterOrNumber(const QChar ch) { return ch.isLetterOrNumber(); }
    static inline bool notLetterOrNumber(const QChar ch) { return !ch.isLetterOrNumber(); }
};
