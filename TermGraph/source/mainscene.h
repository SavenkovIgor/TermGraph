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

#pragma once

#include <stdlib.h>

#include <QObject>
#include <QClipboard>
#include <QElapsedTimer>

#include "source/Managers/paintqueuemanager.h"
#include "source/Managers/groupsmanager.h"
#include "source/Managers/nodesmanager.h"
#include "source/Helpers/appconfig.h"
#include "source/Helpers/fsworks.h"
#include "source/Model/TerminGroup/termgroup.h"
#include "source/Model/Termin/nodegadgetwrapper.h"

class MainScene : public QObject
{
    Q_OBJECT

public:
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, PaintManager* paintManager);
    ~MainScene();

    void setAnimSpeed(int val);

    Q_PROPERTY(bool hasSelection READ isAnyNodeSelected NOTIFY selectionChanged)
    Q_PROPERTY(NodeGadgetWrapper currentNode READ getCurrentNode) // Read only. without notify
    Q_PROPERTY(QRectF sceneRect READ getSceneRect WRITE setSceneRect NOTIFY sceneRectChanged)
    Q_PROPERTY(QString currentGroup READ currentGroupUuid WRITE setCurrentGroup NOTIFY currentGroupChanged)

signals:
    // Scene signals
    void selectionChanged();
    void selectionDoubleClick();
    void currentGroupChanged();

    void sceneRectChanged();

public slots:
    void setCurrentGroup(const QString& groupUuid);
    void setCurrentGroup(const QUuid& groupUuid);
    QString currentGroupUuid();

    void showAllGroups();

    void updateSceneRect();
    void centerViewOn(QPointF point);  // TODO: Realize!

    void updateModel();
    void deleteSelectedNode();

    void startAllGroupTimers();
    void stopAllGroupTimers();

    NodeGadgetWrapper getCurrentNode();
    QString getCurrNodeDebugInfo();
    QString getCurrNodeNameAndDefinition();
    QString getCurrNodeHierarchyDefinition();


    TermGroup* getNearestNotPaintedGroup();

    bool isAnyNodeSelected();

    // Drawing API
    QRectF getSceneRect() const;
    void setSceneRect(const QRectF& newRect);

    QColor getSceneBackgroundColor() const;

    void resetPaintFlags();

    void setMousePos(qreal x, qreal y);
    void setMouseClick(qreal x, qreal y);

    // For testing
    void createTestGroups();

private:
    // Timers
    QTimer mouseMoveReactionTimer;

    // Managers
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;
    PaintManager* paintManager;

    void requestPaint(bool paintAll = false);
    void sendGroupsToPaintManager(bool requestPaint = true, bool paintAll = false);

    // Scene rect
    QRectF mSceneRect = QRectF(0, 0, 100, 100);
    QRectF sceneViewRect = QRectF(0, 0, 100, 100);

    // Mouse interaction
    PaintedTerm* hoverNode = nullptr;
    PaintedTerm* selectedNode = nullptr;

    PaintedTerm* getSelectedNode();
    void dropSelection(bool withSignal = true);
    void dropHover();

    void findHover(const QPointF& atPt);
    void findClick(const QPointF& atPt);

    PaintedTerm* getNodeAtPoint(const QPointF& pt) const;

    // Groups fields
    QList<TermGroup*> groupList;
    QUuid mCurrGroupUuid;

    void initAllGroups();
    void addGroupToScene(TermGroup* group);
    void deleteAllGroups();

    void locateGroupsVertically();
};
