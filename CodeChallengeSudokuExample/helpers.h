#pragma once
#include <vector>
#include <unordered_map>
#include "MiscHeaderFiles-master/point.h"
#include "constants.h"

namespace BoardHelper
{
    static std::unordered_map<size_t, std::vector<Point<2, int>>> cliques;

    static int get_clique_index_for_coordinates(int x, int y) { return y + (x / Constants::CLIQUE_WIDTH); }

    static void gen_cords()
    {
        size_t clique_index = 0;
        for (int y = 0; y < Constants::BOARD_HEIGHT; ++y)
        {
            for (int x = 0; x < Constants::BOARD_WIDTH; ++x)
            {
                clique_index = BoardHelper::get_clique_index_for_coordinates(x, y);
                if (!BoardHelper::cliques.contains(clique_index))
                    BoardHelper::cliques.insert({clique_index, {}});

                BoardHelper::cliques.at(clique_index).emplace_back(Point<2, int>{x, y});
            }
        }
    }

}; // namespace BoardHelper
