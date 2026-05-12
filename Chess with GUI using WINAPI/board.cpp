#include "board.h"
using namespace std;

Board::Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            BOARD[i][j] = NULL;
    kingCaptured = false;
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
    for (int i = 0; i < 8; i++)
        BOARD[1][i] = new Pawn(false);
    BOARD[7][0] = new Rook(true);
    BOARD[7][1] = new Knight(true);
    BOARD[7][2] = new Bishop(true);
    BOARD[7][3] = new Queen(true);
    BOARD[7][4] = new King(true);
    BOARD[7][5] = new Bishop(true);
    BOARD[7][6] = new Knight(true);
    BOARD[7][7] = new Rook(true);
    for (int i = 0; i < 8; i++)
        BOARD[6][i] = new Pawn(true);
}

void Board::display() {
    cout << endl;
    cout << "    0 1 2 3 4 5 6 7" << endl;
    cout << "   -----------------" << endl;
    for (int i = 0; i < 8; i++) {
        cout << " " << i << " | ";
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

char Board::getSymbolAt(int r, int c) {
    if (BOARD[r][c] == NULL) return 0;
    return BOARD[r][c]->getsymbol();
}

bool Board::movePiece(int sr, int sc, int dr, int dc) {
    if (sr<0 || sr>7 || sc<0 || sc>7 || dr<0 || dr>7 || dc<0 || dc>7) return false;
    if (BOARD[sr][sc] == NULL) return false;
    if (!(BOARD[sr][sc]->isValidMove(sr, sc, dr, dc))) return false;

    char sym = BOARD[sr][sc]->getsymbol();
    if (sym == 'P' || sym == 'p') {
        if (abs(dc - sc) == 1) {
            if (BOARD[dr][dc] == NULL) return false;
        }
        if (dc == sc) {
            if (BOARD[dr][dc] != NULL) return false;
        }
    }

    if (BOARD[sr][sc]->isSlider()) {
        int rowDir = 0, colDir = 0;
        if (dr > sr) rowDir = 1;
        else if (dr < sr) rowDir = -1;
        if (dc > sc) colDir = 1;
        else if (dc < sc) colDir = -1;
        int r = sr + rowDir;
        int c = sc + colDir;
        while (r != dr || c != dc) {
            if (BOARD[r][c] != NULL) return false;
            r += rowDir;
            c += colDir;
        }
    }
    if (BOARD[dr][dc] != NULL && BOARD[dr][dc]->iswhite() == BOARD[sr][sc]->iswhite()) return false;

    if (BOARD[dr][dc] != NULL && BOARD[dr][dc]->getsymbol() == 'K') kingCaptured = true;
    if (BOARD[dr][dc] != NULL && BOARD[dr][dc]->getsymbol() == 'k') kingCaptured = true;

    delete BOARD[dr][dc];
    BOARD[dr][dc] = BOARD[sr][sc];
    BOARD[sr][sc] = NULL;
    return true;
}