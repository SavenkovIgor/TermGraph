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

#include "paintedterm.h"

bool PaintedTerm::someoneHover  = false;
bool PaintedTerm::someoneSelect = false;

const qreal PaintedTerm::verScale = 0.0200;
QList<Qt::Edge> PaintedTerm::sides;

PaintedTerm::PaintedTerm(const NodeInfoContainer& info, QObject* parent)
    : QObject(parent), GraphTerm(info), GraphicItem()
{
    if (sides.isEmpty()) {
        sides << Qt::BottomEdge;
        sides << Qt::TopEdge;
        sides << Qt::RightEdge;
        sides << Qt::LeftEdge;
    }
    adjustRectSizeForName();
}

QRectF PaintedTerm::rect() const
{
    return getNodeRect(CoordType::scene);
}

int PaintedTerm::getUpLevels([[maybe_unused]] int pLevel)  // TODO: check why plevel unused
{
    int ret = -1;
    for (GraphTerm* t : getLeafNodes()) {
        PaintedTerm* n = static_cast<PaintedTerm*>(t);
        ret = std::max(ret, n->getUpLevels());
    }

    if (ret == -1)
        ret = getPaintLevel();

    return ret;
}

void PaintedTerm::setRelatedPaintUp(bool val)
{
    for (auto node : getLeafNodes())
        static_cast<PaintedTerm*>(node)->relativePaint = val;

    for (auto* edge : getEdgesToLeafs())
        dynamic_cast<Edge*>(edge)->setSelectedForward(val);

    for (auto node : getLeafNodes())
        static_cast<PaintedTerm*>(node)->setRelatedPaintUp(val);


    emit colorChanged();
}

void PaintedTerm::setRelatedPaintDown(bool val)
{
    for (auto* node : getRootNodes())
        static_cast<PaintedTerm*>(node)->relativePaint = val;

    for (auto* edge : getEdgesToRoots())
        dynamic_cast<Edge*>(edge)->setSelectedBackward(val);

    for (auto node : getRootNodes())
        static_cast<PaintedTerm*>(node)->setRelatedPaintDown(val);

    emit colorChanged();
}

QLineF PaintedTerm::getRectLine(Qt::Edge side)
{
    QRectF rc = getNodeRect(CoordType::local);

    switch (side) {
    case Qt::TopEdge:
        return QLineF(rc.topLeft(), rc.topRight());

    case Qt::RightEdge:
        return QLineF(rc.topRight(), rc.bottomRight());

    case Qt::BottomEdge:
        return QLineF(rc.bottomLeft(), rc.bottomRight());

    case Qt::LeftEdge:
        return QLineF(rc.topLeft(), rc.bottomLeft());
    }
    return QLineF();
}

QRectF PaintedTerm::getNodeRect(CoordType inCoordinates) const
{
    switch (inCoordinates) {
    case CoordType::zeroPoint: return QRectF(QPointF(),  nodeSize);
    case CoordType::local:     return QRectF(pos(),      nodeSize);
    case CoordType::scene:     return QRectF(scenePos(), nodeSize);
    }
}

QRectF PaintedTerm::getFrameRect(CoordType inCoordinates) const
{
    qreal val = qBound(0.0, nodeSize.width()*0.2, 8.0);
    QRectF ret = getNodeRect(inCoordinates);
    return addMarginsToRect(ret, val);
}

QPointF PaintedTerm::getCenter(CoordType inCoordinates) const
{
    return getNodeRect(inCoordinates).center();
}

bool PaintedTerm::applyMove()
{
    // Если сильно смещение - кидаем true
    PaintedTerm* left = getNearestLeftNeigh();
    PaintedTerm* right = getNearestRightNeigh();

    QRectF myRc = getFrameRect(CoordType::scene);
    myRc = myRc.translated(newPosOffset, 0.0);
    QRectF leftRc;
    QRectF rightRc;

    qreal k = 0.01;
    newPosOffset = qBound(-myRc.width()*k, newPosOffset, myRc.width()*k);

    if (newPosOffset < 0) {
        if (left != nullptr) {
            leftRc = left->getFrameRect(CoordType::scene);
            newPosOffset = qBound(leftRc.right() - myRc.left(), newPosOffset, 0.0);
        }
    }

    if (newPosOffset > 0) {
        if (right != nullptr) {
            rightRc = right->getFrameRect(CoordType::scene);
            newPosOffset = qBound(0.0, newPosOffset, rightRc.left() - myRc.right());
        }
    }

    if (std::abs(newPosOffset) > 0.05) {
        moveBy(0.0, newPosOffset);
        return true;
    }

    return false;
}

