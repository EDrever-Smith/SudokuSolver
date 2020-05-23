#include "sudokugui.h"
#include "ui_sudokugui.h"

SudokuGui::SudokuGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SudokuGui)
{
    ui->setupUi(this);
}

SudokuGui::~SudokuGui()
{
    delete ui;
}
