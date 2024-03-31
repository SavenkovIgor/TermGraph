// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/group/termgroup.h"

#include <ranges>

#include <QElapsedTimer>
#include <QThread>

#include <source/CommonTools/HandyTypes.h>
#include <Text/Fonts.h>

#include "source/helpers/appstyle.h"
#include "source/helpers/link/linktools.h"

namespace rng = std::ranges;

LinkTermDistanceCache TermGroup::linkCache;

TermGroup::TermGroup(const GroupSummary& info, const TermData::List& termData, QObject* parent)
    : QObject(parent)
    , mInfo(info)
{
    assert(info.uuid);

    for (const auto& node : termData) {
        Q_ASSERT_X(node.groupUuid == this->uuid(), Q_FUNC_INFO, "Node group error");
    }

    mOrphansRect.setParentItem(&mBaseRect);

    QElapsedTimer t;
    t.start();

    PaintedTerm::List nodes;

    // Creating nodes
    for (const auto& term : termData) {
        nodes.push_back(std::make_shared<PaintedTerm>(term));
    }

    nodes = collapseSynonyms(nodes);

    auto [ghostNodes, edges] = searchAllConnections(nodes);
    // Adding ghost nodes
    nodes.insert(nodes.end(), ghostNodes.begin(), ghostNodes.end());
    mGraphData = GraphT({.nodes = nodes, .edges = edges});

    auto subgraphs = mGraphData.bondedSubgraphs();

    for (auto subgraph : subgraphs) {
        mForests.push_back(std::make_shared<PaintedForest>(subgraph));
    }

    if (isThreadInterrupted()) {
        return;
    }

    // Removing of exceed edges
    for (auto edge : allBrokenEdges()) {
        edge->brokeEdge();
    }

    addTreeRectsToScene();

    addOrphansToParents();

    setTreeCoords();
    setOrphCoords();

    // Positioning
    updateRectsPositions();
    updateBaseRectSize();

    groupCreationTime = t.elapsed();
    qInfo() << "Group creation time:" << groupCreationTime << "ms";
}

void TermGroup::setBasePoint(QPointF pt) { mBaseRect.setPos(pt); }

QRectF TermGroup::getGroupRect() const { return mBaseRect.getRect(CoordType::scene); }

UuidList TermGroup::searchNearest(const QString& text, int limit) const
{
    QString                        searchText = text.toLower();
    std::vector<QPair<int, QUuid>> searchResults;
    // Taking distances
    for (const auto& term : mGraphData.nodeList()) {
        auto lowerTerm = term->cache().lowerTerm();

        // Exact match
        if (searchText == lowerTerm) {
            searchResults.push_back({0, term->data().uuid->get()});
            continue;
        }

        auto cuttedTerm = lowerTerm.left(searchText.size()); // Compare only left n characters

        auto acceptableLimit = asInt(asInt(cuttedTerm.size()) * 0.25);
        auto distance        = LinkTools::levDistance(cuttedTerm, searchText, acceptableLimit);

        if (distance <= acceptableLimit) {
            searchResults.push_back({distance, term->data().uuid->get()});
        }
    }

    // Sorting
    rng::sort(searchResults, [](auto s1, auto s2) { return s1.first < s2.first; });

    // Removing numbers
    UuidList ret;
    int      count = 0;
    for (auto [dist, uuid] : searchResults) {
        if (count >= limit) {
            break;
        }

        ret.push_back(uuid);

        count++;
    }

    return ret;
}

UuidList TermGroup::searchContains(const QString& text, int limit) const
{
    UuidList ret;
    auto     lowerSearch = text.toLower();

    for (const auto& term : mGraphData.nodeList()) {
        if (term->cache().lowerTerm().contains(lowerSearch)) {
            ret.push_back(term->data().uuid->get());
        }

        if (asInt(ret.size()) == limit) {
            break;
        }
    }

    return ret;
}

