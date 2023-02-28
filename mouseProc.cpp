#include "mouseProc.hpp"

void MouseInputProc()
{
    int tps = 0, ttps = 0, rtps = 2;
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
            ttps = 0;
            sClock = nowClock;
            sleepTime += tps - rtps ? (tps - rtps) / 2 + 1 : 0;
            if (sleepTime < 1) {
                sleepTime = 1;
            }
            if (sleepTime > 500) {
                sleepTime = 500;
            }
            fprintf(flog, "<Info> MouseProcThread tps: %d sleeptime: %dms\n", tps, sleepTime);
        }
        ++ttps;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        if (cbroad.getNowInRound() == pAI_01.ptype) {
            pAI_01.AIWork(2);
        }
        else if (cbroad.getNowInRound() == pAI_02.ptype) {
            pAI_02.AIWork(256);
        }
        else if (cbroad.getNowInRound() == pAI_03.ptype) {
            // pAI_03.AIWork();
        }
        else if (cbroad.getNowInRound() == pAI_04.ptype) {
            // pAI_04.AIWork();
        }

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
        if (KEY_DOWN(VK_ESCAPE)) {
            exitFlag = 1;
        }
    }
}
