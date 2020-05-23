#ifndef SUDOKUGUI_H
#define SUDOKUGUI_H

#include <QWidget>

namespace Ui {
class SudokuGui;
}

class SudokuGui : public QWidget
{
    Q_OBJECT

public:
    explicit SudokuGui(QWidget *parent = nullptr);
    ~SudokuGui();

private:
    Ui::SudokuGui *ui;
};

#endif // SUDOKUGUI_H
