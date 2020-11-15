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

#include "source/Helpers/link/linksdecorator.h"
#include "source/Helpers/link/linkutils.h"
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
    mCurrentTerm       = termWrapper;
    mCurrentDefinition = mCurrentTerm.getDefinition();

    mLinksText.reset(new LinksText(mCurrentDefinition));

    mLinkIndex = linkCount() >= 1 ? 0 : -1;
    emit indexChanged();
    mReplacePreparations.clear();
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

void LinksHardeningManager::hardenLink(QUuid uuid)
{
    mReplacePreparations[mLinkIndex] = uuid;
    emit indexChanged();
}

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

NodeGadgetWrapper LinksHardeningManager::appliedReplacement()
{
    auto ret        = mCurrentTerm;
    auto definition = applyLinkUuids(mCurrentDefinition, mReplacePreparations);
    ret.setDefinition(definition);

    return NodeGadgetWrapper(ret);
}

void LinksHardeningManager::updateNearestVariants()
{
    beginResetModel();
    mLastNearestVariants = getNearestVariants();
    endResetModel();
}

TextLink::List LinksHardeningManager::currentLinks() const
{
    if (mLinksText.isNull())
        return TextLink::List();

    return mLinksText->links();
}

bool LinksHardeningManager::isValidIndex() const { return 0 <= mLinkIndex && mLinkIndex < linkCount(); }

bool LinksHardeningManager::canMoveNext() const { return 0 <= mLinkIndex && mLinkIndex < linkCount() - 1; }

bool LinksHardeningManager::canMovePrev() const { return 1 <= mLinkIndex && mLinkIndex < linkCount(); }

QString LinksHardeningManager::currentLinkText()
{
    if (!isValidIndex() || !mLinksText)
        return "";

    return mLinksText->links()[mLinkIndex].fullLink().toString();
}

QString LinksHardeningManager::definitionWithHighlightedLink() const
{
    if (!isValidIndex() || !mLinksText)
        return "";

    assert(!mCurrentTerm.isNull());

    return appliedLinkFixationText();
}

int LinksHardeningManager::linkCount() const
{
    if (!mLinksText || mLinksText.isNull())
        return 0;

    return mLinksText->links().size();
}

QString LinksHardeningManager::appliedLinkFixationText() const
{
    if (!isValidIndex())
        return "";

    auto ret = applyLinkUuids(mCurrentDefinition, mReplacePreparations);

    auto links = LinksText(ret);

    auto decorColor = []([[maybe_unused]] int index, const TextLink &link) {
        if (link.hasUuid())
            return QColor("#90ee90");

        return QColor("white");
    };

    auto bgColor = [currentIndex = mLinkIndex](int index, [[maybe_unused]] const TextLink &link) {
        if (currentIndex == index)
            return QColor("#6f6f6f");

        return QColor("transparent");
    };

    auto decorator = LinksDecorator(links, decorColor, bgColor);
    return decorator.apply(LinksDecoratorMode::Insert);
}

QString LinksHardeningManager::applyLinkUuids(QString stringWithLinks, QMap<int, QUuid> uuidsToApply)
{
    for (auto &[index, uuid] : uuidsToApply.toStdMap()) {
        auto linksText = LinksText(stringWithLinks);

        TextLink oldLink          = linksText.links()[index];
        auto [cuttedLink, cutPos] = oldLink.cutted();

        const auto newLink = oldLink.createLinkWithUuid(uuid);
        cuttedLink.insert(cutPos, newLink);
        stringWithLinks = cuttedLink;
    }

    return stringWithLinks;
}

TextLink LinksHardeningManager::currentLink() const
{
    assert(!mCurrentTerm.isNull());
    assert(isValidIndex());
    return currentLinks()[mLinkIndex];
}
