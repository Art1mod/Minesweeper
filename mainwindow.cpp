#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), board(nullptr), firstClick(true) {
    ui->setupUi(this);

    // CRITICAL: Force the window to be one absolute, unchangeable size!
    // 750 pixels wide, 700 pixels tall works beautifully for all formats.
    this->setFixedSize(750, 700);

    // Initialize master stacked layout container
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Set up the menu page
    menuWidget = new QWidget(this);
    createMainMenu();
    stackedWidget->addWidget(menuWidget); // Index 0

    // Set up the gameplay page framework
    gameWidget = new QWidget(this);
    QVBoxLayout *gameLayout = new QVBoxLayout(gameWidget);

    // Create a return button so players can get back to the menu
    QPushButton *backBtn = new QPushButton("Back to Main Menu", gameWidget);
    backBtn->setFixedHeight(35);
    gameLayout->addWidget(backBtn);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::returnToMenu);

    // Create the Scroll Area to house our massive grid grids safely
    scrollArea = new QScrollArea(gameWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    gameLayout->addWidget(scrollArea);

    // The container widget inside the scroll area that physically holds the grid
    scrollContainer = new QWidget(scrollArea);
    gridLayout = new QGridLayout(scrollContainer);
    gridLayout->setSpacing(2);
    scrollArea->setWidget(scrollContainer);

    stackedWidget->addWidget(gameWidget); // Index 1

    // Show menu first
    stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
    delete ui;
    if (board) delete board;
}

void MainWindow::createMainMenu() {
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setAlignment(Qt::AlignCenter);
    menuLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("MINESWEEPER", menuWidget);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #333; margin-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    menuLayout->addWidget(titleLabel);

    QPushButton *easyBtn = new QPushButton("Easy (10x10, 10 Mines)", menuWidget);
    QPushButton *mediumBtn = new QPushButton("Medium (16x16, 40 Mines)", menuWidget);
    QPushButton *hardBtn = new QPushButton("Hard (20x20, 85 Mines)", menuWidget);

    easyBtn->setFixedSize(250, 45);
    mediumBtn->setFixedSize(250, 45);
    hardBtn->setFixedSize(250, 45);

    menuLayout->addWidget(easyBtn);
    menuLayout->addWidget(mediumBtn);
    menuLayout->addWidget(hardBtn);

    connect(easyBtn, &QPushButton::clicked, this, [this]() { startNewGame(10, 10, 10); });
    connect(mediumBtn, &QPushButton::clicked, this, [this]() { startNewGame(16, 16, 40); });
    connect(hardBtn, &QPushButton::clicked, this, [this]() { startNewGame(20, 20, 85); });
}

void MainWindow::startNewGame(int rows, int cols, int mines) {
    if (board != nullptr) {
        delete board;
        board = nullptr;
    }

    // Safely clear old buttons out of the grid layout
    if (gridLayout != nullptr) {
        QLayoutItem *item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            if (QWidget *widget = item->widget()) {
                widget->hide();
                widget->setParent(nullptr);
                delete widget;
            }
            delete item;
        }
    }
    buttons.clear();

    board = new MinesweeperBoard(rows, cols, mines);
    firstClick = true;

    // Build grid inside our scrollable container
    buttons.resize(rows, std::vector<MinesweeperButton*>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            MinesweeperButton *btn = new MinesweeperButton(r, c, scrollContainer);
            buttons[r][c] = btn;
            gridLayout->addWidget(btn, r, c);

            connect(btn, &MinesweeperButton::leftClicked, this, &MainWindow::handleLeftClick);
            connect(btn, &MinesweeperButton::rightClicked, this, &MainWindow::handleRightClick);
        }
    }

    updateUI();
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::returnToMenu() {
    stackedWidget->setCurrentIndex(0);
}

/* ================= CORE LOGIC LOOP ================= */

void MainWindow::handleLeftClick(int r, int c) {
    if (!board || board->isGameOver() || board->isGameWon()) return;

    if (firstClick) {
        board->initializeBoard(r, c);
        firstClick = false;
    }

    board->revealCell(r, c);
    updateUI();

    if (board->isGameOver()) {
        revealAllMines();
        QMessageBox::critical(this, "Game Over", "Boom! You hit a mine.");
        returnToMenu();
    } else if (board->isGameWon()) {
        QMessageBox::information(this, "Congratulations", "You cleared the field!");
        returnToMenu();
    }
}

void MainWindow::handleRightClick(int r, int c) {
    if (!board || board->isGameOver() || board->isGameWon()) return;
    board->toggleFlag(r, c);
    updateUI();
}

void MainWindow::updateUI() {
    if (board == nullptr) return;

    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            const Cell& cell = board->getCell(r, c);
            MinesweeperButton *btn = buttons[r][c];

            if (cell.isRevealed) {
                btn->setEnabled(false);
                if (cell.isMine) {
                    btn->setText("💣");
                    btn->setStyleSheet("background-color: red;");
                } else if (cell.neighborMines > 0) {
                    btn->setText(QString::number(cell.neighborMines));
                    if(cell.neighborMines == 1) btn->setStyleSheet("color: blue; font-weight: bold; background-color: #d3d3d3;");
                    else if(cell.neighborMines == 2) btn->setStyleSheet("color: green; font-weight: bold; background-color: #d3d3d3;");
                    else btn->setStyleSheet("color: red; font-weight: bold; background-color: #d3d3d3;");
                } else {
                    btn->setText("");
                    btn->setStyleSheet("background-color: #d3d3d3;");
                }
            } else {
                if (cell.isFlagged) {
                    btn->setText("🚩");
                    btn->setStyleSheet("color: red;");
                } else {
                    btn->setText("");
                    btn->setStyleSheet("");
                }
            }
        }
    }
}

void MainWindow::revealAllMines() {
    if (board == nullptr) return;
    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            if (board->getCell(r, c).isMine) {
                buttons[r][c]->setText("💣");
            }
        }
    }
}
