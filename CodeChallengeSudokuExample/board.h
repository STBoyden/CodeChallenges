#pragma once
#include "Constants.h"
#include "MiscHeaderFiles-master/min_mat.h"
#include "MiscHeaderFiles-master/point.h"
#include "helpers.h"
#include <unordered_set>
#include <vector>

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

        if (!is_clique_valid(BoardHelper::get_clique_index_for_coordinates(p[0], p[1])))
            return false;

        return true;
    }

    //changed from dict in py to vec in c++... I couldnt help myself, O(1) vs O(log n) and all
    bool is_colum_valid(int x) const
    {
        std::unordered_set<int> occurrence_dict;

        int square_value;
        for (int y = 0; y < Constants::BOARD_HEIGHT; ++y)
        {
            square_value = raw_table.At(x, y);

            if (!square_value)
                continue;

            if (occurrence_dict.contains(square_value))
            {
                return false;
            }
            else
            {
                occurrence_dict.emplace(square_value);
            }
        }

        return true;
    }

    bool is_row_valid(int y) const
    {
        std::unordered_set<int> occurrence_dict;

        int square_value;
        for (int x = 0; x < Constants::BOARD_HEIGHT; ++x)
        {
            square_value = raw_table.At(x, y);

            if (!square_value)
                continue;

            if (occurrence_dict.contains(square_value))
            {
                return false;
            }
            else
            {
                occurrence_dict.emplace(square_value);
            }
        }

        return true;
    }

    bool is_clique_valid(int clique_index) const
    {
        std::unordered_set<int> occurrence_dict;

        int square_value;
        for (Point<2, int> e : BoardHelper::cliques.at(clique_index))
        {
            square_value = raw_table.At(e[0], e[1]);

            if (!square_value)
                continue;

            if (occurrence_dict.contains(square_value))
            {
                return false;
            }
            else
            {
                occurrence_dict.emplace(square_value);
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

        for (int j = 0; j < Constants::NUM_CLIQUES; ++j)
        {
            if (!is_clique_valid(j))
                return false;
        }

        return true;
    }
};
