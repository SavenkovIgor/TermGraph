// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSizeF>
#include <QString>

#include <TermDataInterface/TermData.h>

#include "source/helpers/link/link.h"
#include "source/helpers/link/linksstring.h"
#include "source/model/enums.h"

// Mostly const wrapper over TermData + some additional stuff
class TermDataCache
{
public:
    explicit TermDataCache(const TermData& info);
    ~TermDataCache() = default;

    inline QString     term() const { return mTerms.value().first(); }
    inline QStringList termAndSynonyms() const { return mTerms.value(); }

    // Synonyms are terms with one link to another term
    void addSynonym(const QString& synonym);
    bool isSynonym() const;
    bool hasSynonyms() const;

    QString            definition() const;
    inline QString     lowerTerm() const { return mLowerTerms.value().first(); }
    inline QStringList lowerTermAndSynonyms() const { return mLowerTerms.value(); }

    const Link::List& links() const;

    QString termAndDefinition(bool decorated = false) const;

    KnowledgeArea::Type area() const;

    QProperty<QSizeF> preferredSize;

private:
    QProperty<QStringList>    mTerms;
    QProperty<QStringList>    mLowerTerms;
    const LinksString         mLinksDefinition;
    const KnowledgeArea::Type mArea;
};
