// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/Text/TextTools.h"

#include "include/Text/Fonts.h"
#include <CommonTools/HandyTypes.h>
#include <CommonTools/JsonTools.h>

bool TextTools::isTermWithDefinition(const QString &termDef) { return termDef.contains(JsonTools::termDefSeparator); }

QPair<QString, QString> TextTools::splitTermAndDefinition(const QString &termDef)
{
    assert(isTermWithDefinition(def));

    auto indexOfSeparator = termDef.indexOf(JsonTools::termDefSeparator);

    auto term       = termDef.left(indexOfSeparator);
    auto definition = termDef.mid(indexOfSeparator + QString(JsonTools::termDefSeparator).size());

    return {term, definition};
}

QString TextTools::joinTermDef(const QString &term, const QString &definition)
{
    return term + JsonTools::termDefSeparator + definition;
}

int TextTools::wordCount(const QString &str)
{
    const QString tmp = str.simplified();
    return !tmp.isEmpty() ? asInt(tmp.count(' ')) + 1 : 0;
}

QSizeF TextTools::preferredTextSize(const QString &text, qreal whProportion)
{
    const auto wCount   = wordCount(text);
    const auto lineSize = Fonts::metrics(text);

    if (wCount == 1) {
        return lineSize;
    }

    // Recalculate text area, so it has proportions (3 width : 1 height)
    const auto lineArea = lineSize.width() * lineSize.height();
    // Formula counted from equations:
    // area = width * height
    // ratio = width / height
    // =>
    // width = ratio * sqrt(area / ratio)
    const int width = asInt(whProportion * qSqrt(lineArea / whProportion));

    const auto    baseRect = QRect(0, 0, width, 1);
    QFontMetricsF mtr      = QFontMetricsF(Fonts::defaultFont());

    return mtr.boundingRect(baseRect, Qt::AlignHCenter | Qt::TextWordWrap, text).size();
}
