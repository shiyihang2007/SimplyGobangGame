#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#ifndef DEBUG
#define DEBUG
#endif

#include <cstdio>
#include <ctime>

#include <conio.h>

#include <chrono>
#include <mutex>
#include <random>
#include <string>
#include <thread>

#include <windows.h>

#include "AIWork.hpp"
#include "chess.hpp"
#include "mouseProc.hpp"
#include "proc.hpp"
#include "ui.hpp"

HWND hwnd;
HANDLE hCon, hInCon;

FILE *flog;

chessBroad cbroad(40, 40, 15);

AI pAI_01(1, &cbroad);
AI pAI_02(2, &cbroad);
AI pAI_03(3, &cbroad);
AI pAI_04(4, &cbroad);

POINT lstVkLButtonPos;
POINT lstPlaceChessPos;

std::random_device rd;
std::mt19937 randt(rd());

int exitFlag;
int clickState;

std::thread *thMouseInputProc;

std::vector<int> checkerByShiyihangRestudyValue = {
    -106110967, // 0
    106110967,  // 1
    53055483,   // 2
    26527741,   // 3
    13263870,   // 4
    1001,       // 5
    500,        // 6
    6003,       // 7
    3001,       // 8
    20,         // 9
    10,         // 10
    41,         // 11
    21,         // 12
};

int main(int argc, const char *argv[])
{
    hwnd = GetForegroundWindow();
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    hInCon = GetStdHandle(STD_INPUT_HANDLE);
    DWORD fdw = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hInCon, fdw);
    CONSOLE_CURSOR_INFO hCurInfo;
    hCurInfo.bVisible = 0, hCurInfo.dwSize = 0;
    SetConsoleCursorInfo(hInCon, &hCurInfo);
    ui::canvaHeight = ((cbroad.getHeight() + 2) * cbroad.getBroadPadding());
    ui::canvaWidth = ((cbroad.getWidth() + 2) * cbroad.getBroadPadding()) + 100;
    srand(time(NULL));

    flog = fopen(("./log/" + std::to_string(time(NULL)) + ".log").c_str(), "at+");
    auto initTime = time(NULL);
    fprintf(flog, "\n%s", ctime(&initTime));
    fprintf(flog, "Program Inited.\n");

    COORD dwSize;
    dwSize.X = 80, dwSize.Y = 30;
    if (!SetConsoleScreenBufferSize(hCon, dwSize)) {
        fprintf(flog, "<Error> ScreenBuf Change Error! \n");
    }
    SMALL_RECT cwSize;
    cwSize.Left = 0, cwSize.Top = 0;
    cwSize.Right = 79, cwSize.Bottom = 24;
    if (!SetConsoleWindowInfo(hCon, false, &cwSize)) {
        fprintf(flog, "<Error> Screen Change Error! \n");
    }

    thMouseInputProc = new std::thread(MouseInputProc);
    thMouseInputProc->detach();

    int nogui = argc > 1 && argv[1] == "--nogui";

    int tps = 0, ttps = 0, rtps = 60;
    clock_t sClock = clock();
    clock_t nowClock = 0;
    int sleepTime = 1000 / rtps;
    while (!exitFlag) {

        ui::displayStart(hwnd);
        ui::printTEXT(cbroad.getWidth() * cbroad.getBroadPadding() + 3 * cbroad.getBroadPadding(),
                      4 * cbroad.getBroadPadding() + 5,
                      L"Now",
                      cbroad.getColor(cbroad.getNowInRound()),
                      RGB(0, 0, 0));
        ui::printCircleLine(cbroad.getWidth() * cbroad.getBroadPadding() + 2 * cbroad.getBroadPadding(),
                            5 * cbroad.getBroadPadding(),
                            cbroad.getBroadPadding() / 2,
                            cbroad.getColor(cbroad.getNowInRound()));
        ui::printCircleLine(lstVkLButtonPos.y * cbroad.getBroadPadding(),
                            lstVkLButtonPos.x * cbroad.getBroadPadding(),
                            cbroad.getBroadPadding() / 2,
                            RGB(192, 192, 192));
        cbroad.redisplay();
        COLORREF tColor = cbroad.getColor(cbroad.getChess(lstPlaceChessPos.x, lstPlaceChessPos.y));
        ui::printCircleLine(lstPlaceChessPos.y * cbroad.getBroadPadding(),
                            lstPlaceChessPos.x * cbroad.getBroadPadding(),
                            cbroad.getBroadPadding() / 2 - 3,
                            RGB(R(tColor) * 0.4 + 127, G(tColor) * 0.4 + 127, B(tColor) * 0.4 + 127));
        if (!nogui) {
            ui::redisplay();
        }
        ui::displayCleanUp(hwnd);

        INPUT_RECORD bufConIn[256];
        DWORD cntConIn;
        PeekConsoleInput(hInCon, bufConIn, 255, &cntConIn);

        FlushConsoleInputBuffer(hInCon);

        for (int i = 0; i < cntConIn; ++i) {
            switch (bufConIn[i].EventType) {
            case KEY_EVENT: // keyboard input
                KeyEventProc(bufConIn[i].Event.KeyEvent);
                break;

            case MOUSE_EVENT: // mouse input
                MouseEventProc(bufConIn[i].Event.MouseEvent);
                break;

            case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                ResizeEventProc(bufConIn[i].Event.WindowBufferSizeEvent);
                break;

            case FOCUS_EVENT: // disregard focus events

            case MENU_EVENT: // disregard menu events
                break;

            default:
                fprintf(flog, "<Error> Unknown Event! \n");
                break;
            }
        }

        nowClock = clock();
        if (nowClock - sClock >= CLOCKS_PER_SEC) {
            tps = ttps;
            ttps = 0;
            sClock = nowClock;
            sleepTime += tps - rtps ? (tps - rtps) / 2 + 1 : 0;
            if (sleepTime < 1) {
                sleepTime = 1;
            }
            if (sleepTime > 100) {
                sleepTime = 100;
            }
            fprintf(flog, "<Info> MainThread tps: %d sleeptime: %dms\n", tps, sleepTime);
        }
        ++ttps;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        if (cbroad.checkIsEnding()) {
            exitFlag = 1;
        }

        std::fflush(flog);
    }
    delete thMouseInputProc;

    ui::displayStart(hwnd);

    cbroad.redisplay();

    ui::printTEXT(10, 0, L"Game Over", RGB(255, 255, 255), RGB(0, 0, 0));
    ui::printTEXT(10, 20, (std::wstring(L"Winner is Player ") + std::to_wstring(cbroad.checkIsEnding())).c_str(), RGB(255, 255, 0), RGB(0, 0, 0));
    ui::printTEXT(10, 40, L"Press Any Key To Continue...", RGB(255, 255, 255), RGB(0, 0, 0));

    if (!nogui) {
        ui::redisplay();
    }
    ui::displayCleanUp(hwnd);

    while (kbhit()) {
        getch();
    }
    Sleep(100);
    getch();

    fprintf(flog, "Program Ended.\n");

    printf("%.2lf\n", cbroad.checkIsEnding() == 1 ? (double)clock() / CLOCKS_PER_SEC : 1e9);
    return 0;
}
