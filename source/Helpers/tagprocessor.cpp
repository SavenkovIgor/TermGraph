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
#include "source/Helpers/tagutils.h"

TagProcessor::TagProcessor(QObject* parent)
    : QObject(parent)
{}

bool TagProcessor::isValidCursor(const QString& str, int cursor)
{
    return TagUtils::isValidCursor(QStringView(str), cursor);
}

bool TagProcessor::isInsideTag(const QString& str, int cursor)
{
    return TagUtils::isInsideTag(QStringView(str), cursor);
}

QString TagProcessor::addTag(QString str, int cursor)
{
    return TagUtils::addTag(str, cursor);
}

QString TagProcessor::removeTag(QString str, int cursor)
{
    return TagUtils::removeTag(str, cursor);
}

QString TagProcessor::expandTagRight(QString str, int cursor)
{
    return TagUtils::expandTagRight(str, cursor);
}

QString TagProcessor::decorateTags(const QString& str)
{
    return TagUtils::replaceTags(str, "<font color=\"#6d9a28\">", "</font>");
}
