#include "minesweeperbutton.h"

MinesweeperButton::MinesweeperButton(int r, int c, QWidget *parent)
    : QPushButton(parent), row(r), col(c) {
    setFixedSize(35, 35);
}

void MinesweeperButton::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        emit rightClicked(row, col);
    } else if (e->button() == Qt::LeftButton) {
        emit leftClicked(row, col);
    }
}
