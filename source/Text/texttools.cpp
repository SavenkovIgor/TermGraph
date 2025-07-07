// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/Text/TextTools.h"

#include <QFontMetricsF>

#include "source/CommonTools/JsonTools.h"

import TextModule.Fonts;
import CommonTools.HandyTypes;

bool TextTools::isTermAndDefinition(QStringView termDef) {
    return termDef.contains(JsonTools::termDefSeparatorSv);
}

QPair<QString, QString> TextTools::splitTermAndDefinition(QStringView termDef)
{
    assert(isTermAndDefinition(termDef));

    auto indexOfSeparator = termDef.indexOf(JsonTools::termDefSeparatorSv);

    auto term       = termDef.first(indexOfSeparator);
    auto definition = termDef.sliced(indexOfSeparator + JsonTools::termDefSeparatorSv.size());

    return {term.toString(), definition.toString()};
}

QString TextTools::joinTermDef(QStringView term, QStringView definition)
{
    auto ret = term.toString();
    ret += JsonTools::termDefSeparator;
    ret += definition;
    return ret;
}

int TextTools::wordCount(QStringView str)
{
    const QString tmp = str.toString().simplified();
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
