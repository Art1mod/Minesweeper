#pragma once
#include <QMainWindow>
#include <QGridLayout>
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

private:
    Ui::MainWindow *ui;
    MinesweeperBoard *board;
    bool firstClick;

    // A 2D tracking matrix of visual UI buttons
    std::vector<std::vector<MinesweeperButton*>> buttons;
    QGridLayout *gridLayout;

    void updateUI();
    void revealAllMines();
};
