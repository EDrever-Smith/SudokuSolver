#ifndef SUDOKUBOARD_H_INCLUDED
#define SUDOKUBOARD_H_INCLUDED

#include "NumberSquare.h"
#include <iostream>
#include <cmath>
#include <vector>
class SudokuBoard
{
public:
    SudokuBoard(std::vector<int> boardValues);
    SudokuBoard(NumberSquare &boardValues);
private:
    std::vector<NumberSquare> board;
};

#endif // SUDOKUBOARD_H_INCLUDED
