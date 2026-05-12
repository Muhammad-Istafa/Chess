#include "board.h"
#include <windows.h>
#include <string>

// g++ chess.cpp board.cpp -o chess.exe -mwindows

const int CELL    = 80;
const int OX      = 40;
const int OY      = 50;
const int WIN_W   = OX * 2 + CELL * 8;
const int WIN_H   = OY + CELL * 8 + 90;

Board*      gBoard        = nullptr;
bool        whiteTurn     = true;
bool        pieceSelected = false;
int         selRow        = -1;
int         selCol        = -1;
bool        gameOver      = false;
std::string winnerMsg     = "";

void DrawBoard(HDC hdc) {
    COLORREF clrLight  = RGB(240, 217, 181);
    COLORREF clrDark   = RGB(181, 136,  99);
    COLORREF clrSel    = RGB( 80, 200,  80);
    COLORREF clrBorder = RGB( 30,  30,  30);

    HBRUSH borderBrush = CreateSolidBrush(clrBorder);
    RECT borderRect = { OX - 4, OY - 4, OX + CELL * 8 + 4, OY + CELL * 8 + 4 };
    FillRect(hdc, &borderRect, borderBrush);
    DeleteObject(borderBrush);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int x = OX + c * CELL;
            int y = OY + r * CELL;

            COLORREF color;
            if (pieceSelected && r == selRow && c == selCol)
                color = clrSel;
            else
                color = ((r + c) % 2 == 0) ? clrLight : clrDark;

            HBRUSH br = CreateSolidBrush(color);
            RECT cell = { x, y, x + CELL, y + CELL };
            FillRect(hdc, &cell, br);
            DeleteObject(br);
        }
    }

    HFONT labelFont = CreateFont(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI"
    );
    HGDIOBJ oldFont = SelectObject(hdc, labelFont);
    SetBkMode(hdc, TRANSPARENT);

    for (int i = 0; i < 8; i++) {
        char buf[3];
        wsprintfA(buf, "%d", i);

        SetTextColor(hdc, RGB(200, 200, 200));
        TextOutA(hdc, OX - 20, OY + i * CELL + CELL / 2 - 8, buf, 1);
        TextOutA(hdc, OX + i * CELL + CELL / 2 - 5, OY - 25, buf, 1);
    }

    SelectObject(hdc, oldFont);
    DeleteObject(labelFont);

    HFONT pieceFont = CreateFont(
        52, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, "Courier New"
    );
    oldFont = SelectObject(hdc, pieceFont);
    SetBkMode(hdc, TRANSPARENT);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char sym = gBoard->getSymbolAt(r, c);
            if (sym == 0) continue;

            bool isWhitePiece = (sym >= 'A' && sym <= 'Z');

            int x = OX + c * CELL;
            int y = OY + r * CELL;

            COLORREF shadow = isWhitePiece ? RGB(100, 100, 100) : RGB(180, 180, 180);
            SetTextColor(hdc, shadow);
            RECT shadowRect = { x + 3, y + 7, x + CELL + 3, y + CELL + 7 };
            char str[2] = { sym, 0 };
            DrawTextA(hdc, str, 1, &shadowRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SetTextColor(hdc, isWhitePiece ? RGB(255, 255, 255) : RGB(20, 20, 20));
            RECT cellRect = { x, y + 4, x + CELL, y + CELL + 4 };
            DrawTextA(hdc, str, 1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    SelectObject(hdc, oldFont);
    DeleteObject(pieceFont);
}

void DrawStatus(HDC hdc) {
    HFONT font = CreateFont(
        26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI"
    );
    HGDIOBJ oldFont = SelectObject(hdc, font);
    SetBkMode(hdc, TRANSPARENT);

    std::string msg;
    COLORREF    msgColor;

    if (gameOver) {
        msg      = winnerMsg;
        msgColor = RGB(220, 60, 60);
    } else if (whiteTurn) {
        msg      = "WHITE's turn";
        msgColor = RGB(240, 240, 240);
    } else {
        msg      = "BLACK's turn";
        msgColor = RGB(160, 160, 255);
    }

    RECT statusRect = { OX, OY + CELL * 8 + 12, OX + CELL * 8, OY + CELL * 8 + 70 };
    SetTextColor(hdc, msgColor);
    DrawTextA(hdc, msg.c_str(), -1, &statusRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    if (!gameOver) {
        std::string hint = "Click a piece to select, click again to move";
        HFONT hintFont = CreateFont(
            14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI"
        );
        SelectObject(hdc, hintFont);
        SetTextColor(hdc, RGB(120, 120, 120));
        RECT hintRect = { OX, OY + CELL * 8 + 55, OX + CELL * 8, OY + CELL * 8 + 82 };
        DrawTextA(hdc, hint.c_str(), -1, &hintRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DeleteObject(hintFont);
    }

    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        HDC     memDC  = CreateCompatibleDC(hdc);
        HBITMAP memBmp = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
        HGDIOBJ oldBmp = SelectObject(memDC, memBmp);

        HBRUSH bgBrush = CreateSolidBrush(RGB(35, 35, 38));
        FillRect(memDC, &clientRect, bgBrush);
        DeleteObject(bgBrush);

        DrawBoard(memDC);
        DrawStatus(memDC);

        BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBmp);
        DeleteObject(memBmp);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        if (gameOver) break;

        int mx  = LOWORD(lParam);
        int my  = HIWORD(lParam);
        int col = (mx - OX) / CELL;
        int row = (my - OY) / CELL;

        if (row < 0 || row > 7 || col < 0 || col > 7) {
            pieceSelected = false;
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }

        if (pieceSelected && row == selRow && col == selCol) {
            pieceSelected = false;
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }

        if (!pieceSelected) {
            char sym = gBoard->getSymbolAt(row, col);
            if (sym == 0) break;
            bool isWhitePiece = (sym >= 'A' && sym <= 'Z');
            if (whiteTurn  && !isWhitePiece) break;
            if (!whiteTurn &&  isWhitePiece) break;
            pieceSelected = true;
            selRow = row;
            selCol = col;
        } else {
            bool moved = gBoard->movePiece(selRow, selCol, row, col);
            pieceSelected = false;

            if (moved) {
                if (gBoard->kingCaptured) {
                    gameOver  = true;
                    winnerMsg = whiteTurn ? "WHITE WINS!" : "BLACK WINS!";
                } else {
                    whiteTurn = !whiteTurn;
                }
            }
        }

        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    gBoard = new Board();

    WNDCLASSEX wc     = {};
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.lpfnWndProc    = WndProc;
    wc.hInstance      = hInstance;
    wc.lpszClassName  = "ChessApp";
    wc.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    RECT wr = { 0, 0, WIN_W, WIN_H };
    DWORD style = (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX);
    AdjustWindowRect(&wr, style, FALSE);

    HWND hwnd = CreateWindowEx(
        0, "ChessApp", "Chess Game",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    delete gBoard;
    return (int)message.wParam;
}