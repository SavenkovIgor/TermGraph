// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <QDebug>

#include <StaticDataStorage/StaticDataStorage.h>

TEST(StaticStorageTest, GroupsLoadable)
{
    auto storage = std::make_unique<StaticDataStorage>();

    auto group = storage->groups();

    ASSERT_TRUE(group.result().has_value());
}

TEST(StaticStorageTest, GroupsSerializeSymmetry)
{
    auto storage = std::make_unique<StaticDataStorage>();

    for (const auto& fileInfo : StaticDataStorage::files()) {
        auto fileData = StaticDataStorage::qrcFileData(fileInfo.absoluteFilePath());

        ASSERT_TRUE(fileData.isValidUtf8());

        auto group = StaticGroupData::from(fileData);

        ASSERT_TRUE(group.has_value());
        if (!group.has_value()) {
            qWarning() << "Can't parse file:" << fileInfo.absoluteFilePath();
        }

        auto castedJson = static_cast<QByteArray>(group.value());

        EXPECT_TRUE(castedJson.isValidUtf8());

        EXPECT_EQ(castedJson.size(), fileData.size());
        EXPECT_EQ(castedJson, fileData);

        if (castedJson != fileData) {
            qInfo() << "Raw file data:" << fileData;
            qInfo() << "Expected data:" << castedJson;
        }
    }
}

TEST(StaticStorageTest, DefinitionSimplified)
{
    auto storage = std::make_unique<StaticDataStorage>();

    for (const auto& fileInfo : StaticDataStorage::files()) {
        auto fileData = StaticDataStorage::qrcFileData(fileInfo.absoluteFilePath());

        ASSERT_TRUE(fileData.isValidUtf8());

        auto group = StaticGroupData::from(fileData);

        ASSERT_TRUE(group.has_value());
        if (!group.has_value()) {
            qWarning() << "Can't parse file:" << fileInfo.absoluteFilePath();
        }

        auto firstChar = [](const QString& str) {
            return str.isEmpty() ? QChar() : str[0];
        };

        for (const auto& term : group.value().terms) {
            if (term.definition.isEmpty()) {
                continue;
            }

            if (term.term != term.term.simplified()) {
                qWarning() << "Term is not simplified:" << term.term;
            }

            auto definition = term.definition;
            if (definition != definition.simplified()) {
                qWarning() << "Definition is not simplified:" << definition;
            }

            auto firstCharOfDefinition = firstChar(definition);
            if (firstCharOfDefinition.isUpper()) {
                qWarning() << "Definition starts with uppercase:" << term.definition;
            }

            if (definition.endsWith('.')) {
                qWarning() << "Definition ends with dot:" << term.definition;
            }
        }
    }
}
