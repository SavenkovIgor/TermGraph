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

#pragma once

#include <QObject>

#include "source/Model/Termin/nodeinfocontainer.h"

class NodeGadgetWrapper
{
    Q_GADGET

public:
    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid)
    Q_PROPERTY(QString term READ getTerm WRITE setTerm)
    Q_PROPERTY(QString definition READ getDefinition WRITE setDefinition)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription)
    Q_PROPERTY(QString examples READ getExamples WRITE setExamples)
    Q_PROPERTY(QString wikiUrl READ getWikiUrl WRITE setWikiUrl)
    Q_PROPERTY(QString wikiImage READ getWikiImage WRITE setWikiImage)
    Q_PROPERTY(QString groupUuid READ getGroupUuid WRITE setGroupUuid)
    Q_PROPERTY(QString lastEdit READ getLastEdit)

    NodeGadgetWrapper() = default;
    NodeGadgetWrapper(const NodeInfoContainer& info);

    Q_INVOKABLE bool isNull();

    // Uuid
    QString getUuid() const;
    void    setUuid(const QString& uuid);

    // Term
    QString getTerm() const;
    void    setTerm(const QString& term);

    // Definition
    QString getDefinition() const;
    void    setDefinition(const QString& definition);

    // Description
    QString getDescription() const;
    void    setDescription(const QString& description);

    // Examples
    QString getExamples() const;
    void    setExamples(const QString& examples);

    // WikiUrl
    QString getWikiUrl() const;
    void    setWikiUrl(const QString& wikiUrl);

    // WikiImage
    QString getWikiImage() const;
    void    setWikiImage(const QString& wikiImage);

    // GroupUuid
    QString getGroupUuid() const;
    void    setGroupUuid(const QString& groupUuid);

    // LastEdit
    // Not editable...
    QString getLastEdit() const;

    NodeInfoContainer unwrap() const;

private:
    NodeInfoContainer _info;
};

Q_DECLARE_METATYPE(NodeGadgetWrapper)
