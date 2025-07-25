// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <memory>

#include <QHostAddress>

#include "source/TermDataInterface/DataStorageInterface.h"

#include "source/CommonTools/Platform.h"
#include "source/staticDataStorage/StaticDataStorage.h"

#ifndef Q_OS_WASM
#include "source/TermDataConnection/DataStorageConnection.h"
#include "source/helpers/AppSettings.h"

#include "source/TermDataStorage/LocalDataStorage.h"
#endif

export module DataSourceFactory;

export struct DataSourceFactory
{
    static std::unique_ptr<DataStorageInterface> defaultSource()
    {
        if constexpr (Platform::isWasm()) {
            if constexpr (Platform::isStaticDataStorage()) {
                return std::make_unique<StaticDataStorage>();
            } else {
                return server();
            }
        }

        if constexpr (Platform::isDesktop() || Platform::isMobile()) {
            return localDb();
        }

        Q_UNREACHABLE();
    }

    static std::unique_ptr<DataStorageInterface> localDb()
    {
#ifdef Q_OS_WASM
        return nullptr;
#else
        using namespace AppSettings;
        return std::make_unique<LocalDatabaseStorage>(Paths::defaultDatabaseFilePath(), Paths::backupFolder());
#endif
    }

    static std::unique_ptr<DataStorageInterface> server()
    {
//    auto address = QUrl("http://127.0.0.1");
//    address.setPort(NetworkTools::defaultPort);
#ifdef Q_OS_WASM
        return nullptr;
#else
        auto address = QUrl("https://termgraph.app");
        return std::make_unique<DataStorageConnection>(address);
#endif
    }
};
