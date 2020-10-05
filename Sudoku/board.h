#pragma once
#include <vector>
#include "min_mat.h"
#include "point.h"

class Canidates
{
private:
    std::vector<bool> possible{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //vector to save mem
    int size = 0, last = 0;

public:
    //O(n) funcs, becomes 26x faster after one run because it becomes O(1)
    //any Erase or TurnOn calls allows the possibility to change what front returns, so they reset last to 0
    int Front()
    {
        for (int i = (last * 10); i < 10; ++i)
        {
            if (possible[i])
            {
                last = i;
                return i;
            }
        }

        return last;
    }

    //O(1) funcs
    void Erase(int i)
    {
        size -= possible[i]; //branchless
        possible[i] = 0;
        last = 0;
    }
    void TurnOn(int i)
    {
        size += !possible[i]; //branchless
        possible[i] = 1;
        last = 0;
    }
    bool Solved() const
    {
        return size == 1;
    }
    bool Contains(int i) const
    {
        return possible[i];
    }
    int Size() const
    {
        return size;
    }

    void Reset()
    {
        possible = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        size = 0;
    }
    void Fill()
    {
        possible = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        size = 10;
    }

    bool size_toggle = false;
    friend std::ostream &operator<<(std::ostream &os, Canidates c)
    {
        return os << ((!(c.Size() != 1) * c.Front()) * !c.size_toggle) + (c.Size() * c.size_toggle); //branchless
    }
};

struct SudokuBoard
{
    Mat<Canidates> board, local;
    std::vector<Point<2, uint_fast8_t>> unsolved;
    SudokuBoard() {}
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
                    unsolved.push_back(Point<2, uint_fast8_t>{j, i});
                }
            }
        }
    }
};
