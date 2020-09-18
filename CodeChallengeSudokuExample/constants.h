#pragma once
struct
{
    const int BOARD_HEIGHT = 9;
    const int BOARD_WIDTH = 9;
    const int CLIQUE_HEIGHT = 3;
    const int CLIQUE_WIDTH = 3;
    const int NUM_CLIQUES = (BOARD_HEIGHT * BOARD_WIDTH); // (CLIQUE_HEIGHT * CLIQUE_WIDTH)

    struct
    {
        const char DOUBLE_VERTICAL = 186;
        const char DOUBLE_HORIZONTAL = 205;
        const char DOUBLE_NE = 187;
        const char DOUBLE_NW = 201;
        const char DOUBLE_SW = 200;
        const char DOUBLE_SE = 188;
        const char DOUBLE_SINGLE_S = 203;
        const char DOUBLE_SINGLE_E = 204;
        const char DOUBLE_SINGLE_N = 202;
        const char DOUBLE_SINGLE_W = 185;
        const char SINGLE_VERTICAL = 186;
        const char SINGLE_HORIZONTAL = 205;
        const char SINGLE_CROSS = 206;
    } Border;

    struct
    {
        const float SLEEP_DURATION = 0.001;
        const int BOARD_HEIGHT = 13;
        const int CLIQUE_SIZE = 7;
    } Printing;
} Constants;
