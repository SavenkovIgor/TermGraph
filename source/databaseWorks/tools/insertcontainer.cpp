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

#include "insertcontainer.h"

InsertContainer::InsertContainer(const TColumn& column, const QString& value)
    : columnName(column.name)
    , value(value)
{}

InsertContainer::InsertContainer(const TColumn& column, const int& value)
    : columnName(column.name)
    , value(QString::number(value))
{}

QString InsertContainer::getColumnName() const
{
    return columnName;
}

QString InsertContainer::getValue() const
{
    return value;
}
