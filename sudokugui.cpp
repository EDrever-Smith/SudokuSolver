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
    QGridLayout *grid1 = ui->gridLayout1;
    QGridLayout *grid2 = ui->gridLayout2;
    QGridLayout *grid3 = ui->gridLayout3;
    QGridLayout *grid4 = ui->gridLayout4;
    QGridLayout *grid5 = ui->gridLayout5;
    QGridLayout *grid6 = ui->gridLayout6;
    QGridLayout *grid7 = ui->gridLayout7;
    QGridLayout *grid8 = ui->gridLayout8;
    QGridLayout *grid9 = ui->gridLayout9;
    labelsArray = new QLabel*[81];
    for(int id=0; id<81; id++)
    {
        labelsArray[id] = new QLabel(this);
        labelsArray[id]->setText(tr(" "));
        labelsArray[id]->setStyleSheet("QLabel {"
        "border-style: solid;"
        "border-width: 1px;"
        "border-color: black; "
        "}");
        labelsArray[id]->setAlignment(Qt::AlignCenter);
        //layout->addWidget(labelsArray[id], int(id/9), id % 9);
        if(id % 9 < 3 && id/9 <3)
            grid1->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 3)
            grid2->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 3)
            grid3->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 3 && id/9 < 6)
            grid4->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 6)
            grid5->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 6)
            grid6->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 3 && id/9 < 9)
            grid7->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 9)
            grid8->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 9)
            grid9->addWidget(labelsArray[id],(id/9)%3, (id%9)%3);
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
