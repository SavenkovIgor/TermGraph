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

#include <gtest/gtest.h>

#include <QColor>
#include <QCoreApplication>

#include "source/Helpers/link/linksdecorator.h"
#include "source/Helpers/link/linksstring.h"

class LinkDecoratorTest : public ::testing::Test
{
public:
    const QString multipleLinks = "{abc} {bc} {c} ";
};

TEST_F (LinkDecoratorTest, Decorator) {

    LinksString links(multipleLinks);

    LinksDecorator decorator(links, [](auto orderNum, [[maybe_unused]] auto link) {
        if (orderNum == 1)
            return QColor("blue");
        return QColor("red");
    });

    auto result = decorator.apply(LinksDecoratorMode::Insert);

    EXPECT_EQ(result,
             "<font color=\"#ffff0000\" style=\"background-color:#00000000\">{abc}</font> "
             "<font color=\"#ff0000ff\" style=\"background-color:#00000000\">{bc}</font> "
             "<font color=\"#ffff0000\" style=\"background-color:#00000000\">{c}</font> ");

    auto result2 = decorator.apply(LinksDecoratorMode::Replace);

    EXPECT_EQ(result2,
             "<font color=\"#ffff0000\" style=\"background-color:#00000000\">abc</font> "
             "<font color=\"#ff0000ff\" style=\"background-color:#00000000\">bc</font> "
             "<font color=\"#ffff0000\" style=\"background-color:#00000000\">c</font> ");
}

