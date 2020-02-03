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

#include "helpstuff.h"

QString HelpStuff::ptToStr(const int& x, const int& y)
{
    return "point x:" + QString::number(x) + " y:" + QString::number(y);
}

QString HelpStuff::ptToStr(const QPoint& pt)
{
    return ptToStr(pt.x(), pt.y());
}

QString HelpStuff::ptToStr(const QPointF& pt)
{
    return ptToStr(pt.toPoint());
}

QString HelpStuff::sizeToStr(const int& width, const int& height)
{
    return "size w:" + QString::number(width) + " h:" + QString::number(height);
}

QString HelpStuff::sizeToStr(const QSize& size)
{
    return sizeToStr(size.width(), size.height());
}

QString HelpStuff::sizeToStr(const QSizeF& size)
{
    return sizeToStr(size.toSize());
}

QString HelpStuff::rectToStr(const QRect& rect)
{
    return ptToStr(rect.topLeft()) + " " + sizeToStr(rect.size());
}

QString HelpStuff::rectToStr(const QRectF& rect)
{
    return rectToStr(rect.toRect());
}

QSizeF HelpStuff::getStackedSize(const SizesList& sizeList, const Qt::Orientation stackOrientation)
{
    qreal width  = 0;
    qreal height = 0;

    if (stackOrientation == Qt::Vertical) {
        for (auto& size : sizeList) {
            width = std::max(width, size.width());
            height += size.height();
        }
    } else if (stackOrientation == Qt::Horizontal) {
        for (auto& size : sizeList) {
            width += size.width();
            height = std::max(height, size.height());
        }
    }

    return QSizeF(width, height);
}
