#pragma once
struct
{
    int BOARD_HEIGHT = 9;
    int BOARD_WIDTH = 9;
    int CLIQUE_HEIGHT = 3;
    int CLIQUE_WIDTH = 3;
    int NUM_CLIQUES = (BOARD_HEIGHT * BOARD_WIDTH); // (CLIQUE_HEIGHT * CLIQUE_WIDTH)

    struct
    {
        std::string DOUBLE_VERTICAL = "║";
        std::string DOUBLE_HORIZONTAL = "═";
        std::string DOUBLE_NE = "╗";
        std::string DOUBLE_NW = "╔";
        std::string DOUBLE_SW = "╚";
        std::string DOUBLE_SE = "╝";
        std::string DOUBLE_SINGLE_S = "╤";
        std::string DOUBLE_SINGLE_E = "╟";
        std::string DOUBLE_SINGLE_N = "╧";
        std::string DOUBLE_SINGLE_W = "╢";
        std::string SINGLE_VERTICAL = "│";
        std::string SINGLE_HORIZONTAL = "─";
        std::string SINGLE_CROSS = "┼";
    } Border;

    struct
    {
        float SLEEP_DURATION = 0.001;
        int BOARD_HEIGHT = 13;
        int CLIQUE_SIZE = 7;
    } Printing;
} Constants;
