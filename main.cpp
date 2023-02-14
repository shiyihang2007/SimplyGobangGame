#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include <cstdio>
#include <ctime>

#include <conio.h>

#include <chrono>
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

chessBroad cbroad;

AI pAI_01(1, &cbroad);
AI pAI_02(2, &cbroad);
AI pAI_03(3, &cbroad);
AI pAI_04(4, &cbroad);

POINT lstVkLButtonPos;

std::random_device rd;
std::mt19937 randt(rd());

int exitFlag;
int clickState;

std::thread *thMouseInputProc;

int main()
{
    hwnd = GetForegroundWindow();
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    hInCon = GetStdHandle(STD_INPUT_HANDLE);
    DWORD fdw = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hInCon, fdw);
    CONSOLE_CURSOR_INFO hCurInfo;
    hCurInfo.bVisible = 0, hCurInfo.dwSize = 0;
    SetConsoleCursorInfo(hInCon, &hCurInfo);
    srand(time(NULL));

    flog = fopen(("./log/" + std::to_string(time(NULL)) + ".log").c_str(), "at+");
    auto initTime = time(NULL);
    fprintf(flog, "\n%s", ctime(&initTime));
    fprintf(flog, "Program Inited.\n");

    COORD dwSize;
    dwSize.X = 80, dwSize.Y = 26;
    if (!SetConsoleScreenBufferSize(hCon, dwSize)) {
        fprintf(flog, "<Error> ScreenBuf Change Error! \n");
    }
    SMALL_RECT cwSize;
    cwSize.Left = 0, cwSize.Top = 0;
    cwSize.Right = 79, cwSize.Bottom = 24;
    if (!SetConsoleWindowInfo(hCon, false, &cwSize)) {
        fprintf(flog, "<Error> Screen Change Error! \n");
    }

    cbroad.changeSize(20, 20);
    cbroad.changePlayer(2);

    ui::canvaHeight = ((cbroad.getHeight() + 2) * cbroad.getBroadPadding());
    ui::canvaWidth = ((cbroad.getWidth() + 2) * cbroad.getBroadPadding()) + 100;

    thMouseInputProc = new std::thread(MouseInputProc);
    thMouseInputProc->detach();

    int tps = 0, ttps = 0, rtps = 60;
    clock_t sClock = clock();
    clock_t nowClock = 0;
    int sleepTime = 1000 / rtps;

    while (!exitFlag) {

        ui::displayStart(hwnd);
        ui::printTEXT(cbroad.getWidth() * cbroad.getBroadPadding() + 3 * cbroad.getBroadPadding() + 2,
                      4 * cbroad.getBroadPadding(),
                      L"Now",
                      cbroad.getColor(cbroad.getNowInRound()),
                      RGB(0, 0, 0));
        ui::printTEXT(cbroad.getWidth() * cbroad.getBroadPadding() + 3 * cbroad.getBroadPadding() - 2,
                      5 * cbroad.getBroadPadding(),
                      L"Round",
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
        ui::redisplay();
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

        if (cbroad.checkIsEnding()) {
            exitFlag = 1;
        }
    }
    delete thMouseInputProc;

    ui::displayStart(hwnd);

    cbroad.redisplay();

    ui::printTEXT(10, 0, L"Game Over", RGB(255, 255, 255), RGB(0, 0, 0));
    ui::printTEXT(10, 20, (std::wstring(L"Winner is Player ") + std::to_wstring(cbroad.checkIsEnding())).c_str(), RGB(255, 255, 0), RGB(0, 0, 0));
    ui::printTEXT(10, 40, L"Press Any Key To Continue...", RGB(255, 255, 255), RGB(0, 0, 0));

    ui::redisplay();
    ui::displayCleanUp(hwnd);

    while (kbhit()) {
        getch();
    }
    Sleep(100);
    getch();

    fprintf(flog, "Program Ended.\n");
    return 0;
}
