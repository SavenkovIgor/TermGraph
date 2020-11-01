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

#include <QSizeF>
#include <QString>

#include "source/Model/Termin/nodeinfocontainer.h"
#include "source/Model/Termin/textlink.h"

// Const wrapper over NodeInfoContainer + some additional stuff
class InfoTerm
{
public:
    explicit InfoTerm(const NodeInfoContainer& info);
    ~InfoTerm() = default;

    const NodeInfoContainer& info() const;

    bool isNull() const;

    QString lowerTerm() const;
    QString decoratedTerm() const;
    QSizeF  decoratedTermSize() const;

    TextLink::List links() const;

    QString termAndDefinition(bool decorated = false) const;

    static TextLink::List getInnerLinks(const QString& linksString);

private: // Methods
    static QString getDecoratedTerm(const QString& term);
    static QSizeF  getTermSize(const QString& decoratedTerm);

private: // Members
    const NodeInfoContainer mInfo;

    const QString        mLowerTerm;
    const QString        mDecoratedTerm;
    const QSizeF         mDecoratedTermSize;
    const TextLink::List mLinks;
};
