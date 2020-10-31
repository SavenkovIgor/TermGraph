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

#include "source/Managers/linkshardeningmanager.h"

#include "source/Helpers/linkutils.h"
#include "source/Model/Termin/infoterm.h"

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

    if (row < 0 || row >= static_cast<int>(mLastNearestVariants.size()))
        return {};

    const auto &data = mLastNearestVariants[row];
    switch (role) {
    case Role::Uuid: return std::get<0>(data).toString();
    case Role::Text: return std::get<1>(data);
    case Role::Distance: return std::get<2>(data);
    }

    return {};
}

int LinksHardeningManager::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    return mLastNearestVariants.size();
}

void LinksHardeningManager::setGroup(TermGroup *group)
{
    assert(group != nullptr);
    mCurrentGroup = group;
}

void LinksHardeningManager::setTerm(NodeGadgetWrapper termWrapper)
{
    assert(!termWrapper.isNull());
    mCurrentTerm = termWrapper;
    mLinksCount  = currentLinks().size();
    mLinkIndex   = mLinksCount >= 1 ? 0 : -1;
    emit indexChanged();
    replacePreparations.clear();
}

bool LinksHardeningManager::hasLinksForHardening() const
{
    for (auto link : currentLinks())
        if (!link.hasUuid())
            return true;

    return false;
}

int LinksHardeningManager::hardeningCount() const
{
    int count = 0;
    for (auto link : currentLinks())
        if (!link.hasUuid())
            count++;

    return count;
}

bool LinksHardeningManager::prev()
{
    if (canMovePrev()) {
        mLinkIndex--;
        emit indexChanged();
        return true;
    }

    return false;
}

bool LinksHardeningManager::next()
{
    if (canMoveNext()) {
        mLinkIndex++;
        emit indexChanged();
        return true;
    }

    return false;
}

void LinksHardeningManager::hardenLink(QUuid uuid) { replacePreparations[mLinkIndex] = uuid; }

#include <QDebug>

LinksHardeningManager::SearchResultList LinksHardeningManager::getNearestVariants(int limit)
{
    if (mCurrentGroup == nullptr)
        return {};

    if (mCurrentTerm.isNull())
        return {};

    std::vector<std::pair<int, PaintedTerm *>> distances;

    // Getting distances
    for (auto *term : mCurrentGroup->nodes()) {
        if (QUuid(mCurrentTerm.getUuid()) == term->info().uuid)
            continue;

        auto distance = LinkUtils::getLevDistance(currentLink().text(), term->info().term);
        distances.push_back(std::pair(distance, term));
    }

    auto sortCondition = [](auto pair1, auto pair2) { return pair1.first < pair2.first; };
    std::sort(distances.begin(), distances.end(), sortCondition);

    auto min = std::min(limit, static_cast<int>(distances.size()));

    SearchResultList ret;

    for (int i = 0; i < min; i++) {
        const auto &item     = distances[i];
        const auto &uuid     = item.second->info().uuid;
        const auto &term     = item.second->info().term;
        const auto &distance = item.first;

        ret.push_back({uuid, term, distance});
    }

    return ret;
}

NodeGadgetWrapper LinksHardeningManager::applyReplacement()
{
    auto definition = currentDefinition().toString();
    for (auto &replace : replacePreparations.toStdMap()) {
        auto txtRange = LinkUtils::linkAt(definition, replace.first);

        InnerLink  oldLink(definition, txtRange);
        const auto newLink = oldLink.createLinkWithUuid(replace.second);

        const auto startIndex = txtRange.left().pos();
        const auto linkSize   = txtRange.size();

        definition.remove(startIndex, linkSize);
        definition.insert(startIndex, newLink);
    }

    auto ret = mCurrentTerm;
    ret.setDefinition(definition);

    return NodeGadgetWrapper(ret);
}

void LinksHardeningManager::updateNearestVariants()
{
    beginResetModel();
    mLastNearestVariants = getNearestVariants();
    endResetModel();
}

QStringView LinksHardeningManager::currentDefinition() const
{
    assert(!mCurrentTerm.isNull());
    return mCurrentTerm.getDefinition();
}

InnerLink::List LinksHardeningManager::currentLinks() const
{
    assert(!mCurrentTerm.isNull());
    return InfoTerm::getInnerLinks(mCurrentTerm.getDefinition());
}

bool LinksHardeningManager::isValidIndex() const { return 0 <= mLinkIndex && mLinkIndex < mLinksCount; }

bool LinksHardeningManager::canMoveNext() const { return 0 <= mLinkIndex && mLinkIndex < mLinksCount - 1; }

bool LinksHardeningManager::canMovePrev() const { return 1 <= mLinkIndex && mLinkIndex < mLinksCount; }

QString LinksHardeningManager::currentLinkText() const
{
    if (!isValidIndex())
        return "";

    assert(!mCurrentTerm.isNull());

    auto links = currentLinks();
    return links[mLinkIndex].fullLink().toString();
}

InnerLink LinksHardeningManager::currentLink() const
{
    assert(!mCurrentTerm.isNull());
    assert(isValidIndex());
    return currentLinks()[mLinkIndex];
}
