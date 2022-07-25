// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include "source/helpers/asyncgroupbuilder.h"
#include "source/managers/groupsmanager.h"
#include "source/model/edge/edgesmodel.h"
#include "source/model/group/termgroup.h"
#include "source/model/term/termdatawrapper.h"
#include "source/model/term/termsmodel.h"

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
    void setCurrentGroup(const GroupUuid& newGroupUuid);

    QString getCurrNodeNameAndDefinition();
    QString getCurrNodeHierarchyDefinition();

    bool isAnyNodeSelected() const;
    void showNewGroup(TermGroup::OptPtr newGroup);

private: // Methods
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
    TermGroup::OptPtr mCurrentGroup;

    Opt<GroupUuid> currentGroupUuid() const;
    void  dropGroup();

    bool isGroupLoading() const;

private: // Members
    AsyncGroupBuilder mGroupBuilder;
    TermsModel*       mTermsModel = nullptr;
    EdgesModel*       mEdgesModel = nullptr;
};
