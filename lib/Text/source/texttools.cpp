// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/Text/TextTools.h"

#include "include/Text/Fonts.h"
#include <CommonTools/HandyTypes.h>

const QStringList TextTools::mSplitters = {"‐", "-", "-", "—"};

bool TextTools::isTermWithDefinition(const QString &def)
{
    for (const auto &splitter : mSplitters) {
        if (def.contains(splitter)) {
            return true;
        }
    }

    return false;
}

QString TextTools::getTerm(const QString &def)
{
    if (int pos = splitterIndex(def); pos != -1) {
        return def.left(pos).simplified();
    }
    return "";
}

QString TextTools::getDefinition(const QString &def)
{
    if (int pos = splitterIndex(def); pos != -1) {
        return def.mid(pos + 1).simplified();
    }
    return "";
}

int TextTools::splitterIndex(const QString &str)
{
    int pos = -1;
    for (const auto &splitter : mSplitters) {
        pos = asInt(str.indexOf(splitter));
        if (pos != -1) {
            return pos;
        }
    }

    return pos;
}

int TextTools::wordCount(const QString &str)
{
    const QString tmp = str.simplified();
    return !tmp.isEmpty() ? asInt(tmp.count(' ')) + 1 : 0;
}

QSizeF TextTools::preferredTextSize(const QString &text, qreal whProportion)
{
    const auto wCount = wordCount(text);
    const auto lineSize = Fonts::metrics(text);

    if (wCount == 1)
        return lineSize;

    // Recalculate text area, so it has proportions (3 width : 1 height)
    const auto lineArea = lineSize.width() * lineSize.height();
    // Formula counted from equations:
    // area = width * height
    // ratio = width / height
    // =>
    // width = ratio * sqrt(area / ratio)
    const int width = asInt(whProportion * qSqrt(lineArea / whProportion));

    const auto baseRect = QRect(0, 0, width, 1);
    QFontMetricsF mtr = QFontMetricsF(Fonts::defaultFont());

    return mtr.boundingRect(baseRect, Qt::AlignHCenter | Qt::TextWordWrap, text).size();
}
