#include "Assurance.h"
#include "PosControls.h"

bool assOver;
int YesLine, NoLine;

void DrawAss() {
    system("cls");
    cout << WHITE << "Are you sure you want to exit the game?\n" << endl
        << ">" << RED << " Yes";
    YesLine = GetPos().Y;
    cout << endl
        << "  No" << WHITE;
    NoLine = GetPos().Y;
    CurrLine = YesLine;
}
void AssInput() { // it's not about your dick
    int mask = 1 << 15;
    if (Clicked({ myUp, myDown, VK_UP, VK_DOWN })) {
        GoTo(CurrLine, 0, " ");
        CurrLine = CurrLine ^ YesLine ^ NoLine;
        GoTo(CurrLine, 0, ">");
    }
    else if ( Clicked({ 13 }) ) { // Enter
        if (CurrLine == YesLine) {
            assOver = true;
            exitGame = true;
        }
        else if (CurrLine == NoLine) {
            assOver = true;
            Page = PrevPage;
        }
    }
}
void AssCycle() {
    assOver = false;
    DrawAss();
    while (!assOver) {
        AssInput();
        CheckCursor();

    }
}