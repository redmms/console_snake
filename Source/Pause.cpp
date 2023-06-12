#include "Pause.h"
#include "PosControls.h"
bool pauseOver;
int ContLine, MenuLine;
void DrawPause() {
    system("cls");
    cout << WHITE << "You've paused the game. Continue?\n" << endl
        << ">" << RED << " Continue";
    ContLine = GetPos().Y;
    cout << endl
        << "  Go to main menu" << WHITE;
    MenuLine = GetPos().Y;
    CurrLine = ContLine;
}
void PauseInput() {
    if (Pressed({ VK_ESCAPE })) {
        pauseOver = true;
        Page = "Assurance";
    }
    else if (Clicked({ myUp, myDown, VK_UP, VK_DOWN })) {
        GoTo(CurrLine, 0, " ");
        CurrLine = CurrLine ^ ContLine ^ MenuLine;
        GoTo(CurrLine, 0, ">");
    }
    else if (Clicked({ 13 })) { // Enter
        if (CurrLine == ContLine) {
            pauseOver = true;
            Page = "Game"; // тут должен быть переход именно на сохраненную версию игры
        }
        else if (CurrLine == MenuLine) {
            pauseOver = true;
            Page = "Menu";
        }
    }
  
}
void PauseCycle() {
    pauseOver = false;
    DrawPause();
    while (!pauseOver) {
        PauseInput();
        CheckCursor();
    }
}