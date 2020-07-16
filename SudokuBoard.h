#ifndef SUDOKUBOARD_H_INCLUDED
#define SUDOKUBOARD_H_INCLUDED

#include "NumberSquare.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <time.h> 
#include <algorithm>
class SudokuBoard
{
public:
    SudokuBoard(int boardValues[81]);
    SudokuBoard(NumberSquare boardValues[81]);
    SudokuBoard();
    NumberSquare& operator[](int p);
    bool isLegal(int id, int num);
    bool SolveSudoku(); //returns false if no solution exists
    void clearBoard();
    void setBoard(std::vector<int>& board);
    void generateSolvableSudoku(std::vector<int>& blankIDs, int difficulty = 0);
    friend std::ostream& operator<<(std::ostream& out, const SudokuBoard& s);
private:
    NumberSquare board[81];
    bool findBlank(int& id); //returns false if no blanks are found
    bool findBlank(); //returns false if no blanks are found
    bool generateRandomFilledSudoku(bool firstCallFlag = true);
};

#endif // SUDOKUBOARD_H_INCLUDED
