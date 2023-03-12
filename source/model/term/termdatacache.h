// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSizeF>
#include <QString>

#include <TermDataInterface/TermData.h>

#include "source/helpers/link/link.h"
#include "source/helpers/link/linksstring.h"


// Mostly const wrapper over TermData + some additional stuff
class TermDataCache
{
public:
    explicit TermDataCache(const TermData& info);
    ~TermDataCache() = default;

    inline QString term() const { return mTerms.front(); }
    inline const std::vector<QString>& termAndSynonyms() const { return mTerms; }

    // Synonyms are terms with one link to another term
    void addSynonym(const QString& synonym);
    bool isSynonym() const;
    bool hasSynonyms() const;

    QString definition() const;
    QString lowerTerm() const;
    QSizeF  preferredSize() const;

    const Link::List& links() const;

    QString termAndDefinition(bool decorated = false) const;

private: // Members
    std::vector<QString> mTerms;
    const QString        mLowerTerm;
    const QSizeF         mTermSize;
    const LinksString    mLinksDefinition;
};
