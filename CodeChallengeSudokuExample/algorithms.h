#pragma once
#include <chrono>
#include "board.h"

struct BaseAlgorithm
{
    virtual double solve(Board &b) = 0;
};

struct BasicRecursiveBacktrackAlgorithm : public BaseAlgorithm
{
    bool basic_recursive_backtrack_solve(Board &b, int editable_index = 0)
    {
        if (editable_index >= b.editable_indices_size())
            return b.is_board_valid();

        Point<2, int> p = b.editable_indices[editable_index];
        for (int attempt = 1; attempt <= Constants::BOARD_HEIGHT; ++attempt)
        {
            b.raw_table.At(p[0], p[1]) = attempt;

            if (!b.is_square_valid(editable_index))
                continue;

            if (BasicRecursiveBacktrackAlgorithm::basic_recursive_backtrack_solve(b, editable_index + 1))
                return true;
        }

        b.raw_table.At(p[0], p[1]) = 0;

        return false;
    }

    virtual double solve(Board &b) override
    {
        std::chrono::high_resolution_clock::time_point time_start = std::chrono::high_resolution_clock::now();
        BasicRecursiveBacktrackAlgorithm::basic_recursive_backtrack_solve(b);
        std::chrono::high_resolution_clock::time_point time_end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<double>>(time_end - time_start).count();
    }
}; // namespace BasicRecursiveBacktrackAlgorithm
