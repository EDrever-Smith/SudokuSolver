#ifndef SUDOKUGUI_H
#define SUDOKUGUI_H

#include <QWidget>
#include <QLineEdit>
#include <QButtonGroup>
#include <QPushButton>

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
    void handleModeSelect(int modeSelected);

private:
    Ui::SudokuGui *ui;
    void generateLabels();
    QLineEdit** lineEditsArray;
    SudokuBoard board;
    QButtonGroup* modeButtons;
    QPushButton* manualInputButton;
    QPushButton* videoInputButton;
    QPushButton* pictureInputButton;
    QPushButton* generateGameBoardButton;
};

#endif // SUDOKUGUI_H
