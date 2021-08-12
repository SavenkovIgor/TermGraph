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

#pragma once

#include <QStringView>

#include <CommonTools/HandyTypes.h>

#include "source/helpers/link/link.h"

class LinksString
{
public:
    LinksString(QStringView str);
    LinksString(const LinksString& other) = default;

    QString           text() const;
    const Link::List& links() const;

    QString replaceLink(int index, QString text) const;

    // Static stuff
    static bool isValidLinksString(QStringView str);

    static QString addLink(QString str, int cursor);
    static QString expandLinkRight(QString str, int cursor);
    static QString removeLink(QString str, int cursor);

private: // Methods
    static int        getCount(QStringView strView);
    static Link::List extractLinks(QStringView strView);

private: // Members
    const QStringView mString;
    const Link::List  mLinks;
};
