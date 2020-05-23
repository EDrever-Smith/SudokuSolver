#include "NumberSquare.h"

NumberSquare::NumberSquare(int row, int col, int box, int _value):
    rowID(row),columnID(col),boxID(box),value(_value){}
NumberSquare::NumberSquare(){}
NumberSquare::~NumberSquare(){}

const NumberSquare& NumberSquare::operator=(const NumberSquare& n)
{
    if(this==&n) return (*this);
    rowID = n.rowID;
    columnID = n.columnID;
    boxID = n.boxID;
    value = n.value;
    return(*this);
}

NumberSquare::NumberSquare(const NumberSquare& n)
{
    rowID = n.rowID;
    columnID = n.columnID;
    boxID = n.boxID;
    value = n.value;
}

int NumberSquare::getRow()
{
    return this->rowID;
}

int NumberSquare::getCol()
{
    return this->columnID;
}

int NumberSquare::getBox()
{
    return this->boxID;
}

int NumberSquare::getVal()const
{
    return this->value;
}

void NumberSquare::setVal(int v)
{
    this->value = v;
}
