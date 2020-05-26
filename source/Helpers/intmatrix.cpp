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

#include "source/Helpers/intmatrix.h"

IntMatrix::IntMatrix()
{
    matrix = createMatrix(width, height);
}

void IntMatrix::checkSize(sizeT width, sizeT height)
{
    if (this->width < width || this->height < height) {
        clearMatrix(matrix);
        matrix       = createMatrix(width, height);
        this->width  = width;
        this->height = height;
    }
}

std::vector<std::vector<int>> IntMatrix::createMatrix(IntMatrix::sizeT width, IntMatrix::sizeT height)
{
    std::vector<std::vector<int>> mtr(height);

    for (auto& row : mtr)
        row.resize(width);

    return mtr;
}

void IntMatrix::clearMatrix(std::vector<std::vector<int>>& matrix)
{
    for (auto& row : matrix)
        row.clear();

    matrix.clear();
}
