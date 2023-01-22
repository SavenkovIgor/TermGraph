// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

#include <TermDataInterface/TermData.h>

class TermDataWrapper
{
    Q_GADGET
    QML_ANONYMOUS

    // clang-format off
    Q_PROPERTY(QString uuid              READ getUuid        WRITE setUuid        FINAL)
    Q_PROPERTY(QString term              READ getTerm        WRITE setTerm        FINAL)
    Q_PROPERTY(QString definition        READ getDefinition  WRITE setDefinition  FINAL)
    Q_PROPERTY(QString description       READ getDescription WRITE setDescription FINAL)
    Q_PROPERTY(QString examples          READ getExamples    WRITE setExamples    FINAL)
    Q_PROPERTY(QString wikiUrl           READ getWikiUrl     WRITE setWikiUrl     FINAL)
    Q_PROPERTY(QString wikiImage         READ getWikiImage   WRITE setWikiImage   FINAL)
    Q_PROPERTY(QString groupUuid         READ getGroupUuid   WRITE setGroupUuid   FINAL)
    Q_PROPERTY(QString lastEditDecorated READ getLastEditDecorated FINAL)
    Q_PROPERTY(QString lastEdit          READ getLastEdit          FINAL)
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
