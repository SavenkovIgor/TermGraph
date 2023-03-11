// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSizeF>
#include <QString>

#include <TermDataInterface/TermData.h>

#include "source/helpers/link/link.h"
#include "source/helpers/link/linksstring.h"


// Const wrapper over TermData + some additional stuff
class TermDataCache
{
public:
    explicit TermDataCache(const TermData& info);
    ~TermDataCache() = default;

    inline QString term() const { return mTerms.front(); }
    QString definition() const;
    QString lowerTerm() const;
    QSizeF  preferredSize() const;

    const Link::List& links() const;

    QString termAndDefinition(bool decorated = false) const;

    bool isSynonym() const;

private: // Members
    const std::vector<QString> mTerms;
    const QString              mLowerTerm;
    const QSizeF               mTermSize;
    const LinksString          mLinksDefinition;
};
