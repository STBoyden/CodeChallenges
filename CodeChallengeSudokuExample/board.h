#pragma once
#include "Constants.h"
#include "min_mat.h"
#include "point.h"
#include "helpers.h"
#include <vector>
#include <set>

struct Board
{
    Mat<int> raw_table;
    std::vector<Point<2, int>> editable_indices;

    Board() = delete;
    Board(const Mat<int> &raw_table)
    {
        if (raw_table.SizeY() != Constants::BOARD_HEIGHT)
            throw std::invalid_argument("Raw Table height is incorrect!\n\n" + raw_table.ToString());

        if (raw_table.SizeX() != Constants::BOARD_WIDTH)
            throw std::invalid_argument("Raw Table width is incorrect!\n\n" + raw_table.ToString());

        this->raw_table = std::move(raw_table);

        Point<2, int> temp;

        for (int i = 0; i < Constants::BOARD_HEIGHT; ++i)
        {
            for (int j = 0; j < Constants::BOARD_WIDTH; ++j)
            {
                if (!raw_table.At(j, i))
                    editable_indices.push_back(Point<2, int>({j, i}));
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &b)
    {
        os << Constants::Border::DOUBLE_NW;
        for (int j = 0; j < 2; ++j)
        {
            for (int i = 0; i < Constants::Printing::CLIQUE_SIZE; ++i)
            {
                os << Constants::Border::DOUBLE_HORIZONTAL;
            }
            os << Constants::Border::DOUBLE_SINGLE_S;
        }
        for (int i = 0; i < Constants::Printing::CLIQUE_SIZE; ++i)
        {
            os << Constants::Border::DOUBLE_HORIZONTAL;
        }
        os << Constants::Border::DOUBLE_NE << '\n';

        int real_y, real_x;
        for (int y1 = 0; y1 < Constants::BOARD_HEIGHT / Constants::CLIQUE_HEIGHT; ++y1)
        {
            for (int y2 = 0; y2 < Constants::CLIQUE_HEIGHT; ++y2)
            {
                real_y = (y1 * Constants::CLIQUE_HEIGHT) + y2;
                os << Constants::Border::DOUBLE_VERTICAL << ' ';
                for (int x1 = 0; x1 < Constants::BOARD_WIDTH / Constants::CLIQUE_WIDTH; ++x1)
                {
                    for (int x2 = 0; x2 < Constants::CLIQUE_WIDTH; ++x2)
                    {
                        real_x = (x1 * Constants::CLIQUE_WIDTH) + x2;
                        os << (int)b.raw_table.At(real_x, real_y) << ' ';
                    }
                    os << Constants::Border::SINGLE_VERTICAL << ' ';
                }
                os << "\n";
            }
            if (y1 != Constants::BOARD_HEIGHT / Constants::CLIQUE_HEIGHT - 1)
            {
                os << Constants::Border::DOUBLE_SINGLE_E;
                for (int i = 0; i < 2; ++i)
                {
                    for (int j = 0; j < Constants::Printing::CLIQUE_SIZE; ++j)
                    {
                        os << Constants::Border::SINGLE_HORIZONTAL;
                    }
                    os << Constants::Border::SINGLE_CROSS;
                }
                for (int j = 0; j < Constants::Printing::CLIQUE_SIZE; ++j)
                {
                    os << Constants::Border::SINGLE_HORIZONTAL;
                }
                os << Constants::Border::DOUBLE_SINGLE_W << '\n';
            }
        }

        os << Constants::Border::DOUBLE_SW;
        for (int j = 0; j < 2; ++j)
        {
            for (int i = 0; i < Constants::Printing::CLIQUE_SIZE; ++i)
            {
                os << Constants::Border::DOUBLE_HORIZONTAL;
            }
            os << Constants::Border::DOUBLE_SINGLE_N;
        }
        for (int i = 0; i < Constants::Printing::CLIQUE_SIZE; ++i)
        {
            os << Constants::Border::DOUBLE_HORIZONTAL;
        }
        os << Constants::Border::DOUBLE_SE;

        return os;
    }

    size_t editable_indices_size() const
    {
        return editable_indices.size();
    }

    bool is_square_valid(int editable_indices_index)
    {
        Point<2, int> p = editable_indices[editable_indices_index];

        if (!is_row_valid(p[0]))
            return false;

        if (!is_colum_valid(p[1]))
            return false;

        if (!is_clique_valid(p))
            return false;

        return true;
    }

    //changed from dict in py to vec in c++... I couldnt help myself, O(1) vs O(log n) and all
    bool is_colum_valid(int x) const
    {
        std::vector<bool> occurrences_vec{0, 0, 0, 0, 0, 0, 0, 0, 0}; //in py prog this is a dict, which is O(log n), but as vec its just O(1)
        int square_value = 0;
        for (int y = 0; y < Constants::BOARD_WIDTH; ++y)
        {
            square_value = raw_table.At(x, y);

            if (!square_value)
                continue;

            if (occurrences_vec[square_value - 1]) //does it contain
            {
                return false;
            }
            else
            {
                occurrences_vec[square_value - 1] = true; //now it contains it
            }
        }
        return true;
    }

    bool is_row_valid(int y) const
    {
        std::vector<bool> occurrences_vec{0, 0, 0, 0, 0, 0, 0, 0, 0}; //in py prog this is a dict, which is O(log n), but as vec its just O(1)
        int square_value = 0;
        for (int x = 0; x < Constants::BOARD_WIDTH; ++x)
        {
            square_value = raw_table.At(x, y);

            if (!square_value)
                continue;

            if (occurrences_vec[square_value - 1]) //does it contain
            {
                return false;
            }
            else
            {
                occurrences_vec[square_value - 1] = true; //now it contains it
            }
        }
        return true;
    }

    bool is_clique_valid(const Point<2, int> &cliqueStart) const
    {
        std::vector<bool> occurrences_vec{0, 0, 0, 0, 0, 0, 0, 0, 0}; //in py prog this is a dict, which is O(log n), but as vec its just O(1)
        int square_value;
        Point<2, int> local = BoardHelper::GetLocal(cliqueStart);

        for (int y = 0; y < Constants::CLIQUE_HEIGHT; ++y)
        {
            for (int x = 0; x < Constants::CLIQUE_WIDTH; ++x)
            {
                square_value = raw_table.At(local[0] + x, local[1] + y);
                if (!square_value)
                    continue;

                if (occurrences_vec[square_value - 1])
                {
                    return false;
                }
                else
                {
                    occurrences_vec[square_value - 1] = true;
                }
            }
        }

        return true;
    }
    bool is_board_valid() const
    {
        for (int y = 0; y < Constants::BOARD_HEIGHT; ++y)
        {
            if (!is_colum_valid(y))
                return false;
        }
        for (int x = 0; x < Constants::BOARD_WIDTH; ++x)
        {
            if (!is_row_valid(x))
                return false;
        }
        for (int i = 0; i < Constants::BOARD_HEIGHT; i += 3)
        {
            for (int j = 0; j < Constants::BOARD_WIDTH; j += 3)
            {
                if (!is_clique_valid(Point<2, int>{j, i}))
                    return false;
            }
        }

        return true;
    }
};
