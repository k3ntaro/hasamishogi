#include "cell.hpp"

Cell::Cell(CellCondition condition):condition(condition){}

void Cell::setCondition(CellCondition condition){
    this->condition=condition;
    return;
}

void Cell::setEmpty(){
    this->condition=CellCondition::EMPTY;
    return;
}

Cell::CellCondition Cell::getCondition()const{
    return this->condition;
}