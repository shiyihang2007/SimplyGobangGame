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

// made by shiyihang
int checkerByShiyihang(chessBroad *cbroad, int x, int y, int ptype)
{
#define AIR (0)
#define ALLY (ptype)

#define INF (0x3f3f3f3f)

    static const int d[][2][2] = {
        {{1, 0}, {-1, 0}},
        {{0, 1}, {0, -1}},
        {{1, 1}, {-1, -1}},
        {{1, -1}, {-1, 1}}};
    static std::vector<int> value = {
        -INF,    // 0
        INF / 1, // 1
        INF / 2, // 2
        INF / 4, // 3
        INF / 8, // 4
        1001,    // 5
        500,     // 6
        6003,    // 7
        3001,    // 8
        20,      // 9
        10,      // 10
        41,      // 11
        21,      // 12
    };

#define V_DISABLED (value[0])
#define V_A_WILLWIN (value[1])
#define V_E_WILLWIN (value[2])
#define V_A_NEXTWIN (value[3])
#define V_E_NEXTWIN (value[4])
#define V_A_GETCTRL4 (value[5])
#define V_E_GETCTRL4 (value[6])
#define V_A_GETCTRL3 (value[7])
#define V_E_GETCTRL3 (value[8])
#define V_A_PREPARE4 (value[9])
#define V_E_PREPARE4 (value[10])
#define V_A_PREPARE3 (value[11])
#define V_E_PREPARE3 (value[12])

#define WIDTH (cbroad->getWidth())
#define HEIGHT (cbroad->getHeight())

#define IS_AVAILABLE(x, y) (((x) > 0 && (x) <= HEIGHT) && ((y) > 0 && (y) <= WIDTH))

#define MP(x, y) (IS_AVAILABLE((x), (y)) ? cbroad->getChess((x), (y)) : (-1))

#define D_NXT_L_X(nowWays) (d[nowWays][0][0])
#define D_NXT_L_Y(nowWays) (d[nowWays][0][1])
#define D_NXT_R_X(nowWays) (d[nowWays][1][0])
#define D_NXT_R_Y(nowWays) (d[nowWays][1][1])

    // fprintf(flog, "$\n");

    if (MP(x, y) != AIR) {
        return V_DISABLED;
    }

    int res = 0;

    res += 100 - sqrt((x - (HEIGHT / 2)) * (x - (HEIGHT / 2)) +
                      (y - (WIDTH / 2)) * (y - (WIDTH / 2)));

    int maxxA2 = 0, maxlA2 = 0;
    int maxxE2 = 0, maxlE2 = 0;

    int maxxA1 = 0, maxlA1 = 0;
    int maxxE1 = 0, maxlE1 = 0;

    for (int nowWays = 0; nowWays < 4; ++nowWays) {
        // fprintf(flog, "#%d\n", nowWays);
        int cntAlly = 0;
        int cntEnemy = 0;
        int canAdd = 0;
        int tx = 0, ty = 0;

        int nxtLx = x + D_NXT_L_X(nowWays);
        int nxtLy = y + D_NXT_L_Y(nowWays);
        int pEnemyL = MP(nxtLx, nxtLy);
        int cntEnemyL = 0;
        tx = nxtLx;
        ty = nxtLy;
        while (IS_AVAILABLE(tx, ty) &&
               MP(tx, ty) == pEnemyL &&
               MP(tx, ty) != AIR) {
            ++(pEnemyL == ALLY ? cntAlly : cntEnemyL);
            tx += D_NXT_L_X(nowWays);
            ty += D_NXT_L_Y(nowWays);
        }
        canAdd += MP(tx, ty) == AIR;

        int nxtRx = x + D_NXT_R_X(nowWays);
        int nxtRy = y + D_NXT_R_Y(nowWays);
        int pEnemyR = MP(nxtRx, nxtRy);
        int cntEnemyR = 0;
        tx = nxtRx;
        ty = nxtRy;
        while (IS_AVAILABLE(tx, ty) &&
               MP(tx, ty) == pEnemyR &&
               MP(tx, ty) != AIR) {
            ++(pEnemyR == ALLY ? cntAlly : cntEnemyR);
            tx += D_NXT_R_X(nowWays);
            ty += D_NXT_R_Y(nowWays);
        }
        canAdd += MP(tx, ty) == AIR;

        if (pEnemyL == pEnemyR) {
            cntEnemy = cntEnemyL + cntEnemyR;
        }
        else {
            cntEnemy = (cntEnemyL > cntEnemyR ? cntEnemyL : cntEnemyR);
        }

        if (cntAlly >= 4) {
            res = V_A_WILLWIN;
            return res;
        }
        if (cntEnemy >= 4) {
            res = V_E_WILLWIN;
            return res;
        }
        if (cntAlly == 3 && canAdd == 2) {
            res = V_A_NEXTWIN;
            return res;
        }
        if (cntEnemy == 3 && canAdd == 2) {
            res = V_E_NEXTWIN;
            return res;
        }

        if (cntAlly == 3 && canAdd == 1) {
            res += V_A_GETCTRL4;
        }
        if (cntEnemy == 3 && canAdd == 1) {
            res += V_E_GETCTRL4;
        }
        if (cntAlly == 2 && canAdd == 2) {
            res += V_A_GETCTRL3;
        }
        if (cntEnemy == 2 && canAdd == 2) {
            res += V_E_GETCTRL3;
        }

        if (cntAlly == 2 && canAdd == 1) {
            res += V_A_PREPARE4;
        }
        if (cntEnemy == 2 && canAdd == 1) {
            res += V_E_PREPARE4;
        }
        if (cntAlly == 1 && canAdd == 2) {
            res += V_A_PREPARE3;
        }
        if (cntEnemy == 1 && canAdd == 2) {
            res += V_E_PREPARE3;
        }

        res += bool(cntAlly > 0);
        res += bool(cntEnemy > 0);

        if (canAdd == 2 && cntAlly > maxxA2) {
            maxlA2 = maxxA2;
            maxxA2 = cntAlly;
        }
        if (canAdd == 2 && cntEnemy > maxxE2) {
            maxlE2 = maxxE2;
            maxxE2 = cntEnemy;
        }

        if (canAdd == 1 && cntAlly > maxxA2) {
            maxlA1 = maxxA1;
            maxxA1 = cntAlly;
        }
        if (canAdd == 1 && cntEnemy > maxxE2) {
            maxlE1 = maxxE1;
            maxxE1 = cntEnemy;
        }
    }

    if (maxxA2 >= 3 && maxlA2 >= 3) {
        res += V_A_NEXTWIN;
        return res;
    }
    if (maxxE2 >= 3 && maxlE2 >= 3) {
        res += V_E_NEXTWIN;
        return res;
    }

    if (maxxA1 >= 4 && maxlA2 >= 3) {
        res += V_A_NEXTWIN;
        return res;
    }
    if (maxxE1 >= 4 && maxlE2 >= 3) {
        res += V_E_NEXTWIN;
        return res;
    }

    if (maxxA1 >= 4 && maxlA1 >= 4) {
        res += V_A_NEXTWIN;
        return res;
    }
    if (maxxE1 >= 4 && maxlE1 >= 4) {
        res += V_E_NEXTWIN;
        return res;
    }

    return res;

#undef V_DISABLED
#undef V_A_WILLWIN
#undef V_E_WILLWIN
#undef V_A_NEXTWIN
#undef V_E_NEXTWIN
#undef V_A_GETCTRL4
#undef V_E_GETCTRL4
#undef V_A_GETCTRL3
#undef V_E_GETCTRL3
#undef V_A_PREPARE4
#undef V_E_PREPARE4
#undef V_A_PREPARE3
#undef V_E_PREPARE3

#undef WIDTH
#undef HEIGHT

#undef IS_AVAILABLE

#undef MP

#undef D_NXT_L_X
#undef D_NXT_L_Y
#undef D_NXT_R_X
#undef D_NXT_R_Y

#undef INF

#undef AIR
#undef ALLY
}

