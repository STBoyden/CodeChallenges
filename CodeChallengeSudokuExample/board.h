#pragma once
#include "constants.h"
#include "min_mat.h"
#include "point.h"
#include <vector>

class Board
{
private:
    Mat<uint_fast8_t> raw_table;
    std::vector<Point<2, uint_fast8_t>> editable_indices;

public:
    Board(const Mat<uint_fast8_t> &raw_table)
    {
        if (m.SizeY() != Constants.BOARD_HEIGHT)
            throw std::invalid_argument("Raw Table height is incorrect!\n\n" + raw_table.ToString());

        if (m.SizeX() != Constants.BOARD_WIDTH)
            throw std::invalid_argument("Raw Table width is incorrect!\n\n" + raw_table.ToString());

        this.raw_table = std::move(raw_table);

        for (int i = 0; i < Constants.BOARD_HEIGHT; ++i)
            for (int j = 0; j < Constants.BOARD_WIDTH; ++j)
            {
                if (!raw_table.At(j, i))
                    editable_indices.push_back(Point<2, uint_fast8_t>(j, i));
            }
    }
}