#include <iostream>
#include <vector>
#include "SudokuBoard.h"
using namespace std;
/*
{3,0,6, 5,0,8, 4,0,0,
5,2,0, 0,0,0, 0,0,0,
0,8,7, 0,0,0, 0,3,1,

0,0,3, 0,1,0, 0,8,0,
9,0,0, 8,6,3, 0,0,5,
0,5,0, 0,9,0, 6,0,0,

1,3,0, 0,0,0, 2,5,0,
0,0,0, 0,0,0, 0,7,4,
0,0,5, 2,0,6, 3,0,0};
*/
int main()
{
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
    cout<<"Input Sudoku:"<<endl;
    SudokuBoard myBoard(inputSudoku);
    cout<<myBoard<<endl;
    myBoard.SolveSudoku();
    cout<<"\nSolved Sudoku:\n"<<myBoard<<endl;

}

