// Copyright © 2016-2022. Savenkov Igor
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

    QString lowerTerm() const;
    QString decoratedTerm() const;
    QSizeF  decoratedTermSize() const;

    const Link::List& links() const;

    QString termAndDefinition(bool decorated = false) const;

private: // Methods
    static QString getDecoratedTerm(const QString& term);
    static QSizeF  getTermSize(const QString& decoratedTerm);

private: // Members
    const QString     mTerm;
    const QString     mDefinition;
    const QString     mLowerTerm;
    const QString     mDecoratedTerm;
    const QSizeF      mDecoratedTermSize;
    const LinksString mLinksDefinition;
};
