// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

struct IntMatrix
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
