/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include "termgroup.h"

int TermGroup::animSpeed = 300;

TermGroup::TermGroup(const GroupInfoContainer& info, QObject* parent)
    : QObject(parent)
    , TermGroupInfo(info)
{
    this->grNmItem = new LabelGraphicItem(getName());
}

TermGroup::~TermGroup()
{
//    qDebug() << "Delete group" << getName();
    checkSwapTimer.stop();
    animTimer.stop();

    delete grNmItem;
    delete orphansRect;
    delete baseRect;
}

void TermGroup::initNewNodes()
{
    loadEdges();
    removeCycles();
    removeExceedEdges();

    baseRect = new RectGraphicItem(QPoint(0, 0), QSizeF(10.0, 10.0));

    orphansRect = new RectGraphicItem();
    orphansRect->setParentItem(baseRect);

    this->grNmItem->setParentItem(baseRect);

    setLevels();
    initTrees();
    addTreeRectsToScene();

    addOrphansToParents();
    addEdgesToParents();

    setTreeCoords();
    setNeighbours();
    setOrphCoords();

    checkSwapTimer.setSingleShot(false);
    setAnimSpeed(300);
    connect(&checkSwapTimer, &QTimer::timeout, this, &TermGroup::checkSwap);

    animTimer.setSingleShot(false);
    animTimer.setInterval(50);
    connect(&animTimer, &QTimer::timeout, this, &TermGroup::animateGroup);
}

void TermGroup::loadNodes(PaintedTerm::List newNodes)
{
    clearNodesList();

    for (auto node : newNodes) {
        Q_ASSERT_X(node->getGroupUuid() == this->getUuid(), Q_FUNC_INFO, "Node group error");

        if (node->getGroupUuid() != this->getUuid())
            continue;

        addNodeToList(node);
    }

    initNewNodes();
}

bool TermGroup::needPaint() const
{
    for (auto node : getAllNodes()) {
        if (node->needPaint)
            return true;
    }

    for (auto edge : getAllEdges()) {
        if (edge->needPaint)
            return true;
    }

    return false;
}

void TermGroup::resetPaintFlags()
{
    for (auto node : getAllNodes())
        node->needPaint = true;

    for (auto edge : getAllEdges())
        edge->needPaint = true;
}

PaintedTerm *TermGroup::getNodeAtPoint(const QPointF& pt) const
{
    for (auto tree : trees) {
        if (tree->getTreeRect(CoordType::scene).contains(pt)) {
            return tree->getNodeAtPoint(pt);
        }
    }

    auto orphansRect = QRectF(this->orphansRect->scenePos(), this->orphansRect->getSize());
    if (orphansRect.contains(pt)) {
        for (auto orphan : getOrphanNodes()) {
            if (orphan->getNodeRect(CoordType::scene).contains(pt)) {
                return orphan;
            }
        }
    }

    return nullptr;
}

void TermGroup::setHover(QPointF mousePos)
{
    for (auto tree : trees)
        tree->checkHover(mousePos);

    for (auto orphan : getOrphanNodes()) {
        auto hover = orphan->getNodeRect(CoordType::scene).contains(mousePos);
        orphan->setHover(hover);
    }
}

void TermGroup::addOrphansToParents()
{
    for (auto node : getOrphanNodes()) {
        node->setParentItem(orphansRect);
    }
}

void TermGroup::addEdgesToParents()
{
    for (Edge* edge : getAllEdges()) {
        for (auto tree : trees) {
            if (tree->hasEdge(edge)) {
                edge->setParentItem(tree->rect);
            }
        }
    }
}

void TermGroup::startAnimation()
{
    animTimer.start();
    checkSwapTimer.start();
}

void TermGroup::stopAnimation()
{
    animTimer.stop();
    checkSwapTimer.stop();
}

void TermGroup::sceneUpdateSignal()
{
    updateGroupFrame();
}

void TermGroup::checkSwap()
{
    for (auto tree : trees)
        tree->checkSwap();
}

void TermGroup::animateGroup()
{
    for (auto tree : trees)
        tree->animateTree();
}

void TermGroup::addTreeRectsToScene()
{
    for (auto tree : trees)
        tree->rect->setParentItem(baseRect);
}

qreal TermGroup::getGroupMinWidth()
{
    qreal width = 0.0;

    qreal groupNameWidth = grNmItem->getNameSize().width();
    qreal treeWidth = getAllTreesSize().width();
    qreal orphansWidth = NodeVerticalStackTools::getNodeVerticalStackedSize(getOrphanNodes()).width();

    width = std::max(width, groupNameWidth);
    width = std::max(width, treeWidth);
    width = std::max(width, orphansWidth);

    return width;
}

