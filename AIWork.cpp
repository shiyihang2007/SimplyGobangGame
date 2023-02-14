#include "AIWork.hpp"

AI::AI()
{
    ptype = 0;
    cbroad = NULL;
}
AI::AI(int p, chessBroad *cb)
{
    ptype = p;
    cbroad = cb;
}
AI::~AI()
{
    cbroad = NULL;
}

/* cbroad: 棋盘
 * 调用cbroad->getChess(int x, int y)获取一个位置上的棋子,没有则返回0
 * x, y: 表示要判断该点的权重
 * ptype: 表示将要落的棋子的归属
 */

// made by youdu666
int checkerByYoudu666(chessBroad *cbroad, int x, int y, int ptype)
{
    static const int v[11] = {
        0,         //
        5,         // 阻拦 一个点                1
        10,        // 背后远处挡 一个点          2
        50,        // 不阻拦 一个点             3
        60,        // 背后直接挡 两个点         4
        500,       // 背后远处挡 两个点        5
        1000,      // 不阻拦 两个点           6
        10000,     // 背后直接挡 三个点      7
        50000,     // 背后远处挡 三个点      8
        210000000, // 不挡 三个点       9
        210000000, // 四个点            10
    };
    static const int dx[8] = {-1, -1, 1, 1, 1, -1, 0, 0};
    static const int dy[8] = {-1, 1, 1, -1, 0, 0, 1, -1};
    int ta[7], af, n = cbroad->getHeight(), m = cbroad->getWidth(), df;
    if (cbroad->getChess(x, y) != 0)
        return 0;
    int res = 0;
    for (int u = 0; u < 8; u++) {
        af = 0;
        int df = cbroad->getChess(x + dx[u], y + dy[u]);
        if (df == 0)
            continue;
        for (int yp = -1; yp <= 5; yp++) {
            if (yp != -1)
                ta[yp] = 0;
            int xx = x + dx[u] * yp, yy = y + dy[u] * yp;
            if (xx > 0 && yy > 0 && xx <= n && yy <= m) {
                if (yp == -1)
                    af = cbroad->getChess(xx, yy);
                else
                    ta[yp] = cbroad->getChess(xx, yy);
            }
        }
        int ip = 2;
        for (ip = 2; ip <= 5; ip++)
            if (df != ta[ip]) {
                ip--;
                break;
            }
        ip += af == df;
        int vs = ip * 3;
        if (ip == 4) {
            res += v[10];
            continue;
        }
        if ((ta[ip + 1] != 0 && ta[ip + 1] != df) || af == -1)
            res += v[vs - 2];
        else if (ta[ip + 1] == 0 && ta[ip + 2] != df && ta[ip + 2] != 0)
            res += v[vs - 1];
        else
            res += v[vs];
    }
    return res;
}

/* Made By 369Pai */
int checkerBy369Pai(chessBroad *cbroad, int x, int y, int ptype)
{
    static const int N = 1e5 + 5;
    static const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1}, sc[2][N] = {{0, 1, 10, 100, 100}, {0, 2, 20, 200, 2000}};
    int n = cbroad->getHeight(), m = cbroad->getWidth();
    if (cbroad->getChess(x, y) != 0)
        return 0;
    int sum = randt() % 5;
    for (int i = 0; i < 8; i++) {
        int xx = x + dx[i], yy = y + dy[i], c = cbroad->getChess(xx, yy), cnt = 0;
        while (xx >= 1 && xx <= n && yy >= 1 && yy <= n && cbroad->getChess(xx, yy) == c)
            xx += dx[i], yy += dy[i], cnt++;
        if (cnt && c)
            sum += sc[c - 1][cnt];
    }
    if (x <= 5 || y <= 5 || x >= n - 5 || y >= n - 5)
        sum--;
    return sum;
}
int checker(chessBroad *cbroad, int x, int y, int ptype, int id)
{
    int res = 0;
    switch (id) {
    case 1:
        res = checkerBy369Pai(cbroad, x, y, ptype);
        break;
    case 2:
        res = checkerByYoudu666(cbroad, x, y, ptype);

    default:
        break;
    }
    return res;
}

void AI::AIWork(int x = 1)
{
    for (int i = 1; i <= cbroad->getHeight(); ++i) {
        for (int j = 1; j <= cbroad->getWidth(); ++j) {
            value[i][j] = checker(cbroad, i, j, ptype, x);
        }
    }
    int resx = 1, resy = 1, maxx = ~0x3f3f3f3f;
    for (int i = 1; i <= cbroad->getHeight(); ++i) {
        for (int j = 1; j <= cbroad->getWidth(); ++j) {
            if (value[i][j] > maxx) {
                resx = i, resy = j;
                maxx = value[i][j];
            }
        }
    }
    cbroad->setChess(resx, resy, ptype);
    cbroad->nextRound();
    return;
}
