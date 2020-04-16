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

#include "source/Model/Termin/infoterm.h"

#include "source/Helpers/appstyle.h"
#include "source/Helpers/fonts.h"
#include "source/Helpers/helpstuff.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Helpers/tagutils.h"
#include "source/Helpers/textprocessor.h"

InfoTerm::InfoTerm(const NodeInfoContainer& info)
    : mInfo(info)
    , mDecoratedTerm(decorateTerm(mInfo.term))
{
    cachedTermToLower = mInfo.term.toLower();
}

bool InfoTerm::isNull() const
{
    return mInfo.isNull();
}

QString InfoTerm::getCachedLowerTerm() const
{
    return cachedTermToLower;
}

QString InfoTerm::getTermAndDefinition(bool decorated) const
{
    if (decorated) {
        auto ret = "<font color=\"#00a693\">" + mInfo.term + "</font>";
        ret += " - это " + TagProcessor::decorateTags(mInfo.definition);
        return ret;
    }
    return mInfo.term + " - это " + mInfo.definition;
}

QString InfoTerm::decoratedTerm() const
{
    return mDecoratedTerm;
}

QSizeF InfoTerm::getNameSize()
{
    if (!nameSize.isValid()) {
        SizesList sizes;
        auto      nameParts = decoratedTerm().split("\n");

        for (auto& part : nameParts)
            sizes.push_back(Fonts::getTextMetrics(part));

        nameSize = HelpStuff::getStackedSize(sizes, Qt::Vertical);
    }

    return nameSize;
}

QString InfoTerm::decorateTerm(const QString& term)
{
    QString ret = term;

    if (ret.contains(" ")) {
        // Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

        if (Fonts::getTextMetrics(ret).width() + 15 > AppStyle::Sizes::baseBlockWidth) {
            // Пытаемся ужать в 2 строки
            ret = TextProcessor::insertNewLineNearMiddle(ret);
        }
    }

    return ret;
}

const NodeInfoContainer& InfoTerm::info() const
{
    return mInfo;
}

QStringList InfoTerm::getDefinitionTags() const
{
    auto tags = TagUtils::extractTags(mInfo.definition);

    for (auto& tag : tags)
        tag = tag.toLower();

    return tags;
}
