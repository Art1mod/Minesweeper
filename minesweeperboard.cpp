#include "minesweeperboard.h"
#include <cstdlib>
#include <ctime>

MinesweeperBoard::MinesweeperBoard(int r, int c, int mines)
    : rows(r), cols(c), totalMines(mines), gameOver(false), gameWon(false) {
    // Fill the grid with default hidden cells
    grid.resize(rows, std::vector<Cell>(cols));
    std::srand(std::time(nullptr)); // Seed the random number generator
}

bool MinesweeperBoard::isValid(int r, int c) const {
    return (r >= 0 && r < rows && c >= 0 && c < cols);
}

void MinesweeperBoard::initializeBoard(int startRow, int startCol) {
    plantMines(startRow, startCol);
    calculateNeighbors();
}

void MinesweeperBoard::plantMines(int startRow, int startCol) {
    int planted = 0;
    while (planted < totalMines) {
        int r = std::rand() % rows;
        int c = std::rand() % cols;

        // Calculate horizontal and vertical distance from the initial click
        int rowDist = std::abs(r - startRow);
        int colDist = std::abs(c - startCol);

        // A tile is inside the 3x3 safe zone if it is 0 or 1 tile away from the click
        bool inSafeZone = (rowDist <= 1 && colDist <= 1);

        // Only place a mine if it's an empty cell AND outside the entire 3x3 safe zone
        if (!grid[r][c].isMine && !inSafeZone) {
            grid[r][c].isMine = true;
            planted++;
        }
    }
}

void MinesweeperBoard::calculateNeighbors() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c].isMine) continue;

            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (isValid(r + dr, c + dc) && grid[r + dr][c + dc].isMine) {
                        count++;
                    }
                }
            }
            grid[r][c].neighborMines = count;
        }
    }
}

void MinesweeperBoard::revealCell(int r, int c) {
    if (!isValid(r, c) || grid[r][c].isRevealed || grid[r][c].isFlagged || gameOver) {
        return;
    }

    grid[r][c].isRevealed = true;

    // Hit a mine! Game Over.
    if (grid[r][c].isMine) {
        gameOver = true;
        return;
    }

    // Flood-Fill: If it's a 0 cell, automatically reveal neighbors
    if (grid[r][c].neighborMines == 0) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                revealCell(r + dr, c + dc); // Recursive call
            }
        }
    }

    checkWinCondition();
}

void MinesweeperBoard::toggleFlag(int r, int c) {
    if (!isValid(r, c) || grid[r][c].isRevealed || gameOver) return;
    grid[r][c].isFlagged = !grid[r][c].isFlagged;
}

void MinesweeperBoard::checkWinCondition() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // If there's a non-mine cell that hasn't been revealed yet, game isn't won.
            if (!grid[r][c].isMine && !grid[r][c].isRevealed) {
                return;
            }
        }
    }
    gameWon = true;
}