PaintedTerm::OptPtr TermGroup::getTerm(const QPointF& pt) const
{
    for (const auto& forest : mForests) {
        if (forest->frameRect(CoordType::scene).contains(pt)) {
            return forest->getNodeAtPoint(pt);
        }
    }

    auto orphansRect = QRectF(this->mOrphansRect.scenePos(), this->mOrphansRect.getSize());
    if (orphansRect.contains(pt)) {
        for (auto orphan : getOrphanNodes()) {
            if (orphan->getNodeRect(CoordType::scene).contains(pt)) {
                return orphan;
            }
        }
    }

    return std::nullopt;
}

PaintedTerm::OptPtr TermGroup::getTerm(const QUuid& termUuid) const
{
    for (auto term : mGraphData.nodeList()) {
        if (term->data().uuid == termUuid) {
            return term;
        }
    }

    return std::nullopt;
}

PaintedTerm::OptPtr TermGroup::getTerm(const QString& termName) const
{
    for (auto term : mGraphData.nodeList()) {
        if (term->data().term == termName) {
            return term;
        }
    }

    return std::nullopt;
}

void TermGroup::addOrphansToParents()
{
    for (const auto& node : getOrphanNodes()) {
        node->setParentItem(&mOrphansRect);
    }
}

qreal TermGroup::getGroupMinWidth()
{
    qreal width = 0.0;

    qreal groupNameWidth = getNameSize().width();
    qreal treeWidth      = getAllTreesSize().width();

    qreal orphansWidth = NodeVerticalStackTools::getNodeVerticalStackedSize(getOrphanNodes()).width();

    width = std::max(width, groupNameWidth);
    width = std::max(width, treeWidth);
    width = std::max(width, orphansWidth);

    return width;
}

void TermGroup::updateRectsPositions()
{
    qreal   vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal   hSpacer = AppStyle::Sizes::groupHorizontalSpacer;
    QPointF basePoint(QPointF(hSpacer, vSpacer));

    auto nameSize = getNameSize();

    // Setting name base point
    basePoint.ry() += nameSize.height() + vSpacer;

    // Tree calculations
    for (auto forest : mForests) {
        auto size = forest->baseSize();
        forest->rect().setPos(basePoint);
        forest->rect().setSize(size);
        basePoint.ry() += size.height() + vSpacer;
    }

    QSizeF orphansSize = getOrphansSize();

    // Orphans calculations
    mOrphansRect.setPos(basePoint);
    mOrphansRect.setSize(orphansSize);
}

void TermGroup::updateBaseRectSize()
{
    QSizeF nameSize    = getNameSize();
    QSizeF treesSize   = getAllTreesSize();
    QSizeF orphansSize = getOrphansSize();
    qreal  vSpacer     = AppStyle::Sizes::groupVerticalSpacer;
    qreal  hSpacer     = AppStyle::Sizes::groupHorizontalSpacer;

    qreal width  = 0.0;
    qreal height = 0.0;

    width = std::max({width, nameSize.width(), treesSize.width(), orphansSize.width()});
    width += hSpacer * 2;

    height += vSpacer;
    height += nameSize.height();
    if (treesSize.height() > 0) {
        height += treesSize.height() + vSpacer;
    }
    if (orphansSize.height() > 0) {
        height += orphansSize.height() + vSpacer;
    }
    height += vSpacer;

    mBaseRect.setSize(QSizeF(width, height));
}

void TermGroup::setTreeCoords()
{
    rng::for_each(mForests, [](auto f) { f->setTreeNodeCoords(); });
}

