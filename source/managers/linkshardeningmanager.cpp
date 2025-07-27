// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/managers/linkshardeningmanager.h"

#include <ranges>

import CommonTools.HandyTypes;
import link.LinkTools;
import LinksDecorator;
import PaintedTerm;
import TermGroup;

namespace rng = std::ranges;

LinksHardeningManager::LinksHardeningManager(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &LinksHardeningManager::indexChanged, this, &LinksHardeningManager::updateNearestVariants);
}

QHash<int, QByteArray> LinksHardeningManager::roleNames() const
{
    static QHash<int, QByteArray> ret = {{Role::Uuid, "termUuid"},
                                         {Role::Text, "termText"},
                                         {Role::Distance, "termDistance"}};
    return ret;
}

QVariant LinksHardeningManager::data(const QModelIndex &index, int role) const
{
    auto row = index.row();

    if (row < 0 || row >= asInt(mLastNearestVariants.size())) {
        return {};
    }

    const auto &data = mLastNearestVariants[static_cast<SearchResultList::size_type>(row)];
    switch (role) {
    case Role::Uuid: return std::get<0>(data).toString();
    case Role::Text: return std::get<1>(data);
    case Role::Distance: return std::get<2>(data);
    }

    return {};
}

int LinksHardeningManager::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    return asInt(mLastNearestVariants.size());
}

void LinksHardeningManager::setGroup(QtTermGroup *group)
{
    assert(group != nullptr);
    mCurrentGroup = group->termGroup();
}

void LinksHardeningManager::setTerm(TermDataWrapper termWrapper)
{
    assert(!termWrapper.isNull());
    mCurrentTerm       = termWrapper;
    mCurrentDefinition = mCurrentTerm.getDefinition();

    mLinksString = std::make_unique<LinksString>(mCurrentDefinition);

    mLinkIndex = linkCount() >= 1 ? 0 : -1;
    mReplacePreparations.clear();
    emit indexChanged();
}

void LinksHardeningManager::prev()
{
    if (canMovePrev()) {
        mLinkIndex--;
        emit indexChanged();
    }
}

void LinksHardeningManager::next()
{
    if (canMoveNext()) {
        mLinkIndex++;
        emit indexChanged();
    }
}

void LinksHardeningManager::hardenLink(QUuid uuid)
{
    mReplacePreparations[mLinkIndex] = uuid;
    emit indexChanged();
    next();
}

LinksHardeningManager::SearchResultList LinksHardeningManager::getNearestVariants(int limit)
{
    if (mCurrentGroup == nullptr) {
        return {};
    }

    if (mCurrentTerm.isNull() || !isValidIndex() || !mLinksString) {
        return {};
    }

    std::vector<std::pair<int, PaintedTerm *>> distances;

    Link::List currentLinks = mLinksString->links();

    // Getting distances
    for (const auto &term : mCurrentGroup->terms()) {
        if (QUuid(mCurrentTerm.getUuid()) == term->data().uuid) {
            continue;
        }

        auto currentLink = currentLinks[Link::asListSize(mLinkIndex)];

        auto distance = LinkTools::levDistance(currentLink.textLower(), term->data().term.toLower());
        distances.push_back(std::pair(distance, term.get()));
    }

    rng::sort(distances, [](auto pair1, auto pair2) { return pair1.first < pair2.first; });

    auto min = static_cast<decltype(distances)::size_type>(std::min(limit, asInt(distances.size())));

    SearchResultList ret;

    for (decltype(distances)::size_type i = 0; i < min; i++) {
        const auto &item     = distances[i];
        const auto &uuid     = item.second->data().uuid;
        const auto &term     = item.second->data().term;
        const auto &distance = item.first;

        if (uuid) {
            ret.push_back({*uuid, term, distance});
        }
    }

    return ret;
}

TermDataWrapper LinksHardeningManager::appliedReplacement()
{
    auto ret = mCurrentTerm;
    ret.setDefinition(applyLinkUuids(mCurrentDefinition, mReplacePreparations));
    return ret;
}

void LinksHardeningManager::updateNearestVariants()
{
    beginResetModel();
    mLastNearestVariants = getNearestVariants();
    endResetModel();
}

bool LinksHardeningManager::isValidIndex() const { return 0 <= mLinkIndex && mLinkIndex < linkCount(); }

bool LinksHardeningManager::canMoveNext() const { return 0 <= mLinkIndex && mLinkIndex < linkCount() - 1; }

bool LinksHardeningManager::canMovePrev() const { return 1 <= mLinkIndex && mLinkIndex < linkCount(); }

QString LinksHardeningManager::currentLinkText()
{
    if (!isValidIndex() || !mLinksString) {
        return "";
    }

    return mLinksString->links()[Link::asListSize(mLinkIndex)].fullLink().toString();
}

QString LinksHardeningManager::definitionWithHighlightedLink() const
{
    if (!isValidIndex() || !mLinksString) {
        return "";
    }

    assert(!mCurrentTerm.isNull());

    return appliedLinkFixationText();
}

int LinksHardeningManager::linkCount() const
{
    if (!mLinksString) {
        return 0;
    }

    return asInt(mLinksString->links().size());
}

QString LinksHardeningManager::appliedLinkFixationText() const
{
    if (!isValidIndex()) {
        return "";
    }

    auto ret = applyLinkUuids(mCurrentDefinition, mReplacePreparations);

    auto links = LinksString(ret);

    auto decorColor = []([[maybe_unused]] int index, const Link &link) {
        if (link.hasUuid()) {
            return QColor::fromString("#90ee90");
        }

        return QColor::fromString("white");
    };

    auto bgColor = [currentIndex = mLinkIndex](int index, [[maybe_unused]] const Link &link) {
        if (currentIndex == index) {
            return QColor::fromString("#6f6f6f");
        }

        return QColor::fromString("transparent");
    };

    auto decorator = LinksDecorator(links, decorColor, bgColor);
    return decorator.apply(LinksDecoratorMode::Insert);
}

QString LinksHardeningManager::applyLinkUuids(QString stringWithLinks, QMap<int, QUuid> uuidsToApply)
{
    for (auto &[index, uuid] : uuidsToApply.toStdMap()) {
        auto linksString = LinksString(stringWithLinks);

        Link oldLink              = linksString.links()[Link::asListSize(index)];
        auto [cuttedLink, cutPos] = oldLink.cutted();

        const auto newLink = oldLink.createLinkWithUuid(uuid);
        cuttedLink.insert(cutPos, newLink);
        stringWithLinks = cuttedLink;
    }

    return stringWithLinks;
}
