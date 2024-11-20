#include "board.hpp"
#include "hasamishogi_AI.hpp"
#include <iostream>
#include <limits>

using namespace std;

// プレイヤーの入力を処理
void getPlayerMove(int& y, int& x, int& newY, int& newX, Cell::CellCondition direction) {
    while (true) {
        cout << "駒の移動元 (y x): ";
        cin >> y >> x;
        if (cin.fail() || y < 0 || y >= Board::AXIS_SIZE || x < 0 || x >= Board::AXIS_SIZE) {
            cout << "無効な座標です。もう一度入力してください。\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cout << "移動先 (newY newX): ";
        cin >> newY >> newX;
        if (cin.fail() || newY < 0 || newY >= Board::AXIS_SIZE || newX < 0 || newX >= Board::AXIS_SIZE) {
            cout << "無効な座標です。もう一度入力してください。\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }
}

// プレイヤーのターンを処理
bool playerTurn(Board& board, Cell::CellCondition direction) {
    int y, x, newY, newX;

    board.show();
    cout << ((direction == Cell::UP) ? "プレイヤー 1 (UP)" : "プレイヤー 2 (DOWN)") << " の番です。\n";

    while (true) {
        getPlayerMove(y, x, newY, newX, direction);

        std::vector<vector<Cell::CellCondition>> cells_condition=board.copyState();

        // 駒の移動が有効か確認
        if (board.isOnBoard(y, x) && board.isOnBoard(newY, newX) && 
            cells_condition[y][x] == direction &&
            cells_condition[newY][newX] == Cell::EMPTY &&
            (y == newY || x == newX)) {  // 垂直または水平の移動のみ
            // 駒の移動
            board.movePiece(y, x, newY, newX, direction);
            return true;
        } else {
            cout << "無効な移動です。もう一度入力してください。\n";
        }
    }
}

int main() {
    Board board;    
    bool gameOver = false;
    Cell::CellCondition currentPlayer = Cell::UP;

    // ゲームループ
    while (!gameOver) {
        if (playerTurn(board, currentPlayer)) {
            // 勝敗判定
            int upCount = board.countPlayerPiece(Cell::UP);
            int downCount = board.countPlayerPiece(Cell::DOWN);

            if (upCount == 0) {
                cout << "プレイヤー 2 (DOWN) の勝利！\n";
                gameOver = true;
            } else if (downCount == 0) {
                cout << "プレイヤー 1 (UP) の勝利！\n";
                gameOver = true;
            }

            // 次のプレイヤーに交代
            currentPlayer = (currentPlayer == Cell::UP) ? Cell::DOWN : Cell::UP;
        }
    }

    cout << "ゲーム終了。\n";
    return 0;
}
