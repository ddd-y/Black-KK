#include "ScreenDraw.h"
MyScreenDraw::MyScreenDraw()
{
    m_hFrontBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        CONSOLE_TEXTMODE_BUFFER,
        nullptr);
    m_hBackBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        CONSOLE_TEXTMODE_BUFFER,
        nullptr);
    HWND hConsole = GetConsoleWindow();
    GetWindowRect(hConsole, &m_OriginalWindowRect);
    ConfigureConsole();
    SetConsoleActiveScreenBuffer(m_hFrontBuffer);
}
