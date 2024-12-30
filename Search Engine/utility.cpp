#include "utility.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <algorithm>
using namespace std;

void highlightWord(const std::string& line, const std::vector<std::string>& includeWords) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    size_t start = 0;

    while (start < line.size()) {
        size_t nextPos = std::string::npos;
        std::string matchedWord;

        for (const auto& word : includeWords) {
            size_t pos = line.find(word, start);
            if (pos != std::string::npos && (nextPos == std::string::npos || pos < nextPos)) {
                nextPos = pos;
                matchedWord = word;
            }
        }

        if (nextPos == std::string::npos) {
            std::cout << line.substr(start);
            break;
        }

        std::cout << line.substr(start, nextPos - start);

        SetClr(BLACK, CYAN);
        std::cout << line.substr(nextPos, matchedWord.length());
        SetClr(WHITE, BLACK);

        start = nextPos + matchedWord.length();
    }
}

void SetClr(int tcl, int bcl)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

bool isSingleWord(const std::string& input) {
    std::string trimmed = trim(input);
    for (char c : input)
        if (isspace(c))
            return false;
    return true;
}


void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}
void setConsoleWindowSize(int width, int height) {
    // Get the handle to the console window
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to get console handle!" << std::endl;
        return;
    }

    // Calculate the desired window size
    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = width - 1;  // Right boundary
    windowSize.Bottom = height - 1; // Bottom boundary

    // Set the console window size
    if (!SetConsoleWindowInfo(hConsole, TRUE, &windowSize)) {
        std::cerr << "Error: Unable to set console window size!" << std::endl;
        return;
    }
}

void Title()
{
    const char* text_pic = R"===(
                             _____                      __       ______            _          
                            / ___/___  ____ ___________/ /_     / ____/___  ____ _(_)___  ___ 
                            \__ \/ _ \/ __ `/ ___/ ___/ __ \   / __/ / __ \/ __ `/ / __ \/ _ \
                           ___/ /  __/ /_/ / /  / /__/ / / /  / /___/ / / / /_/ / / / / /  __/
                          /____/\___/\__,_/_/   \___/_/ /_/  /_____/_/ /_/\__, /_/_/ /_/\___/ 
                                                                         /____/               
    )===";
    cout << text_pic;
}