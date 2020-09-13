#pragma once
#include "constants.h"
#include "min_mat.h"
#include "point.h"
#include <unordered_map>

class Board
{
private:
    Mat<uint_fast8_t> raw_table;
    std::unordered_map<Point<2, uint_fast8_t>, size_t> editable_indices;

public:
    Board() = delete;
    Board(Mat<uint_fast8_t> &&raw_table)
    {
        if (raw_table.SizeY() != Constants.BOARD_HEIGHT)
            throw std::invalid_argument("Raw Table height is incorrect!\n\n" + raw_table.ToString());

        if (raw_table.SizeX() != Constants.BOARD_WIDTH)
            throw std::invalid_argument("Raw Table width is incorrect!\n\n" + raw_table.ToString());

        this->raw_table = std::move(raw_table);

        Point<2, uint_fast8_t> temp;

        for (uint_fast8_t i = 0; i < Constants.BOARD_HEIGHT; ++i)
            for (uint_fast8_t j = 0; j < Constants.BOARD_WIDTH; ++j)
            {
                if (!raw_table.At(j, i))
                {
                    temp = std::move(Point<2, uint_fast8_t>({j, i}));
                    editable_indices.insert(std::make_pair(temp, std::hash<Point<2, uint_fast8_t>>{}(temp)));
                }
            }
    }
};
