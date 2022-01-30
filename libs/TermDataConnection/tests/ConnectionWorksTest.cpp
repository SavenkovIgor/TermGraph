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

#include <QDir>

#include <gtest/gtest.h>

#include <TermDataStorage/LocalDataStorage.h>

// TODO: Check creation of empty database with LocalDatabaseStorage

//class DBWorksTest : public ::testing::Test
//{
//public:
//    static constexpr auto sDbFileName = "testDatabase.termGraph";
//    // Group strings
//    static const QUuid   mGroupUuid1;
//    static const QString mGroupName1;
//    static const QString mGroupName2;
//    static const QString mGroupComment1;
//    static const QString mGroupComment2;
//    static const QString mSpecSymbols;

//    static std::unique_ptr<DataStorageInterface> mStorage;

//    static void SetUpTestCase()
//    {
//        Q_INIT_RESOURCE(SqlQueries);

//        mStorage = std::make_unique<LocalDatabaseStorage>(QString(sDbFileName), QString(""));

//        QDir dir;
//        EXPECT_TRUE(dir.exists(sDbFileName));

//        EXPECT_EQ(mStorage->storageVersion(), 2);
//        EXPECT_TRUE(mStorage->getAllGroupsUuids().empty());
//        EXPECT_TRUE(mStorage->getGroups().empty());
//        EXPECT_TRUE(mStorage->getAllTermsUuids().empty());
//    }

//    static void TearDownTestCase()
//    {
//        QDir dir;
//        EXPECT_TRUE(dir.remove(sDbFileName));
//    }

//    GroupData getGroupWithUuid() { return GroupData{mGroupUuid1, mGroupName1, mGroupComment1}; }

//    GroupData getGroupWithoutUuid() { return GroupData{QUuid(), mGroupName2, mGroupComment2}; }

//    TermData::List getTermDataList()
//    {
//        std::vector<TermData> ret;

//        std::vector<std::pair<const char*, const char*>> data = {{"1", ""},
//                                                                 {"2", ""},
//                                                                 {"3", "{1}{2}"},
//                                                                 {"4", "{1}{2}"},
//                                                                 {"5", "{1}"},
//                                                                 {"6", "{5}"},
//                                                                 {"7", "{5}"},
//                                                                 {"8", "{6}{7}"},
//                                                                 {"9", ""}};

//        auto group = getGroupWithUuid();

//        for (const auto& pair : data) {
//            TermData term;
//            term.uuid        = QUuid::createUuid();
//            term.term        = QString(pair.first) + mSpecSymbols;
//            term.definition  = QString(pair.second) + mSpecSymbols;
//            term.description = mSpecSymbols;
//            term.examples    = mSpecSymbols;
//            term.wikiUrl     = mSpecSymbols;
//            term.wikiImage   = mSpecSymbols;
//            term.groupUuid   = group.uuid;

//            ret.push_back(term);
//        }

//        return ret;
//    }
//};

//const QUuid   DBWorksTest::mGroupUuid1    = QUuid::createUuid();
//const QString DBWorksTest::mGroupName1    = QStringLiteral("TestGroup1");
//const QString DBWorksTest::mGroupName2    = QStringLiteral("TestGroup2");
//const QString DBWorksTest::mGroupComment1 = QStringLiteral("commentText1");
//const QString DBWorksTest::mGroupComment2 = QStringLiteral("commentText2");
//const QString DBWorksTest::mSpecSymbols   = QStringLiteral("!@#$%^&*()-+=*:/'\"\\/");

//std::unique_ptr<DataStorageInterface> DBWorksTest::mStorage;

//TEST_F(DBWorksTest, GroupsTest)
//{
//    EXPECT_TRUE(mStorage->getAllGroupsUuids().empty());

//    auto withUuid    = getGroupWithUuid();
//    auto withoutUuid = getGroupWithoutUuid();

//    ASSERT_FALSE(mStorage->groupExist(GroupUuid::create(withUuid.uuid).value()));

//    // Add groups test
//    auto brokenName = withUuid;
//    brokenName.name = " ";
//    EXPECT_EQ(mStorage->addGroup(brokenName).error(), DbErrorCodes::GroupNameEmpty);

//    EXPECT_FALSE(mStorage->addGroup(withUuid).has_error());

//    // Duplicate test
//    EXPECT_EQ(mStorage->addGroup(withUuid).error(), DbErrorCodes::GroupUuidAlreadyExist);
//    auto sameName = withUuid;
//    sameName.uuid = QUuid::createUuid();
//    EXPECT_EQ(mStorage->addGroup(sameName).error(), DbErrorCodes::GroupNameAlreadyExist);