void PaintedTerm::countForces()
{
    if (!hasConnections() && !isRoot())
        return;

    qreal edForce = 0.0;
    qreal myX = getCenter(CoordType::scene).y();

    qreal tmp = 0.0;
    qreal notMyPos = 0.0;

    EdgesList edges;
    edges << Edge::castToEdgeList(getEdgesToRoots());
    edges << Edge::castToEdgeList(getEdgesToLeafs());

    for (Edge *e : edges) {
        if (!GraphTerm::isInGroupEdge(e))
            continue;

        tmp = e->getYProjection();

        PaintedTerm* otherSide = dynamic_cast<PaintedTerm*>(e->getOtherSide(this));
        if (otherSide == nullptr) {
            continue;
        }

        notMyPos = otherSide->getCenter(CoordType::scene).y();

        tmp *= verScale;

        if (notMyPos > myX)
            edForce += tmp;
        else
            edForce += (-1)*tmp;
    }

    //    testStr += "edf:" + QString::number(edForce);

    newPosOffset = 0.0;
    newPosOffset += edForce;
}

int PaintedTerm::getIntersections(bool swapped)
{
    EdgesList edges;
    //    edges << edgesUpList;
    edges << Edge::castToEdgeList(getEdgesToRoots());

    edges << getEdgesInLayer();

    //    qDebug()<<"edges.size"<<this->getName()<<edges.size();

    QList< QPointF > interList;

    for (int i = 0; i < edges.size(); i++) {
        for (int j = i+1; j < edges.size(); j++) {
            QLineF l1 = edges[i]->getLine(swapped),
                    l2 = edges[j]->getLine(swapped);

            QPointF *interPt = new QPointF();
            if (l1.intersect(l2, interPt) == QLineF::BoundedIntersection) {
                bool nearFound = false;

                qreal dist = 0.01;
                if (isNearPoints(*interPt, l1.p1(), dist))
                    nearFound = true;
                if (isNearPoints(*interPt, l1.p2(), dist))
                    nearFound = true;
                if (isNearPoints(*interPt, l2.p1(), dist))
                    nearFound = true;
                if (isNearPoints(*interPt, l2.p2(), dist))
                    nearFound = true;

                if (!nearFound) {
                    bool interFound = false;
                    for (QPointF inPt : interList) {
                        if (isNearPoints(inPt, *interPt, dist)) {
                            interFound = true;
                            break;
                        }
                    }
                    if (!interFound)
                        interList << *interPt;
                }
            }
            delete interPt;
        }
    }
    return interList.size();
}

EdgesList PaintedTerm::getEdgesInLayer()
{
    // Taking all edges in this paint level
    GraphEdge::List allEdgesInLayerList;
    for (GraphTerm* t : getNeighbourNodes()) {
        allEdgesInLayerList << t->getEdgesToRoots();
        allEdgesInLayerList << t->getEdgesToLeafs();
    }

    // Stay only with distance 1
    EdgesList edgLst = Edge::castToEdgeList(allEdgesInLayerList);
    EdgesList ret2;
    for (Edge *e : edgLst) {
        if (e->getLayerDistance() == 1)
            ret2 << e;
    }
    return ret2;
}

QRectF PaintedTerm::addMarginsToRect(QRectF rc, qreal mrg)
{
    QMarginsF mrgObj(mrg, mrg, mrg, mrg);
    return rc.marginsAdded(mrgObj);
}

void PaintedTerm::updateCornerRadius()
{
    cornerRadius = nodeSize.height()*0.2;
}

void PaintedTerm::dropSwap()
{
    EdgesList lst;
    lst = Edge::castToEdgeList(getUpDownEdges());

    for (Edge *e : lst) {
        e->swapPointLeaf = QPointF();
        e->swapPointRoot = QPointF();
    }
}

