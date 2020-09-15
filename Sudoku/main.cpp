#include <iostream>
#include <sstream>
#include <chrono>
#include "board.h"
#include "tpool.h"

const unsigned int cores = std::thread::hardware_concurrency();
const std::vector<int> all{1, 2, 3, 4, 5, 6, 7, 8, 9};

inline void UpdateUnsolved(SudokuBoard &s)
{
    std::vector<Point<2, int>> res;
    for (Point<2, int> p : s.unsolved)
    {
        if (s.board.At(p[0], p[1]).Size() != 1)
        {
            res.push_back(p);
        }
    }
    s.unsolved = std::move(res);
}

// //branchless
// inline Point<2, int> GetLocal(const Point<2, int> &p)
// {
//     Point<2, int> res{p[0], p[1]};

//     res[0] -= 2 * ((res[0] + 1) % 3 == 0);
//     res[0] -= (res[0] + 2) % 3 == 0;

//     res[1] -= 2 * ((res[1] + 1) % 3 == 0);
//     res[1] -= (res[1] + 2) % 3 == 0;

//     return res;
// }

inline bool TrimCanidates(SudokuBoard &s)
{
    int res = false;
    Point<2, int> localPoint;

    for (const Point<2, int> &p : s.unsolved)
    {
        //checking vertical and horizontal
        for (int i = p[1] - 1; i >= 0; --i) //y1-1 -> 0
        {
            s.board.At(p[0], p[1]).Erase(s.board.At(p[0], i).Front() * s.board.At(p[0], i).Solved());
        }
        for (int i = p[1] + 1; i < s.board.SizeY(); ++i) // y1+1 -> y2
        {
            s.board.At(p[0], p[1]).Erase(s.board.At(p[0], i).Front() * s.board.At(p[0], i).Solved());
        }
        for (int i = p[0] - 1; i >= 0; --i) // x1-1 -> 0
        {
            s.board.At(p[0], p[1]).Erase(s.board.At(i, p[1]).Front() * s.board.At(i, p[1]).Solved());
        }
        for (int i = p[0] + 1; i < s.board.SizeX(); ++i) // x1+1 -> x2
        {
            s.board.At(p[0], p[1]).Erase(s.board.At(i, p[1]).Front() * s.board.At(i, p[1]).Solved());
        }

        res += s.board.At(p[0], p[1]).Solved(); //branchless
    }

    return res;
}

inline void TrimLocalCanidates(SudokuBoard &s)
{
    int res = 0;
    for (int i = 0; i < s.local.SizeY(); ++i)
    {
        for (int j = 0; j < s.local.SizeX(); ++j)
        {
            for (int y = 0; y < 3; ++y)
            {
                for (int x = 0; x < 3; ++x)
                {
                    s.local.At(j, i).Erase(s.board.At((j * 3) + x, (i * 3) + y).Front() * s.board.At((j * 3) + x, (i * 3) + y).Solved()); //erasing solved bits
                    for (int k = 0; k < 10; ++k)
                    {
                        s.board.At((j * 3) + x, (i * 3) + y).Erase((!s.local.At(j, i).Contains(k) * k) * !s.board.At((j * 3) + x, (i * 3) + y).Solved());
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    using namespace std::chrono;

    // std::vector<int> cells;
    // std::string a = argv[3];
    // std::stringstream ss(a);
    // std::string line;
    // while (std::getline(ss, line, ','))
    // {
    //     cells.push_back(std::stoi(line));
    // }

    //std::stoi(argv[1]), std::stoi(argv[2])

    //   {7, 2, 0,     0, 9, 6,    0, 0, 3,
    //    0, 0, 0,     2, 0, 5,    0, 0, 0,
    //    0, 8, 0,     0, 0, 4,    0, 2, 0,

    //    0, 0, 0,     0, 0, 0,    0, 6, 0,
    //    1, 0, 6,     5, 0, 3,    8, 0, 7,
    //    0, 4, 0,     0, 0, 0,    0, 0, 0,

    //    0, 3, 0,     8, 0, 0,    0, 9, 0,
    //    0, 0, 0,     7, 0, 2,    0, 0, 0,
    //    2, 0, 0,     4, 3, 0,    0, 1, 8});

    SudokuBoard gameBoard({7, 2, 0, 0, 9, 6, 0, 0, 3,
                           0, 0, 0, 2, 0, 5, 0, 0, 0,
                           0, 8, 0, 0, 0, 4, 0, 2, 0,
                           0, 0, 0, 0, 0, 0, 0, 6, 0,
                           1, 0, 6, 5, 0, 3, 8, 0, 7,
                           0, 4, 0, 0, 0, 0, 0, 0, 0,
                           0, 3, 0, 8, 0, 0, 0, 9, 0,
                           0, 0, 0, 7, 0, 2, 0, 0, 0,
                           2, 0, 0, 4, 3, 0, 0, 1, 8});

    //start timer
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    {
        //zoomMachine.Start();
        while (gameBoard.unsolved.size()) //while not solved
        {
        start:
            UpdateUnsolved(gameBoard);

            //strats
            TrimLocalCanidates(gameBoard);
            if (TrimCanidates(gameBoard))
                goto start;

            break;

            //std::cout << "Cells Left: " << gameBoard.unsolved.size() << '\n';
        }
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    //end timer
    std::cout << gameBoard.unsolved.size() << '\n';
    std::cout << "wowee solved!\n";
    std::cout << time_span.count() << '\n';
    std::cout << gameBoard.board << '\n'
              << gameBoard.local << '\n';

    return 0;
}
