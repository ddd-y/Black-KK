#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <algorithm>
#include<mutex>
class MyScreenDraw 
{
private:
    HANDLE m_hFrontBuffer;
    HANDLE m_hBackBuffer;
    std::mutex TheMutex;
    int m_nCols;
    int m_nRows;
    std::unique_ptr<CHAR_INFO[]> m_pScreenBuffer;
    RECT m_OriginalWindowRect;
    bool m_bMaximized = false;
    void ConfigureConsole() {
        // 禁用快速编辑模式
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hInput, &mode);
        SetConsoleMode(hInput, mode & ~ENABLE_QUICK_EDIT_MODE);

        // 设置初始控制台参数
        UpdateConsoleSize();
        SetConsoleTitle(L"Console Renderer");

        // 隐藏光标
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.bVisible = false;
        cursorInfo.dwSize = 1;
        SetConsoleCursorInfo(m_hFrontBuffer, &cursorInfo);
        SetConsoleCursorInfo(m_hBackBuffer, &cursorInfo);
    }

    void UpdateConsoleSize() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);

        m_nCols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        m_nRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        // 调整缓冲区尺寸
        COORD bufferSize = { static_cast<SHORT>(m_nCols), static_cast<SHORT>(m_nRows * 2) };
        SetConsoleScreenBufferSize(m_hFrontBuffer, bufferSize);
        SetConsoleScreenBufferSize(m_hBackBuffer, bufferSize);

        // 重置屏幕缓冲区
        m_pScreenBuffer = std::make_unique<CHAR_INFO[]>(m_nCols * m_nRows);
        Clear();
    }
public:
    MyScreenDraw();
    ~MyScreenDraw() {
        CloseHandle(m_hFrontBuffer);
        CloseHandle(m_hBackBuffer);
    }

    void Maximize() {
        if (!m_bMaximized) {
            HWND hConsole = GetConsoleWindow();
            ShowWindow(hConsole, SW_MAXIMIZE);
            UpdateConsoleSize();
            m_bMaximized = true;
        }
    }

    void Restore() {
        if (m_bMaximized) {
            HWND hConsole = GetConsoleWindow();
            SetWindowPos(hConsole, nullptr,
                m_OriginalWindowRect.left,
                m_OriginalWindowRect.top,
                m_OriginalWindowRect.right - m_OriginalWindowRect.left,
                m_OriginalWindowRect.bottom - m_OriginalWindowRect.top,
                SWP_NOZORDER);
            m_bMaximized = false;
        }
    }

    void Clear(WORD attrib = 0x000F) {
        for (int i = 0; i < m_nCols * m_nRows; ++i) {
            m_pScreenBuffer[i].Char.UnicodeChar = L' ';
            m_pScreenBuffer[i].Attributes = attrib;
        }
    }

    void Draw(int x, int y, wchar_t c, WORD attrib = 0x000F) {
        if (x >= 0 && x < m_nCols && y >= 0 && y < m_nRows) {
            const int index = y * m_nCols + x;
            m_pScreenBuffer[index].Char.UnicodeChar = c;
            m_pScreenBuffer[index].Attributes = attrib;
        }
    }

    void Render() {
        // 设置写入区域参数
        COORD bufferSize = { static_cast<SHORT>(m_nCols), static_cast<SHORT>(m_nRows) };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT writeRegion = { 0, 0,
            static_cast<SHORT>(m_nCols - 1),
            static_cast<SHORT>(m_nRows - 1) };

        // 写入后台缓冲区
        WriteConsoleOutputW(
            m_hBackBuffer,
            m_pScreenBuffer.get(),
            bufferSize,
            bufferCoord,
            &writeRegion);

        // 切换显示缓冲区
        SetConsoleActiveScreenBuffer(m_hBackBuffer);
        std::swap(m_hBackBuffer, m_hFrontBuffer);
    }

    int GetWidth() const { return m_nCols; }
    int GetHeight() const { return m_nRows; }
};

