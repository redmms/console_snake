#pragma once
#include "PosControls.h"
// ��� PosControls.cpp
#include <stdio.h>
#include <string>
#include <set>
//#define RESET   "\033[0m"

// ���������� ����������:
int width = 20, height = 15; 
frm bond{ 0, width + 1, 0, height + 1 };  // ��������� ������� ����� ����� ������ �� 2 ������, �� 1 �� ������ ������� �����
string Page = "Menu";
string PrevPage = Page;
bool exitGame = false;
int CurrLine;
int myUp = 'W', myDown = 'S', myLeft = 'A', myRight = 'D';
int myUpVsc = myUp, myDownVsc = myDown, myLeftVsc = myLeft, myRightVsc = myRight;

// ��������� ����������:
COORD CursPos;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;
bool GotCursor = GetConsoleCursorInfo(console, &cursorInfo);
COORD previousWindowSize = { 0, 0 };
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
COORD currentWindowSize;

// ���������� ���������� �� �������, ����� �� ����������� ��� ������ ������ �������, ��� ��� ����������� ��� ����� �����.

int IntInput() {
    int res = 0;
    int ch;
    bool done = false;
    while (true) {
        ch = _getche();
        if (isdigit(ch)) {
            done = true;
            res *= 10;
            res += (ch - '0');
        }
        else
            break;
        if (res >= 1000000000)
            break;
    }
    if (done)
        return res;
    else 
        return -1;
}
void PrintConsoleInputError(DWORD errorCode)
{
    LPWSTR errorMessage = nullptr;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&errorMessage,
        0,
        NULL
    );

    if (errorMessage)
    {
        std::wcout << "Error code: " << errorCode << std::endl;
        std::wcout << "Error message: " << errorMessage << std::endl;
        LocalFree(errorMessage);
    }
    else
    {
        std::cout << "Unknown error occurred." << std::endl;
    }
}
pair <int, WCHAR> GetKeyCode() {
    INPUT_RECORD inputRecord;
    DWORD eventsRead;
    DWORD resCode = ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &inputRecord, 1, &eventsRead);
    if (!resCode) {
        DWORD errorCode = GetLastError();
        PrintConsoleInputError(errorCode);

    }
    if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
    {
        return make_pair(inputRecord.Event.KeyEvent.wVirtualKeyCode, 
                            inputRecord.Event.KeyEvent.uChar.UnicodeChar);


    }
}
void PrintSpaces() {
    cout << "            ";
}
void FillConsoleLineWithSpaces() // ���� �������� ����, � ��������� ������ ��������� � ���, ��������� ������ �����
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // �������� ���������� � ������ �������
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    COORD currentPos = csbi.dwCursorPosition;
    COORD bufferSize = csbi.dwSize;
    int spacesToFill = bufferSize.X - currentPos.X;

    // ������� ������� �� ������� ����
    for (int i = 0; i < spacesToFill; i++)
    {
        std::cout << ' ';
    }
}
bool WindowResized() {
    GetConsoleScreenBufferInfo(console, &consoleInfo);
    currentWindowSize = consoleInfo.dwSize;
    if (currentWindowSize.X != previousWindowSize.X || currentWindowSize.Y != previousWindowSize.Y) {
        // ������ ���� ���������
        previousWindowSize = currentWindowSize;
        return true;
    }
    return false;
}
void CheckCursor() { // �� �� �������������� ��� � windowResized �� ������ �������, ����� ���� �������, ��� ��� ���������� � ������� ������ �������� ���������� �� ���� ������� 8 ���� ������ 22
    GetConsoleCursorInfo(console, &cursorInfo);
    if (cursorInfo.bVisible) {
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(console, &cursorInfo);
    }
}
void HideCursor() {
    cursorInfo.bVisible = FALSE; // ��������� �������� false, ����� ������ �������������
    SetConsoleCursorInfo(console, &cursorInfo);
}
void ReturnCursor() {
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(console, &cursorInfo);
}
bool IsNum(const string& str) // ���������
{
    try {
        size_t pos = -1;
        stoi(str, &pos); // ��� ����� �� ��������
        return pos == -1;  // ��� ������� ������ ���� ������������� � �����
    }
    catch (const exception&) {
        return false;  // ������ ��� �������������� ��� ������������ ������ �����
    }
}
COORD GetPos()
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(console, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}
void SetConsoleFont(const wchar_t* fontName, int fontSize)
{
    CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    wcscpy_s(fontInfo.FaceName, fontName);
    fontInfo.dwFontSize.Y = fontSize;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
}
bool Pressed(initializer_list <int> keys) {
    int whole{ 0 };
    for (auto key : keys) {
        whole |= GetKeyState(key);
    }
    return (whole & 0x8000);
}

bool Clicked(initializer_list <int> keys) {
    int whole{ 0 };
    for (auto key : keys) {
        whole |= GetKeyState(key);
    }
    if (whole & 0x8000)
        while (true) {
            whole = 0;
            for (auto key : keys) {
                whole |= GetKeyState(key);
            }
            if (!(whole & 0x8000)) {
                return true;
            }
        }
    else
        return false;
}

void GoTo(int i, int j)
{
    CursPos.X = j;
    CursPos.Y = i;
    SetConsoleCursorPosition(console, CursPos);
}
void GoTo(int i, int j, string text)
{
    CursPos.X = j;
    CursPos.Y = i;
    SetConsoleCursorPosition(console, CursPos);
    printf("%s", text.c_str());
}
void GoTo(int i, int j, const string& color, string text)
{
    CursPos.X = j;
    CursPos.Y = i;
    SetConsoleCursorPosition(console, CursPos);
    printf("%s%s", color.c_str(), text.c_str());
}
//void GoTo(int i, int j, const string & color, string text)
//{
//    CursPos.X = j;
//    CursPos.Y = i;
//    SetConsoleCursorPosition(console, CursPos);
//    printf("%s%s%s", color.c_str(), text.c_str(), RESET);
//}
void GoToFrmMdl(frm boundaries) {
    int mdlX = (boundaries.left + boundaries.right) / 2,
        mdlY = (boundaries.up + boundaries.down) / 2;
    GoTo(mdlY, mdlX);

}
void GoToFrmMdl(frm boundaries, string text) {
    int mdlX = max((boundaries.left + boundaries.right) / 2 - text.size() / 2, 0),
        mdlY = (boundaries.up + boundaries.down) / 2;
    GoTo(mdlY, mdlX);
    printf("%s", text.c_str());
}
void GoToFrmEnd(int downBoundary) {
    int endX = 0, endY = downBoundary + 2;
    GoTo(endY, endX);
}
void GoToFrmEnd(int downBoundary, string text) {
    int endX = 0, endY = downBoundary + 2;
    GoTo(endY, endX);
    printf("%s", text.c_str());
}
void GoToWndEnd() {
    int endX = 0, endY = GetLargestConsoleWindowSize(console).Y;
    GoTo(endY, endX);
}
void GoToWndEnd(string text) {
    int endX = 0, endY = GetLargestConsoleWindowSize(console).Y;
    GoTo(endY, endX);
    printf("%s", text.c_str());
}
