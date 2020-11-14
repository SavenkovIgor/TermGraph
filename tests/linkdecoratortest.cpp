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

#include <memory>

#include <QColor>
#include <QCoreApplication>
#include <QtTest>

#include "source/Helpers/link/linksdecorator.h"
#include "source/Helpers/link/linkstext.h"
#include "source/Helpers/link/textlink.h"

// TODO: TextLink -> Link

class LinkDecoratorTest : public QObject
{
    Q_OBJECT

private slots:
    void linkDecorate()
    {
        LinksText links(multipleLinks);

        LinksDecorator decorator(links, [](auto orderNum, [[maybe_unused]] auto link) {
            if (orderNum == 1)
                return QColor("blue");
            return QColor("red");
        });

        auto result = decorator.apply(LinksDecoratorMode::Insert);

        QCOMPARE(result,
                 "<font color=\"#ff0000\">{abc}</font> "
                 "<font color=\"#0000ff\">{bc}</font> "
                 "<font color=\"#ff0000\">{c}</font> ");

        auto result2 = decorator.apply(LinksDecoratorMode::Replace);

        QCOMPARE(result2,
                 "<font color=\"#ff0000\">abc</font> "
                 "<font color=\"#0000ff\">bc</font> "
                 "<font color=\"#ff0000\">c</font> ");
    }

private:
    const QString multipleLinks = "{abc} {bc} {c} ";
};

QTEST_APPLESS_MAIN(LinkDecoratorTest)

#include "linkdecoratortest.moc"
