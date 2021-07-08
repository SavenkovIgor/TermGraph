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

#include "source/model/term/termdatawrapper.h"

TermDataWrapper::TermDataWrapper(const TermData& info)
    : TermData(info)
{}

bool TermDataWrapper::isNull() const { return TermData::isNull(); }

QString TermDataWrapper::getUuid() const { return uuid.toString(); }

void TermDataWrapper::setUuid(const QString& uuid) { this->uuid = QUuid(uuid); }

QString TermDataWrapper::getGroupUuid() const { return groupUuid.toString(); }

void TermDataWrapper::setGroupUuid(const QString& groupUuid) { this->groupUuid = QUuid(groupUuid); }

QString TermDataWrapper::getLastEdit() const { return lastEdit.toString("dd MMM yyyy hh:mm:ss t"); }
