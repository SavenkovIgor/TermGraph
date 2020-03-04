/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include "groupgadgetwrapper.h"

GroupGadgetWrapper::GroupGadgetWrapper(const GroupInfoContainer& info)
    : _info(info)
{}

QString GroupGadgetWrapper::getUuid() const
{
    return _info.uuid.toString();
}

void GroupGadgetWrapper::setUuid(const QString& uuid)
{
    _info.uuid.fromString(uuid);
}

QString GroupGadgetWrapper::getName() const
{
    return _info.name;
}

void GroupGadgetWrapper::setName(const QString& name)
{
    _info.name = name;
}

QString GroupGadgetWrapper::getComment() const
{
    return _info.comment;
}

void GroupGadgetWrapper::setComment(const QString& comment)
{
    _info.comment = comment;
}

GroupInfoContainer GroupGadgetWrapper::unwrap() const
{
    return _info;
}
