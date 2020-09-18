#pragma once
#include <vector>
#include <map>
#include "point.h"
#include "constants.h"

//this entire thing makes me wanna die but I did it this way as to best mirror the py equivelant
namespace BoardHelper
{
    inline Point<2, int> GetLocal(const Point<2, int> &p)
    {
        Point<2, int> res{p[0], p[1]};

        res[0] -= 2 * ((res[0] + 1) % 3 == 0);
        res[0] -= (res[0] + 2) % 3 == 0;

        res[1] -= 2 * ((res[1] + 1) % 3 == 0);
        res[1] -= (res[1] + 2) % 3 == 0;

        return res;
    }
}; // namespace BoardHelper