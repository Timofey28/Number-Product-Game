#pragma once

enum colors {
    RED = 4,
    ORANGE = 6,
    HELP = 7,
    GRAY = 8,
    GREEN = 10,
    DARK_GREEN = 2,
    SEA_WAVE = 11,
    LIGHT_RED = 12,
    BEIGE = 14,
    WHITE = 15
};

struct Position
{
    int x, y;
    Position(int x_ = -1, int y_ = -1) : x(x_), y(y_) {}
    Position(const Position& otherPos) : x(otherPos.x), y(otherPos.y) {}
};

void setPosition(const Position& position)
{
    static COORD coord;
    coord.X = position.x;
    coord.Y = position.y;
    if(!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord)) {
        std::cout << GetLastError();
        exit(-1);
    }
}

void setPosition(int x, int y)
{
    static COORD coord;
    coord.X = x;
    coord.Y = y;
    if(!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord)) {
        std::cout << GetLastError();
        exit(-1);
    }
}

void setColor(int color)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD) color);
}
