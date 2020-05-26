#ifndef SUDOKUGUI_H
#define SUDOKUGUI_H

#include <QWidget>
#include <QLabel>
#include <QLine>
#include <QLineEdit>
#include <QSizePolicy>

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
    void updateNumberSquare();

private:
    Ui::SudokuGui *ui;
    void generateLabels();
    QLineEdit** lineEditsArray;
    SudokuBoard board;
};

#endif // SUDOKUGUI_H
