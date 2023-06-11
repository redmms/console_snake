#include "Game.h"
#include "PosControls.h"
#include <list>
#include <string>
#include <tuple>

#define KEY_DOWN(inputRecord) \
    (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
#define VSC inputRecord.Event.KeyEvent.uChar.UnicodeChar
#define VK inputRecord.Event.KeyEvent.wVirtualKeyCode


bool gameOver, inputOver;
extern bool isSaved = false;
int x, y, fruitX, fruitY;
extern int score;
int stepTime;
list <int> snakeX, snakeY;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir, prevDir;

//extern int speed;
//extern bool walls;
//extern bool acceleration;
bool walls = true;
bool acceleration = true;
int speed = 7;


void DrawWholeSnake() {
    auto itX = snakeX.begin(), itY = snakeY.begin();
    GoTo(*itY, *itX, GREEN, "0");
    for (itX++, itY++; itX != snakeX.end(); itX++, itY++) {
        GoTo(*itY, *itX, GREEN, "o");
    }
    GoTo(fruitY, fruitX, PURPLE, "F");
}
void Setup() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    if (!isSaved || PrevPage == "Menu") { // когда добавятся опции в меню, надо добавить проверку что настройки сохранены
        srand(time(NULL));
        if (acceleration) {
            stepTime = 500;
        }
        else {
            stepTime = (11 - speed) * 50;
        }
        if (stepTime <= 0) {
            cout << "StepTime is incorrect:";
            system("pause");
        }
        dir = STOP;
        x = width / 2;
        y = height / 2;
        snakeX.clear();
        snakeY.clear();
        snakeX.push_front(x);
        snakeY.push_front(y);
        fruitX = 1 + rand() % width;  // 1 <= x <= width
        fruitY = 1 + rand() % height; // 1 <= y <= height
        score = 0;
    }
}
void DrawFrame()
{
    system("cls");
    cout << RED;
    // первая строка:
    for (int j = -2; j < width; j++){
        cout << "#";
    }
    cout << '\n';

    // строки посередине:
    for (int i = 1; i <= height; i++){
        cout << "#";
        GoTo(i, bond.right);
        cout << "#\n";
    }

    // последняя строка: 
    for (int j = -2; j < width; j++){
        cout << "#";
    }
    cout << "\nScore: "
         << WHITE 
         << "\nUse arrows to move, ESC to stop, any other key to pause";
}
void DrawScore(){
    GoTo(bond.down + 1, 7, WHITE, to_string(score));
}
void EraseFruit(){ // Не используется так как вместо фрукта рисуется голова змеи should be called before DrawFruit(), потому что последняя меняет fruitY и fruitX
    GoTo(fruitY, fruitX, " ");
}
void DrawFruit(){
    GoTo(fruitY, fruitX, PURPLE, "F");
}

