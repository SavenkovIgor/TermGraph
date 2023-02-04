// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <memory>

#include <QDir>

#include <gtest/gtest.h>

#include <TermDataStorage/LocalDataStorage.h>

// TODO: Check creation of empty database with LocalDatabaseStorage

class DBWorksTest : public ::testing::Test
{
public:
    static constexpr auto sDbFileName = "testDatabase.termGraph";
    // Group strings
    static const GroupUuid mGroupUuid1;
    static const QString   mGroupName1;
    static const QString   mGroupName2;
    static const QString   mGroupComment1;
    static const QString   mGroupComment2;
    static const QString   mSpecSymbols;

    static std::unique_ptr<DataStorageInterface> mStorage;

    static void SetUpTestCase()
    {
        Q_INIT_RESOURCE(SqlQueries);

        mStorage = std::make_unique<LocalDatabaseStorage>(QString(sDbFileName), QString(""));

        QDir dir;
        EXPECT_TRUE(dir.exists(sDbFileName));

        EXPECT_EQ(mStorage->storageVersion(), 2);
        EXPECT_TRUE(mStorage->groups().result().value().empty());
    }

    static void TearDownTestCase()
    {
        QDir dir;
        EXPECT_TRUE(dir.remove(sDbFileName));
    }

    GroupSummary groupWithUuid() { return GroupSummary{.uuid = mGroupUuid1, .name = mGroupName1, .comment = mGroupComment1}; }

    GroupSummary groupWithoutUuid()
    {
        return GroupSummary{.uuid = std::nullopt, .name = mGroupName2, .comment = mGroupComment2};
    }

    TermData::List termDataList()
    {
        TermData::List ret;

        std::vector<std::pair<const char*, const char*>> data = {{"1", ""},
                                                                 {"2", ""},
                                                                 {"3", "{1}{2}"},
                                                                 {"4", "{1}{2}"},
                                                                 {"5", "{1}"},
                                                                 {"6", "{5}"},
                                                                 {"7", "{5}"},
                                                                 {"8", "{6}{7}"},
                                                                 {"9", ""}};

        auto group = groupWithUuid();

        for (const auto& pair : data) {
            TermData term{.uuid        = TermUuid::generate(),
                          .term        = QString(pair.first) + mSpecSymbols,
                          .definition  = QString(pair.second) + mSpecSymbols,
                          .description = mSpecSymbols,
                          .examples    = mSpecSymbols,
                          .wikiUrl     = mSpecSymbols,
                          .wikiImage   = mSpecSymbols,
                          .groupUuid   = *group.uuid,
                          .lastEdit    = QDateTime()};

            ret.push_back(term);
        }

        return ret;
    }
};

const GroupUuid DBWorksTest::mGroupUuid1    = GroupUuid::generate();
const QString   DBWorksTest::mGroupName1    = QStringLiteral("TestGroup1");
const QString   DBWorksTest::mGroupName2    = QStringLiteral("TestGroup2");
const QString   DBWorksTest::mGroupComment1 = QStringLiteral("commentText1");
const QString   DBWorksTest::mGroupComment2 = QStringLiteral("commentText2");
const QString   DBWorksTest::mSpecSymbols   = QStringLiteral("!@#$%^&*()-+=*:/'\"\\/");

std::unique_ptr<DataStorageInterface> DBWorksTest::mStorage;

