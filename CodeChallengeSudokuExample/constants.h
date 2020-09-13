#pragma once
struct
{
    constexpr int BOARD_HEIGHT = 9;
    constexpr int BOARD_WIDTH = 9;
    constexpr int CLIQUE_HEIGHT = 3;
    constexpr int CLIQUE_WIDTH = 3;
    constexpr int NUM_CLIQUES = (BOARD_HEIGHT * BOARD_WIDTH); // (CLIQUE_HEIGHT * CLIQUE_WIDTH)

    struct
    {
        constexpr char DOUBLE_VERTICAL = "║";
        constexpr char DOUBLE_HORIZONTAL = "═";
        constexpr char DOUBLE_NE = "╗";
        constexpr char DOUBLE_NW = "╔";
        constexpr char DOUBLE_SW = "╚";
        constexpr char DOUBLE_SE = "╝";
        constexpr char DOUBLE_SINGLE_S = "╤";
        constexpr char DOUBLE_SINGLE_E = "╟";
        constexpr char DOUBLE_SINGLE_N = "╧";
        constexpr char DOUBLE_SINGLE_W = "╢";
        constexpr char SINGLE_VERTICAL = "│";
        constexpr char SINGLE_HORIZONTAL = "─";
        constexpr char SINGLE_CROSS = "┼";
    } Border;

    struct
    {
        constexpr float SLEEP_DURATION = 0.001;
        constexpr int BOARD_HEIGHT = 13;
        constexpr int CLIQUE_SIZE = 7;
    } Printing;
} Constants;
