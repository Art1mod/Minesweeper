// Cell.h
#pragma once

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int neighborMines = 0; // Number of mines surrounding this cell (0 to 8)
};
