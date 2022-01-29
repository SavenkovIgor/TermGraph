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

#include <TermDataInterface/TermData.h>

class TermDataWrapper
{
    Q_GADGET

    // clang-format off
    Q_PROPERTY(QString uuid              READ getUuid        WRITE setUuid)
    Q_PROPERTY(QString term              READ getTerm        WRITE setTerm)
    Q_PROPERTY(QString definition        READ getDefinition  WRITE setDefinition)
    Q_PROPERTY(QString description       READ getDescription WRITE setDescription)
    Q_PROPERTY(QString examples          READ getExamples    WRITE setExamples)
    Q_PROPERTY(QString wikiUrl           READ getWikiUrl     WRITE setWikiUrl)
    Q_PROPERTY(QString wikiImage         READ getWikiImage   WRITE setWikiImage)
    Q_PROPERTY(QString groupUuid         READ getGroupUuid   WRITE setGroupUuid)
    Q_PROPERTY(QString lastEditDecorated READ getLastEditDecorated)
    Q_PROPERTY(QString lastEdit          READ getLastEdit)
    // clang-format on

public:
    TermDataWrapper()
        : mData{.uuid        = std::nullopt,
                .term        = "",
                .definition  = "",
                .description = "",
                .examples    = "",
                .wikiUrl     = "",
                .wikiImage   = "",
                .groupUuid   = GroupUuid::generate(),
                .lastEdit    = QDateTime::currentDateTime()}
    {}
    TermDataWrapper(const TermData& info)
        : mData(info)
    {}

    Q_INVOKABLE inline bool isNull() const { return mData.isNull(); }

    inline QString getUuid() const { return mData.uuid ? mData.uuid->toString() : ""; }
    inline void    setUuid(const QString& uuid) { mData.uuid = TermUuid::create(uuid); }

    inline QString getTerm() const { return mData.term; }
    inline void    setTerm(const QString& term) { mData.term = term; }

    inline QString getDefinition() const { return mData.definition; }
    inline void    setDefinition(const QString& definition) { mData.definition = definition; }

    inline QString getDescription() const { return mData.description; }
    inline void    setDescription(const QString& description) { mData.description = description; }

    inline QString getExamples() const { return mData.examples; }
    inline void    setExamples(const QString& examples) { mData.examples = examples; }

    inline QString getWikiUrl() const { return mData.wikiUrl; }
    inline void    setWikiUrl(const QString& wikiUrl) { mData.wikiUrl = wikiUrl; }

    inline QString getWikiImage() const { return mData.wikiImage; }
    inline void    setWikiImage(const QString& wikiImage) { mData.wikiImage = wikiImage; }

    inline QString getGroupUuid() const { return mData.groupUuid.toString(); }
    inline void    setGroupUuid(const QString& groupUuid)
    {
        auto gUuid = GroupUuid::create(groupUuid);
        assert(gUuid);
        mData.groupUuid = *gUuid;
    }

    // Not editable...
    inline QString getLastEdit() const { return mData.lastEdit.toString(Qt::ISODate); }
    inline QString getLastEditDecorated() const { return mData.lastEdit.toString("dd MMM yyyy hh:mm:ss t"); }

private:
    TermData mData;
};

Q_DECLARE_METATYPE(TermDataWrapper)
