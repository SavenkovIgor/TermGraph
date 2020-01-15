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
#include <QQmlListProperty>

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
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, PaintManager* paintManager, QObject* parent = nullptr);
    ~MainScene() override = default;

    Q_PROPERTY(bool hasSelection READ isAnyNodeSelected NOTIFY selectionChanged)
    Q_PROPERTY(NodeGadgetWrapper currentNode READ getCurrentNode) // Read only. without notify
    Q_PROPERTY(QRectF sceneRect READ sceneRect WRITE setSceneRect NOTIFY sceneRectChanged)
    Q_PROPERTY(QString currentGroup READ currentGroupUuid WRITE setCurrentGroup NOTIFY currentGroupChanged)
    Q_PROPERTY(QString currentGroupName READ currentGroupName NOTIFY currentGroupChanged)
    Q_PROPERTY(QQmlListProperty<PaintedTerm> nodes READ getNodes NOTIFY nodesChanged)

    Q_INVOKABLE void selectTerm(const QString& nodeUuid);
    Q_INVOKABLE void selectTerm(const QUuid& nodeUuid);
    Q_INVOKABLE QString getTermName(const QString& termUuid);
    Q_INVOKABLE QStringList search(const QString& text);
    Q_INVOKABLE void        moveTo(const QString& termUuid);
    Q_INVOKABLE void        moveTo(QPointF point);
    Q_INVOKABLE void deleteSelectedTerm();

    Q_INVOKABLE QString getCurrNodeDebugInfo();

signals:
    // Scene signals
    void selectionChanged();
    void selectionDoubleClick();
    void currentGroupChanged();
    void nodesChanged();

    void sceneRectChanged();

    void showPt(QPointF pt);

public slots:
    void setCurrentGroup(const QString& groupUuid);
    void setCurrentGroup(const QUuid& groupUuid);


    QString getCurrNodeNameAndDefinition();
    QString getCurrNodeHierarchyDefinition();

    bool isAnyNodeSelected() const;

    void setMouseClick(qreal x, qreal y);

private slots:
    void updateGroup();

    // Groups modify reaction
    void checkGroupAddition();
    void checkGroupDeletion();

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

    QRectF sceneRect() const;
    void   setSceneRect(const QRectF& newRect);
    void   updateSceneRect();

    // Mouse interaction
    PaintedTerm* selectedTerm = nullptr;

    PaintedTerm* getSelectedTerm() const;
    void         selectTerm(PaintedTerm* term);
    void         dropTermSelection();

    PaintedTerm*      findNode(const QUuid& nodeUuid);
    NodeGadgetWrapper getCurrentNode();
    void              checkGroupColors();

    void findClick(const QPointF& atPt);

    PaintedTerm* getNodeAtPoint(const QPointF& pt) const;

    // Groups fields
    QScopedPointer<TermGroup> mCurrentGroup;
    QString currentGroupUuid() const;
    QString currentGroupName() const;
    void dropGroup();

    // For qml nodes list property
    QQmlListProperty<PaintedTerm> getNodes();

    int                 termCount() const;
    PaintedTerm*        term(int index) const;
    static int          termCount(QQmlListProperty<PaintedTerm>* list);
    static PaintedTerm* term(QQmlListProperty<PaintedTerm>* list, int i);

    // Paint tools
    void resetPaintFlags();

    // For testing
    void createTestGroups();
};
