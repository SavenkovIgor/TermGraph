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

#include "source/helpers/fsworks.h"

TEST(FSWorks, PathWorks)
{
    auto testPath = FSWorks::workingDirPath() + "/testPath";

    ASSERT_FALSE(FSWorks::pathExist(testPath));
    ASSERT_TRUE(FSWorks::createPath(testPath));
    ASSERT_TRUE(FSWorks::pathExist(testPath));
    ASSERT_TRUE(FSWorks::deletePath(testPath));
    ASSERT_FALSE(FSWorks::pathExist(testPath));
}

TEST(FSWorks, FileWorks)
{
    auto testPath     = FSWorks::workingDirPath() + "/testPath";
    auto testFilePath = testPath + "/testFile.tst";

    // Path not exiting
    ASSERT_FALSE(FSWorks::createFile(testFilePath));
    ASSERT_FALSE(FSWorks::fileExist(testFilePath));

    // Creating path
    ASSERT_TRUE(FSWorks::createPath(testPath));

    // Creating file
    ASSERT_TRUE(FSWorks::createFile(testFilePath));
    ASSERT_TRUE(FSWorks::fileExist(testFilePath));

    // Deleting file
    ASSERT_TRUE(FSWorks::deleteFile(testFilePath));
    ASSERT_FALSE(FSWorks::deleteFile(testFilePath)); // Already deleted
    ASSERT_FALSE(FSWorks::fileExist(testFilePath));

    // Deleting path
    ASSERT_TRUE(FSWorks::deletePath(testPath));
    ASSERT_FALSE(FSWorks::deletePath(testPath));
}
