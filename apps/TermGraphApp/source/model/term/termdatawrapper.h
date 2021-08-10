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

#include <QObject>

#include <termdata.h>

class TermDataWrapper : public TermData
{
    Q_GADGET

    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid)
    Q_PROPERTY(QString term MEMBER term)
    Q_PROPERTY(QString definition MEMBER definition)
    Q_PROPERTY(QString description MEMBER description)
    Q_PROPERTY(QString examples MEMBER examples)
    Q_PROPERTY(QString wikiUrl MEMBER wikiUrl)
    Q_PROPERTY(QString wikiImage MEMBER wikiImage)
    Q_PROPERTY(QString groupUuid READ getGroupUuid WRITE setGroupUuid)
    Q_PROPERTY(QString lastEdit READ getLastEdit)

public:
    TermDataWrapper() = default;
    TermDataWrapper(const TermData& info);

    Q_INVOKABLE bool isNull() const;

    QString getUuid() const;
    void    setUuid(const QString& uuid);

    QString getGroupUuid() const;
    void    setGroupUuid(const QString& groupUuid);

    // Not editable...
    QString getLastEdit() const;
};

Q_DECLARE_METATYPE(TermDataWrapper)
