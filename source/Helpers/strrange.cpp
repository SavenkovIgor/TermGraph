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

#include "source/Helpers/strrange.h"

StrRange StrRange::fromLeftRight(int leftIndex, int rightIndex)
{
    auto left  = toSizeT(leftIndex);
    auto right = toSizeT(rightIndex);

    assert(rightIndex >= leftIndex);

    auto size = getSize(left, right);

    return StrRange(left, size);
}

StrRange StrRange::fromPosSize(int leftIndex, int size)
{
    auto left  = toSizeT(leftIndex);
    auto sizeT = toSizeT(size);

    return StrRange(left, sizeT);
}

int StrRange::left() const { return static_cast<int>(mFrom); }

int StrRange::right() const { return static_cast<int>(mFrom + mSize - 1); }

int StrRange::size() const { return static_cast<int>(mSize); }

bool StrRange::isValidFor(QStringView str) const
{
    auto sSize = str.size();
    return left() < sSize && right() < sSize;
}

StrRange::StrRange(size_t from, size_t size)
    : mFrom(from)
    , mSize(size)
{}

size_t StrRange::toSizeT(int from)
{
    assert(from >= 0);
    return static_cast<size_t>(from);
}

size_t StrRange::getSize(size_t left, size_t right) { return right - left + 1; }