//    EXPECT_FALSE(mStorage->addGroup(withoutUuid).has_error());

//    EXPECT_TRUE(mStorage->groupExist(GroupUuid::create(withUuid.uuid).value()));

//    // GetAllGroupsUuids test
//    auto groupList = mStorage->getAllGroupsUuids();

//    EXPECT_EQ(groupList.size(), 2);
//    EXPECT_TRUE(groupList[0] == withUuid.uuid || groupList[1] == withUuid.uuid);

//    // Read group test
//    EXPECT_EQ(mStorage->getGroup(GroupUuid::generate()).error(), DbErrorCodes::GroupUuidNotFound);

//    auto readedWithUuid = mStorage->getGroup(GroupUuid::create(withUuid.uuid).value()).value();

//    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
//    EXPECT_EQ(withUuid.name, readedWithUuid.name);
//    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

//    // Update group test
//    EXPECT_EQ(mStorage->updateGroup(withoutUuid).error(), DbErrorCodes::GroupUuidInvalid);

//    withUuid.name += mSpecSymbols;
//    withUuid.comment += mSpecSymbols;

//    EXPECT_TRUE(mStorage->updateGroup(withUuid));

//    readedWithUuid = mStorage->getGroup(*GroupUuid::create(withUuid.uuid)).value();

//    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
//    EXPECT_EQ(withUuid.name, readedWithUuid.name);
//    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

//    withUuid = getGroupWithUuid();

//    // Delete group test
//    EXPECT_EQ(mStorage->deleteGroup(GroupUuid::generate()).error(), DbErrorCodes::GroupUuidNotFound);

//    EXPECT_TRUE(mStorage->deleteGroup(*GroupUuid::create(withUuid.uuid)));
//    groupList = mStorage->getAllGroupsUuids();
//    EXPECT_EQ(groupList.size(), 1);
//    EXPECT_TRUE(mStorage->deleteGroup(*GroupUuid::create(groupList.front())));
//    groupList = mStorage->getAllGroupsUuids();
//    EXPECT_TRUE(groupList.empty());
//}

//TEST_F(DBWorksTest, TermsTest)
//{
//    ASSERT_TRUE(mStorage->getAllGroupsUuids().empty());

//    auto withUuid = getGroupWithUuid();

//    EXPECT_TRUE(mStorage->addGroup(withUuid));
//    EXPECT_TRUE(mStorage->getAllTermsUuids().empty());

//    // Adding terms

//    auto termList = getTermDataList();

//    for (auto& term : termList) {
//        EXPECT_TRUE(mStorage->addTerm(term));
//        EXPECT_EQ(mStorage->findTerm(term.term, *GroupUuid::create(term.groupUuid)).value(), term.uuid);
//        auto gettedTerm = mStorage->getTerm(*TermUuid::create(term.uuid)).value();
//        term.lastEdit   = gettedTerm.lastEdit; // Last edit was refreshed
//        EXPECT_TRUE(gettedTerm == term);
//    }

//    // Checking all uuids without group
//    auto allTermUuids = mStorage->getAllTermsUuids();

//    EXPECT_EQ(allTermUuids.size(), termList.size());
//    for (const auto& term : termList) {
//        auto searchResult = std::find(allTermUuids.begin(), allTermUuids.end(), term.uuid);
//        EXPECT_NE(searchResult, allTermUuids.end());
//    }

//    // Checking all uuids with group
//    allTermUuids = mStorage->getAllTermsUuids(*GroupUuid::create(withUuid.uuid));

//    EXPECT_EQ(allTermUuids.size(), termList.size());
//    for (const auto& term : termList) {
//        auto searchResult = std::find(allTermUuids.begin(), allTermUuids.end(), term.uuid);
//        EXPECT_NE(searchResult, allTermUuids.end());
//    }

//    for (auto& term : termList) {
//        term.term += "1";
//        term.definition += "1";
//        term.description += "1";
//        term.examples += "1";
//        term.wikiUrl += "1";
//        term.wikiImage += "1";

//        EXPECT_TRUE(mStorage->updateTerm(term, DataStorageInterface::LastEditSource::TakeFromTermData));

//        EXPECT_TRUE(term == mStorage->getTerm(*TermUuid::create(term.uuid)).value());
//    }

//    for (const auto& term : termList)
//        EXPECT_TRUE(mStorage->deleteTerm(*TermUuid::create(term.uuid)));

//    EXPECT_TRUE(mStorage->getAllTermsUuids().empty());
//}
