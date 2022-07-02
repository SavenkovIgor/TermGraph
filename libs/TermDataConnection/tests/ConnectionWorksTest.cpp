// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

//#include <memory>

//#include <QCoreApplication>
//#include <QDir>
//#include <QTimer>

//#include <gtest/gtest.h>

//#include <TermDataConnection/DataStorageConnection.h>

//// TODO: Check creation of empty database with LocalDatabaseStorage

//class DBWorksTest : public ::testing::Test
//{
//public:
//    static constexpr auto sDbFileName = "127.0.0.1";
//    // Group strings
//    static const GroupUuid mGroupUuid1;
//    static const QString   mGroupName1;
//    static const QString   mGroupName2;
//    static const QString   mGroupComment1;
//    static const QString   mGroupComment2;
//    static const QString   mSpecSymbols;

//    static std::unique_ptr<DataStorageInterface> mStorage;

//    static void SetUpTestCase()
//    {
//        mStorage = std::make_unique<DataStorageConnection>(QHostAddress(sDbFileName));

//        ASSERT_FALSE(mStorage->getAllGroupsUuids().result().has_error());

//        EXPECT_EQ(mStorage->storageVersion(), 2);
//        EXPECT_TRUE(mStorage->getAllGroupsUuids().result().value().empty());
//        EXPECT_TRUE(mStorage->getGroups().result().value().empty());
//    }

//    static void TearDownTestCase()
//    {
//        QDir dir;
//        EXPECT_TRUE(dir.remove(sDbFileName));
//    }

//    GroupData getGroupWithUuid() { return GroupData{mGroupUuid1, mGroupName1, mGroupComment1}; }

//    GroupData getGroupWithoutUuid() { return GroupData{std::nullopt, mGroupName2, mGroupComment2}; }

//    TermData::List getTermDataList()
//    {
//        TermData::List ret;

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
//            TermData term{
//                .uuid        = TermUuid::generate(),
//                .term        = QString(pair.first) + mSpecSymbols,
//                .definition  = QString(pair.second) + mSpecSymbols,
//                .description = mSpecSymbols,
//                .examples    = mSpecSymbols,
//                .wikiUrl     = mSpecSymbols,
//                .wikiImage   = mSpecSymbols,
//                .groupUuid   = *group.uuid,
//            };

//            ret.push_back(term);
//        }

//        return ret;
//    }
//};

//const GroupUuid DBWorksTest::mGroupUuid1    = GroupUuid::generate();
//const QString   DBWorksTest::mGroupName1    = QStringLiteral("TestGroup1");
//const QString   DBWorksTest::mGroupName2    = QStringLiteral("TestGroup2");
//const QString   DBWorksTest::mGroupComment1 = QStringLiteral("commentText1");
//const QString   DBWorksTest::mGroupComment2 = QStringLiteral("commentText2");
//const QString   DBWorksTest::mSpecSymbols   = QStringLiteral("!@#$%^&*()-+=*:/'\"\\/");

//std::unique_ptr<DataStorageInterface> DBWorksTest::mStorage;

//TEST_F(DBWorksTest, GroupsTest)
//{
//    EXPECT_TRUE(mStorage->getAllGroupsUuids().result().value().empty());

//    auto withUuid    = getGroupWithUuid();
//    auto withoutUuid = getGroupWithoutUuid();

//    ASSERT_FALSE(mStorage->getGroup(*withUuid.uuid).result().has_value());

//    // Add groups test
//    auto brokenName = withUuid;
//    brokenName.name = " ";
//    EXPECT_EQ(mStorage->addGroup(brokenName).result().error(), DbErrorCodes::GroupNameEmpty);

//    EXPECT_EQ(mStorage->addGroup(withUuid).result().value(), withUuid);

//    // Duplicate test
//    EXPECT_EQ(mStorage->addGroup(withUuid).result().error(), DbErrorCodes::GroupUuidAlreadyExist);
//    auto sameName = withUuid;
//    sameName.uuid = GroupUuid::generate();
//    EXPECT_EQ(mStorage->addGroup(sameName).result().error(), DbErrorCodes::GroupNameAlreadyExist);

