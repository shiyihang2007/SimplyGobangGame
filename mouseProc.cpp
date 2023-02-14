#include "mouseProc.hpp"

void MouseInputProc()
{
    int tps = 0, ttps = 0, rtps = 520;
    clock_t sClock = clock();
    clock_t nowClock = 0;
    int sleepTime = 1000 / rtps;
    int lstVkLButton = 0;
    lstVkLButtonPos = {};
    clickState = 0;

    while (!exitFlag) {
        nowClock = clock();
        if (nowClock - sClock >= CLOCKS_PER_SEC) {
            tps = ttps;
            sClock = nowClock;
            if (tps > rtps) {
                ++sleepTime;
            }
            else if (tps < rtps) {
                --sleepTime;
            }
            fprintf(flog, "<Info> tps: %d sleeptime: %dms\n", tps, sleepTime);
        }
        ++ttps;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        POINT MousePos;
        GetCursorPos(&MousePos);
        ScreenToClient(hwnd, &MousePos);
        POINT MousePosOnBroad;
        MousePosOnBroad = cbroad.translatePosition(MousePos);

        if (KEY_DOWN(VK_LBUTTON)) {
            clickState = 1;
            if (!lstVkLButton && (MousePosOnBroad.x == lstVkLButtonPos.x && MousePosOnBroad.y == lstVkLButtonPos.y)) {
                clickState = 2;
            }
            lstVkLButton = 1;
            if (!(MousePosOnBroad.x <= 0 || MousePosOnBroad.x > cbroad.getHeight()) &&
                !(MousePosOnBroad.y <= 0 || MousePosOnBroad.y > cbroad.getWidth()) &&
                (!cbroad.getChess(MousePosOnBroad.x, MousePosOnBroad.y))) {
                lstVkLButtonPos = MousePosOnBroad;
            }
            if (!(MousePosOnBroad.x <= 0 || MousePosOnBroad.x > cbroad.getHeight()) &&
                !(MousePosOnBroad.y <= 0 || MousePosOnBroad.y > cbroad.getWidth()) &&
                (!cbroad.getChess(MousePosOnBroad.x, MousePosOnBroad.y)) &&
                (clickState >= 2)) {
                cbroad.setChess(MousePosOnBroad.x, MousePosOnBroad.y, cbroad.getNowInRound());
                cbroad.nextRound();
                lstVkLButtonPos = {};
            }
            if (clickState >= 2) {
                clickState = 0;
            }
        }
        else {
            lstVkLButton = 0;
        }
        if (KEY_DOWN(VK_RBUTTON)) {
        }
        else {
        }
        if (KEY_DOWN(VK_MBUTTON)) {
        }
        else {
        }

        if (cbroad.getNowInRound() == pAI_01.ptype) {
            pAI_01.AIWork(1);
        }
        if (cbroad.getNowInRound() == pAI_02.ptype) {
            pAI_02.AIWork(2);
        }
        if (cbroad.getNowInRound() == pAI_03.ptype) {
            // pAI_03.AIWork();
        }
        if (cbroad.getNowInRound() == pAI_04.ptype) {
            // pAI_04.AIWork();
        }
    }
}
