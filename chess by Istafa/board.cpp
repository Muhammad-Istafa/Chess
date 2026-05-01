#include "board.h"
using namespace std;

Board::Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            BOARD[i][j] = NULL;
        }
    }

    setupBoard();
}

void Board::setupBoard() {  

    BOARD[0][0] = new Rook(false);
    BOARD[0][1] = new Knight(false);
    BOARD[0][2] = new Bishop(false);
    BOARD[0][3] = new Queen(false);
    BOARD[0][4] = new King(false);
    BOARD[0][5] = new Bishop(false);
    BOARD[0][6] = new Knight(false);
    BOARD[0][7] = new Rook(false);

    for (int i = 0; i < 8; i++) {
        BOARD[1][i] = new Pawn(false);
    }

    BOARD[7][0] = new Rook(true);
    BOARD[7][1] = new Knight(true);
    BOARD[7][2] = new Bishop(true);
    BOARD[7][3] = new Queen(true);
    BOARD[7][4] = new King(true);
    BOARD[7][5] = new Bishop(true);
    BOARD[7][6] = new Knight(true);
    BOARD[7][7] = new Rook(true);

    for (int i = 0; i < 8; i++) {
        BOARD[6][i] = new Pawn(true);
    }
}

void Board::display() {

    cout << endl;

    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < 8; j++) {

            if (BOARD[i][j] == NULL)
                cout << ". ";
            else
                cout << BOARD[i][j]->getsymbol() << " ";
        }

        cout << endl;
    }

    cout << endl;
}