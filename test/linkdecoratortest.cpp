// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <QColor>
#include <QCoreApplication>

#include "source/helpers/link/LinksDecorator.h"
#include "source/helpers/link/LinksString.h"

class LinkDecoratorTest : public ::testing::Test
{
public:
    const QString multipleLinks = "{abc} {bc} {c} ";
};

TEST_F(LinkDecoratorTest, Decorator)
{
    LinksString links(multipleLinks);

    LinksDecorator decorator(links, [](auto orderNum, [[maybe_unused]] auto link) {
        if (orderNum == 1) {
            return QColor::fromString("blue");
        }
        return QColor::fromString("red");
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
