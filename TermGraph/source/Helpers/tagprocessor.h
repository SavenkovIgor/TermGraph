/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <functional>

#include <QDebug>
#include <QObject>
#include <QString>
#include <QStringView>

class TagProcessor : public QObject
{
    Q_OBJECT

    constexpr static auto leftBracket   = '{';
    constexpr static auto rightBracket  = '}';
    constexpr static auto emptyBrackets = "{}";

public:
    enum class SearchDirection { left, right };

    explicit TagProcessor(QObject* parent = nullptr);

    static int   searchWordBorder(const SearchDirection direction, QStringView str, int cursorPos);
    static QChar getNearesBracket(const SearchDirection direction, QStringView str, int cursorPos);

    static bool isPairedBrackets(QStringView str);
    static int  getMaxDepthOfNestedBrackets(QStringView str);

    static QString getErrorFor(QString str);  // TODO: Realize!

    static int getLevDistance(QStringView src, QStringView dst, int limit = 100000);

    static std::pair<bool, int> isTagCorrespondToTermName(QString termName, QString tag);

public slots:
    static bool        isInsideTag(QStringView str, int cursorPos);
    static QStringList extractTags(QString str);
    static QString     addTag(QString str, int cursorPosition);
    static QString     removeTag(QString str, int cursorPosition);
    static QString     expandTagRight(QString str, int cursorPosition);
    static QString     decorateTags(QString str);

private:
    // Exit conditions
    static inline bool isBracket(const QChar& ch);
    static inline bool isLeftBracket(const QChar& ch);
    static inline bool isRightBracket(const QChar& ch);
    static inline bool isLetterOrNumber(const QChar& ch);
    static inline bool isLetterOrNumberInverse(const QChar& ch);
    static inline bool isSpaceCharacter(const QChar& ch);

    // SearchCursorPosition
    static int getCursorPosition(const SearchDirection&     direction,
                                 QStringView                str,
                                 int                        cursorPos,
                                 std::function<bool(QChar)> exitCondition);

    static int moveLeft(QStringView str, int cursorPos, std::function<bool(const QChar)> exitCondition);
    static int moveRight(QStringView str, int cursorPos, std::function<bool(const QChar)> exitCondition);

    static QString replaceTags(QString str, const QString& leftBrReplacement, const QString& rightBrReplacement);

    static inline bool isValidCursor(QStringView str, int cursorPosition)
    {
        // Cursor can be after last symbol
        auto valid = 0 <= cursorPosition && cursorPosition <= str.size();
        if (!valid)
            qWarning() << "Invalid cursor!";

        return valid;
    }
};
