#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), firstClick(true) {
    ui->setupUi(this);

    board = new MinesweeperBoard(10, 10, 10); // Create a 10x10 board with 10 mines

    // Set up a container widget to hold grid layout
    QWidget *centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(2); // Pixel space between buttons

    buttons.resize(board->getRows(), std::vector<MinesweeperButton*>(board->getCols()));

    // Populate the visual grid with buttons
    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            MinesweeperButton *btn = new MinesweeperButton(r, c, this);
            buttons[r][c] = btn;
            gridLayout->addWidget(btn, r, c);

            // Connect button signals to MainWindow handler functions
            connect(btn, &MinesweeperButton::leftClicked, this, &MainWindow::handleLeftClick);
            connect(btn, &MinesweeperButton::rightClicked, this, &MainWindow::handleRightClick);
        }
    }

    setCentralWidget(centralWidget);
    updateUI();
}

MainWindow::~MainWindow() {
    delete ui;
    delete board;
}

void MainWindow::handleLeftClick(int r, int c) {
    if (board->isGameOver() || board->isGameWon()) return;

    // First click trick: Initialize board layout around this click coordinate
    if (firstClick) {
        board->initializeBoard(r, c);
        firstClick = false;
    }

    board->revealCell(r, c);
    updateUI();

    if (board->isGameOver()) {
        revealAllMines();
        QMessageBox::critical(this, "Game Over", "Boom! You hit a mine.");
    } else if (board->isGameWon()) {
        QMessageBox::information(this, "Congratulations", "You cleared the field!");
    }
}

void MainWindow::handleRightClick(int r, int c) {
    if (board->isGameOver() || board->isGameWon()) return;

    board->toggleFlag(r, c);
    updateUI();
}

// Read data from Backend (Model) and push changes to Frontend UI (View)
void MainWindow::updateUI() {
    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            const Cell& cell = board->getCell(r, c);
            MinesweeperButton *btn = buttons[r][c];

            if (cell.isRevealed) {
                btn->setEnabled(false); // Can't click anymore
                if (cell.isMine) {
                    btn->setText("💣");
                    btn->setStyleSheet("background-color: red;");
                } else if (cell.neighborMines > 0) {
                    btn->setText(QString::number(cell.neighborMines));
                    // Style number colors like classic Minesweeper
                    if(cell.neighborMines == 1) btn->setStyleSheet("color: blue; font-weight: bold;");
                    else if(cell.neighborMines == 2) btn->setStyleSheet("color: green; font-weight: bold;");
                    else btn->setStyleSheet("color: red; font-weight: bold;");
                } else {
                    btn->setText(""); // Empty space
                    btn->setStyleSheet("background-color: #d3d3d3;"); // light gray
                }
            } else {
                // Hidden cells
                if (cell.isFlagged) {
                    btn->setText("🚩");
                    btn->setStyleSheet("color: red;");
                } else {
                    btn->setText("");
                    btn->setStyleSheet(""); // default native look
                }
            }
        }
    }
}

void MainWindow::revealAllMines() {
    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            if (board->getCell(r, c).isMine) {
                buttons[r][c]->setText("💣");
            }
        }
    }
}
