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

#include "source/Helpers/link/linksstring.h"

#include "source/Helpers/text/chartools.h"

LinksString::LinksString(QStringView str)
    : mString(str)
    , mLinks(extractLinks(mString))
{}

QString LinksString::replaceLink(int index, QString text) const
{
    auto [linkCut, pos] = mLinks[index].cutted();
    linkCut.insert(pos, text);
    return linkCut;
}

bool LinksString::isValidLinksString(QStringView str)
{
    int brCount = 0;

    for (auto ch : str) {
        if (ch == CharTools::leftBracket)
            brCount++;

        if (ch == CharTools::rightBracket)
            brCount--;

        if (brCount < 0 || brCount > 1)
            return false;
    }

    return brCount == 0;
}

QString LinksString::text() const { return mString.toString(); }

const Link::List& LinksString::links() const { return mLinks; }

int LinksString::getCount(QStringView strView)
{
    if (!isValidLinksString(strView))
        return 0;

    auto str = strView.toString();
    return str.count(CharTools::leftBracket);
}

Link::List LinksString::extractLinks(QStringView strView)
{
    Link::List ret;

    if (!isValidLinksString(strView))
        return ret;

    int count = getCount(strView);

    for (int i = 0; i < count; i++) {
        int strPos = -1;

        for (int j = 0; j <= i; j++)
            strPos = strView.indexOf(CharTools::leftBracket, strPos + 1);

        auto link = Link::select(strView, strPos + 1);

        assert(link.has_value());

        ret.push_back(link.value());
    }

    return ret;
}
