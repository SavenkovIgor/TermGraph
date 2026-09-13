// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

export module Helpers.IntMatrix;

using namespace std;

export class IntMatrix
{
    vector<vector<int>> matrix;

    size_t height = 1;
    size_t width  = 1;

public:
    IntMatrix() { matrix = createMatrix(width, height); }

    void checkSize(size_t width, size_t height)
    {
        if (this->width < width || this->height < height) {
            clearMatrix(matrix);
            matrix       = createMatrix(width, height);
            this->width  = width;
            this->height = height;
        }
    }

    static vector<vector<int>> createMatrix(size_t width, size_t height)
    {
        vector<vector<int>> mtr(height);

        for (auto& row : mtr) {
            row.resize(width);
        }

        return mtr;
    }

    static void clearMatrix(vector<vector<int>>& matrix)
    {
        for (auto& row : matrix) {
            row.clear();
        }

        matrix.clear();
    }

    vector<vector<int>>& matrixView() { return matrix; }
};
