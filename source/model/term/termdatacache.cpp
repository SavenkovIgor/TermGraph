// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/termdatacache.h"

#include "source/CommonTools/JsonTools.h"

#include "source/Text/TextTools.h"

// Initialization order is important!
TermDataCache::TermDataCache(const TermData& info)
    : mTerms{QStringList() << info.term}
    , mLinksDefinition(info.definition)
    , mArea(KnowledgeArea::from(info.knowledgeArea))
{
    assert(!mTerms.value().empty());

    mLowerTerms.setBinding([this]() {
        QStringList ret;
        for (const auto& termOrSym : mTerms.value()) {
            ret.push_back(termOrSym.toLower());
        }
        return ret;
    });

    preferredSize.setBinding([this]() -> QSizeF {
        int  spacing = 8;
        auto terms   = mTerms.value();

        QSizeF termsSize;

        // If term has synonyms, it must be one line
        int whProportion = hasSynonyms() ? 10000 : 5;

        for (const auto& term : terms) {
            auto termSize = TextTools::preferredTextSize(term, whProportion);
            termsSize.setWidth(std::max(termsSize.width(), termSize.width()));
            termsSize.setHeight(termsSize.height() + termSize.height() + spacing);
        }

        if (mArea != KnowledgeArea::Type::None) {
            termsSize += QSizeF(16, 0);
        }

        if (mLinksDefinition.text().isEmpty()) {
            return termsSize;
        }

        auto defWithoutLinks = mLinksDefinition.toPlainString();
        auto defSize         = TextTools::preferredTextSize(defWithoutLinks, 5);

        auto width  = std::max(termsSize.width(), defSize.width());
        auto height = termsSize.height() + defSize.height();

        return {width, height};
    });
}

void TermDataCache::addSynonym(const QString& synonym)
{
    auto currentVal = mTerms.value();
    currentVal.push_back(synonym);
    mTerms.setValue(currentVal);
}

bool TermDataCache::isSynonym() const
{
    if (mLinksDefinition.links().size() != 1) {
        return false;
    }

    auto withoutLinks = mLinksDefinition.replaceLink(0, "");
    return withoutLinks.simplified().isEmpty();
}

bool TermDataCache::hasSynonyms() const { return mTerms.value().size() > 1; }

QString TermDataCache::definition() const { return mLinksDefinition.text(); }

const Link::List& TermDataCache::links() const { return mLinksDefinition.links(); }

QString TermDataCache::termAndDefinition(bool decorated) const
{
    if (decorated) {
        auto term = "<font color=\"#00a693\">" + this->term() + "</font>";
        auto def  = mLinksDefinition.toRichText();

        return TextTools::joinTermDef(term, def);
    }
    return TextTools::joinTermDef(term(), mLinksDefinition.text());
}

KnowledgeArea::Type TermDataCache::area() const { return mArea; }