void Input() {
    // direction control
    static HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    static DWORD numberOfEvents = 0;
    GetNumberOfConsoleInputEvents(hConsoleInput, &numberOfEvents);
    if (numberOfEvents != 0) {
        static INPUT_RECORD inputRecord;
        static DWORD eventsRead;
        ReadConsoleInput(hConsoleInput, &inputRecord, 1, &eventsRead);
        if (KEY_DOWN(inputRecord)) {
            if (VK == VK_ESCAPE) {
                gameOver = true;
                isSaved = true;
                Page = "Assurance";
            }
            else if (snakeX.size() == 1)
            {
                if (VK == myLeft || VK == VK_LEFT) {
                    dir = LEFT;
                }
                else if (VK == myRight || VK == VK_RIGHT) {
                    dir = RIGHT;
                }
                else if (VK == myUp || VK == VK_UP) {
                    dir = UP;
                }
                else if (VK == myDown || VK == VK_DOWN) {
                    dir = DOWN;
                }
                else {
                    gameOver = true;
                    isSaved = true;
                    Page = "Pause";
                }
            }
            else
            {
                if (VK == myLeft || VK == VK_LEFT) {
                    if (prevDir != RIGHT)
                        dir = LEFT;
                }
                else if (VK == myRight || VK == VK_RIGHT) {
                    if (prevDir != LEFT)
                        dir = RIGHT;
                }
                else if (VK == myUp || VK == VK_UP) {
                    if (prevDir != DOWN)
                        dir = UP;
                }
                else if (VK == myDown || VK == VK_DOWN) {
                    if (prevDir != UP)
                        dir = DOWN;
                }
                else {
                    gameOver = true;
                    isSaved = true;
                    Page = "Pause";
                }
            }
        }
    }
    
}
int Logic() {

    // Реакция на ввод
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    if (walls) {
        // Столкновение с границами
        if (x >= bond.right || x <= bond.left || y <= bond.up || y >= bond.down) {
            gameOver = true;
            isSaved = false;
            return 0;
        }
    }
    else {
        // Телепорт
        if (x == bond.right)
            x = bond.left + 1;
        else if (x == bond.left)
            x = bond.right - 1;
        else if (y == bond.up)
            y = bond.down - 1;
        else if (y == bond.down)
            y = bond.up + 1;
    }

    // Проверка запутывания, должна срабатывать только при движении
    for (auto ix = snakeX.begin(), iy = snakeY.begin(); ix != snakeX.end(); ix++, iy++) {
        if (*ix == x && *iy == y) {
            gameOver = true;
            isSaved = false;
            return 0;
        }
    }

    // Движение змеи
// Если съели фрукт, хвост остается на месте старая голова заменяется на "o", 
// новая пишется в новых x, y, которые будут известны следующим шагом, взависимости от того поменяется направление 
// или останется прежним, только первые два  GoTo. Если холостое движение, то хвост удаляется, голова на новых x, y, текущих, а настарой голове "o"
    snakeX.push_front(x);
    snakeY.push_front(y);
    GoTo(y, x, GREEN, "0");
    if (x != fruitX || y != fruitY) {
        int endX = *--snakeX.end(), endY = *--snakeY.end();
        if (endX != fruitX || endY != fruitY)
            GoTo(endY, endX, " ");
        else
            GoTo(endY, endX, PURPLE, "F");
// Фрукт никогда не должен перекрываться, если это тело змеи, то не сташно, она все равно рисуется до фрукта,
// если голова - тоже, даже если новый фрукт появится на месте головы, новая голова уже будет в следующей ячейке, 
// если появится на месте новой головы - тоже, голова отрисуется заново, но если на месте второй ячейки змеи либо
// последней (кончик хвоста), то могут возникнуть проблемы и без дополнительной проверки не обойтись
        snakeX.pop_back();
        snakeY.pop_back();
    }
    if (snakeX.size() > 1) {
        int secX = *++snakeX.begin(), secY = *++snakeY.begin();
        if (secX != fruitX || secY != fruitY)
            GoTo(secY, secX, GREEN, "o");
        else
            GoTo(secY, secX, PURPLE, "F");
    }


    // Перемещение фрукта на новое место и другие события когда съели фрукт
    if (x == fruitX && y == fruitY) {
        score += 10;
        if(acceleration)
            stepTime = max(stepTime - 20, 50);
        //int randX = rand() % int(snakeX.size());
        //fruitX = *next(snakeX.begin(), randX);
        //int randY = rand() % int(snakeY.size());
        //fruitY = *next(snakeY.begin(), randY);
        //fruitY = *--snakeY.end();
        fruitX = 1 + rand() % width;  // 1 <= x <= width
        fruitY = 1 + rand() % height; // 1 <= y <= height
        DrawFruit();
        DrawScore();
    }
    return 0;
}
void GameCycle() {

    gameOver = false;
    Page = "After";
    Setup();
    DrawFrame();
    DrawScore();
    DrawFruit();
    if (isSaved)
        DrawWholeSnake();
    else
        GoTo(y, x, GREEN, "0");
    while (dir == STOP && !gameOver) {
        Input();
        CheckCursor();
    }
    GoToFrmEnd(bond.down);
    FillConsoleLineWithSpaces();
    GoTo(bond.down + 3, 0);
    FillConsoleLineWithSpaces();
    while (!gameOver) {
        prevDir = dir;
        int startTime = clock();
        while (clock() - startTime < stepTime)
            Input();
        Logic();
        CheckCursor();
    }
    printf("%s", WHITE);
}

// С подсчетом времени змейка может умудриться съесть саму себя при вращении даже будучи два элемента
// в длину, но при этом, при повторе игры длина становится на единицу больше...
// И когда змея съела свой первый фрукт, голова прорисовывается с задержкой.