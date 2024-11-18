#ifndef CELL_HPP
#define CELL_HPP

class Cell {
public:
    enum CellCondition:int{
        UP = -1,     // 上向きの駒
        DOWN = 1,    // 下向きの駒
        EMPTY = 0   // 空のマ
    };
private:
    CellCondition condition;

public:
    // コンストラクタ
    Cell(CellCondition condition);
    // 駒の状態を設定
    void setCondition(CellCondition condition);
    // 空のマスに設定
    void setEmpty();
    // 駒の状態を取得
    CellCondition getCondition() const;
};

#endif
