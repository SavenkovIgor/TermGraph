// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QtGlobal>

export module AppStyle;

export class AppStyle
{
public:
    class Sizes
    {
    public:
        static constexpr qreal groupVerticalSpacer   = 10.0;
        static constexpr qreal groupHorizontalSpacer = 10.0;

        static constexpr qreal orphansVerticalSpacer   = 10.0;
        static constexpr qreal orphansHorizontalSpacer = 10.0;

        static constexpr qreal treeLayerHorizontalSpacer = 15.0;
    };
};
