#ifndef _AI_WORK_HPP_
#define _AI_WORK_HPP_

#include <ctime>
#include <random>

#include "chess.hpp"

extern std::mt19937 randt;

class AI {
public:
    int value[105][105];
    int ptype;
    chessBroad *cbroad;

    AI();
    AI(int p, chessBroad *cb);
    ~AI();

    void AIWork(int x);
};

#endif
