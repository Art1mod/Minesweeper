#pragma once
#include <vector>
#include "Cell.h"

class MinesweeperBoard {
private:
    int rows;
    int cols;
    int totalMines;
    std::vector<std::vector<Cell>> grid;
    bool gameOver;
    bool gameWon;

    void plantMines(int startRow, int startCol);
    void calculateNeighbors();
    bool isValid(int r, int c) const;

public:
    MinesweeperBoard(int r = 10, int c = 10, int mines = 10);

    void initializeBoard(int startRow, int startCol);
    void revealCell(int r, int c);
    void toggleFlag(int r, int c);

    // Getters so the UI can read the board state safely
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    const Cell& getCell(int r, int c) const { return grid[r][c]; }
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }
    void checkWinCondition();
};
