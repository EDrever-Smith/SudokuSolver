#ifndef SUDOKUGUI_H
#define SUDOKUGUI_H

#include <QWidget>
#include <QLabel>
#include <QLine>

#include "SudokuBoard.h"

namespace Ui {
class SudokuGui;
}

class SudokuGui : public QWidget
{
    Q_OBJECT

public:
    explicit SudokuGui(QWidget *parent = nullptr);
    ~SudokuGui();
    void displaySudokuBoard(SudokuBoard& s);
public slots:
    void solveCurrentBoard();

private:
    Ui::SudokuGui *ui;
    void generateLabels();
    QLabel** labelsArray;
    SudokuBoard board;
};

#endif // SUDOKUGUI_H
