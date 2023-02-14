#include "proc.hpp"

void KeyEventProc(KEY_EVENT_RECORD event)
{
}

void MouseEventProc(MOUSE_EVENT_RECORD event)
{
    if (event.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) { // Mouse Left Button
        // fprintf(flog, "<INFO> MouseEvent: FROM_LEFT_1ST_BUTTON_PRESSED (Left Button)\n");
    }
    if (event.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) { // Mouse Middle Button
        // fprintf(flog, "<INFO> MouseEvent: FROM_LEFT_2ND_BUTTON_PRESSED (Middle Button)\n");
    }
    if (event.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED) {
        // fprintf(flog, "<INFO> MouseEvent: FROM_LEFT_3RD_BUTTON_PRESSED\n");
    }
    if (event.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED) {
        // fprintf(flog, "<INFO> MouseEvent: FROM_LEFT_4TH_BUTTON_PRESSED\n");
    }
    if (event.dwButtonState & RIGHTMOST_BUTTON_PRESSED) { // Mouse Right Button
        // fprintf(flog, "<INFO> MouseEvent: RIGHTMOST_BUTTON_PRESSED     (Right Button)\n");
    }
}

void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD event)
{
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
}
