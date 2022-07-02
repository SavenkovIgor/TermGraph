// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "source/helpers/fsworks.h"

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
