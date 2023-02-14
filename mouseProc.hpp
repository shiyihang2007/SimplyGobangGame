#ifndef _MOUSE_PROC_HPP_
#define _MOUSE_PROC_HPP_

#include <cstdio>
#include <ctime>

#include <chrono>
#include <thread>

#include <windows.h>

#include "AIWork.hpp"
#include "chess.hpp"

extern HWND hwnd;

extern FILE *flog;

extern chessBroad cbroad;

extern AI pAI_01;
extern AI pAI_02;
extern AI pAI_03;
extern AI pAI_04;

extern POINT lstVkLButtonPos;

extern int exitFlag;
extern int clickState;

/* 判断按键是否按下
 * 返回值:
 *   1: 按下
 *   0: 弹起
 * VK_VALUE:
 *   虚拟按键编码
 *   VK_LBUTTON 鼠标左键
 *   VK_RBUTTON 鼠标右键
 */
#define KEY_DOWN(VK_VALUE) ((GetAsyncKeyState(VK_VALUE) & 0x8800) ? 1 : 0)

void MouseInputProc();

#endif
