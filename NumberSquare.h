#ifndef NUMBERSQUARE_H_INCLUDED
#define NUMBERSQUARE_H_INCLUDED

class NumberSquare
{
public:
    NumberSquare(int row, int col, int box, int value);
    ~NumberSquare();
private:
    int rowID;
    int columnID;
    int boxID;
    int value;
};

#endif // NUMBERSQUARE_H_INCLUDED