void TermGroup::setOrphCoords(qreal maxWidth)
{
    using list_size  = PaintedTerm::List::size_type;
    auto orphansList = getOrphanNodes();
    if (orphansList.empty()) {
        return;
    }

    for (list_size i = 0; i < orphansList.size(); i++) {
        list_size nMax = i;
        for (list_size j = i + 1; j < orphansList.size(); j++) {
            qreal currentWidth = orphansList[j]->getFrameRect(CoordType::zeroPoint).size().width();
            qreal maxWidth     = orphansList[nMax]->getFrameRect(CoordType::zeroPoint).size().width();
            if (currentWidth < maxWidth) {
                nMax = j;
            }
        }
        auto tmp          = orphansList[i];
        orphansList[i]    = orphansList[nMax];
        orphansList[nMax] = tmp;
    }

    QPointF pt(0, 0);
    qreal   maxHeightInRow = 0.0;
    qreal   groupMinWidth  = getGroupMinWidth();
    maxWidth               = qBound(10.0, groupMinWidth, 2000.0);

    for (const auto& currNode : orphansList) {
        QSizeF nodeSize = currNode->getNodeRect(CoordType::zeroPoint).size();

        if (pt.x() + nodeSize.width() > maxWidth) {
            pt.ry() += maxHeightInRow + AppStyle::Sizes::orphansVerticalSpacer;
            //Переходим на следующий ряд
            maxHeightInRow = 0;
            pt.rx()        = 0;
        }

        currNode->setPos(pt);
        pt.rx() += nodeSize.width() + AppStyle::Sizes::orphansHorizontalSpacer;

        maxHeightInRow = std::max(maxHeightInRow, nodeSize.height());
    }
}

PaintedTerm::List TermGroup::collapseSynonyms(PaintedTerm::List nodes)
{
    auto synonymCount = [](const auto& terms) {
        return rng::count_if(terms, [](const auto& term) { return term->cache().isSynonym(); });
    };

    for (;;) {
        auto startCount = synonymCount(nodes);

        auto exactMatchCache = createExactLinkMatchCacheFor(nodes);

        for (const auto& node : nodes) {
            if (!node->cache().isSynonym()) {
                continue;
            }

            auto synonymTo = node->cache().links().front().text().toString();
            synonymTo      = synonymTo.toLower();

            if (exactMatchCache.contains(synonymTo)) {
                auto synonymParent = exactMatchCache[synonymTo];

                for (const auto& term : node->cache().termAndSynonyms()) {
                    synonymParent->addSynonym(term);
                }

                // Removing this node
                nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
                break;
            }
        }

        auto endCount = synonymCount(nodes);

        // We make this step until no synonyms left
        if (startCount == endCount) {
            break;
        }
    }

    return nodes;
}

void TermGroup::addTreeRectsToScene()
{
    for (auto forest : mForests) {
        forest->rect().setParentItem(&mBaseRect);
    }
}

QSizeF TermGroup::getNameSize() const { return Fonts::metrics(name(), Fonts::weightFont()); }

QString TermGroup::qmlUuid() const { return uuid().toString(); }

PaintedTerm::List TermGroup::getRootNodes() const
{
    PaintedTerm::List ret;

    for (const auto& forest : mForests) {
        for (const auto& root : forest->roots()) {
            ret.push_back(root);
        }
    }

    return ret;
}

QSizeF TermGroup::getAllTreesSize()
{
    SizeList sizeList;

    for (const auto& forest : mForests) {
        sizeList.push_back(forest->baseSize());
    }

    auto totalSize = sizeList.totalStackedSize(Qt::Vertical);

    if (!mForests.empty()) {
        totalSize.rheight() += (asInt(mForests.size()) - 1) * AppStyle::Sizes::groupVerticalSpacer;
    }

    return totalSize;
}

