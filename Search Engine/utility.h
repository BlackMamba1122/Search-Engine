#pragma once
#include<string>
#include<vector>
bool isSingleWord(const std::string& input);
std::string trim(const std::string& str);
void highlightWord(const std::string& line, const std::vector<std::string>& includeWords);
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 1286
void SetClr(int tcl, int bcl);
void hideCursor();
void showCursor();
void setConsoleWindowSize(int width, int height);
void Title();