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

#include "source/Model/Termin/paintedterm.h"

class TermNodeWrapper : public QObject
{
    Q_OBJECT

public:
    TermNodeWrapper(PaintedTerm* term, QObject* parent = nullptr);

    Q_PROPERTY(QString term   READ term   CONSTANT)
    Q_PROPERTY(QColor  color  READ color  NOTIFY colorChanged)
    Q_PROPERTY(qreal   radius READ radius CONSTANT)
    Q_PROPERTY(QRectF  rect   READ rect   CONSTANT)

    QString term() const;
    QColor  color() const;
    qreal   radius() const;
    QRectF  rect() const;

signals:
    void colorChanged();

private:
    PaintedTerm* mTerm;
};

