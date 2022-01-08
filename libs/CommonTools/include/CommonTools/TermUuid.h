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

#pragma once

#include <CommonTools/SafeUuid.h>

class TermUuid final : public SafeUuid
{
public:
    using List = std::vector<TermUuid>;

    inline static Opt<TermUuid> create(const QString& text)
    {
        if (auto safe = SafeUuid::create(text))
            return TermUuid(text);

        return std::nullopt;
    }

    inline static Opt<TermUuid> create(const QUuid& uuid)
    {
        if (auto safe = SafeUuid::create(uuid))
            return TermUuid(uuid.toString());

        return std::nullopt;
    }

    inline static TermUuid generate() { return {QUuid::createUuid().toString()}; }

private:
    inline TermUuid(const QString& text)
        : SafeUuid(text)
    {}
};
