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

#include "source/Model/TerminGroup/groupinfocontainer.h"

class GroupGadgetWrapper
{
    Q_GADGET

public:
    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString comment READ getComment WRITE setComment)

    GroupGadgetWrapper() = default;
    GroupGadgetWrapper(const GroupInfoContainer& info);

    // Uuid
    QString getUuid() const;
    void    setUuid(const QString& uuid);

    // Name
    QString getName() const;
    void    setName(const QString& name);

    // Comment
    QString getComment() const;
    void    setComment(const QString& comment);

    GroupInfoContainer unwrap() const;

private:
    GroupInfoContainer _info;
};

Q_DECLARE_METATYPE(GroupGadgetWrapper)
