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

#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include <QString>

#include <CommonTools/HandyTypes.h>

class HelpStuff
{
public:
    static QString ptToStr(const int& x, const int& y);
    static QString ptToStr(const QPoint& pt);
    static QString ptToStr(const QPointF& pt);

    static QString sizeToStr(const int& width, const int& height);
    static QString sizeToStr(const QSize& size);
    static QString sizeToStr(const QSizeF& size);

    static QString rectToStr(const QRect& rect);
    static QString rectToStr(const QRectF& rect);

    static QSizeF getStackedSize(const SizeList& sizeList, const Qt::Orientation stackOrientation = Qt::Vertical);
};
