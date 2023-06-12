#pragma once
#include <Windows.h>
// Для других файлов
#include <iostream>
#include <conio.h>
#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define WHITE   "\033[1;37m"
#define GREY "\033[1;30m"
using namespace std;

extern int width, height;
struct frm { int left, right, up, down; };
extern frm bond;
extern string Page, PrevPage;
extern int CurrLine;
extern bool exitGame;
extern HANDLE console;
extern CONSOLE_CURSOR_INFO cursorInfo;
extern int myUp, myDown, myLeft, myRight;
extern int myUpVsc, myDownVsc, myLeftVsc, myRightVsc;


//struct cntrls { string LEFT, RIGHT, UP, DOWN };
//struct opt { int width, height; bool walls, acceleration; int speed; cntrl controls };
int IntInput();
void PrintConsoleInputError(DWORD errorCode);
pair <int, WCHAR> GetKeyCode();
void PrintSpaces();
void FillConsoleLineWithSpaces();
bool WindowResized();
void CheckCursor();
void HideCursor();
void ReturnCursor();
bool IsNum(const string& str);
COORD GetPos();
void SetConsoleFont(const wchar_t* fontName, int fontSize);
bool Pressed(initializer_list <int> keys);
bool Clicked(initializer_list <int> keys);
void GoTo(int i, int j);
void GoTo(int i, int j, string text);
void GoTo(int i, int j, const string & color, string text); 
void GoToFrmMdl(frm boundaries);
void GoToFrmMdl(frm boundaries, string text);
void GoToFrmEnd(int downBoundary);
void GoToFrmEnd(int downBoundary, string text);
void GoToWndEnd();
void GoToWndEnd(string text);
