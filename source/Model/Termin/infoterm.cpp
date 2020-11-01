/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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
#include "source/Helpers/linkutils.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Helpers/textutils.h"
#include "source/Helpers/validators/linktextvalidator.h"

// Initialization order is important!
InfoTerm::InfoTerm(const NodeInfoContainer& info)
    : mInfo(info)
    , mLowerTerm(info.term.toLower())
    , mDecoratedTerm(getDecoratedTerm(info.term))
    , mDecoratedTermSize(getTermSize(mDecoratedTerm))
    , mLinks(getInnerLinks(info.definition))
{}

const NodeInfoContainer& InfoTerm::info() const { return mInfo; }

bool InfoTerm::isNull() const { return mInfo.isNull(); }

QString InfoTerm::lowerTerm() const { return mLowerTerm; }

QString InfoTerm::decoratedTerm() const { return mDecoratedTerm; }

QSizeF InfoTerm::decoratedTermSize() const { return mDecoratedTermSize; }

TextLink::List InfoTerm::links() const { return mLinks; }

QString InfoTerm::termAndDefinition(bool decorated) const
{
    if (decorated) {
        auto ret = "<font color=\"#00a693\">" + mInfo.term + "</font>";
        ret += " - это " + TagProcessor::decorateTags(mInfo.definition);
        return ret;
    }
    return mInfo.term + " - это " + mInfo.definition;
}

QString InfoTerm::getDecoratedTerm(const QString& term)
{
    QString ret = term;

    if (ret.contains(" ")) {
        // Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

        if (Fonts::getTextMetrics(ret).width() + 15 > AppStyle::Sizes::baseBlockWidth) {
            // Пытаемся ужать в 2 строки
            ret = TextUtils::insertNewLineNearMiddle(ret);
        }
    }

    return ret;
}

QSizeF InfoTerm::getTermSize(const QString& decoratedTerm)
{
    SizesList sizes;

    auto termParts = decoratedTerm.split("\n");

    for (auto& part : termParts)
        sizes.push_back(Fonts::getTextMetrics(part));

    return HelpStuff::getStackedSize(sizes, Qt::Vertical);
}

TextLink::List InfoTerm::getInnerLinks(const QString& linksString)
{
    if (!LinkTextValidator::isValidLinkString(linksString))
        return {};

    auto ranges = LinkUtils::extractLinkRanges(linksString);

    TextLink::List ret;

    for (const auto& range : ranges)
        ret.push_back(TextLink(linksString, range));

    return ret;
}
