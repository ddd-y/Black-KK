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

void MyScreenDraw::DrawString(int x, int y, const std::wstring& text, WORD attrib)
{
    for (size_t i = 0; i < text.size(); ++i) 
    {
        int currentX = x + static_cast<int>(i);
        if (currentX >= 0 && currentX < m_nCols && y >= 0 && y < m_nRows) {
            Draw(currentX, y, text[i], attrib);
        }
    }
}
