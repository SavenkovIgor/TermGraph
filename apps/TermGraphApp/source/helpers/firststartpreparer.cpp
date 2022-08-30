// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/firststartpreparer.h"

#include "source/helpers/appsettings.h"

FirstStartPreparer::FirstStartPreparer() { AppSettings::Paths::createDefaultFoldersIfNeed(); }
