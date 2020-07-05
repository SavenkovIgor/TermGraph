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

#include <cassert>
#include <stdlib.h>
#include <vector>

#include <QStringView>

class StrRange
{
public:
    using List = std::vector<StrRange>;

    // Build methods
    static StrRange fromLeftRight(int leftIndex, int rightIndex);
    static StrRange fromPosSize(int leftIndex, int size);

    int left() const;
    int right() const;
    int size() const;

    bool isValidFor(QStringView str) const;

private:
    StrRange(size_t from, size_t size);

    static size_t toSizeT(int from);
    static size_t getSize(size_t left, size_t right);

private: // Members
    const size_t mFrom;
    const size_t mSize;
};
