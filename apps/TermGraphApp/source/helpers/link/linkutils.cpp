// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/link/linkutils.h"

#include <limits>

#include "source/helpers/intmatrix.h"
#include "source/helpers/text/chartools.h"
#include "source/helpers/text/checkingtextcursor.h"
#include "source/helpers/text/textcursor.h"
#include "source/helpers/text/textutils.h"

bool LinkUtils::tagLengthSuitTerm(const QString& tag, const QString& termName)
{
    int wordsCountInTag = TextUtils::wordCount(tag);
    int maxWordDistance = 4 * wordsCountInTag; // Magic numbers. Would be replaced further

    auto stringSizeDiffer = std::abs(termName.size() - tag.size());
    // No need to check. Term is not suit already
    return stringSizeDiffer <= maxWordDistance;
}

int LinkUtils::getLevDistance(QStringView src, QStringView dst, int limit)
{
    const int m = src.size();
    const int n = dst.size();

    if (m == 0)
        return n;

    if (n == 0)
        return m;

    static IntMatrix matrixContainer;
    matrixContainer.checkSize(n + 1, m + 1);

    auto& matrix = matrixContainer.matrix;

    for (int i = 0; i <= m; ++i) {
        matrix[i][0] = i;
    }

    for (int i = 0; i <= n; ++i) {
        matrix[0][i] = i;
    }

    int above_cell;
    int left_cell;
    int diagonal_cell;
    int cost;
    int min_in_row;

    for (int i = 1; i <= m; ++i) {
        min_in_row = std::numeric_limits<int>::max();
        for (int j = 1; j <= n; ++j) {
            cost          = src[i - 1] == dst[j - 1] ? 0 : 1;
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

Opt<int> LinkUtils::getDistanceBetweenTagAndTerm(const QString& tag, const QString& termName, int maxLimit)
{
    // For developing needs
    assert(termName == termName.toLower());
    assert(tag == tag.toLower());

    // Exact match
    if (termName.size() == tag.size() && termName == tag)
        return 0;

    int wordsCountInTag = TextUtils::wordCount(tag);
    int maxWordDistance = std::min(4 * wordsCountInTag, maxLimit);

    auto stringSizeDiffer = std::abs(termName.size() - tag.size());
    if (stringSizeDiffer > maxWordDistance) // No need to check. Term is not suit already
        return std::nullopt;

    auto distance = getLevDistance(termName, tag, maxWordDistance);
    if (distance <= maxWordDistance) {
        if (termName.left(3) == tag.left(3)) {
            return distance;
        }
    }

    return std::nullopt;
}
