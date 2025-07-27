// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <limits>

#include <QList>
#include <QString>
#include <QStringView>

#include "source/Text/TextCursor.h"
#include "source/helpers/link/Link.h"
#include "source/helpers/link/LinksString.h"

export module link.LinkTools;

import CommonTools.HandyTypes;
import Helpers.IntMatrix;
import LinksDecorator;
import TextModule.TextTools;

export class LinkTools
{
public:
    static bool isValidCursor(const QString& str, int cursor)
    {
        return TextCursor::isValidCursor(QStringView(str), cursor);
    }

    static bool isCursorOnLink(const QString& str, int cursor) { return Link::isCursorOnLink(str, cursor); }

    static bool hasSoftLinks(const QString& linkedText)
    {
        LinksString txt(linkedText);

        for (const auto& link : txt.links()) {
            if (!link.hasUuid()) {
                return true;
            }
        }

        return false;
    }

    static QString add(QString str, int cursor) { return LinksString::addLink(str, cursor); }

    static QString expandRight(QString str, int cursor) { return LinksString::expandLinkRight(str, cursor); }

    static QString remove(QString str, int cursor) { return LinksString::removeLink(str, cursor); }

    static QString decorate(const QString& str) {
        auto linksString = LinksString(str);
        return LinksDecorator::toRichText(linksString);
    }

    static bool linkAndTermSimilarWordDistance(const QString& link, const QString& term)
    {
        int wordsCountInLink = TextTools::wordCount(link);
        int maxWordDistance  = 4 * wordsCountInLink; // Magic numbers. Would be replaced further

        auto stringSizeDiffer = std::abs(term.size() - link.size());
        // No need to check. Term is not suit already
        return stringSizeDiffer <= maxWordDistance;
    }

    static int levDistance(QStringView src, QStringView dst, int limit = 100000)
    {
        auto asULong     = [](auto num) { return static_cast<ulong>(num); };
        auto asQSizeType = [](auto num) { return static_cast<qsizetype>(num); };

        const auto m = asULong(src.size());
        const auto n = asULong(dst.size());

        if (m == 0) {
            return asInt(n);
        }

        if (n == 0) {
            return asInt(m);
        }

        static IntMatrix matrixContainer;
        matrixContainer.checkSize(n + 1, m + 1);

        auto& matrix = matrixContainer.matrix;

        for (ulong i = 0; i <= m; i++) {
            matrix[i][0] = asInt(i);
        }

        for (ulong i = 0; i <= n; i++) {
            matrix[0][i] = asInt(i);
        }

        int cost          = 0;
        int above_cell    = 0;
        int left_cell     = 0;
        int diagonal_cell = 0;
        int min_in_row    = 0;

        for (ulong i = 1; i <= m; i++) {
            min_in_row = std::numeric_limits<int>::max();
            for (ulong j = 1; j <= n; j++) {
                cost          = src[asQSizeType(i - 1)] == dst[asQSizeType(j - 1)] ? 0 : 1;
                above_cell    = matrix[i - 1][j];
                left_cell     = matrix[i][j - 1];
                diagonal_cell = matrix[i - 1][j - 1];
                matrix[i][j]  = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
                min_in_row    = std::min(min_in_row, matrix[i][j]);
            }
            if (min_in_row > limit) {
                return std::numeric_limits<int>::max();
            }
        }

        return matrix[m][n];
    }

    static std::optional<int> linkAndTermDistance(const QString& link, const QString& term, int maxLimit)
    {
        // For developing needs
        assert(term == term.toLower());
        assert(link == link.toLower());

        // Exact match
        if (term.size() == link.size() && term == link) {
            return 0;
        }

        int wordsCountInLink = TextTools::wordCount(link);
        int maxWordDistance  = std::min(4 * wordsCountInLink, maxLimit);

        auto stringSizeDiffer = std::abs(term.size() - link.size());
        if (stringSizeDiffer > maxWordDistance) { // No need to check. Term is not suit already
            return std::nullopt;
        }

        auto distance = levDistance(term, link, maxWordDistance);
        if (distance <= maxWordDistance) {
            if (term.left(3) == link.left(3)) {
                return distance;
            }
        }

        return std::nullopt;
    }
};
