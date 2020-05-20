#include "SudokuBoard.h"

SudokuBoard::SudokuBoard(std::vector<int> boardValues)
{
    //Assumed that passed array is of length 81 since this represents a standard sudoku board
    for(int i = 0; i < 81; i++)
    {
        int column = i % 9;
        int row = floor(i/9);

        int box;
        if(column < 3 && row < 3)
            box = 0;
        else if(column < 6 && row < 3)
            box = 1;
        else if(column < 9 && row < 3)
            box = 2;
        else if(column < 3 && row < 6)
            box = 3;
        else if(column < 6 && row < 6)
            box = 4;
        else if(column < 9 && row < 6)
            box = 5;
        else if(column < 3 && row < 9)
            box = 6;
        else if(column < 6 && row < 9)
            box = 7;
        else if(column < 9 && row < 9)
            box = 8;

        NumberSquare square(row, column, box, boardValues[i]);
        this->board.push_back(square);
    }
}

SudokuBoard::SudokuBoard(NumberSquare &boardValues)
{

}
