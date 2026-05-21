// mainwindow.h
#pragma once
#include <QMainWindow>
#include <QGridLayout>
#include <QStackedWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include "minesweeperboard.h"
#include "minesweeperbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLeftClick(int r, int c);
    void handleRightClick(int r, int c);
    void startNewGame(int rows, int cols, int mines);
    void returnToMenu();

private:
    Ui::MainWindow *ui;
    MinesweeperBoard *board;
    bool firstClick;

    QStackedWidget *stackedWidget;
    QWidget *menuWidget;

    // Gameplay layout adjustments
    QWidget *gameWidget;
    QScrollArea *scrollArea;
    QWidget *scrollContainer;
    QGridLayout *gridLayout;

    std::vector<std::vector<MinesweeperButton*>> buttons;

    void createMainMenu();
    void updateUI();
    void revealAllMines();
};