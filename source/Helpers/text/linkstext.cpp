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

#include "source/Helpers/text/linkstext.h"

opt<LinksText> LinksText::create(QStringView str) { return LinksText(str); }

int LinksText::count() const { return 0; }

QString LinksText::replaceLink(int index, QString text) const { return QString(); }

QString LinksText::text() const { return QString(); }

TextLink LinksText::operator[](int index) const
{
    QString str("{}");
    return TextLink(str, 0, 2);
}

LinksText::LinksText(QStringView str)
    : mString(str)
{}
