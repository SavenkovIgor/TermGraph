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

#include <gtest/gtest.h>

#include "source/Helpers/fsworks.h"
#include "source/Managers/localdatabasestorage.h"

class DBWorksTest : public ::testing::Test
{
public:
    static constexpr auto sDbFileName = "testDatabase.termGraph";
    // Group strings
    static const QUuid   mGroupUuid1;
    static const QString mGroupName1;
    static const QString mGroupName2;
    static const QString mGroupComment1;
    static const QString mGroupComment2;
    static const QString mSpecSymbols;

    static std::unique_ptr<DataStorageInterface> mStorage;

    static void SetUpTestCase() {
        mStorage = std::make_unique<LocalDatabaseStorage>(QString(sDbFileName));

        EXPECT_TRUE(FSWorks::fileExist(sDbFileName));

        EXPECT_EQ(mStorage->storageVersion(), 2);
        EXPECT_TRUE(mStorage->getAllGroupsUuids().empty());
        EXPECT_TRUE(mStorage->getGroups().empty());
        EXPECT_TRUE(mStorage->getAllNodesUuids().empty());
    }

    static void TearDownTestCase() {
        EXPECT_TRUE(FSWorks::deleteFile(sDbFileName));
    }

    GroupInfoContainer getGroupWithUuid() { return GroupInfoContainer{mGroupUuid1, mGroupName1, mGroupComment1}; }

    GroupInfoContainer getGroupWithoutUuid() { return GroupInfoContainer{QUuid(), mGroupName2, mGroupComment2}; }

    std::vector<NodeInfoContainer> getNodes()
    {
        std::vector<NodeInfoContainer> ret;

        std::vector<std::pair<const char*, const char*>> data = {{"1", ""},
                                                                 {"2", ""},
                                                                 {"3", "{1}{2}"},
                                                                 {"4", "{1}{2}"},
                                                                 {"5", "{1}"},
                                                                 {"6", "{5}"},
                                                                 {"7", "{5}"},
                                                                 {"8", "{6}{7}"},
                                                                 {"9", ""}};

        auto group = getGroupWithUuid();

        for (const auto& pair : data) {
            NodeInfoContainer node;
            node.uuid        = QUuid::createUuid();
            node.term        = QString(pair.first) + mSpecSymbols;
            node.definition  = QString(pair.second) + mSpecSymbols;
            node.description = mSpecSymbols;
            node.examples    = mSpecSymbols;
            node.wikiUrl     = mSpecSymbols;
            node.wikiImage   = mSpecSymbols;
            node.groupUuid   = group.uuid;

            ret.push_back(node);
        }

        return ret;
    }
};

const QUuid   DBWorksTest::mGroupUuid1    = QUuid::createUuid();
const QString DBWorksTest::mGroupName1    = QStringLiteral("TestGroup1");
const QString DBWorksTest::mGroupName2    = QStringLiteral("TestGroup2");
const QString DBWorksTest::mGroupComment1 = QStringLiteral("commentText1");
const QString DBWorksTest::mGroupComment2 = QStringLiteral("commentText2");
const QString DBWorksTest::mSpecSymbols   = QStringLiteral("!@#$%^&*()-+=*:/'\"\\/");

std::unique_ptr<DataStorageInterface> DBWorksTest::mStorage;

TEST_F (DBWorksTest, GroupsTest) {
    EXPECT_TRUE(mStorage->getAllGroupsUuids().empty());

    auto withUuid    = getGroupWithUuid();
    auto withoutUuid = getGroupWithoutUuid();

    ASSERT_FALSE(mStorage->groupExist(withUuid.uuid));

    // Add groups test
    EXPECT_TRUE(mStorage->addGroup(withUuid));
    EXPECT_TRUE(mStorage->addGroup(withoutUuid));

    EXPECT_TRUE(mStorage->groupExist(withUuid.uuid));

    // GetAllGroupsUuids test
    auto groupList = mStorage->getAllGroupsUuids();

    EXPECT_EQ(groupList.size(), 2);
    EXPECT_TRUE(groupList[0] == withUuid.uuid || groupList[1] == withUuid.uuid);
    EXPECT_FALSE(groupList[0].isNull());

    // Read group test
    auto readedWithUuid = mStorage->getGroup(withUuid.uuid);

    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
    EXPECT_EQ(withUuid.name, readedWithUuid.name);
    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

    // Update group test
    withUuid.name += mSpecSymbols;
    withUuid.comment += mSpecSymbols;

    EXPECT_TRUE(mStorage->updateGroup(withUuid));

    readedWithUuid = mStorage->getGroup(withUuid.uuid);

    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
    EXPECT_EQ(withUuid.name, readedWithUuid.name);
    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

    withUuid = getGroupWithUuid();

    // Delete group test
    mStorage->deleteGroup(withUuid.uuid);
    groupList = mStorage->getAllGroupsUuids();
    EXPECT_EQ(groupList.size(), 1);
    mStorage->deleteGroup(groupList.front());
    groupList = mStorage->getAllGroupsUuids();
    EXPECT_TRUE(groupList.empty());
}

TEST_F (DBWorksTest, NodesTest) {

    ASSERT_TRUE(mStorage->getAllGroupsUuids().empty());

    auto withUuid = getGroupWithUuid();

    EXPECT_TRUE(mStorage->addGroup(withUuid));
    EXPECT_TRUE(mStorage->getAllNodesUuids().empty());

    // Adding nodes

    auto nodesList = getNodes();

    for (auto& node : nodesList) {
        EXPECT_FALSE(mStorage->nodeExist(node.uuid));
        EXPECT_TRUE(mStorage->addNode(node));
        EXPECT_TRUE(mStorage->nodeExist(node.uuid));
        EXPECT_EQ(mStorage->findNode(node.term, node.groupUuid), node.uuid);
        auto gettedNode = mStorage->getNode(node.uuid);
        node.lastEdit   = gettedNode.lastEdit; // Last edit was refreshed
        EXPECT_TRUE(gettedNode.isEqualTo(node));
    }

    // Checking all uuids without group
    auto allNodesUuids = mStorage->getAllNodesUuids();

    EXPECT_EQ(allNodesUuids.size(), nodesList.size());
    for (const auto& node : nodesList) {
        auto searchResult = std::find(allNodesUuids.begin(), allNodesUuids.end(), node.uuid);
        EXPECT_NE(searchResult, allNodesUuids.end());
    }

    // Checking all uuids with group
    allNodesUuids = mStorage->getAllNodesUuids(withUuid.uuid);

    EXPECT_EQ(allNodesUuids.size(), nodesList.size());
    for (const auto& node : nodesList) {
        auto searchResult = std::find(allNodesUuids.begin(), allNodesUuids.end(), node.uuid);
        EXPECT_NE(searchResult, allNodesUuids.end());
    }

    for (auto& node : nodesList) {
        node.term += "1";
        node.definition += "1";
        node.description += "1";
        node.examples += "1";
        node.wikiUrl += "1";
        node.wikiImage += "1";

        EXPECT_TRUE(mStorage->updateNode(node, DataStorageInterface::LastEditSource::TakeFromNodeInfo));

        EXPECT_TRUE(node.isEqualTo(mStorage->getNode(node.uuid)));
    }

    for (const auto& node : nodesList)
        mStorage->deleteNode(node.uuid);

    EXPECT_TRUE(mStorage->getAllNodesUuids().empty());
}
