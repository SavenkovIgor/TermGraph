// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

export module Helpers.IntMatrix;

using namespace std;

export struct IntMatrix
{
    using sizeT = vector<int>::size_type;

    IntMatrix();

    vector<vector<int>> matrix;

    void checkSize(sizeT width, sizeT height);

    static vector<vector<int>> createMatrix(sizeT width, sizeT height);

    static void clearMatrix(vector<vector<int>>& matrix);

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

vector<vector<int>> IntMatrix::createMatrix(IntMatrix::sizeT width, IntMatrix::sizeT height)
{
    vector<vector<int>> mtr(height);

    for (auto& row : mtr) {
        row.resize(width);
    }

    return mtr;
}

void IntMatrix::clearMatrix(vector<vector<int>>& matrix)
{
    for (auto& row : matrix) {
        row.clear();
    }

    matrix.clear();
}
