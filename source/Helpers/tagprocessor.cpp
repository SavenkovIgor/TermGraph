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

#include "source/Helpers/tagprocessor.h"

#include "source/Helpers/intmatrix.h"
#include "source/Helpers/link/link.h"
#include "source/Helpers/link/linksdecorator.h"
#include "source/Helpers/link/linksstring.h"
#include "source/Helpers/link/linkutils.h"
#include "source/Helpers/text/textcursor.h"

TagProcessor::TagProcessor(QObject* parent)
    : QObject(parent)
{}

bool TagProcessor::isValidCursor(const QString& str, int cursor)
{
    return TextCursor::isValidCursor(QStringView(str), cursor);
}

bool TagProcessor::isCursorOnLink(const QString& str, int cursor) { return Link::isCursorOnLink(str, cursor); }

bool TagProcessor::hasSoftLinks(const QString& linkedText)
{
    LinksString txt(linkedText);

    for (const auto& link : txt.links()) {
        if (!link.hasUuid())
            return true;
    }

    return false;
}

QString TagProcessor::addLink(QString str, int cursor) { return LinksString::addLink(str, cursor); }

QString TagProcessor::expandLinkRight(QString str, int cursor) { return LinksString::expandLinkRight(str, cursor); }

QString TagProcessor::removeLink(QString str, int cursor) { return LinksString::removeLink(str, cursor); }

QString TagProcessor::decorateTags(const QString& str)
{
    LinksString    linkText(str);
    LinksDecorator decorator(linkText, LinksDecorator::blueDecorator);
    return decorator.apply(LinksDecoratorMode::Replace);
}
