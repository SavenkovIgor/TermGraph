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

#include "nodegadgetwrapper.h"

NodeGadgetWrapper::NodeGadgetWrapper(const NodeInfoContainer& info)
    : _info(info)
{}

bool NodeGadgetWrapper::isNull()
{
    return _info.isNull();
}

QString NodeGadgetWrapper::getUuid() const
{
    return _info.uuid.toString();
}

void NodeGadgetWrapper::setUuid(const QString& uuid)
{
    _info.uuid = QUuid(uuid);
}

QString NodeGadgetWrapper::getTerm() const
{
    return _info.term;
}

void NodeGadgetWrapper::setTerm(const QString& term)
{
    _info.term = term;
}

QString NodeGadgetWrapper::getDefinition() const
{
    return _info.definition;
}

void NodeGadgetWrapper::setDefinition(const QString& definition)
{
    _info.definition = definition;
}

QString NodeGadgetWrapper::getDescription() const
{
    return _info.description;
}

void NodeGadgetWrapper::setDescription(const QString& description)
{
    _info.description = description;
}

QString NodeGadgetWrapper::getExamples() const
{
    return _info.examples;
}

void NodeGadgetWrapper::setExamples(const QString& examples)
{
    _info.examples = examples;
}

QString NodeGadgetWrapper::getWikiUrl() const
{
    return _info.wikiUrl;
}

void NodeGadgetWrapper::setWikiUrl(const QString& wikiUrl)
{
    _info.wikiUrl = wikiUrl;
}

QString NodeGadgetWrapper::getWikiImage() const
{
    return _info.wikiImage;
}

void NodeGadgetWrapper::setWikiImage(const QString& wikiImage)
{
    _info.wikiImage = wikiImage;
}

QString NodeGadgetWrapper::getGroupUuid() const
{
    return _info.groupUuid.toString();
}

void NodeGadgetWrapper::setGroupUuid(const QString& groupUuid)
{
    _info.groupUuid = QUuid(groupUuid);
}

QString NodeGadgetWrapper::getLastEdit() const
{
    return _info.lastEdit.toString("dd MMM yyyy hh:mm:ss t");
}

NodeInfoContainer NodeGadgetWrapper::unwrap() const
{
    return _info;
}
