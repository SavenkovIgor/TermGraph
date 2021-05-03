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

#include <vector>

#include <QDateTime>
#include <QString>
#include <QUuid>

struct TermData
{
    using List = std::vector<TermData>;

    QUuid     uuid;
    QString   term;
    QString   definition;
    QString   description;
    QString   examples;
    QString   wikiUrl;
    QString   wikiImage;
    QUuid     groupUuid;
    QDateTime lastEdit;

    // TODO: Replace with operator
    bool isEqualTo(const TermData& target)
    {
        if (uuid != target.uuid)
            return false;

        if (term != target.term)
            return false;

        if (definition != target.definition)
            return false;

        if (description != target.description)
            return false;

        if (examples != target.examples)
            return false;

        if (wikiUrl != target.wikiUrl)
            return false;

        if (wikiImage != target.wikiImage)
            return false;

        if (groupUuid != target.groupUuid)
            return false;

        if (lastEdit != target.lastEdit)
            return false;

        return true;
    }

    bool isNull() const { return uuid.isNull() && term.isEmpty(); }
};
