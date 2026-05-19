#pragma once
#include <QPushButton>
#include <QMouseEvent>

class MinesweeperButton : public QPushButton {
    Q_OBJECT
private:
    int row;
    int col;

protected:
    void mousePressEvent(QMouseEvent *e) override;

public:
    MinesweeperButton(int r, int c, QWidget *parent = nullptr);
    int getRow() const { return row; }
    int getCol() const { return col; }

signals:
    void rightClicked(int r, int c);
    void leftClicked(int r, int c);
};