void TermGroup::setAnimSpeed(int val)
{
    checkSwapTimer.setInterval(static_cast<int>(val*1.5));
    for (auto tree : trees) {
        tree->swAnim1.setDuration(val);
        tree->swAnim2.setDuration(val);
    }
    animSpeed = val;
}

int TermGroup::getAnimSpeed()
{
    return animSpeed;
}

void TermGroup::setBasePoint(QPointF pt)
{
    baseRect->setPos(pt);
}

void TermGroup::updateRectsPositions()
{
    qreal vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal hSpacer = AppStyle::Sizes::groupHorizontalSpacer;
    QPointF basePoint(QPointF(hSpacer, vSpacer));

    auto nameSize = grNmItem->getNameSize();

    // Устанавливаем базовую точку имени
    grNmItem->setPos(basePoint);
    basePoint.ry() += nameSize.height() + vSpacer;

    // Вычисляем под дерево
    for (auto tree : trees) {
        auto treeSize = tree->getTreeSize();
        tree->rect->setPos(basePoint);
        tree->rect->setSize(treeSize);
        basePoint.ry() += treeSize.height() + vSpacer;
    }

    QSizeF orphansSize = getOrphansSize();
    // Вычисляем под несвязанные вершины
    orphansRect->setPos(basePoint);
    orphansRect->setSize(orphansSize);  // Применяем
}

void TermGroup::updateBaseRectSize()
{
    QSizeF nameSize = grNmItem->getNameSize();
    QSizeF treesSize = getAllTreesSize();
    QSizeF orphansSize = getOrphansSize();
    qreal vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal hSpacer = AppStyle::Sizes::groupHorizontalSpacer;

    qreal width = 0.0;
    qreal height = 0.0;

    width = std::max(width, nameSize.width());
    width = std::max(width, treesSize.width());
    width = std::max(width, orphansSize.width());
    width += hSpacer*2;

    height += vSpacer;
    height += nameSize.height();
    if (treesSize.height() > 0)
        height += treesSize.height() + vSpacer;
    if (orphansSize.height() > 0)
        height += orphansSize.height() + vSpacer;
    height += vSpacer;

    baseRect->setSize(QSizeF(width, height));
}

void TermGroup::updateGroupFrame()
{
    updateRectsPositions();
    updateBaseRectSize();
}

void TermGroup::setOrphCoords(qreal maxWidth)
{
    auto orphansList = getOrphanNodes();
    if (orphansList.isEmpty()) {
        return;
    }

    for (int i = 0; i < orphansList.size(); i++) {
        int nMax = i;
        for (int j = i+1; j < orphansList.size(); j++) {
            qreal currentWidth = orphansList[j]->getFrameRect(CoordType::zeroPoint).size().width();
            qreal maxWidth = orphansList[nMax]->getFrameRect(CoordType::zeroPoint).size().width();
            if (currentWidth < maxWidth) {
                nMax = j;
            }
        }
        orphansList.swap(i, nMax);
    }

    qreal x = 0, y = 0;
    qreal maxHeightInRow = 0.0;
    qreal groupMinWidth = getGroupMinWidth();
    maxWidth = qBound(10.0, groupMinWidth, 2000.0);

    for (auto currNode : orphansList) {
        QSizeF nodeSize = currNode->getNodeRect(CoordType::zeroPoint).size();

        if (x + nodeSize.width() > maxWidth) {
            y+= maxHeightInRow + AppStyle::Sizes::orphansVerticalSpacer;
            //Переходим на следующий ряд
            maxHeightInRow = 0;
            x = 0;
        }

        currNode->setPos(x, y);
        x += nodeSize.width() + AppStyle::Sizes::orphansHorizontalSpacer;

        maxHeightInRow = std::max(maxHeightInRow, nodeSize.height());
    }
}

void TermGroup::setTreeCoords()
{
    for (auto tree : trees) {
        tree->setTreeNodeCoors();
    }
}

QString TermGroup::getNameLabel()
{
    return grNmItem->getLabel();
}

QPointF TermGroup::getNamePos()
{
    auto pt = grNmItem->scenePos();
    pt.ry() += grNmItem->getNameSize().height() / 2;
    return pt;
}

QRectF TermGroup::getGroupRect() const
{
    return baseRect->getRect(CoordType::scene);
}

QColor TermGroup::getGroupColor()
{
    return AppStyle::Colors::Groups::border;
}

QColor TermGroup::getGroupFillColor()
{
    return AppStyle::Colors::Groups::backgroundTerms;
}
