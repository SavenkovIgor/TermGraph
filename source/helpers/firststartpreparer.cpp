// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/firststartpreparer.h"

#include "source/helpers/AppSettings.h"

FirstStartPreparer::FirstStartPreparer() { AppSettings::Paths::createDefaultFoldersIfNeed(); }
