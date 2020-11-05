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

#include <QCoreApplication>
#include <QtTest>

#include "source/Helpers/text/textlink.h"
#include "source/Helpers/text/textrange.h"

#include <QDebug>

class TextLinkTest : public QObject
{
    Q_OBJECT

private slots:
    void textLinkWithUuid()
    {
        auto range = TextRange(linkedTextWithUuid, 5, 48);
        auto link  = TextLink(linkedTextWithUuid, range);

        auto res = link.createLinkWithUuid(QUuid());

        QCOMPARE(res, "{Ab c|00000000-0000-0000-0000-000000000000}");
        QCOMPARE(link.fullLink(), QString("{Ab c|94810de3-51b8-469e-b316-00248ffa2a45}"));
        QCOMPARE(link.text(), QString("Ab c"));
        QCOMPARE(link.textLower(), "ab c");
        QCOMPARE(link.hasUuid(), true);
        QCOMPARE(link.uuid(), QUuid::fromString(QString("{94810de3-51b8-469e-b316-00248ffa2a45}")));
    }

    void textLinkNoUuid()
    {
        auto range = TextRange(linkedText, 5, 11);
        auto link  = TextLink(linkedText, range);

        QCOMPARE(link.createLinkWithUuid(QUuid()), QString("{Ab c|00000000-0000-0000-0000-000000000000}"));
        QCOMPARE(link.fullLink(), QString("{Ab c}"));
        QCOMPARE(link.text(), QString("Ab c"));
        QCOMPARE(link.textLower(), "ab c");
        QCOMPARE(link.hasUuid(), false);
    }

private:
    const QString linkedTextWithUuid = " abc {Ab c|94810de3-51b8-469e-b316-00248ffa2a45} abc ";
    const QString linkedText         = " abc {Ab c} abc ";
};

QTEST_APPLESS_MAIN(TextLinkTest)

#include "textlinktest.moc"
