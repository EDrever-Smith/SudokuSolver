#ifndef NUMBERSQUARE_H_INCLUDED
#define NUMBERSQUARE_H_INCLUDED

class NumberSquare
{
public:
    NumberSquare(int row, int col, int box, int value);
    NumberSquare();
    NumberSquare(const NumberSquare&);
    const NumberSquare& operator=(const NumberSquare&);
    ~NumberSquare();

    int getRow();
    int getCol();
    int getBox();
    int getVal()const;

    void setVal(int v);
private:
    int rowID;
    int columnID;
    int boxID;
    int value;
};

#endif // NUMBERSQUARE_H_INCLUDED
