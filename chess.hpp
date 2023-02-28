#ifndef _CHESS_HPP_
#define _CHESS_HPP_

#include <cstdio>
#include <cstring>

#include "ui.hpp"

extern FILE *flog;

extern POINT lstPlaceChessPos;

typedef int playerType;

class chessBroad {
    playerType broad[205][205];
    COLORREF colorSet[32768];
    int height, width;
    int BroadPadding;
    int nowInRound;
    int playerCnt;
    int winner;

public:
    chessBroad();
    chessBroad(int x, int y, int padding = 20);

    ~chessBroad();

    int getHeight() const;
    int getWidth() const;
    int getBroadPadding() const;
    COLORREF getColor(playerType p) const;

    POINT translatePosition(const POINT &pos) const;

    void changeSize(int x, int y);
    void changePlayer(int x);

    int getNowInRound() const;
    void nextRound();
    void nextRound(int x);

    int getChess(int x, int y) const;
    void setChess(int x, int y, playerType p);

    int checkIsEnding() const;
    int checkIsEnding(int x, int y) const;

    void redisplay() const;
};

#endif
