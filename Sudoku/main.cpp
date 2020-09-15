#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
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

inline void TrimCanidates(SudokuBoard &s)
{
    Point<2, int> localPoint;

    //triming local
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

    //trimming individual
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
    }
    UpdateUnsolved(s);
}

int main(int argc, char *argv[])
{
    using namespace std::chrono;

    // std::vector<int> cells;
    // std::string a = argv[1];
    // std::stringstream ss(a);
    // std::string line;
    // while (std::getline(ss, line, ','))
    // {
    //     cells.push_back(std::stoi(line));
    // }

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

    int x = gameBoard.unsolved.size();

    //start timer
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    {
        while (x) //while not solved
        {
        start:
            std::cout << gameBoard.board << '\n'
                      << gameBoard.local << '\n';

            UpdateUnsolved(gameBoard);

            //strats
            TrimCanidates(gameBoard);
            if (x != gameBoard.unsolved.size())
            {
                x = gameBoard.unsolved.size();
                goto start;
            }
            break;
        }
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    //end timer
    std::cout << gameBoard.unsolved.size() << '\n';
    std::cout << "wowee solved!\n";
    std::cout << std::setprecision(7) << std::fixed << time_span.count() << '\n';
    std::cout << gameBoard.board << '\n'
              << gameBoard.local << '\n';

    //function call count
    // erase - 6412
    // front - 3622
    // turnon - 586

    return 0;
}
