#include "After.h"
#include "PosControls.h"

bool afterOver = false;
int RepLine, RetLine;
int score;

void DrawAfter() {
    system("cls");
    printf("%sGame Over. Your score: %d\n\n>%s", WHITE, score, RED);
    printf(" Repeat the game");
    RepLine = GetPos().Y;
    printf("\n  Return to main menu");
    RetLine = GetPos().Y;
    CurrLine = RepLine;
    printf(WHITE);
}
void AfterInput() {
    if (Pressed({VK_ESCAPE})) {
        afterOver = true;
        Page = "Assurance";
    }
    else if (Clicked({ myUp, myDown, VK_UP, VK_DOWN }) ){
        GoTo(CurrLine, 0, " ");
        CurrLine = CurrLine ^ RepLine ^ RetLine;
        GoTo(CurrLine, 0, ">");
    }
    else if (Clicked({ 13 }) ) { // Enter
        if (CurrLine == RepLine) {
            afterOver = true;
            Page = "Game";
        }
        else if (CurrLine == RetLine) {
            afterOver = true;
            Page = "Menu";
        }
    }
}
void AfterCycle() {
    afterOver = false;
    DrawAfter();
    while (!afterOver) {
        AfterInput();
        CheckCursor();

    }
}