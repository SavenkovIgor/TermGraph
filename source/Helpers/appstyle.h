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

#include <QColor>

class AppStyle
{
public:
    class Colors
    {
    public:
        inline static auto transparent = QColor(0, 0, 0, 0);
        inline static auto testColor   = QColor(0, 255, 0);

        class Nodes
        {
        public:
            inline static auto border = QColor(0, 0, 0);
            inline static auto text   = QColor(255, 255, 255);

            inline static auto leaf   = QColor(109, 154, 40);
            inline static auto root   = QColor(128, 41, 21);
            inline static auto orphan = QColor(18, 75, 106);

            inline static auto leafSelected   = QColor(131, 195, 27);
            inline static auto rootSelected   = QColor(220, 76, 29);
            inline static auto orphanSelected = QColor(11, 119, 177);

            inline static auto atLearn = QColor(245, 222, 179);
        };

        class Groups
        {
        public:
            inline static auto border                    = QColor(255, 255, 255);
            inline static auto text                      = QColor(255, 255, 255);
            inline static auto backgroundTerms           = QColor(0, 0, 0, 0);
            inline static auto backgroundFreeConnections = QColor(0, 0, 0, 0);
        };

        class Edges
        {
        public:
            inline static auto standard    = QColor(Qt::black);
            inline static auto termin      = QColor(34, 91, 122);
            inline static auto description = QColor(Qt::green);
            inline static auto selected    = QColor(0, 166, 147);
            inline static auto selectedAlt = QColor(220, 76, 29);
            inline static auto broken      = Qt::red;
        };
    };

    class Sizes
    {
    public:
        static constexpr qreal groupVerticalSpacer   = 10.0;
        static constexpr qreal groupHorizontalSpacer = 10.0;

        static constexpr qreal orphansVerticalSpacer   = 10.0;
        static constexpr qreal orphansHorizontalSpacer = 10.0;

        static constexpr qreal treeLayerHorizontalSpacer = 15.0;

        static constexpr qreal baseBlockWidth = 40.0;
    };
};
