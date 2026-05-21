#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>
#include "Config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), board(nullptr), firstClick(true) {
    ui->setupUi(this);

    // Apply absolute fixed dimensions directly from Config
    this->setFixedSize(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);

    // Inject the centralized styles decoupled from this source code
    this->setStyleSheet(Config::MAIN_STYLE_SHEET);

    // Initialize layout containers
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    menuWidget = new QWidget(this);
    createMainMenu();
    stackedWidget->addWidget(menuWidget); // Index 0

    gameWidget = new QWidget(this);
    QVBoxLayout *gameLayout = new QVBoxLayout(gameWidget);

    QPushButton *backBtn = new QPushButton("Back to Main Menu", gameWidget);
    backBtn->setFixedHeight(35);
    gameLayout->addWidget(backBtn);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::returnToMenu);

    scrollArea = new QScrollArea(gameWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    gameLayout->addWidget(scrollArea);

    scrollContainer = new QWidget(scrollArea);
    scrollContainer->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    gridLayout = new QGridLayout(scrollContainer);
    gridLayout->setSpacing(1); // Keeps tiles closely packed
    gridLayout->setContentsMargins(0, 0, 0, 0);
    scrollArea->setWidget(scrollContainer);

    stackedWidget->addWidget(gameWidget); // Index 1
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
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; margin-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    menuLayout->addWidget(titleLabel);

    // --- DYNAMICALLY CONSTRUCT BUTTONS VIA CONFIG DATA ---
    auto makeMenuButton = [this, menuLayout](const Config::Difficulty& diff) {
        // Formats string automatically like: "Easy (10x10, 10 Mines)"
        QString label = QString("%1 (%2x%3, %4 Mines)")
                            .arg(diff.name)
                            .arg(diff.rows)
                            .arg(diff.cols)
                            .arg(diff.mines);

        QPushButton *btn = new QPushButton(label, menuWidget);
        btn->setFixedSize(250, 45);
        menuLayout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [this, diff]() {
            startNewGame(diff.rows, diff.cols, diff.mines);
        });
    };

    // Instantiate buttons strictly bound to Config variables
    makeMenuButton(Config::EASY);
    makeMenuButton(Config::MEDIUM);
    makeMenuButton(Config::HARD);
}

void MainWindow::startNewGame(int rows, int cols, int mines) {
    if (board != nullptr) {
        delete board;
        board = nullptr;
    }

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
                    btn->setStyleSheet("background-color: #fed7d7; color: #c53030;");
                } else if (cell.neighborMines > 0) {
                    btn->setText(QString::number(cell.neighborMines));

                    if (cell.neighborMines == 1)
                        btn->setStyleSheet("color: #3182ce; background-color: #e2e8f0; font-weight: bold;");
                    else if (cell.neighborMines == 2)
                        btn->setStyleSheet("color: #38a169; background-color: #e2e8f0; font-weight: bold;");
                    else if (cell.neighborMines == 3)
                        btn->setStyleSheet("color: #e53e3e; background-color: #e2e8f0; font-weight: bold;");
                    else
                        btn->setStyleSheet("color: #dd6b20; background-color: #e2e8f0; font-weight: bold;");
                } else {
                    btn->setText("");
                    btn->setStyleSheet("background-color: #e2e8f0;");
                }
            } else {
                if (cell.isFlagged) {
                    btn->setText("🚩");
                    btn->setStyleSheet("color: #e53e3e; font-size: 16px; background-color: #cbd5e0;");
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
