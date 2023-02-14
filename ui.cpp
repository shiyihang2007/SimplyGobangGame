
#include "ui.hpp"

namespace ui {

#define max(a, b) ((a) > (b) ? (a) : (b))

    HDC hdc;
    HDC hdcBuf;
    HBITMAP displayBuf;
    int canvaWidth, canvaHeight;

    void roll(double &wx, double &wy, double angle)
    {
        double tx = wx * cos(angle) - wy * sin(angle);
        double ty = wx * sin(angle) + wy * cos(angle);
        wx = tx, wy = ty;
    }

    void displayStart(HWND hwnd)
    {
        hdc = GetDC(hwnd);
        hdcBuf = CreateCompatibleDC(hdc);
        displayBuf = CreateCompatibleBitmap(hdc, canvaWidth, canvaHeight);
        SelectObject(hdcBuf, displayBuf);
    }
    void redisplay()
    {
        BitBlt(hdc, 0, 0, canvaWidth, canvaHeight, hdcBuf, 0, 0, SRCCOPY);
    }
    void displayCleanUp(HWND hwnd)
    {
        DeleteDC(hdcBuf);
        DeleteObject(displayBuf);
        ReleaseDC(hwnd, hdc);
    }
    void printTEXT(int x, int y, const wchar_t *text, COLORREF color, COLORREF bkcolor)
    {
        SetTextColor(hdcBuf, color);
        SetBkColor(hdcBuf, bkcolor);
        TextOutW(hdcBuf, (x + canvaWidth) % canvaWidth, (y + canvaHeight) % canvaHeight, text, lstrlenW(text));
    }
    void printCircleLine(int x, int y, int width, COLORREF color)
    {
        // It's too slow, But I have no idea to improve it
        for (double i = 0; i <= PI * 2; i += 2.0 / width / PI) {
            SetPixel(hdcBuf, (int)(x + sin(i) * width + canvaWidth) % canvaWidth, (int)(y + cos(i) * width + canvaHeight) % canvaHeight, color);
        }
    }
    void printRectLine(double x, double y, double width, double height, double angle, COLORREF color)
    {
        for (double i = -width; i <= width; ++i) {
            for (double j = -height; j <= height; j += height * 2) {
                double tx = i, ty = j;
                roll(tx, ty, angle);
                tx += x, ty += y;
                SetPixel(hdcBuf, tx, ty, color);
            }
        }
        for (double j = -height; j <= height; ++j) {
            for (double i = -width; i <= width; i += width * 2) {
                double tx = i, ty = j;
                roll(tx, ty, angle);
                tx += x, ty += y;
                SetPixel(hdcBuf, tx, ty, color);
            }
        }
    }
    void printLine(double fx, double fy, double tx, double ty, COLORREF color)
    {
        double deltax, deltay, x, y;
        int steps = max(abs(tx - fx), abs(ty - fy));
        deltax = (tx - fx) / steps;
        deltay = (ty - fy) / steps;
        x = fx;
        y = fy;
        SetPixel(hdcBuf, (int)(x + 0.5), (int)(y + 0.5), color);
        for (int i = 0; i < steps; i++) {
            x += deltax;
            y += deltay;
            SetPixel(hdcBuf, (int)(x + 0.5), (int)(y + 0.5), color);
        }
    }
}
