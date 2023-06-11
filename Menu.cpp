#include "Menu.h"
#include "PosControls.h"
#include <map>
#include <iostream>
#include <string> 
#include <fcntl.h>
#include <io.h>



using namespace std;

bool menuOver;
map <int, string> Lines;
initializer_list <string> d;
string l;

// Глобальные:
extern bool walls;
extern bool acceleration;
extern int speed;


void bDraw(bool tumblr) {
    if (tumblr)
        GoTo(GetPos().Y, 20, WHITE, "ON");
    else
        GoTo(GetPos().Y, 20, WHITE, "OFF");
}
void DrawMenu()
{
    system("cls");
    cout << WHITE << "Menu";

    cout << RED << "\n\n  Width:"; // 10 (минимальная) 100(макс)
    Lines[GetPos().Y] = "Width";
    GoTo(GetPos().Y, 20, WHITE, to_string(width));
    //cout << GREY << "\n(Enter any width from 10 to 100)\n";

    cout << RED << "\n  Height:"; // 10 (минимальная) 100(макс)
    Lines[GetPos().Y] = "Height";
    GoTo(GetPos().Y, 20, WHITE, to_string(height));
    //cout << GREY << "\n(Enter any height from 10 to 100)\n";

    cout << RED << "\n  Solid walls:";  // ON/OFF
    Lines[GetPos().Y] = "Walls";
    bDraw(walls);
    //cout << GREY << "\n(Choose if hitting a wall should stop the game)\n";

    cout << RED << "\n  Acceleration:"; // ON/OFF
    Lines[GetPos().Y] = "Acceleration";
    bDraw(acceleration);
    //cout << GREY << "\n(Choose if speed should grow gradually)\n";

    cout << RED << "\n  Speed:"; // мин 1 (шаг 500), макс 10 (шаг 50)
    Lines[GetPos().Y] = "Speed";
    GoTo(GetPos().Y, 20, WHITE, to_string(speed));
    //cout << "\n(only for OFF acceleration)";
    //cout << GREY << "\n(Choose any speed from 1 to 10)\n";

    cout << RED << "\n  LEFT:"; 
    Lines[GetPos().Y] = "LEFT";
    GoTo(GetPos().Y, 20);
    int modeCode = _setmode(_fileno(stdout), _O_U16TEXT); // Установка режима вывода в юникоде
    wcout << WHITE << static_cast <wchar_t> (myLeftVsc) << " (" << myLeft << ")";
    modeCode = _setmode(_fileno(stdout), _O_TEXT);

    cout << RED << "\n  RIGHT:";
    Lines[GetPos().Y] = "RIGHT";
    GoTo(GetPos().Y, 20);
    modeCode = _setmode(_fileno(stdout), _O_U16TEXT); // Установка режима вывода в юникоде
    wcout << WHITE << static_cast <wchar_t> (myRightVsc) << " (" << myRight << ")";
    modeCode = _setmode(_fileno(stdout), _O_TEXT); 

    
    cout << RED << "\n  UP:";
    Lines[GetPos().Y] = "UP";
    GoTo(GetPos().Y, 20);
    modeCode = _setmode(_fileno(stdout), _O_U16TEXT); // Установка режима вывода в юникоде
    wcout << WHITE << static_cast <wchar_t> (myUpVsc) << " (" << myUp << ")";
    modeCode = _setmode(_fileno(stdout), _O_TEXT);  

    
    cout << RED << "\n  DOWN:";
    Lines[GetPos().Y] = "DOWN";
    GoTo(GetPos().Y, 20);
    modeCode = _setmode(_fileno(stdout), _O_U16TEXT); // Установка режима вывода в юникоде
    wcout << WHITE << static_cast <wchar_t> (myDownVsc) << " (" << myDown << ")";
    modeCode = _setmode(_fileno(stdout), _O_TEXT);   

    
    cout << WHITE << "\n\n> Start!";
    CurrLine = GetPos().Y;
    Lines[GetPos().Y] = "Start";
}


