#ifndef _UI_HPP_
#define _UI_HPP_

#include <cmath>

#include <windows.h>

#define PI acos(-1)

#define R(x) (((x) >> (0)) & 0xff)
#define G(x) (((x) >> (8)) & 0xff)
#define B(x) (((x) >> (16)) & 0xff)

namespace ui {

    extern HDC hdc;
    extern HDC hdcBuf;
    extern HBITMAP displayBuf;
    extern int canvaWidth, canvaHeight;

    void displayStart(HWND);
    void redisplay();
    void displayCleanUp(HWND);
    void printTEXT(int, int, const wchar_t *, COLORREF, COLORREF);

    void printCircleLine(int, int, int, COLORREF);
    void printRectLine(double, double, double, double, double, COLORREF);
    void printLine(double, double, double, double, COLORREF);
}

#endif
