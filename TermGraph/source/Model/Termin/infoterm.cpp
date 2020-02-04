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

#include "infoterm.h"

#include <QDebug>

#include "source/Helpers/appstyle.h"
#include "source/Helpers/fonts.h"
#include "source/Helpers/helpstuff.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Helpers/textprocessor.h"

InfoTerm::InfoTerm(const NodeInfoContainer& info)
{
    this->info        = info;
    cachedTermToLower = getTerm().toLower();
}

QUuid InfoTerm::getUuid() const
{
    return info.uuid;
}

QString InfoTerm::getTerm() const
{
    return info.term;
}

QString InfoTerm::getCachedLowerTerm() const
{
    return cachedTermToLower;
}

QUuid InfoTerm::getGroupUuid() const
{
    return info.groupUuid;
}

QString InfoTerm::getTermAndDefinition(bool decorated) const
{
    if (decorated) {
        auto ret = "<font color=\"#00a693\">" + getTerm() + "</font>";
        ret += " - это " + TagProcessor::decorateTags(getDefinition());
        return ret;
    }
    return getTerm() + " - это " + getDefinition();
}

QString InfoTerm::getSmallName() const
{
    if (smallName.isNull()) {
        smallName = getTerm();

        if (smallName.contains(" ")) {
            // Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

            if (Fonts::getTextMetrics(smallName).width() + 15 > AppStyle::Sizes::baseBlockWidth) {
                // Пытаемся ужать в 2 строки
                smallName = TextProcessor::insertNewLineNearMiddle(smallName);
            }
        }
    }

    return smallName;
}

QSizeF InfoTerm::getNameSize()
{
    if (!nameSize.isValid()) {
        SizesList sizes;
        auto      nameParts = getSmallName().split("\n");

        for (auto& part : nameParts)
            sizes.push_back(Fonts::getTextMetrics(part));

        nameSize = HelpStuff::getStackedSize(sizes, Qt::Vertical);
    }

    return nameSize;
}

NodeInfoContainer InfoTerm::infoContainer() const
{
    return info;
}

QString InfoTerm::getDefinition() const
{
    return info.definition;
}

QStringList InfoTerm::getDefinitionTags() const
{
    QString error;
    auto    tags = TagProcessor::extractTags(getDefinition());
    // TODO: Fix error work in tagProcessor
    if (!error.isEmpty()) {
        qDebug() << getUuid().toString() << error;
    }

    for (auto& tag : tags)
        tag = tag.toLower();

    return tags;
}