// made by youdu666
int checkerByYoudu666(chessBroad *cbroad, int x, int y, int ptype)
{
    //	fprintf(flog, "<Info> Youdu666\n");
    static const int v[11] = {
        0,          //
        5,          // 阻拦 一个点
        10,         // 背后远处挡 一个点
        50,         // 不阻拦 一个点
        60,         // 背后直接挡 两个点
        500,        // 背后远处挡 两个点
        2000,       // 不阻拦 两个点
        900,        // 背后直接挡 三个点
        10000000,   // 背后远处挡 三个点
        210000000,  // 不挡 三个点
        1000000000, // 四个点
    };
    static const int dx[8] = {-1, 1, 1, 0, 1, -1, -1, 0};
    static const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int ta[7], af[3], n = cbroad->getHeight(), m = cbroad->getWidth(), df;
    int tyuanx = n / 2, tyuany = m / 2;
    if (cbroad->getChess(x, y) != 0)
        return ~0x3f3f3f3f;
    int res = (-abs(x - tyuanx) - abs(y - tyuany)) * 5;
    int sres[8];
    //	randt() % 100;
    for (int u = 0; u < 8; u++) {
        sres[u] = 0;
        int tmpx = x + dx[u], tmpy = y + dy[u];
        if (tmpx < 1 || tmpy < 1 || tmpx > n || tmpy > m)
            continue;
        int df = cbroad->getChess(x + dx[u], y + dy[u]);
        if (df == 0)
            continue;
        for (int yp = -2; yp <= 5; yp++) {
            if (yp >= 0)
                ta[yp] = -df;
            else
                af[-yp] = -df;
            int xx = x + dx[u] * yp, yy = y + dy[u] * yp;
            if (xx > 0 && yy > 0 && xx <= n && yy <= m) {
                if (yp < 0)
                    af[abs(yp)] = cbroad->getChess(xx, yy);
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
        ip += af[1] == df;
        if (af[1] == df)
            ip += af[2] == df;
        int vs = ip * 3;
        if (ip == 4) {
            sres[u] += v[10];
            continue;
        }
        if ((ta[ip + 1] != 0 && ta[ip + 1] != df) || af[1] == -1)
            sres[u] += v[vs - 2];
        else if (ta[ip + 1] == 0 && ta[ip + 2] != df && ta[ip + 2] != 0)
            sres[u] += v[vs - 1];
        else
            sres[u] += v[vs];
    }
    for (int i = 0; i <= 3; i++)
        res += (sres[i] > sres[i + 4] ? sres[i] : sres[i + 4]);
    if (res >= 1000000000)
        return res;
    if (df == -1)
        return res * 2;
    return res;
}

/* Made By 369Pai */
int checkerBy369Pai(chessBroad *cbroad, int x, int y, int ptype)
{
    static const int N = 1e5 + 5;
    static const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    static const int sc[2][N] = {{0, 1, 10, 100, 100}, {0, 2, 20, 200, 2000}};
    int n = cbroad->getHeight(), m = cbroad->getWidth();
    if (cbroad->getChess(x, y) != 0)
        return 0;
    int sum = 0;
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
        break;
    case 3:
        res = checkerByShiyihang(cbroad, x, y, ptype);
        break;

    default:
        break;
    }
    return res;
}

void __AIWork(AI *self, int x = 1)
{
    // FILE *fdebug = fopen("debug.log", "w+");
    for (int i = 1; i <= self->cbroad->getHeight(); ++i) {
        for (int j = 1; j <= self->cbroad->getWidth(); ++j) {
            self->value[i][j] = checker(self->cbroad, i, j, self->ptype, x);
            // fprintf(fdebug, "%4d", value[i][j] < 0 ? -1 : value[i][j]);
        }
        // fprintf(fdebug, "\n");
    }
    // fclose(fdebug);
    int resx = 1, resy = 1, maxx = ~0x3f3f3f3f;
    for (int i = 1; i <= self->cbroad->getHeight(); ++i) {
        for (int j = 1; j <= self->cbroad->getWidth(); ++j) {
            if (self->value[i][j] > maxx) {
                resx = i, resy = j;
                maxx = self->value[i][j];
            }
        }
    }
    self->cbroad->setChess(resx, resy, self->ptype);
    self->cbroad->nextRound();
    return;
}

/* self 指向AI对象的指针
 * AI:
 *   value[][] 权重矩阵
 *   ptype     己方颜色
 *   *cbroad   所在棋盘
 */

// Author: ShiYihang
#define INF (0x3f3f3f3f)
#define ALLY (self->ptype)
#define AIR (0)
#define HEIGHT (self->cbroad->getHeight())
#define WIDTH (self->cbroad->getWidth())
#define IS_AVAILABLE(x, y) (((x) > 0 && (x) <= HEIGHT) && ((y) > 0 && (y) <= WIDTH))
#define MP(x, y) (IS_AVAILABLE(x, y) ? (self->cbroad->getChess(x, y)) : (-1))

#ifdef DEBUG
#define debug(format, x) fprintf(flog, format, x)
#else
#define debug(format, x)
#endif

static const int __AIWork_ByShiyihang_D[][2][2] = {
    {{1, 0}, {-1, 0}},
    {{0, 1}, {0, -1}},
    {{1, 1}, {-1, -1}},
    {{1, -1}, {-1, 1}}};

#define D __AIWork_ByShiyihang_D

#define D_NXT_L_X(nowWays) (D[nowWays][0][0])
#define D_NXT_L_Y(nowWays) (D[nowWays][0][1])
#define D_NXT_R_X(nowWays) (D[nowWays][1][0])
#define D_NXT_R_Y(nowWays) (D[nowWays][1][1])

void __AIWork_ByShiyihang_GetCnt(
    AI *self, int x, int y, int nowWays,
    int &cntAlly, int &cntEnemy, int &canAdd)
{
    int tx = 0, ty = 0;

    int nxtLx = x + D_NXT_L_X(nowWays);
    int nxtLy = y + D_NXT_L_Y(nowWays);
    int pEnemyL = MP(nxtLx, nxtLy);
    int cntEnemyL = 0;
    tx = nxtLx;
    ty = nxtLy;
    while (IS_AVAILABLE(tx, ty) &&
           MP(tx, ty) == pEnemyL &&
           pEnemyL != AIR) {
        ++(pEnemyL == ALLY ? cntAlly : cntEnemyL);
        tx += D_NXT_L_X(nowWays);
        ty += D_NXT_L_Y(nowWays);
    }
    canAdd += MP(tx, ty) == AIR;

    int nxtRx = x + D_NXT_R_X(nowWays);
    int nxtRy = y + D_NXT_R_Y(nowWays);
    int pEnemyR = MP(nxtRx, nxtRy);
    int cntEnemyR = 0;
    tx = nxtRx;
    ty = nxtRy;
    while (IS_AVAILABLE(tx, ty) &&
           MP(tx, ty) == pEnemyR &&
           pEnemyR != AIR) {
        ++(pEnemyR == ALLY ? cntAlly : cntEnemyR);
        tx += D_NXT_R_X(nowWays);
        ty += D_NXT_R_Y(nowWays);
    }
    canAdd += MP(tx, ty) == AIR;

    if (pEnemyL == pEnemyR) {
        cntEnemy = cntEnemyL + cntEnemyR;
    }
    else {
        cntEnemy = (cntEnemyL > cntEnemyR ? cntEnemyL : cntEnemyR);
    }
}

int __AIWork_ByShiyihang_Checker(AI *self, int x, int y)
{

    static std::vector<int> value = {
        -INF,    // 0
        INF / 1, // 1
        INF / 2, // 2
        INF / 4, // 3
        INF / 8, // 4
        1001,    // 5
        500,     // 6
        6003,    // 7
        3001,    // 8
        26,      // 9
        14,      // 10
        101,     // 11
        51,      // 12
    };

#define V_DISABLED (value[0])
#define V_A_WILLWIN (value[1])
#define V_E_WILLWIN (value[2])
#define V_A_NEXTWIN (value[3])
#define V_E_NEXTWIN (value[4])
#define V_A_GETCTRL4 (value[5])
#define V_E_GETCTRL4 (value[6])
#define V_A_GETCTRL3 (value[7])
#define V_E_GETCTRL3 (value[8])
#define V_A_PREPARE4 (value[9])
#define V_E_PREPARE4 (value[10])
#define V_A_PREPARE3 (value[11])
#define V_E_PREPARE3 (value[12])

    if (MP(x, y) != AIR) {
        return V_DISABLED;
    }

    int res = 0 + randt() % 5 - 2;

    res += 50 - sqrt((x - (HEIGHT / 2)) * (x - (HEIGHT / 2)) +
                     (y - (WIDTH / 2)) * (y - (WIDTH / 2))) /
                    4;

    int maxxA2 = 0, maxlA2 = 0;
    int maxxE2 = 0, maxlE2 = 0;

    int maxxA1 = 0, maxlA1 = 0;
    int maxxE1 = 0, maxlE1 = 0;

    for (int nowWays = 0; nowWays < 4; ++nowWays) {
        // fprintf(flog, "#%d\n", nowWays);
        int cntAlly = 0;
        int cntEnemy = 0;
        int canAdd = 0;

        __AIWork_ByShiyihang_GetCnt(self, x, y, nowWays, cntAlly, cntEnemy, canAdd);

        // 连五或长连
        if (cntAlly >= 4) {
            res = V_A_WILLWIN;
            return res;
        }
        if (cntEnemy >= 4) {
            res = V_E_WILLWIN;
            return res;
        }

        // 连四
        if (cntAlly == 3 && canAdd == 2) {
            res = V_A_NEXTWIN;
            return res;
        }
        if (cntEnemy == 3 && canAdd == 2) {
            res = V_E_NEXTWIN;
            return res;
        }

        // 连三
        if (cntAlly == 3 && canAdd == 1) {
            res += V_A_GETCTRL4;
        }
        if (cntEnemy == 3 && canAdd == 1) {
            res += V_E_GETCTRL4;
        }
        if (cntAlly == 2 && canAdd == 2) {
            res += V_A_GETCTRL3;
        }
        if (cntEnemy == 2 && canAdd == 2) {
            res += V_E_GETCTRL3;
        }

        // 连二
        if (cntAlly == 2 && canAdd == 1) {
            res += V_A_PREPARE4;
        }
        if (cntEnemy == 2 && canAdd == 1) {
            res += V_E_PREPARE4;
        }
        if (cntAlly == 1 && canAdd == 2) {
            res += V_A_PREPARE3;
        }
        if (cntEnemy == 1 && canAdd == 2) {
            res += V_E_PREPARE3;
        }

        res *= bool(cntAlly > 0 && canAdd) ? 2 : 1;
        res *= bool(cntEnemy > 0 && canAdd) ? 3 : 1;

        // 特判: 下在此处是否是禁手
        if (canAdd == 2 && cntAlly > maxxA2) {
            maxlA2 = maxxA2;
            maxxA2 = cntAlly;
        }
        if (canAdd == 2 && cntEnemy > maxxE2) {
            maxlE2 = maxxE2;
            maxxE2 = cntEnemy;
        }

        if (canAdd == 1 && cntAlly > maxxA2) {
            maxlA1 = maxxA1;
            maxxA1 = cntAlly;
        }
        if (canAdd == 1 && cntEnemy > maxxE2) {
            maxlE1 = maxxE1;
            maxxE1 = cntEnemy;
        }
    }

    // 三三
    if (maxxA2 >= 2 && maxlA2 >= 2) {
        res += V_A_NEXTWIN;
        return res;
    }
    if (maxxE2 >= 2 && maxlE2 >= 2) {
        res += V_E_NEXTWIN;
        return res;
    }

    // 三四
    if (maxxA1 >= 3 && maxlA2 >= 2) {
        res += V_A_NEXTWIN;
        return res;
    }
    if (maxxE1 >= 3 && maxlE2 >= 2) {
        res += V_E_NEXTWIN;
        return res;
    }

    // 四四
    if (maxxA1 >= 3 && maxlA1 >= 3) {
        res += V_A_NEXTWIN;
        return res;
    }
    if (maxxE1 >= 3 && maxlE1 >= 3) {
        res += V_E_NEXTWIN;
        return res;
    }

    if (x <= 5 || y <= 5 || x > HEIGHT - 5 || y > WIDTH - 5) {
        res *= 0.8;
    }

    return res;
}

void __AIWork_ByShiyihang(AI *self)
{
    int resx = 1;
    int resy = 1;
    int maxx = ~0x3f3f3f3f;

    for (int i = 1; i <= HEIGHT; ++i) {
        for (int j = 1; j <= WIDTH; ++j) {
            self->value[i][j] = __AIWork_ByShiyihang_Checker(self, i, j);
        }
    }

    for (int i = 1; i <= HEIGHT; ++i) {
        for (int j = 1; j <= WIDTH; ++j) {
            if (self->value[i][j] > maxx) {
                resx = i;
                resy = j;
                maxx = self->value[i][j];
            }
        }
    }

    self->cbroad->setChess(resx, resy, ALLY);
    self->cbroad->nextRound();
    return;
}
#undef INF
#undef ALLY
#undef AIR
#undef HEIGHT
#undef WIDTH
#undef IS_AVAILIABLE
#undef MP

// Author: lanos212
#define N (self->cbroad->getHeight())
#define M (self->cbroad->getWidth())
#define pii std::pair<double, double>
#define double long double
const double ed = 1.5;
inline double val(pii aa)
{
    return std::max(aa.first, aa.second * ed);
}
inline pii val(int nn, int mm, int x, int y, int p, chessBroad *cbroad)
{
    const int n = nn, m = mm, maaa = std::max(nn, mm);
    int a[n + 1][m + 1], f[maaa + 1], sum[6] = {0, 0, 0, 0, 0, 0}, s[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            a[i][j] = cbroad->getChess(i, j);
    if (x)
        a[x][y] = p;
    for (int i = 1; i <= n; ++i) {
        f[0] = 0;
        for (int j = 1; j <= m; ++j)
            f[j] = f[j - 1] + (a[i][j] == p) - 100 * (a[i][j] != 0 && a[i][j] != p);
        for (int j = 0; j + 5 <= m; ++j)
            if (f[j + 5] - f[j] >= 0)
                ++s[f[j + 5] - f[j]];
            else
                for (int j = 0; j <= 5; ++j)
                    sum[j] += 1 << (1 << std::min(3, s[j])), s[j] = 0;
    }
    for (int j = 0; j <= 5; ++j)
        sum[j] += 1 << (1 << std::min(3, s[j])), s[j] = 0;
    for (int j = 1; j <= m; ++j) {
        f[0] = 0;
        for (int i = 1; i <= n; ++i)
            f[i] = f[i - 1] + (a[i][j] == p) - 100 * (a[i][j] != 0 && a[i][j] != p);
        for (int i = 0; i + 5 <= n; ++i)
            if (f[i + 5] - f[i] >= 0)
                ++s[f[i + 5] - f[i]];
            else
                for (int i = 0; i <= 5; ++i)
                    sum[i] += 1 << (1 << std::min(3, s[i])), s[i] = 0;
    }
    for (int j = 0; j <= 5; ++j)
        sum[j] += 1 << (1 << std::min(3, s[j])), s[j] = 0;
    pii ans;
    ans.first = 1000000 * (sum[4] + sum[5]) + 1000 * sum[3] + 100 * sum[2] + 10 * sum[1] + 100;

    // almost the same,but it's for the enemy
    memset(sum, 0, sizeof(sum));
    for (int i = 1; i <= n; ++i) {
        f[0] = 0;
        for (int j = 1; j <= m; ++j)
            f[j] = f[j - 1] - 100 * (a[i][j] == p) + (a[i][j] != 0 && a[i][j] != p);
        for (int j = 0; j + 5 <= m; ++j)
            if (f[j + 5] - f[j] >= 0)
                ++s[f[j + 5] - f[j]];
            else
                for (int j = 0; j <= 5; ++j)
                    sum[j] += 1 << (1 << std::min(3, s[j])), s[j] = 0;
    }
    for (int j = 0; j <= 5; ++j)
        sum[j] += 1 << (1 << std::min(3, s[j])), s[j] = 0;
    for (int j = 1; j <= m; ++j) {
        f[0] = 0;
        for (int i = 1; i <= n; ++i)
            f[i] = f[i - 1] - 100 * (a[i][j] == p) + (a[i][j] != 0 && a[i][j] != p);
        for (int i = 0; i + 5 <= n; ++i)
            if (f[i + 5] - f[i] >= 0)
                ++s[f[i + 5] - f[i]];
            else
                for (int i = 0; i <= 5; ++i)
                    sum[i] += 1 << (1 << std::min(3, s[i])), s[i] = 0;
    }
    for (int j = 0; j <= 5; ++j)
        sum[j] += 1 << (1 << std::min(3, s[j])), s[j] = 0;

    ans.second = 1000000 * (sum[4] + sum[5]) + 1000 * sum[3] + 100 * sum[2] + 10 * sum[1] + 100;
    return ans;
}
void __AIWork_By_lanos_the_cat(AI *self)
{
    int na = 1, nb = 1;
    pii now;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            if (!self->cbroad->getChess(i, j)) {
                pii aa = val(N, M, i, j, self->ptype, self->cbroad);
                int dis = std::min(i - 1, N - i) + std::min(j - 1, M - j);
                aa.first *= (100 - dis) * 0.01, aa.second *= (100 - dis) * 0.01;
                if ((i == 1 && j == 1) || val(aa) < val(now))
                    now = aa, na = i, nb = j;
            }
    self->cbroad->setChess(na, nb, self->ptype);
    self->cbroad->nextRound();
    return;
}
#undef N
#undef M
#undef double
#undef pii

void AI::AIWork(int x = 1)
{
    if (x <= 255) {
        __AIWork(this, x);
    }
    else {
        switch (x) {
        case 256:
            __AIWork_ByShiyihang(this);
            break;
        case 257:
            __AIWork_By_lanos_the_cat;
            break;
        default:
            break;
        }
    }
}