TermGroup::SearchConnectionResult TermGroup::searchAllConnections(const PaintedTerm::List& terms)
{
    SearchConnectionResult ret;

    // Pre-heating of cache with exact terms match
    auto exactMatchCache = createExactLinkMatchCacheFor(terms);
    auto termUuids       = createUuidCacheFor(terms);

    static int counter = 0;

    // Compare everything with everything
    for (auto node : terms) {
        for (const auto& link : node->cache().links()) {
            Opt<PaintedTerm::Ptr> foundNode = std::nullopt;

            if (!foundNode) {
                if (link.hasUuid()) {
                    if (termUuids.contains(link.uuid())) {
                        foundNode = termUuids[link.uuid()];
                    }
                }
            }

            // If we have same search earlier this cycle
            if (!foundNode && exactMatchCache.contains(link.textLower())) {
                foundNode = exactMatchCache[link.textLower()];
            }

            if (!foundNode) {
                foundNode = findLinkTarget(link.textLower(), terms);
            }

            if (!foundNode) {
                foundNode = findLinkTarget(link.textLower(), ret.ghostTerms);
            }

            if (foundNode) {
                if (foundNode.value() != node) { // TODO: Real case, need check
                    ret.edges.push_back(std::make_shared<PaintedEdge>(foundNode.value(), node));
                    exactMatchCache.insert(link.textLower(), foundNode.value());
                }
            } else {
                auto term = link.text().toString();
                term[0]   = term[0].toUpper();

                assert(mInfo.uuid.has_value());
                auto ghostData = TermData::createGhost(term, mInfo.uuid.value());
                auto ghostTerm = std::make_shared<PaintedTerm>(ghostData);
                ghostTerm->setGhost(true);
                ret.ghostTerms.push_back(ghostTerm);
                auto ghostEdge = std::make_shared<PaintedEdge>(ghostTerm, node);
                ret.edges.push_back(ghostEdge);
            }

            counter++;
            if (counter % 20 == 0) {
                if (isThreadInterrupted()) {
                    return ret;
                }
            }
        }
    }

    return ret;
}

Opt<PaintedTerm::Ptr> TermGroup::findLinkTarget(const QString& link, const PaintedTerm::List& terms)
{
    Opt<PaintedTerm::Ptr> targetTerm = std::nullopt;

    int minDistance = 100000;

    Opt<int> optionalResult;

    for (auto node : terms) {
        auto termName = node->cache().lowerTerm();

        if (!LinkTools::linkAndTermSimilarWordDistance(link, termName)) {
            continue;
        }

        auto cacheMatch = linkCache.get(link, termName);
        if (cacheMatch) {
            optionalResult = cacheMatch.value();
        } else {
            optionalResult = LinkTools::linkAndTermDistance(link, termName, minDistance);
            linkCache.add(link, termName, optionalResult);
        }

        if (optionalResult) {
            if (optionalResult.value() == 0) { // Already best match, no need to count further
                return node;
            }

            if (optionalResult.value() < minDistance) {
                minDistance = optionalResult.value();
                targetTerm  = node;
            }
        }
    }

    return targetTerm;
}

PaintedEdge::List TermGroup::filterFromEdgesList(std::function<bool(PaintedEdge::Ptr)> condition) const
{
    PaintedEdge::List ret;

    for (const auto& edge : allEdges()) {
        if (condition(edge)) {
            ret.push_back(edge);
        }
    }

    return ret;
}

PaintedEdge::List TermGroup::edgesForPaint() const
{
    PaintedEdge::List lst;

    auto softEdgesFilter     = [](auto e) { return !e->isSelected() && !e->isHard(); };
    auto hardEdgesFilter     = [](auto e) { return !e->isSelected() && e->isHard(); };
    auto selectedEdgesFilter = [](auto e) { return e->isSelected(); };

    auto softEdges     = filterFromEdgesList(softEdgesFilter);
    auto hardEdges     = filterFromEdgesList(hardEdgesFilter);
    auto selectedEdges = filterFromEdgesList(selectedEdgesFilter);

    auto addToList = [&lst](auto e) { lst.push_back(e); };

    for_each(begin(softEdges), end(softEdges), addToList);
    for_each(begin(hardEdges), end(hardEdges), addToList);
    for_each(begin(selectedEdges), end(selectedEdges), addToList);
    auto brokenEdges = allBrokenEdges();
    for_each(begin(brokenEdges), end(brokenEdges), addToList);

    return lst;
}

