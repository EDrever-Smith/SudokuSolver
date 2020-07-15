#include "SudokuBoard.h"

// random generator function:
int randomGen(int i) { return std::rand() % i; }

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
            else
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

bool SudokuBoard::findBlank()
{
    for (int i = 0; i < 81; i++)
    {
        if (this->board[i].getVal() == 0)
        {
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
            i++; //TODO check if can replace with continue
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

bool SudokuBoard::generateRandomFilledSudoku(bool firstCallFlag)
{
    static std::vector<int> randomCellArray(81, 0);
    if (firstCallFlag) //only called first time
    {
        clearBoard();
        for (int n = 0; n < 81; n++)
        {
            randomCellArray[n] = n;
        }
        std::random_shuffle(randomCellArray.begin(), randomCellArray.end(), randomGen);
    }
    static std::vector<int>::iterator it;
    if (firstCallFlag)
        it = randomCellArray.begin();

    //Base case - once iterator has reached end of array it implies all squares are filled
    if (it == randomCellArray.end())
        return true;

    //Initialise 
    std::vector<int> randomNumArray(9, 0);
    for (int n = 0; n < 9; n++)
        randomNumArray[n] = n + 1;
    std::random_shuffle(randomNumArray.begin(), randomNumArray.end(), randomGen);
    std::vector<int>::iterator numIt = randomNumArray.begin();

    while (numIt != randomNumArray.end()) //try every number posibility but in a random order
    {
        if (!isLegal(*it, *numIt))
        {
            numIt++;
            std::random_shuffle(numIt, randomNumArray.end(), randomGen);
        }
        else //It is legal
        {
            board[*it].setVal(*numIt);

            //Store a temp copy of the board since SolveSudoku will attempt to fill it entirely
            std::vector<NumberSquare> temp(81, NumberSquare()); //TODO replace this with a getfunction
            for (int j = 0; j < 81; j++)
                temp[j] = this->board[j];

            if (SolveSudoku())
            {
                it++;

                //retrieve stored copy of board prior to  solvesudoku call
                for (int j = 0; j < 81; j++) //TODO replace this with a set function
                    this->board[j] = temp[j];

                if (generateRandomFilledSudoku(false))
                    return true;
                else
                {
                    it--;
                    std::random_shuffle(it, randomCellArray.end(), randomGen);
                }
            }
            else //number is legal but cannot result in solvable sudoku so reset value to 0 and move on
            {
                numIt++;
                board[*it].setVal(0);
            }
        }
    }
    return false;
}

//Function which removes numbers from a filled in sudoku. Number depends on difficulty (0 = easy, 1 = medium, 2 = hard)
//This function in no way ensures that there is only one solution or that it is solvable without guessing
//Difficulty is a rough  guide since numbers removed are entierly random, any other method would be too time consuming for real time generation
void SudokuBoard::generateSolvableSudoku(int difficulty)
{
    srand((unsigned int)time(NULL));  //Setup random seed
    generateRandomFilledSudoku();

    int easyRemoves = 45;
    int mediumRemoves = 50;
    int hardRemoves = 55;

    std::vector<int> randomArray(81, 0);
    for (int i = 0; i < 81; i++)
        randomArray[i] = i;
    std::random_shuffle(randomArray.begin(), randomArray.end());

    switch (difficulty)
    {
    case 0:
        for (int i = 0; i < easyRemoves; i++)
            board[*(randomArray.begin() + i)].setVal(0);
        break;
    case 1:
        for (int i = 0; i < mediumRemoves; i++)
            board[*(randomArray.begin() + i)].setVal(0);
        break;
    case 2:
        for (int i = 0; i < hardRemoves; i++)
            board[*(randomArray.begin() + i)].setVal(0);
        break;
    }
}