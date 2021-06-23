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

#include <QObject>

#include "source/Helpers/appconfig.h"
#include "source/Helpers/asyncgroupbuilder.h"
#include "source/Helpers/fsworks.h"
#include "source/Managers/groupsmanager.h"
#include "source/Model/Termin/termdatawrapper.h"
#include "source/Model/Termin/termsmodel.h"
#include "source/Model/TerminEdge/edgesmodel.h"
#include "source/Model/TerminGroup/termgroup.h"

class MainScene : public QObject
{
    Q_OBJECT

public:
    MainScene(GroupsManager* groupsMgr, QObject* parent = nullptr);
    ~MainScene() override = default;

    // Scene
    Q_PROPERTY(QRectF sceneRect READ sceneRect WRITE setSceneRect NOTIFY sceneRectChanged)

    // Group
    Q_PROPERTY(TermGroup* currentGroup READ getCurrentGroup NOTIFY currentGroupChanged)
    Q_PROPERTY(bool hasCurrentGroup READ hasCurrentGroup NOTIFY currentGroupChanged)
    Q_PROPERTY(bool groupLoading READ isGroupLoading NOTIFY groupLoadingChanged)

    // Node
    Q_PROPERTY(TermDataWrapper currentNode READ getCurrentNode NOTIFY selectionChanged)
    Q_PROPERTY(bool hasSelection READ isAnyNodeSelected NOTIFY selectionChanged)
    Q_PROPERTY(QString currNodeNameAndDefinition READ getCurrNodeNameAndDefinition NOTIFY selectionChanged)
    Q_PROPERTY(QString currNodeHierarchyDefinition READ getCurrNodeHierarchyDefinition NOTIFY selectionChanged)
    Q_PROPERTY(TermsModel* terms MEMBER mTermsModel CONSTANT FINAL)
    Q_PROPERTY(EdgesModel* edges MEMBER mEdgesModel CONSTANT FINAL)

    // Invokables
    Q_INVOKABLE void        selectGroup(const QUuid groupUuid);
    Q_INVOKABLE void        selectTerm(const QUuid termUuid);
    Q_INVOKABLE void        deleteSelectedTerm();
    Q_INVOKABLE QStringList search(const QString& text);

    Q_INVOKABLE QPointF getTermPosition(const QUuid termUuid) const;

    // Casts uuid <-> name
    Q_INVOKABLE QString termUuidToName(const QUuid termUuid) const;
    Q_INVOKABLE QUuid   termNameToUuid(const QString termName) const;

    Q_INVOKABLE void setMouseClick(qreal x, qreal y);

signals:
    // Scene signals
    void selectionChanged();
    void selectionDoubleClick();
    void currentGroupChanged();
    void nodesChanged();
    void edgesChanged();

    void sceneRectChanged();

    void groupLoadingChanged();

private slots:
    void updateGroup();

    // Groups modify reaction
    void checkGroupAddition();
    void checkGroupDeletion();

    void takeBuildGroupAndShow();

private:
    void setCurrentGroup(const QUuid& newGroupUuid);

    QString getCurrNodeNameAndDefinition();
    QString getCurrNodeHierarchyDefinition();

    bool isAnyNodeSelected() const;
    void showNewGroup(TermGroup* newGroup);

private: // Methods
    // Timers
    QTimer mouseMoveReactionTimer;

    // Managers
    GroupsManager* groupsMgr;

    // Scene rect
    QRectF mSceneRect = QRectF(0, 0, 100, 100);

    QRectF sceneRect() const;
    void   setSceneRect(const QRectF& newRect);
    void   updateSceneRect();

    // Mouse interaction
    PaintedTerm::OptPtr selectedTerm = std::nullopt;

    PaintedTerm::OptPtr getSelectedTerm() const;
    void                selectTerm(PaintedTerm::OptPtr term);
    void                dropTermSelection();

    PaintedTerm::OptPtr findTerm(const QUuid& termUuid) const;
    TermDataWrapper     getCurrentNode();
    TermGroup*          getCurrentGroup() const;
    bool                hasCurrentGroup() const;

    void findClick(const QPointF& atPt);

    PaintedTerm::OptPtr getNodeAtPoint(const QPointF& pt) const;

    // Groups fields
    QScopedPointer<TermGroup> mCurrentGroup;

    QUuid currentGroupUuid() const;
    void  dropGroup();

    bool isGroupLoading() const;

private: // Members
    AsyncGroupBuilder mGroupBuilder;
    TermsModel*       mTermsModel = nullptr;
    EdgesModel*       mEdgesModel = nullptr;
};
