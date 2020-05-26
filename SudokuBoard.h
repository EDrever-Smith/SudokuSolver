#ifndef SUDOKUBOARD_H_INCLUDED
#define SUDOKUBOARD_H_INCLUDED

#include "NumberSquare.h"
#include <iostream>
#include <cmath>
#include <vector>
class SudokuBoard
{
public:
    SudokuBoard(int boardValues[81]);
    SudokuBoard(NumberSquare boardValues[81]);
    SudokuBoard();
    NumberSquare& operator[](int p);
    bool isLegal(int id, int num);
    bool SolveSudoku(); //returns false if no solution exists

    friend std::ostream& operator<<(std::ostream& out, const SudokuBoard& s);
private:
    NumberSquare board[81];
    bool findBlank(int& id); //returns false if no blanks are found
};

#endif // SUDOKUBOARD_H_INCLUDED
