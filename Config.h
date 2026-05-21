// Config.h
#pragma once
#include <QString>

namespace Config {
// --- Window and Layout Sizing ---
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 30; // Pixel size for each square grid tile

// --- Difficulty Struct Blueprint ---
struct Difficulty {
    QString name;
    int rows;
    int cols;
    int mines;
};

// --- Difficulty Levels Configuration ---
const Difficulty EASY   = { "Easy",   10, 10, 10 };
const Difficulty MEDIUM = { "Medium", 16, 16, 40 };
const Difficulty HARD   = { "Hard",   20, 20, 85 };

// --- Centralized Theme Stylesheet ---
const QString MAIN_STYLE_SHEET = R"(
        QMainWindow {
            background-color: #f4f5f7;
        }
        QStackedWidget, QScrollArea {
            background-color: #f4f5f7;
            border: none;
        }
        QWidget#scrollContainer {
            background-color: #ffffff;
        }
        QLabel {
            font-family: 'Segoe UI', -apple-system, sans-serif;
            color: #4a5568;
            font-weight: 500;
            letter-spacing: 1px;
        }
        QPushButton {
            background-color: #ffffff;
            color: #4a5568;
            font-family: 'Segoe UI', -apple-system, sans-serif;
            font-size: 14px;
            font-weight: 600;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #edf2f7;
            color: #2d3748;
        }
        QPushButton:pressed {
            background-color: #e2e8f0;
        }
        MinesweeperButton {
            background-color: #cbd5e0;
            border: none;
            border-radius: 0px;
            font-family: 'Segoe UI', -apple-system, sans-serif;
            font-size: 16px;
            font-weight: bold;
        }
        MinesweeperButton:hover {
            background-color: #b8c2cc;
        }
        MinesweeperButton:disabled {
            background-color: #e2e8f0;
        }
    )";
}
