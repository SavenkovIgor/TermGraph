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

#include <QObject>
#include <QDebug>

class TagProcessor : public QObject
{
    Q_OBJECT

    constexpr static char leftBracket = '{';
    constexpr static char rightBracket = '}';
    constexpr static auto emptyBrackets = "{}";

public:
    enum class SearchDirection { left, right };

    explicit TagProcessor(QObject *parent = nullptr);

    static int searchWordBorder(const SearchDirection direction, const QString& text, int cursorPos);
    static QChar getNearesBracket(const SearchDirection direction, const QString& text, int cursorPos);
    static bool isInsideTag(const QString& text, int cursorPos);

    static bool isPairedBrackets(QString text);
    static int getMaxDepthOfNestedBrackets(QString text);

    static QString getErrorFor(QString str);  // TODO: Realize!

    static int getLevDistance(const QString& src, const QString& dst, int limit = 100000);

    static std::pair<bool, int> isTagCorrespondToTermName(QString termName, QString tag);

public slots:
    static QStringList extractTags(QString str);
    static QString addTagInPosition(int cursorPosition, QString str);
    static QString expandRight(int cursorPosition, QString str);
    static QString decorateTags(const QString& src);

private:
    // Exit conditions
    static inline bool isBracket(const QChar &ch);
    static inline bool isLeftBracket(const QChar & ch);
    static inline bool isRightBracket(const QChar & ch);
    static inline bool isLetterOrNumber(const QChar& ch);
    static inline bool isLetterOrNumberInverse(const QChar& ch);
    static inline bool isSpaceCharacter(const QChar& ch);

    // SearchCursorPosition
    static int getCursorPosition(
            const SearchDirection& direction,
            const QString& text,
            int cursorPos,
            std::function<bool(QChar)> exitCondition);

    static int moveLeft(const QString& text, int cursorPos, std::function<bool(const QChar)> exitCondition);
    static int moveRight(const QString& text, int cursorPos, std::function<bool(const QChar)> exitCondition);

    static QString replaceTags(
            const QString &src,
            const QString &leftBrReplacement,
            const QString &rightBrReplacement);
};
