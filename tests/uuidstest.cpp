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

#include "source/Helpers/uuid/uuidtools.h"

// TODO: Make all paths lowercase

class UuidsTest : public QObject
{
    Q_OBJECT

private slots:
    void validatorDefault_data()
    {
        QTest::addColumn<QString>("uuidString");
        QTest::addColumn<bool>("result");

        // clang-format off
        QTest::newRow("case0") << "{94810de3-51b8-469e-b316-00248ffa2a45}" << true;
        QTest::newRow("case1") << "{94810DE3-51B8-469E-B316-00248FFA2A45}" << true;
        QTest::newRow("case2") << "94810de3-51b8-469e-b316-00248ffa2a45" << false;
        QTest::newRow("case3") << "94810DE3-51B8-469E-B316-00248FFA2A45" << false;
        QTest::newRow("case4") << " 999-999 " << false;
        QTest::newRow("case5") << "{}" << false;
        QTest::newRow("case6") << "" << false;
        // clang-format on
    }

    void validatorDefault()
    {
        QFETCH(QString, uuidString);
        QFETCH(bool, result);
        QVERIFY(UuidTools::isValidUuidString(uuidString) == result);
    }

    void validatorWithoutBraces_data()
    {
        QTest::addColumn<QString>("uuidString");
        QTest::addColumn<bool>("result");

        // clang-format off
        QTest::newRow("case0") << "{94810de3-51b8-469e-b316-00248ffa2a45}" << false;
        QTest::newRow("case1") << "{94810DE3-51B8-469E-B316-00248FFA2A45}" << false;
        QTest::newRow("case2") << "94810de3-51b8-469e-b316-00248ffa2a45" << true;
        QTest::newRow("case3") << "94810DE3-51B8-469E-B316-00248FFA2A45" << true;
        QTest::newRow("case4") << " 999-999 " << false;
        QTest::newRow("case5") << "{}" << false;
        QTest::newRow("case6") << "" << false;
        // clang-format on
    }

    void validatorWithoutBraces()
    {
        QFETCH(QString, uuidString);
        QFETCH(bool, result);
        QVERIFY(UuidTools::isValidUuidStringWihtoutBraces(uuidString) == result);
    }
};

QTEST_APPLESS_MAIN(UuidsTest)

#include "uuidstest.moc"
