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

#include "source/Model/TerminGroup/groupdata.h"

class GroupDataWrapper : GroupData
{
    Q_GADGET

    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString comment MEMBER comment)

public:
    GroupDataWrapper() = default;
    GroupDataWrapper(const GroupData& info);

    // Uuid
    QString getUuid() const;
    void    setUuid(const QString& uuid);
};

Q_DECLARE_METATYPE(GroupDataWrapper)
