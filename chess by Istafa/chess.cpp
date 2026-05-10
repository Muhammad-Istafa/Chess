#include "board.h"
#include <iostream>
using namespace std;

int main() {
    Board chessboard;
    bool whiteTurn = true;

    cout << "  === CHESS GAME ===" << endl;
    chessboard.display();
    cout << (whiteTurn ? "WHITE's turn" : "BLACK's turn") << endl;

    return 0;
}
