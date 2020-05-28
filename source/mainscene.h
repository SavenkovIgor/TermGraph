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

#pragma once

#include <stdlib.h>

#include <QClipboard>
#include <QObject>
#include <QQmlListProperty>

#include "source/Helpers/appconfig.h"
#include "source/Helpers/asyncgroupbuilder.h"
#include "source/Helpers/fsworks.h"
#include "source/Managers/groupsmanager.h"
#include "source/Managers/nodesmanager.h"
#include "source/Model/Termin/nodegadgetwrapper.h"
#include "source/Model/TerminGroup/termgroup.h"

class MainScene : public QObject
{
    Q_OBJECT

public:
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, QObject* parent = nullptr);
    ~MainScene() override = default;

    Q_PROPERTY(bool hasSelection READ isAnyNodeSelected NOTIFY selectionChanged)
    Q_PROPERTY(NodeGadgetWrapper currentNode READ getCurrentNode)  // Read only. without notify
    Q_PROPERTY(QRectF sceneRect READ sceneRect WRITE setSceneRect NOTIFY sceneRectChanged)
    Q_PROPERTY(QString currentGroupUuid READ currentGroupUuid NOTIFY currentGroupChanged)
    Q_PROPERTY(QString currentGroupName READ currentGroupName NOTIFY currentGroupChanged)
    Q_PROPERTY(QQmlListProperty<PaintedTerm> nodes READ getNodes NOTIFY nodesChanged)
    Q_PROPERTY(QQmlListProperty<Edge> edges READ getEdges NOTIFY edgesChanged)
    Q_PROPERTY(bool groupLoading READ isGroupLoading NOTIFY groupLoadingChanged)

    Q_INVOKABLE void selectGroup(const QString& groupUuid);
    Q_INVOKABLE void selectTerm(const QString& termUuid);
    Q_INVOKABLE void selectTerm(const QUuid& termUuid);
    Q_INVOKABLE void deleteSelectedTerm();
    Q_INVOKABLE QStringList search(const QString& text);

    Q_INVOKABLE QPointF getTermPosition(const QString& termUuid) const;
    Q_INVOKABLE QString getCurrNodeDebugInfo();

    // Casts uuid <-> name
    Q_INVOKABLE QString termUuidToName(const QString& termUuid) const;
    Q_INVOKABLE QString termNameToUuid(const QString& termName) const;

signals:
    // Scene signals
    void selectionChanged();
    void selectionDoubleClick();
    void currentGroupChanged();
    void nodesChanged();
    void edgesChanged();

    void sceneRectChanged();

    void groupLoadingChanged();

public slots:
    QString getCurrNodeNameAndDefinition();
    QString getCurrNodeHierarchyDefinition();

    bool isAnyNodeSelected() const;

    void setMouseClick(qreal x, qreal y);

private slots:
    void updateGroup();

    // Groups modify reaction
    void checkGroupAddition();
    void checkGroupDeletion();

    void takeBuildGroupAndShow();
    void showNewGroup(TermGroup* newGroup);

private:
    void setCurrentGroup(const QUuid& newGroupUuid);

private:
    // Timers
    QTimer mouseMoveReactionTimer;

    // Managers
    GroupsManager* groupsMgr;
    NodesManager*  nodesMgr;

    // Scene rect
    QRectF mSceneRect = QRectF(0, 0, 100, 100);

    QRectF sceneRect() const;
    void   setSceneRect(const QRectF& newRect);
    void   updateSceneRect();

    // Mouse interaction
    PaintedTerm* selectedTerm = nullptr;

    PaintedTerm* getSelectedTerm() const;
    void         selectTerm(PaintedTerm* term, bool needRepaint = true);
    void         dropTermSelection(bool needRepaint = false);

    PaintedTerm*      findTerm(const QUuid& termUuid) const;
    NodeGadgetWrapper getCurrentNode();

    void findClick(const QPointF& atPt);

    PaintedTerm* getNodeAtPoint(const QPointF& pt) const;

    // Groups fields
    QScopedPointer<TermGroup> mCurrentGroup;

    QString currentGroupUuid() const;
    QString currentGroupName() const;
    void    dropGroup();

    // For qml nodes list property
    QQmlListProperty<PaintedTerm> getNodes();

    int                 termCount() const;
    PaintedTerm*        term(int index) const;
    static int          termCount(QQmlListProperty<PaintedTerm>* list);
    static PaintedTerm* term(QQmlListProperty<PaintedTerm>* list, int i);

    // For qml edges list property
    QQmlListProperty<Edge> getEdges();

    int          edgeCount() const;
    Edge*        edge(int index) const;
    static int   edgeCount(QQmlListProperty<Edge>* list);
    static Edge* edge(QQmlListProperty<Edge>* list, int i);

    Edge::List mCachedEdges;
    void       updateEdgeCache();

    bool isGroupLoading() const;

private:
    // For testing
    void createTestGroups();

    AsyncGroupBuilder mGroupBuilder;
};
