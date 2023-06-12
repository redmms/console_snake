#pragma once
#include "PosControls.h"
#include "Menu.h"
#include "Game.h"
#include "Pause.h"
#include "After.h"
#include "Assurance.h"
using namespace std;
int main()
{   
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_MAXIMIZE);  // открываем консоль в полноэкранном режиме
    SetConsoleCP(1251);  // устанавливаем кодировку, работающую с русским для ввода
    SetConsoleOutputCP(1251);  // теперь для вывода
    CONSOLE_FONT_INFOEX oldFontInfo;
    oldFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(console, FALSE, &oldFontInfo);  // запоминаем изначальный шрифт, чтобы потом вернуть
    SetConsoleFont(L"Lucida Console", 22);  // устанавливаем подходящий нам ("PosControls.h")
    HideCursor();  // прячем консольный курсор ("PosControls.h")
    while (!exitGame) {  // показываем пользователю выбранную страницу
        if (Page == "Game") {
            GameCycle();
            PrevPage = "Game";
        }
        else if (Page == "Pause") {
            PauseCycle();
            PrevPage = "Pause";
        }
        else if (Page == "After") {
            AfterCycle();
            PrevPage = "After";

        }
        else if (Page == "Menu") {
            MenuCycle();
            PrevPage = "Menu";

        }
        else if (Page == "Assurance") {
            AssCycle();
        }
        else {
            cout << "ERROR OCCURED: page status is incorret\n";
        }
    }
    SetCurrentConsoleFontEx(console, FALSE, &oldFontInfo);  // возвращаем изначальный шрифт
    //SetConsoleFont(oldFontInfo.FaceName, oldFontInfo.dwFontSize.Y); // собственная функция, менее эффективно
    ReturnCursor();  // возвращаем нижнее подчеркивание курсора ("PosControls.h")
}