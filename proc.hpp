#ifndef _PROC_HPP_
#define _PROC_HPP_

#include <cstdio>

#include <windows.h>

#include "chess.hpp"

#define ConTextHeight (18)
#define ConTextWidth (8)

extern HANDLE hCon;

extern FILE *flog;

extern chessBroad cbroad;

void KeyEventProc(KEY_EVENT_RECORD);
void MouseEventProc(MOUSE_EVENT_RECORD);
void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

#endif