QUuid TermGroup::uuid() const { return mInfo.uuid->get(); }

QString TermGroup::name() const { return mInfo.name; }

QString TermGroup::getHierarchyDefinition(PaintedTerm::Ptr term)
{
    for (auto forest : mForests) {
        if (forest->contains(term)) {
            return forest->getHierarchyDefinition(term);
        }
    }

    return "";
}

void TermGroup::selectTerm(const PaintedTerm::Ptr& term, bool selection)
{
    for (auto forest : mForests) {
        if (forest->hasTerm(term)) {
            forest->selectTerm(term, selection);
        }
    }

    for (auto& orphTerm : getOrphanNodes()) {
        orphTerm->setSelection(orphTerm == term);
    }
}

NodeType::Type TermGroup::termType(const PaintedTerm::Ptr& term) const
{
    for (const auto& forest : mForests) {
        if (forest->hasTerm(term)) {
            if (term->isGhost()) {
                return NodeType::Type::Ghost;
            } else {
                return NodeType::fromTermType(forest->nodeType(term));
            }
        }
    }

    return NodeType::Type::Orphan;
}

QMap<QString, PaintedTerm::Ptr> TermGroup::createExactLinkMatchCacheFor(const PaintedTerm::List& terms)
{
    QMap<QString, PaintedTerm::Ptr> ret;

    for (const auto& term : terms) {
        auto lowerTerms = term->cache().lowerTermAndSynonyms();

        for (const auto& termOrSynonym : lowerTerms) {
            ret.insert(termOrSynonym, term);
        }
    }

    return ret;
}

QMap<QUuid, PaintedTerm::Ptr> TermGroup::createUuidCacheFor(const PaintedTerm::List& terms)
{
    QMap<QUuid, PaintedTerm::Ptr> ret;

    for (const auto& node : terms) {
        ret.insert(node->data().uuid->get(), node);
    }

    return ret;
}

QSizeF TermGroup::getOrphansSize()
{
    QRectF orphansRc;
    for (const auto& node : getOrphanNodes()) {
        orphansRc = orphansRc.united(node->getNodeRect(CoordType::scene));
    }
    return orphansRc.size();
}

PaintedTerm::List TermGroup::getInTreeNodes() const { return mGraphData.connectedNodes(); }

PaintedTerm::List TermGroup::getOrphanNodes() const { return mGraphData.isolatedNodes(); }

PaintedTerm::List TermGroup::filterFromNodesList(std::function<bool(PaintedTerm::Ptr)> filterCheck) const
{
    PaintedTerm::List ret;
    for (const auto& node : mGraphData.nodeList()) {
        if (filterCheck(node)) {
            ret.push_back(node);
        }
    }
    return ret;
}

PaintedEdge::List TermGroup::allEdges() const
{
    PaintedEdge::List ret;

    for (const auto& forest : mForests) {
        for (const auto& edge : forest->edgeList()) {
            ret.push_back(edge);
        }
    }

    return ret;
}

PaintedEdge::List TermGroup::allBrokenEdges() const
{
    PaintedEdge::List ret;
    for (const auto& forest : mForests) {
        for (const auto& edge : forest->brokenEdges()) {
            ret.push_back(edge);
        }
    }

    return ret;
}

PaintedEdge::List TermGroup::allExceedEdges() const
{
    PaintedEdge::List ret;

    for (const auto& forest : mForests) {
        for (const auto& edge : forest->wasteEdges()) {
            ret.push_back(edge);
        }
    }

    return ret;
}

PaintedTerm::List TermGroup::terms() const { return mGraphData.nodeList(); }

bool TermGroup::isThreadInterrupted() { return QThread::currentThread()->isInterruptionRequested(); }
