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

#include <QObject>
#include <QQueue>
#include <QElapsedTimer>

#include "source/Model/TerminGroup/termgroup.h"

class PaintManager : public QObject
{
    Q_OBJECT

public:
    PaintManager() = default;
    ~PaintManager() = default;

    void sendPaintGroupSignal();

signals:
    void paintGroupQueue();

public slots:
    // Clear functions
    void addClearRect(const QRectF& rect, bool paintNow = false);
    QRectF currentClearRect();
    void nextClearRect();
    bool clearQueueEmpty() const;

    // Fill functions
    void clearAllQueues();
    void addGroup(TermGroup* group, bool ignoreNeedPaintFlag = false, bool paintNow = true);
    void addRect(const QRectF& rect, const QColor& color = Qt::white);

    // GroupRects
    void nextGroupRect();
    bool groupRectQueueEmpty() const;

    QRectF currentGroupRect() const;
    QColor currentGroupFillColor() const;

    // GroupNames
    void nextGroupName();
    bool groupNamesQueueEmpty() const;

    QString currentGroupName() const;
    QPointF currentGroupNamePos() const;

    // Edges
    void nextEdge();
    bool edgeQueueEmpty() const;

    QColor getEdgeColor() const;
    QPointF currentFirstEdgePoint() const;
    QPointF currentLastEdgePoint() const;

    // Flags
    void setPaintInProcessFlag(bool paintNow);
    bool isPaintInProcessNow() const;

    // Paint speed check
    void startCheckTimer();
    void restartCheckTimer(const QString& label = "paint speed");

private:
    bool paintInProcessFlag = false;

    // Paint queues
    QQueue<QRectF>                   clearPaintArea;
    QQueue<QPair<QRectF, QColor>>    groupRectsForPaint;
    QQueue<QPair<QPointF, QString>>  groupNamesForPaint;
    QQueue<Edge*>                    edgesForPaint;

    // Timer
    QElapsedTimer paintSpeedTimer;
};
