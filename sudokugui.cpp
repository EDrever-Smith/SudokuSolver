#include "sudokugui.h"
#include "ui_sudokugui.h"
SudokuGui::SudokuGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SudokuGui)
{
    ui->setupUi(this);
    generateLabels();
    //Static array holding input sudoku puzzle to be solved. 0s represent free spaces
    int inputSudoku[81]      =   {3,0,6, 5,0,8, 4,0,0,
                                  5,2,0, 0,0,0, 0,0,0,
                                  0,8,7, 0,0,0, 0,3,1,

                                  0,0,3, 0,1,0, 0,8,0,
                                  9,0,0, 8,6,3, 0,0,5,
                                  0,5,0, 0,9,0, 6,0,0,

                                  1,3,0, 0,0,0, 2,5,0,
                                  0,0,0, 0,0,0, 0,7,4,
                                  0,0,5, 2,0,6, 3,0,0};
    board = SudokuBoard(inputSudoku);
    displaySudokuBoard(board);

    connect(ui->solveButton, SIGNAL(released()), this, SLOT(solveCurrentBoard()));
}

SudokuGui::~SudokuGui()
{
    delete ui;
}

void SudokuGui::generateLabels()
{
    QGridLayout *layout = ui->gridLayout0;
    labelsArray = new QLabel*[81];
    for(int id=0; id<81; id++)
    {
        labelsArray[id] = new QLabel(this);
        labelsArray[id]->setText(tr(" "));
        layout->addWidget(labelsArray[id], int(id/9), id % 9);
    }
}

void SudokuGui::displaySudokuBoard(SudokuBoard &s)
{
    for(int i = 0; i < 81; i++)
    {
        //if(s[i].getVal() == 0)
            //i++;
        labelsArray[i]->setNum(s[i].getVal());
    }
    return;
}

void SudokuGui::solveCurrentBoard()
{
    //TODO check case that unsolvable
    board.SolveSudoku();
    displaySudokuBoard(board);
}
