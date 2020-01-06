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

#include "termnodewrapper.h"

TermNodeWrapper::TermNodeWrapper(PaintedTerm* term, QObject* parent) : QObject(parent)
{
    mTerm = term;
    assert(mTerm != nullptr);
    connect(mTerm, &PaintedTerm::colorChanged, this, &TermNodeWrapper::colorChanged);
}

QString TermNodeWrapper::term() const
{
    return mTerm->getSmallName();
}

QColor TermNodeWrapper::color() const
{
    return mTerm->getColor();
}

qreal TermNodeWrapper::radius() const
{
    return mTerm->getCornerRadius();
}

QRectF TermNodeWrapper::rect() const
{
    return mTerm->getNodeRect(CoordType::scene);
}

