#include "SudokuBoard.h"

SudokuBoard::SudokuBoard(int boardValues[81])
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
        this->board[i] = square;
    }
}

SudokuBoard::SudokuBoard(NumberSquare boardValues[81])
{

}

SudokuBoard::SudokuBoard()
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

        NumberSquare square(row, column, box, 0);
        this->board[i] = square;
    }
}

NumberSquare& SudokuBoard::operator[](int p)
{
    return this->board[p];
}

bool SudokuBoard::SolveSudoku()
{
    int blankID;
    if(!findBlank(blankID))
        return true;
    for(int num = 1; num<=9; num++)
    {
        if(this->isLegal(blankID,num))
        {
            board[blankID].setVal(num);
            if(SolveSudoku())
                return true;

            board[blankID].setVal(0);
        }

    }
    return false;
}

bool SudokuBoard::findBlank(int& id)
{
    for(int i = 0; i < 81; i++)
    {
        if(this->board[i].getVal() == 0)
        {
            id = i;
            return true;
        }
    }
    return false;
}

bool SudokuBoard::isLegal(int id, int num)
{
    //this is dreadfully inefficient
    for(int i = 0; i < 81; i++)
    {
        if(i == id)
            i++;
        if((board[i].getCol() == board[id].getCol() ||
            board[i].getRow() == board[id].getRow() ||
            board[i].getBox() == board[id].getBox()) && board[i].getVal() == num)
        {
            return false;
        }
    }
    return true;
}
std::ostream& operator<<(std::ostream& out, const SudokuBoard& s)
{
    for(int i = 0; i < 81; i++)
    {
        if(i%9 == 0)
            out<<"\n";
        out<<s.board[i].getVal()<<" ";
    }
    return out;
}

void SudokuBoard::clearBoard()
{
    for(int i = 0; i < 81; i++)
        board[i].setVal(0);
}

void SudokuBoard::setBoard(std::vector<int>& board)
{
    for (int i = 0; i < 81; i++)
    {
        int column = i % 9;
        int row = floor(i / 9);

        int box;
        if (column < 3 && row < 3)
            box = 0;
        else if (column < 6 && row < 3)
            box = 1;
        else if (column < 9 && row < 3)
            box = 2;
        else if (column < 3 && row < 6)
            box = 3;
        else if (column < 6 && row < 6)
            box = 4;
        else if (column < 9 && row < 6)
            box = 5;
        else if (column < 3 && row < 9)
            box = 6;
        else if (column < 6 && row < 9)
            box = 7;
        else if (column < 9 && row < 9)
            box = 8;

        NumberSquare square(row, column, box, board[i]);
        this->board[i] = square;
    }
}