//    auto addResult = mStorage->addGroup(withoutUuid).result();
//    EXPECT_TRUE(addResult.has_value());
//    EXPECT_EQ(mStorage->getGroup(*(addResult.value().uuid)).result(), addResult);

//    // GetAllGroupsUuids test
//    auto groupList = mStorage->getAllGroupsUuids().result().value();

//    EXPECT_EQ(groupList.size(), 2);
//    EXPECT_TRUE(groupList[0] == withUuid.uuid || groupList[1] == withUuid.uuid);

//    // Read group test
//    EXPECT_EQ(mStorage->getGroup(GroupUuid::generate()).result().error(), DbErrorCodes::GroupUuidNotFound);

//    auto readedWithUuid = mStorage->getGroup(*withUuid.uuid).result().value();

//    EXPECT_EQ(withUuid, readedWithUuid);
//    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
//    EXPECT_EQ(withUuid.name, readedWithUuid.name);
//    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

//    // Update group test
//    EXPECT_EQ(mStorage->updateGroup(withoutUuid).result().error(), DbErrorCodes::GroupUuidInvalid);

//    withUuid.name += mSpecSymbols;
//    withUuid.comment += mSpecSymbols;

//    EXPECT_TRUE(mStorage->updateGroup(withUuid).result().has_value());

//    readedWithUuid = mStorage->getGroup(*withUuid.uuid).result().value();

//    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
//    EXPECT_EQ(withUuid.name, readedWithUuid.name);
//    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

//    withUuid = getGroupWithUuid();

//    // Delete group test
//    EXPECT_EQ(mStorage->deleteGroup(GroupUuid::generate()).result().error(), DbErrorCodes::GroupUuidNotFound);

//    EXPECT_TRUE(mStorage->deleteGroup(*withUuid.uuid).result().has_value());
//    groupList = mStorage->getAllGroupsUuids().result().value();
//    EXPECT_EQ(groupList.size(), 1);
//    EXPECT_TRUE(mStorage->deleteGroup(groupList.front()).result().has_value());
//    groupList = mStorage->getAllGroupsUuids().result().value();
//    EXPECT_TRUE(groupList.empty());
//}

//TEST_F(DBWorksTest, TermsTest)
//{
//    ASSERT_TRUE(mStorage->getAllGroupsUuids().result().value().empty());

//    auto withUuid = getGroupWithUuid();

//    EXPECT_TRUE(mStorage->addGroup(withUuid).result().has_value());

//    // Adding terms

//    auto termList = getTermDataList();

//    for (auto& term : termList) {
//        auto termRes = mStorage->addTerm(term).result();
//        EXPECT_TRUE(termRes.has_value());
//        EXPECT_EQ(mStorage->getTerm(term.term, term.groupUuid).result().value().uuid, term.uuid);
//        auto gettedTerm = mStorage->getTerm(*term.uuid).result().value();
//        term.lastEdit   = gettedTerm.lastEdit; // Last edit was refreshed
//        EXPECT_TRUE(gettedTerm == term);
//    }

//    for (auto& term : termList) {
//        term.term += "1";
//        term.definition += "1";
//        term.description += "1";
//        term.examples += "1";
//        term.wikiUrl += "1";
//        term.wikiImage += "1";

//        EXPECT_TRUE(
//            mStorage->updateTerm(term, DataStorageInterface::LastEditSource::TakeFromTermData).result().has_value());

//        EXPECT_TRUE(term == mStorage->getTerm(*term.uuid).result().value());
//    }

//    for (const auto& term : termList)
//        EXPECT_TRUE(mStorage->deleteTerm(*term.uuid).result().has_value());
//}

//int main(int argc, char* argv[])
//{
//    qDebug("appstart");
//    QCoreApplication app{argc, argv};

//    QTimer::singleShot(0, [&]() {
//        qDebug("tstart");
//        ::testing::InitGoogleTest(&argc, argv);
//        auto testResult = RUN_ALL_TESTS();
//        app.exit(testResult);
//    });

//    return app.exec();
//}
