#include "minesweeperbutton.h"
#include "Config.h"

MinesweeperButton::MinesweeperButton(int r, int c, QWidget *parent)
    : QPushButton(parent), row(r), col(c) {
    setFixedSize(Config::TILE_SIZE, Config::TILE_SIZE);
}

void MinesweeperButton::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        emit rightClicked(row, col);
    } else if (e->button() == Qt::LeftButton) {
        emit leftClicked(row, col);
    }
}
