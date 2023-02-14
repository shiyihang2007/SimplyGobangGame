#include "chess.hpp"

chessBroad::chessBroad()
{
    height = 20;
    width = 20;
    BroadPadding = 20;
    nowInRound = 1;
    playerCnt = 2;
    winner = 0;
    memset(broad, 0, sizeof(broad));
    colorSet[1] = RGB(0, 127, 255);
    colorSet[2] = RGB(255, 127, 0);
}
chessBroad::chessBroad(int x, int y)
{
    height = 20;
    width = 20;
    BroadPadding = 20;
    nowInRound = 1;
    playerCnt = 2;
    winner = 0;
    memset(broad, 0, sizeof(broad));
}

chessBroad::~chessBroad()
{
}

int chessBroad::getHeight() const
{
    return height;
}
int chessBroad::getWidth() const
{
    return width;
}
int chessBroad::getBroadPadding() const
{
    return BroadPadding;
}

COLORREF chessBroad::getColor(playerType p) const
{
    if (p < 0 || p >= 32768) {
        fprintf(flog, "<Info> Unavaible Color!\n");
        return RGB(255, 255, 255);
    }
    return colorSet[p];
}

void chessBroad::changeSize(int x, int y)
{
    height = x;
    width = y;
}
void chessBroad::changePlayer(int x)
{
    playerCnt = x;
    for (int i = 1, r = 0, g = 128, b = 255; i <= x; ++i, r = (r + 255 / x) % 255, g = (g + 255 / x) % 255, b = (b - 255 / x + 255) % 255) {
        colorSet[i] = RGB(r, g, b);
    }
}

int chessBroad::getNowInRound() const
{
    return nowInRound;
}
void chessBroad::nextRound()
{
    ++(nowInRound %= playerCnt);
}
void chessBroad::nextRound(int x)
{
    nowInRound = (x - 1) % playerCnt + 1;
}

playerType chessBroad::getChess(int x, int y) const
{
    if (x <= 0 || x > height || y <= 0 || y > height) {
        fprintf(flog, "<Info> GetChess Unavaible Position! %d %d\n", x, y);
        return 0;
    }
    return broad[x][y];
}
POINT chessBroad::translatePosition(const POINT &pos) const
{
    POINT tpos;
    tpos.x = (pos.y + BroadPadding / 2) / BroadPadding;
    tpos.y = (pos.x + BroadPadding / 2) / BroadPadding;
    return tpos;
}
void chessBroad::setChess(int x, int y, playerType p)
{
    if (x <= 0 || y <= 0 || x > height || y > width) {
        fprintf(flog, "<Info> Unavaible Position! %d %d\n", x, y);
        return;
    }
    if (p >= 32768) {
        fprintf(flog, "<Info> Unavaible Color!\n");
        return;
    }
    broad[x][y] = p;
    if (this->checkIsEnding(x, y)) {
        winner = p;
    }
}

int chessBroad::checkIsEnding() const
{
    return winner;
}
int chessBroad::checkIsEnding(int x, int y) const
{
    int d[][2] = {
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
        {-1, -1}};
    int t[4] = {};
    int maxx = 0, maxl = 0;
    int kx = 0, kl = 0;
    for (int i = 0; i < 4; ++i) {
        t[i] = 1;
        for (int tx = x + d[i * 2][0], ty = y + d[i * 2][1];
             broad[tx][ty] == broad[x][y];
             tx += d[i * 2][0], ty += d[i * 2][1]) {
            ++t[i];
        }
        for (int tx = x + d[i * 2 + 1][0], ty = y + d[i * 2 + 1][1];
             broad[tx][ty] == broad[x][y];
             tx += d[i * 2 + 1][0], ty += d[i * 2 + 1][1]) {
            ++t[i];
        }
        if (maxx < t[i]) {
            maxl = maxx, kl = kx;
            maxx = t[i], kx = i;
        }
    }
    if ((maxx == 5)) {
        return 1;
    }
    int cnt = 0;
    if (maxx >= 3) {
        int tmp1 = 0, tmp2 = 0;
        int isNone = 0;
        for (int tx = x + d[kx * 2][0], ty = y + d[kx * 2][1];
             broad[tx][ty] == broad[x][y];
             tx += d[kx * 2][0], ty += d[kx * 2][1]) {
            ++tmp1;
        }
        for (int tx = x + d[kx * 2 + 1][0], ty = y + d[kx * 2 + 1][1];
             broad[tx][ty] == broad[x][y];
             tx += d[kx * 2 + 1][0], ty += d[kx * 2 + 1][1]) {
            ++tmp2;
        }
        int edx1 = (tmp1 + 1) * d[kx * 2][0];
        int edy1 = (tmp1 + 1) * d[kx * 2][1];
        isNone += !broad[edx1][edy1];
        int edx2 = (tmp2 + 1) * d[kx * 2 + 1][0];
        int edy2 = (tmp2 + 1) * d[kx * 2 + 1][1];
        isNone += !broad[edx2][edy2];
        cnt += isNone + maxx >= 6;
    }
    if (maxl >= 3) {
        int tmp1 = 0, tmp2 = 0;
        int isNone = 0;
        for (int tx = x + d[kl * 2][0], ty = y + d[kl * 2][1];
             broad[tx][ty] == broad[x][y];
             tx += d[kl * 2][0], ty += d[kl * 2][1]) {
            ++tmp1;
        }
        for (int tx = x + d[kl * 2 + 1][0], ty = y + d[kl * 2 + 1][1];
             broad[tx][ty] == broad[x][y];
             tx += d[kl * 2 + 1][0], ty += d[kl * 2 + 1][1]) {
            ++tmp2;
        }
        int edx1 = (tmp1 + 1) * d[kl * 2][0];
        int edy1 = (tmp1 + 1) * d[kl * 2][1];
        isNone += !broad[edx1][edy1];
        int edx2 = (tmp2 + 1) * d[kl * 2 + 1][0];
        int edy2 = (tmp2 + 1) * d[kl * 2 + 1][1];
        isNone += !broad[edx2][edy2];
        cnt += isNone + maxl >= 6;
    }
    return cnt >= 2;
}

void chessBroad::redisplay() const
{
    for (int i = 1; i <= height; ++i) {
        ui::printLine(0, i * BroadPadding,
                      (width + 1) * BroadPadding, i * BroadPadding,
                      RGB(128, 128, 128));
    }
    for (int j = 1; j <= width; ++j) {
        ui::printLine(j * BroadPadding, 0,
                      j * BroadPadding, (height + 1) * BroadPadding,
                      RGB(128, 128, 128));
    }
    for (int i = 1; i <= height; ++i) {
        for (int j = 1; j <= width; ++j) {
            if (i == height / 2 && j == width / 2) {
                ui::printCircleLine(j * BroadPadding, i * BroadPadding,
                                    BroadPadding / 4, RGB(128, 128, 128));
            }
            if ((i == height / 4 || i == height - height / 4) &&
                (j == width / 4 || j == width - width / 4)) {
                ui::printCircleLine(j * BroadPadding, i * BroadPadding,
                                    BroadPadding / 6, RGB(128, 128, 128));
            }
            if (!broad[i][j]) {
                continue;
            }
            ui::printCircleLine(j * BroadPadding, i * BroadPadding,
                                BroadPadding / 2, colorSet[broad[i][j]]);
        }
    }
}
