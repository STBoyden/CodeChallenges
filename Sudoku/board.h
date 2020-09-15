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
        size += !possible[i]; //possible[i] is 0, toggling adds to size
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

    bool size_toggle = false;
    friend std::ostream &operator<<(std::ostream &os, const Canidates &c)
    {
        return os << ((!(c.Size() != 1) * c.Front()) * !c.size_toggle) + (c.Size() * c.size_toggle);
    }
};

struct SudokuBoard
{
    Mat<Canidates> board, local;
    std::vector<Point<2, int>> unsolved;

    SudokuBoard(const std::vector<int> &boardStart, int dim = 3)
    {
        if (boardStart.size() != ((dim * dim) * (3 * 3)))
            throw std::invalid_argument("board data must be the same as the inputed dimensions");

        board = std::move(Mat<Canidates>(dim * 3, dim * 3));
        local = std::move(Mat<Canidates>(dim, dim));

        const std::vector<int> all{1, 2, 3, 4, 5, 6, 7, 8, 9};

        for (Canidates *pL = local.begin(); pL != local.end(); ++pL)
        {
            for (int i = 1; i < 10; ++i)
            {
                pL->TurnOn(i);
                pL->size_toggle = true;
            }
        }

        //setup board
        Canidates *pB = board.begin();
        for (int i : boardStart)
        {
            if (i) //not zero
            {
                pB->TurnOn(i);
            }
            else
            {
                for (int j = 1; j < 10; ++j)
                {
                    pB->TurnOn(j);
                }
            }
            ++pB;
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