QColor PaintedTerm::getColor() const
{
    QColor col = getBaseColor();

    if (someoneSelect) {
        if (relativePaint || thisSelected) {
            col = getSelectedColor();
        } else {
            col.setAlpha(100);
        }
    }

    return col;
}

void PaintedTerm::setSwap(QPointF toPt)
{
    for (Edge *e : Edge::castToEdgeList(getEdgesToRoots())) {
        e->swapPointLeaf = toPt;
    }

    for (Edge *e : Edge::castToEdgeList(getEdgesToLeafs())) {
        e->swapPointRoot = toPt;
    }
}

qreal PaintedTerm::getSumEdgesLength(bool swap)
{
    EdgesList edges;
    edges << Edge::castToEdgeList(getEdgesToLeafs());
    edges << Edge::castToEdgeList(getEdgesToRoots());
    qreal ret = 0.0;
    for (Edge *e : edges) {
        if (!GraphTerm::isInGroupEdge(e))
            continue;
        ret += std::abs(e->getLine(swap).dx());
    }
    return ret;
}

void PaintedTerm::adjustRectSizeForName()
{
    QSizeF nameSize = getNameSize();
    nodeSize.setWidth(nameSize.width() + 16);
    nodeSize.setHeight(nameSize.height() + 4);
    updateCornerRadius();
}

PaintedTerm* PaintedTerm::getNearestLeftNeigh()
{
    PaintedTerm* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getNodeRect(CoordType::scene);
    QRectF neighRect;

    for (GraphTerm* n : getNeighbourNodes()) {
        PaintedTerm* t = static_cast<PaintedTerm*>(n);
        neighRect = t->getNodeRect(CoordType::scene);

        if (neighRect.center().x() > myRect.center().x()) {
            continue;
        }

        if (std::abs(neighRect.center().x() - myRect.center().x()) < diff) {
            diff = std::abs(neighRect.center().x() - myRect.center().x());
            ret = t;
        }
    }
    return ret;
}

PaintedTerm *PaintedTerm::getNearestRightNeigh()
{
    PaintedTerm* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getNodeRect(CoordType::scene);
    QRectF neighRect;
    for (GraphTerm* n : getNeighbourNodes()) {
        PaintedTerm* t = static_cast<PaintedTerm*>(n);
        neighRect = t->getNodeRect(CoordType::scene);

        if (neighRect.center().x() < myRect.center().x())
            continue;

        if (std::abs(neighRect.left() - myRect.right()) < diff) {
            diff = std::abs(neighRect.left() - myRect.right());
            ret = t;
        }
    }
    return ret;
}

QColor PaintedTerm::getBaseColor() const
{
    switch (getNodeType()) {
    case NodeType::orphan: return AppStyle::Colors::Nodes::orphan;
    case NodeType::root: return AppStyle::Colors::Nodes::root;
    case NodeType::endLeaf: return AppStyle::Colors::Nodes::leaf;
    case NodeType::middleLeaf: return AppStyle::Colors::Nodes::leaf;
    }
}

QColor PaintedTerm::getSelectedColor() const
{
    switch (getNodeType()) {
    case NodeType::orphan: return AppStyle::Colors::Nodes::orphanSelected;
    case NodeType::root: return AppStyle::Colors::Nodes::rootSelected;
    case NodeType::endLeaf: return AppStyle::Colors::Nodes::leafSelected;
    case NodeType::middleLeaf: return AppStyle::Colors::Nodes::leafSelected;
    }
}

qreal PaintedTerm::getCornerRadius() const
{
    return cornerRadius;
}

void PaintedTerm::setSelection(const bool &selected)
{
    if (thisSelected != selected) {
        thisSelected = selected;

        someoneSelect = selected;
        relativePaint = selected;

        setRelatedPaintDown(selected);
        setRelatedPaintUp(selected);

        emit colorChanged();
    }
}

void PaintedTerm::colorChange()
{
    emit colorChanged();
}

bool PaintedTerm::isNearPoints(QPointF pt1, QPointF pt2, qreal dist) {
    pt1 -= pt2;
    //    pt1.setX(std::abs(pt1.x()));
    //    pt1.setY(std::abs(pt1.y()));
    //    if(std::abs(pt1.x()) <= dist && std::abs(pt1.y()) <= dist)
    //        return true;
    if (pt1.manhattanLength() <= dist)
        return true;
    return false;
}
