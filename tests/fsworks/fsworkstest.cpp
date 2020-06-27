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

#include <QCoreApplication>
#include <QtTest>

#include <QDebug>

// add necessary includes here
#include "source/Helpers/fsworks.h"

class FSWorksTest : public QObject
{
    Q_OBJECT

public:
    FSWorksTest()           = default;
    ~FSWorksTest() override = default;

private slots:
    void pathWorks()
    {
        auto testPath = FSWorks::workingDirPath() + "/testPath";

        QVERIFY(!FSWorks::pathExist(testPath));
        QVERIFY(FSWorks::createPath(testPath));
        QVERIFY(FSWorks::pathExist(testPath));
        QVERIFY(FSWorks::deletePath(testPath));
        QVERIFY(!FSWorks::pathExist(testPath));
    }

    void fileWorks()
    {
        auto testPath     = FSWorks::workingDirPath() + "/testPath";
        auto testFilePath = testPath + "/testFile.tst";

        // Path not exiting
        QVERIFY(!FSWorks::createFile(testFilePath));
        QVERIFY(!FSWorks::fileExist(testFilePath));

        // Creating path
        QVERIFY(FSWorks::createPath(testPath));

        // Creating file
        QVERIFY(FSWorks::createFile(testFilePath));
        QVERIFY(FSWorks::fileExist(testFilePath));

        // Deleting file
        QVERIFY(FSWorks::deleteFile(testFilePath));
        QVERIFY(!FSWorks::deleteFile(testFilePath)); // Already deleted
        QVERIFY(!FSWorks::fileExist(testFilePath));

        // Deleting path
        QVERIFY(FSWorks::deletePath(testPath));
        QVERIFY(!FSWorks::deletePath(testPath));
    }
};

QTEST_APPLESS_MAIN(FSWorksTest)

#include "fsworkstest.moc"