TEST_F(DBWorksTest, GroupsTest)
{
    EXPECT_TRUE(mStorage->groups().result().value().empty());

    auto withUuid    = groupWithUuid();
    auto withoutUuid = groupWithoutUuid();

    ASSERT_FALSE(mStorage->group(*withUuid.uuid).result().has_value());

    // Add groups test
    auto brokenName = withUuid;
    brokenName.name = " ";
    EXPECT_EQ(mStorage->addGroup(brokenName).result().error(), ErrorCodes::GroupNameEmpty);

    EXPECT_EQ(mStorage->addGroup(withUuid).result().value(), withUuid);

    // Duplicate test
    EXPECT_EQ(mStorage->addGroup(withUuid).result().error(), ErrorCodes::GroupUuidAlreadyExist);
    auto sameName = withUuid;
    sameName.uuid = GroupUuid::generate();
    EXPECT_EQ(mStorage->addGroup(sameName).result().error(), ErrorCodes::GroupNameAlreadyExist);

    auto addResult = mStorage->addGroup(withoutUuid).result();
    EXPECT_TRUE(addResult.has_value());
    EXPECT_EQ(mStorage->group(*(addResult.value().uuid)).result().value(), addResult.value());

    // GetAllGroupsUuids test
    auto groupList = mStorage->groups().result().value();

    EXPECT_EQ(groupList.size(), 2);
    EXPECT_TRUE(groupList[0].uuid == withUuid.uuid || groupList[1].uuid == withUuid.uuid);

    // Read group test
    EXPECT_EQ(mStorage->group(GroupUuid::generate()).result().error(), ErrorCodes::GroupUuidNotFound);

    auto readedWithUuid = mStorage->group(*withUuid.uuid).result().value();

    EXPECT_EQ(withUuid, readedWithUuid);
    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
    EXPECT_EQ(withUuid.name, readedWithUuid.name);
    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

    // Update group test
    EXPECT_EQ(mStorage->updateGroup(withoutUuid).result().error(), ErrorCodes::GroupUuidInvalid);

    withUuid.name += mSpecSymbols;
    withUuid.comment += mSpecSymbols;

    EXPECT_TRUE(mStorage->updateGroup(withUuid).result().has_value());

    readedWithUuid = mStorage->group(*withUuid.uuid).result().value();

    EXPECT_EQ(withUuid.uuid, readedWithUuid.uuid);
    EXPECT_EQ(withUuid.name, readedWithUuid.name);
    EXPECT_EQ(withUuid.comment, readedWithUuid.comment);

    withUuid = groupWithUuid();

    // Delete group test
    EXPECT_EQ(mStorage->deleteGroup(GroupUuid::generate()).result().error(), ErrorCodes::GroupUuidNotFound);

    EXPECT_TRUE(mStorage->deleteGroup(*withUuid.uuid).result().has_value());
    groupList = mStorage->groups().result().value();
    EXPECT_EQ(groupList.size(), 1);
    EXPECT_TRUE(mStorage->deleteGroup(groupList.front().uuid.value()).result().has_value());
    groupList = mStorage->groups().result().value();
    EXPECT_TRUE(groupList.empty());
}

TEST_F(DBWorksTest, TermsTest)
{
    ASSERT_TRUE(mStorage->groups().result().value().empty());

    auto withUuid = groupWithUuid();

    EXPECT_TRUE(mStorage->addGroup(withUuid).result().has_value());

    // Adding terms

    auto termList = termDataList();

    for (auto& term : termList) {
        auto termRes = mStorage->addTerm(term).result();
        EXPECT_TRUE(termRes.has_value());
        EXPECT_EQ(mStorage->term(term.term, term.groupUuid).result().value().uuid, term.uuid);
        auto gettedTerm = mStorage->term(*term.uuid).result().value();
        term.lastEdit   = gettedTerm.lastEdit; // Last edit was refreshed
        EXPECT_TRUE(gettedTerm == term);
    }

    for (auto& term : termList) {
        term.term += "1";
        term.definition += "1";
        term.description += "1";
        term.examples += "1";
        term.wikiUrl += "1";
        term.wikiImage += "1";

        EXPECT_TRUE(mStorage->updateTerm(term, DataStorageInterface::LastEditSource::FromData, true).result().has_value());

        EXPECT_TRUE(term == mStorage->term(*term.uuid).result().value());
    }

    for (const auto& term : termList) {
        EXPECT_TRUE(mStorage->deleteTerm(*term.uuid).result().has_value());
    }
}