void MenuInput()
{
    if (_kbhit()) {
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        if (Pressed({ VK_ESCAPE })) {
            menuOver = true;
            Page = "Assurance";
        }
        else if (Clicked({ myUp, VK_UP })) {
            GoTo(CurrLine, 0, " ");
            if (Lines.find(CurrLine) != Lines.begin())
                CurrLine = (--Lines.find(CurrLine))->first;
            else
                CurrLine = (--Lines.end())->first;
            GoTo(CurrLine, 0, ">");
        }
        else if (Clicked({ myDown, VK_DOWN })) {
            GoTo(CurrLine, 0, " ");
            if (++Lines.find(CurrLine) != Lines.end())
                CurrLine = (++Lines.find(CurrLine))->first;
            else
                CurrLine = (Lines.begin())->first;
            GoTo(CurrLine, 0, ">");
        }
        else if (Pressed({ 13 })) { // Enter
            if (Lines[CurrLine] == "Width") {
                GoTo(CurrLine, 20);
                PrintSpaces();
                GoTo(CurrLine, 20);
                ReturnCursor();
                int inp = IntInput();
                if (inp != -1) {
                    width = inp;
                    if (width < 10)
                        width = 10;
                    else if (width > 100)
                        width = 100;
                    bond.right = width + 1;
                }

                GoTo(CurrLine, 20, to_string(width));
                PrintSpaces();
            }
            else if (Lines[CurrLine] == "Height") {
                GoTo(CurrLine, 20);
                PrintSpaces();
                GoTo(CurrLine, 20);
                ReturnCursor();
                int inp = IntInput();
                if (inp != -1) {
                    height = inp;
                    if (height < 10)
                        height = 10;
                    else if (height > 100)
                        height = 100;
                    bond.down = height + 1;
                }
                GoTo(CurrLine, 20, to_string(height));
                PrintSpaces();
            }
            else if (Lines[CurrLine] == "Walls") {
                GoTo(CurrLine, 20);
                ReturnCursor();
                bool optionOver = false;
                while (!optionOver) {
                    if (_kbhit()) {
                        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                        if (Clicked({ myLeft, myRight, VK_LEFT, VK_RIGHT })) {
                            walls = !walls;
                            bDraw(walls);
                            PrintSpaces();
                        }
                        else if (Pressed({ 13 }))
                            optionOver = true;
                        GoTo(CurrLine, 20);
                        ReturnCursor();
                    }

                }

            }
            else if (Lines[CurrLine] == "Speed") {
                GoTo(CurrLine, 20);
                ReturnCursor();
                bool optionOver = false;
                while (!optionOver) {
                    if (_kbhit()) {
                        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                        if (Clicked({ myUp, myRight, VK_UP, VK_RIGHT })) {
                            speed = min(speed + 1, 10);
                            GoTo(CurrLine, 20, to_string(speed));
                            PrintSpaces();
                            GoTo(CurrLine, 20);
                        }
                        else if (Clicked({ myDown, myLeft, VK_DOWN, VK_LEFT })) {
                            speed = max(speed - 1, 1);
                            GoTo(CurrLine, 20, to_string(speed));
                            PrintSpaces();
                            GoTo(CurrLine, 20);
                        }
                        else if (Pressed({ 13 })) {
                            optionOver = true;

                        }
                    }
                }
            }
            else if (Lines[CurrLine] == "Acceleration") {
                GoTo(CurrLine, 20);
                ReturnCursor();
                bool optionOver = false;
                while (!optionOver) {
                    if (_kbhit()) {
                        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                        if (Clicked({ myLeft, myRight, VK_LEFT, VK_RIGHT })) {
                            acceleration = !acceleration;
                            bDraw(acceleration);
                            PrintSpaces();
                        }
                        else if (Pressed({ 13 }))
                            optionOver = true;
                        GoTo(CurrLine, 20);
                        ReturnCursor();
                    }

                }
            }
            else if (find((d = { "UP", "DOWN", "LEFT", "RIGHT" }).begin(), d.end(), (l = Lines[CurrLine])) != d.end()) {
                GoTo(CurrLine, 20);
                //PrintSpaces();
                //GoTo(CurrLine, 20);
                ReturnCursor();
                INPUT_RECORD inputRecord;
                memset(&inputRecord, 0, sizeof(INPUT_RECORD));
                DWORD eventsRead;
                int vk; WCHAR vsc;
                HANDLE hConsoleInp = GetStdHandle(STD_INPUT_HANDLE);
                while (true) {
                    DWORD readCode = ReadConsoleInput(hConsoleInp, &inputRecord, sizeof(INPUT_RECORD), &eventsRead);
                    if (!readCode) {
                        DWORD errorCode = GetLastError();
                        PrintConsoleInputError(errorCode);

                    }
                    if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
                        vk = inputRecord.Event.KeyEvent.wVirtualKeyCode;
                        vsc = inputRecord.Event.KeyEvent.uChar.UnicodeChar;
                        if (vk == 13) {
                            break;
                        }
                        if (l == "UP") {
                            myUp = vk;
                            myUpVsc = vsc;
                        }
                        else if (l == "DOWN") {
                            myDown = vk;
                            myDownVsc = vsc;
                        }
                        else if (l == "LEFT") {
                            myLeft = vk;
                            myLeftVsc = vsc;
                        }
                        else if (l == "RIGHT") {
                            myRight = vk;  // Можно заменить строки map на ссылки на нужные переменные, это сократит данный блок до одной строчки.
                            myRightVsc = vsc;
                        }
                        int modeCode = _setmode(_fileno(stdout), _O_U16TEXT); // Установка режима вывода в юникоде
                        wcout << vsc << L" (" << vk << L")"; // Преобразование типа WCHAR в wchar_t
                        modeCode = _setmode(_fileno(stdout), _O_TEXT);
                        PrintSpaces(); // Где-то утечка памяти. Возможно тут?
                        break;
                    }
                }
            }
            else if (Lines[CurrLine] == "Start") {
                menuOver = true;
                Page = "Game";
            }
        }

    }
}
void MenuCycle() {
    menuOver = false;
    DrawMenu();
    while(!menuOver){
        MenuInput();
        CheckCursor();
    }

}