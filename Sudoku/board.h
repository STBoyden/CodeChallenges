#pragma once
#include <vector>
#include "min_mat.h"
#include "point.h"

class Canidates
{
private:
    std::vector<bool> possible{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //vector to save mem
    int size = 0, solved = 0;

public:
    //O(n), collapses to O(1) when solved
    int Front() const
    {
        for (int i = 1 + (9 * solved); i < 10; ++i) // 1+(9*solved) skips if solved already
        {
            if (possible[i]) //maybe will add offset = i; in future
                return i;    //1-9
        }
        return solved;
    }

    //all of these functions are O(1)
    void Erase(int i)
    {
        solved = Front() * (size == 1); //branchless
        size -= possible[i];            //branchless
        possible[i] = 0;
    }
    bool Solved() const
    {
        return size == 1;
    }
    void TurnOn(int i)
    {
        size += !possible[i]; //branchless
        possible[i] = 1;
    }
    int Find(int i) const
    {
        return possible[i];
    }
    bool Contains(int i) const
    {
        return possible[i];
    }
    int Size() const
    {
        return size;
    }

    friend std::ostream &operator<<(std::ostream &os, const Canidates &c)
    {
        return os << (c.Size() != 1 ? 0 : c.Front());
    }
};

struct SudokuBoard
{
    Mat<Canidates> board;
    std::vector<Point<2, int>> unsolved;

    SudokuBoard(const std::vector<int> &boardStart, int dim = 3)
    {
        if (boardStart.size() != ((dim * dim) * (3 * 3)))
            throw std::invalid_argument("board data must be the same as the inputed dimensions");

        board = std::move(Mat<Canidates>(dim * 3, dim * 3));

        //setup board
        Canidates *p = board.begin();
        const std::vector<int> all{1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int i : boardStart)
        {
            if (i) //not zero
            {
                p->TurnOn(i);
            }
            else
            {
                for (int j = 1; j < 10; ++j)
                {
                    p->TurnOn(j);
                }
            }
            ++p;
        }

        //adding all unsolved
        for (int i = 0; i < board.SizeY(); ++i)
        {
            for (int j = 0; j < board.SizeX(); ++j)
            {
                if (board.At(j, i).Size() > 1) //is unsolved
                {
                    unsolved.push_back(Point<2, int>{j, i});
                }
            }
        }
    }
};
