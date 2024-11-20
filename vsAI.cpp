#include "board.hpp"
#include "hasamishogi_AI.hpp"
#include <iostream>
#include <limits>

using namespace std;

// プレイヤーの入力を処理
void getPlayerMove(int& y, int& x, int& newY, int& newX) {
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
bool playerTurn(Board& board, Cell::CellCondition direction=Cell::UP) {
    int y, x, newY, newX;

    board.show();
    cout << "あなたの番です。\n";

    while (true) {
        getPlayerMove(y, x, newY, newX);
        std::vector<vector<Cell::CellCondition>> cells = board.copyState();

        // 駒の移動が有効か確認
        if (Board::isOnBoard(y, x) && Board::isOnBoard(newY, newX) &&
            cells[y][x] == direction &&   // プレイヤーの駒
            cells[newY][newX] == Cell::EMPTY &&  // 移動先が空
            (y == newY || x == newX)) {  // 垂直または水平の移動のみ
            // 駒の移動
            board.movePiece(y, x, newY, newX, Cell::CellCondition::UP);
            return true;
        } else {
            cout << "無効な移動です。もう一度入力してください。\n";
        }
    }
}

// AIのターンを処理
bool aiTurn(Board& board, HasamiShogiAI& ai) {
    board.show();
    cout << "AIの番です...\n";

    auto move = ai.findBestMove(board);
    int oldY = move.first.first;
    int oldX = move.first.second;
    int newY = move.second.first;
    int newX = move.second.second;

    cout << "AIが (" << oldY << ", " << oldX << ") から (" << newY << ", " << newX << ") に移動しました。\n";

    // 駒の移動
    board.movePiece(oldY, oldX, newY, newX, Cell::CellCondition::DOWN);
    return true;
}

int main() {
    Board board;
    HasamiShogiAI ai(Cell::CellCondition::DOWN);
    bool gameOver = false;
    int playerChoice;

    // 先攻・後攻を選択
    cout << "先攻は1、後攻は2を選んでください: ";
    cin >> playerChoice;

    if (playerChoice != 1 && playerChoice != 2) {
        cout << "無効な入力です。先攻 (1) に設定します。\n";
        playerChoice = 1;
    }

    bool playerFirst = (playerChoice == 1);

    // ゲームループ
    while (!gameOver) {
        if (playerFirst) {
            if (playerTurn(board)) {
                // 勝敗判定
                int playerCount = board.countPlayerPiece(Cell::CellCondition::UP);
                int aiCount = board.countPlayerPiece(Cell::CellCondition::DOWN);

                if (aiCount == 0) {
                    cout << "あなたの勝利です！\n";
                    gameOver = true;
                } else if (playerCount == 0) {
                    cout << "AIの勝利です！\n";
                    gameOver = true;
                }

                playerFirst = false;  // 次はAIのターン
            }
        } else {
            if (aiTurn(board, ai)) {
                // 勝敗判定
                int playerCount = board.countPlayerPiece(Cell::CellCondition::UP);
                int aiCount = board.countPlayerPiece(Cell::CellCondition::DOWN);

                if (aiCount == 0) {
                    cout << "あなたの勝利です！\n";
                    gameOver = true;
                } else if (playerCount == 0) {
                    cout << "AIの勝利です！\n";
                    gameOver = true;
                }

                playerFirst = true;  // 次はプレイヤーのターン
            }
        }
    }

    cout << "ゲーム終了。\n";
    return 0;
}
