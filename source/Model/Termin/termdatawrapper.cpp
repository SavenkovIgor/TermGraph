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

#include "source/Model/Termin/termdatawrapper.h"

TermDataWrapper::TermDataWrapper(const TermData& info)
    : mInfo(info)
{}

bool TermDataWrapper::isNull() const { return mInfo.isNull(); }

QString TermDataWrapper::getUuid() const { return mInfo.uuid.toString(); }

void TermDataWrapper::setUuid(const QString& uuid) { mInfo.uuid = QUuid(uuid); }

QString TermDataWrapper::getTerm() const { return mInfo.term; }

void TermDataWrapper::setTerm(const QString& term) { mInfo.term = term; }

QString TermDataWrapper::getDefinition() const { return mInfo.definition; }

void TermDataWrapper::setDefinition(const QString& definition) { mInfo.definition = definition; }

QString TermDataWrapper::getDescription() const { return mInfo.description; }

void TermDataWrapper::setDescription(const QString& description) { mInfo.description = description; }

QString TermDataWrapper::getExamples() const { return mInfo.examples; }

void TermDataWrapper::setExamples(const QString& examples) { mInfo.examples = examples; }

QString TermDataWrapper::getWikiUrl() const { return mInfo.wikiUrl; }

void TermDataWrapper::setWikiUrl(const QString& wikiUrl) { mInfo.wikiUrl = wikiUrl; }

QString TermDataWrapper::getWikiImage() const { return mInfo.wikiImage; }

void TermDataWrapper::setWikiImage(const QString& wikiImage) { mInfo.wikiImage = wikiImage; }

QString TermDataWrapper::getGroupUuid() const { return mInfo.groupUuid.toString(); }

void TermDataWrapper::setGroupUuid(const QString& groupUuid) { mInfo.groupUuid = QUuid(groupUuid); }

QString TermDataWrapper::getLastEdit() const { return mInfo.lastEdit.toString("dd MMM yyyy hh:mm:ss t"); }

TermData TermDataWrapper::unwrap() const { return mInfo; }
