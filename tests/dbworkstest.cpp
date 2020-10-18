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

#include "source/Helpers/fsworks.h"
#include "source/Managers/localdatabasestorage.h"

class DBWorksTest : public QObject
{
    Q_OBJECT

public:
    DBWorksTest()
        : mStorage(new LocalDatabaseStorage(QString(sDbFileName)))
    {
        QVERIFY(FSWorks::fileExist(sDbFileName));

        QVERIFY(mStorage->storageVersion() == 2);
        QVERIFY(mStorage->getAllGroupsUuids().empty());
        QVERIFY(mStorage->getGroups().empty());
        QVERIFY(mStorage->getAllNodesUuids().empty());
    }

    ~DBWorksTest() { QVERIFY(FSWorks::deleteFile(sDbFileName)); }

private slots:
    void groupsTest()
    {
        QVERIFY(mStorage->getAllGroupsUuids().empty());

        auto withUuid    = getGroupWithUuid();
        auto withoutUuid = getGroupWithoutUuid();

        QVERIFY(!mStorage->groupExist(withUuid.uuid));

        // Add groups test
        QVERIFY(mStorage->addGroup(withUuid));
        QVERIFY(mStorage->addGroup(withoutUuid));

        QVERIFY(mStorage->groupExist(withUuid.uuid));

        // GetAllGroupsUuids test
        auto groupList = mStorage->getAllGroupsUuids();

        QVERIFY(groupList.size() == 2);
        QVERIFY(groupList[0] == withUuid.uuid || groupList[1] == withUuid.uuid);
        QVERIFY(!groupList[0].isNull());

        // Read group test
        auto readedWithUuid = mStorage->getGroup(withUuid.uuid);

        QVERIFY(withUuid.uuid == readedWithUuid.uuid);
        QVERIFY(withUuid.name == readedWithUuid.name);
        QVERIFY(withUuid.comment == readedWithUuid.comment);

        // Update group test
        withUuid.name += mSpecSymbols;
        withUuid.comment += mSpecSymbols;

        QVERIFY(mStorage->updateGroup(withUuid));

        readedWithUuid = mStorage->getGroup(withUuid.uuid);

        QVERIFY(withUuid.uuid == readedWithUuid.uuid);
        QVERIFY(withUuid.name == readedWithUuid.name);
        QVERIFY(withUuid.comment == readedWithUuid.comment);

        withUuid = getGroupWithUuid();

        // Delete group test
        mStorage->deleteGroup(withUuid.uuid);
        groupList = mStorage->getAllGroupsUuids();
        QVERIFY(groupList.size() == 1);
        mStorage->deleteGroup(groupList.front());
        groupList = mStorage->getAllGroupsUuids();
        QVERIFY(groupList.empty());
    }

    void nodesTest()
    {
        QVERIFY(mStorage->getAllGroupsUuids().empty());

        auto withUuid = getGroupWithUuid();

        QVERIFY(mStorage->addGroup(withUuid));
        QVERIFY(mStorage->getAllNodesUuids().empty());

        // Adding nodes

        auto nodesList = getNodes();

        for (auto& node : nodesList) {
            QVERIFY(!mStorage->nodeExist(node.uuid));
            QVERIFY(mStorage->addNode(node));
            QVERIFY(mStorage->nodeExist(node.uuid));
            QVERIFY(mStorage->findNode(node.term, node.groupUuid) == node.uuid);
            auto gettedNode = mStorage->getNode(node.uuid);
            node.lastEdit   = gettedNode.lastEdit; // Last edit was refreshed
            QVERIFY(gettedNode.isEqualTo(node));
        }

        // Checking all uuids without group
        auto allNodesUuids = mStorage->getAllNodesUuids();

        QVERIFY(allNodesUuids.size() == nodesList.size());
        for (const auto& node : nodesList) {
            auto searchResult = std::find(allNodesUuids.begin(), allNodesUuids.end(), node.uuid);
            QVERIFY(searchResult != allNodesUuids.end());
        }

        // Checking all uuids with group
        allNodesUuids = mStorage->getAllNodesUuids(withUuid.uuid);

        QVERIFY(allNodesUuids.size() == nodesList.size());
        for (const auto& node : nodesList) {
            auto searchResult = std::find(allNodesUuids.begin(), allNodesUuids.end(), node.uuid);
            QVERIFY(searchResult != allNodesUuids.end());
        }

        for (auto& node : nodesList) {
            node.term += "1";
            node.definition += "1";
            node.description += "1";
            node.examples += "1";
            node.wikiUrl += "1";
            node.wikiImage += "1";

            QVERIFY(mStorage->updateNode(node, DataStorageInterface::LastEditSource::TakeFromNodeInfo));

            QVERIFY(node.isEqualTo(mStorage->getNode(node.uuid)));
        }

        for (const auto& node : nodesList)
            mStorage->deleteNode(node.uuid);

        QVERIFY(mStorage->getAllNodesUuids().empty());
    }

private:
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

    static constexpr auto sDbFileName = "testDatabase.termGraph";
    // Group strings
    const QUuid   mGroupUuid1    = QUuid::createUuid();
    const QString mGroupName1    = QStringLiteral("TestGroup1");
    const QString mGroupName2    = QStringLiteral("TestGroup2");
    const QString mGroupComment1 = QStringLiteral("commentText1");
    const QString mGroupComment2 = QStringLiteral("commentText2");

    const QString mSpecSymbols = QStringLiteral("!@#$%^&*()-+=*:/'\"\\/");

    std::unique_ptr<DataStorageInterface> mStorage;
};

QTEST_APPLESS_MAIN(DBWorksTest)

#include "dbworkstest.moc"
