// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

export module Helpers.IntMatrix;

export struct IntMatrix
{
    using sizeT = std::vector<int>::size_type;

    IntMatrix();

    std::vector<std::vector<int>> matrix;

    void checkSize(sizeT width, sizeT height);

    static std::vector<std::vector<int>> createMatrix(sizeT width, sizeT height);

    static void clearMatrix(std::vector<std::vector<int>>& matrix);

private:
    sizeT height = 1;
    sizeT width  = 1;
};

IntMatrix::IntMatrix() { matrix = createMatrix(width, height); }

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

    for (auto& row : mtr) {
        row.resize(width);
    }

    return mtr;
}

void IntMatrix::clearMatrix(std::vector<std::vector<int>>& matrix)
{
    for (auto& row : matrix) {
        row.clear();
    }

    matrix.clear();
}
