// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QSizeF>
#include <QString>

#include "source/TermDataInterface/TermData.h"

#include "source/helpers/link/Link.h"
#include "source/helpers/link/LinksString.h"
#include "source/model/enums.h"

export module TermDataCache;

import TextModule.TextTools;
import LinksDecorator;

// Mostly const wrapper over TermData + some additional stuff
export class TermDataCache
{
public:
    // Initialization order is important!
    explicit TermDataCache(const TermData& info)
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

            auto defWithoutLinks = LinksDecorator::toPlainString(mLinksDefinition);
            auto defSize         = TextTools::preferredTextSize(defWithoutLinks, 5);

            auto width  = std::max(termsSize.width(), defSize.width());
            auto height = termsSize.height() + defSize.height();

            return {width, height};
        });
    }

    ~TermDataCache() = default;

    inline QString     term() const { return mTerms.value().first(); }
    inline QStringList termAndSynonyms() const { return mTerms.value(); }

    // Synonyms are terms with one link to another term
    void addSynonym(const QString& synonym)
    {
        auto currentVal = mTerms.value();
        currentVal.push_back(synonym);
        mTerms.setValue(currentVal);
    }

    bool isSynonym() const
    {
        if (mLinksDefinition.links().size() != 1) {
            return false;
        }

        auto withoutLinks = mLinksDefinition.replaceLink(0, "");
        return withoutLinks.simplified().isEmpty();
    }

    bool hasSynonyms() const { return mTerms.value().size() > 1; }

    QString            definition() const { return mLinksDefinition.text(); }
    inline QString     lowerTerm() const { return mLowerTerms.value().first(); }
    inline QStringList lowerTermAndSynonyms() const { return mLowerTerms.value(); }

    const Link::List& links() const { return mLinksDefinition.links(); }

    QString termAndDefinition(bool decorated = false) const
    {
        if (decorated) {
            auto term = "<font color=\"#00a693\">" + this->term() + "</font>";
            auto def  = LinksDecorator::toRichText(mLinksDefinition);

            return TextTools::joinTermDef(term, def);
        }
        return TextTools::joinTermDef(term(), mLinksDefinition.text());
    }

    KnowledgeArea::Type area() const { return mArea; }

    QProperty<QSizeF> preferredSize;

private:
    QProperty<QStringList>    mTerms;
    QProperty<QStringList>    mLowerTerms;
    const LinksString         mLinksDefinition;
    const KnowledgeArea::Type mArea;
};
