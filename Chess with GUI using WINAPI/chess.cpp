#include "board.h"
#include <windows.h>
#include <algorithm>
#include <string>
using std::max;
using std::min;

int gSW = 0, gSH = 0, gCELL = 0, gOX = 0, gOY = 0;

Board*      gBoard        = nullptr;
bool        whiteTurn     = true;
bool        pieceSelected = false;
int         selRow = -1, selCol = -1;
bool        gameOver = false, isDraw = false;
std::string statusMsg = "";

RECT gDrawBtn = {};
#define TIMER_ID 1

void ResetGame() {
    delete gBoard; gBoard = new Board();
    whiteTurn = true; pieceSelected = false;
    selRow = -1; selCol = -1;
    gameOver = false; isDraw = false; statusMsg = "";
}

const wchar_t* Glyph(char s) {
    switch(s){
    case 'K': return L"\u2654";
    case 'Q': return L"\u2655";
    case 'R': return L"\u2656";
    case 'B': return L"\u2657";
    case 'N': return L"\u2658";
    case 'P': return L"\u2659";
    case 'k': return L"\u265A";
    case 'q': return L"\u265B";
    case 'r': return L"\u265C";
    case 'b': return L"\u265D";
    case 'n': return L"\u265E";
    case 'p': return L"\u265F";
    default:  return L"";
    }
}

void Paint(HDC hdc, int W, int H) {
    HBRUSH bg = CreateSolidBrush(RGB(28,28,32));
    RECT all = {0,0,W,H};
    FillRect(hdc, &all, bg); DeleteObject(bg);

    SetBkMode(hdc, TRANSPARENT);

    HBRUSH border = CreateSolidBrush(RGB(8,8,8));
    RECT bord = {gOX-4, gOY-4, gOX+gCELL*8+4, gOY+gCELL*8+4};
    FillRect(hdc, &bord, border); DeleteObject(border);

    for(int r=0;r<8;r++) for(int c=0;c<8;c++) {
        int x=gOX+c*gCELL, y=gOY+r*gCELL;
        COLORREF col;
        if(pieceSelected && r==selRow && c==selCol) col=RGB(80,200,80);
        else col=((r+c)%2==0)?RGB(240,217,181):RGB(181,136,99);
        HBRUSH b=CreateSolidBrush(col);
        RECT sq={x,y,x+gCELL,y+gCELL};
        FillRect(hdc,&sq,b); DeleteObject(b);
    }

    int lsz = max(12, gCELL/7);
    HFONT lf = CreateFontA(lsz,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,"Segoe UI");
    HGDIOBJ pv = SelectObject(hdc, lf);
    for(int i=0;i<8;i++){
        char buf[4]; wsprintfA(buf,"%d",i);
        SetTextColor(hdc,RGB(130,130,130));
        TextOutA(hdc, gOX-lsz-6, gOY+i*gCELL+gCELL/2-lsz/2, buf, 1);
        TextOutA(hdc, gOX+i*gCELL+gCELL/2-lsz/3, gOY-lsz-8, buf, 1);
    }
    SelectObject(hdc,pv); DeleteObject(lf);

    int psz=(int)(gCELL*0.78);
    HFONT pf=CreateFontW(psz,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI Symbol");
    pv=SelectObject(hdc,pf);
    for(int r=0;r<8;r++) for(int c=0;c<8;c++){
        char sym=gBoard->getSymbolAt(r,c); if(!sym) continue;
        bool iw=(sym>='A'&&sym<='Z');
        const wchar_t* g=Glyph(sym);
        int x=gOX+c*gCELL, y=gOY+r*gCELL, sh=max(2,gCELL/36);
        RECT sr2={x+sh,y+sh,x+gCELL+sh,y+gCELL+sh};
        RECT cr2={x,y,x+gCELL,y+gCELL};
        SetTextColor(hdc,iw?RGB(40,40,40):RGB(150,150,150));
        DrawTextW(hdc,g,-1,&sr2,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        SetTextColor(hdc,iw?RGB(255,252,215):RGB(5,5,5));
        DrawTextW(hdc,g,-1,&cr2,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    }
    SelectObject(hdc,pv); DeleteObject(pf);

    int tsz=max(18,gCELL/4);
    HFONT tf=CreateFontA(tsz,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,"Segoe UI");
    pv=SelectObject(hdc,tf);
    const char* tmsg; COLORREF tcol;
    if(gameOver){tmsg=statusMsg.c_str();tcol=RGB(230,70,70);}
    else if(isDraw){tmsg=statusMsg.c_str();tcol=RGB(220,200,50);}
    else if(whiteTurn){tmsg="WHITE's turn";tcol=RGB(245,245,245);}
    else{tmsg="BLACK's turn";tcol=RGB(150,150,255);}
    int bot=gOY+gCELL*8;
    RECT tr={gOX, bot+10, gOX+gCELL*8, bot+10+tsz+6};
    SetTextColor(hdc,tcol);
    DrawTextA(hdc,tmsg,-1,&tr,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    SelectObject(hdc,pv); DeleteObject(tf);

    int hsz=max(11,gCELL/10);
    HFONT hf=CreateFontA(hsz,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,"Segoe UI");
    pv=SelectObject(hdc,hf);
    SetTextColor(hdc,RGB(60,60,60));
    RECT hr={gOX, bot+10+tsz+8, gOX+gCELL*8, bot+10+tsz+8+hsz+4};
    DrawTextA(hdc,"Click piece then destination  |  ESC = quit",-1,&hr,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    SelectObject(hdc,pv); DeleteObject(hf);

    if(!gameOver && !isDraw){
        int bw=180, bh=56;
        int bx=(W-bw)/2, by=14;
        gDrawBtn={bx,by,bx+bw,by+bh};

        HBRUSH bb=CreateSolidBrush(RGB(210,150,0));
        FillRect(hdc,&gDrawBtn,bb); DeleteObject(bb);

        HPEN pen=CreatePen(PS_SOLID,3,RGB(255,220,0));
        HGDIOBJ op=SelectObject(hdc,pen);
        HBRUSH nb=(HBRUSH)GetStockObject(NULL_BRUSH);
        HGDIOBJ ob=SelectObject(hdc,nb);
        Rectangle(hdc,bx,by,bx+bw,by+bh);
        SelectObject(hdc,op); SelectObject(hdc,ob); DeleteObject(pen);

        HFONT bf=CreateFontA(26,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,"Segoe UI");
        pv=SelectObject(hdc,bf);
        SetTextColor(hdc,RGB(255,255,255));
        DrawTextA(hdc,"DRAW",-1,&gDrawBtn,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        SelectObject(hdc,pv); DeleteObject(bf);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg){
    case WM_PAINT:{
        PAINTSTRUCT ps; HDC hdc=BeginPaint(hwnd,&ps);
        RECT cr; GetClientRect(hwnd,&cr);
        HDC mem=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,cr.right,cr.bottom);
        HGDIOBJ old=SelectObject(mem,bmp);
        Paint(mem,cr.right,cr.bottom);
        BitBlt(hdc,0,0,cr.right,cr.bottom,mem,0,0,SRCCOPY);
        SelectObject(mem,old); DeleteObject(bmp); DeleteDC(mem);
        EndPaint(hwnd,&ps); break;
    }
    case WM_TIMER:
        if(wp==TIMER_ID){ KillTimer(hwnd,TIMER_ID); ResetGame(); InvalidateRect(hwnd,NULL,FALSE); }
        break;
    case WM_KEYDOWN:
        if(wp==VK_ESCAPE) PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:{
        int mx=LOWORD(lp), my=HIWORD(lp);
        if(!gameOver && !isDraw &&
           mx>=gDrawBtn.left && mx<=gDrawBtn.right &&
           my>=gDrawBtn.top  && my<=gDrawBtn.bottom){
            isDraw=true; pieceSelected=false;
            statusMsg="Draw agreed! Restarting in 3s...";
            InvalidateRect(hwnd,NULL,FALSE);
            SetTimer(hwnd,TIMER_ID,3000,NULL); break;
        }
        if(gameOver||isDraw) break;
        int col=(mx-gOX)/gCELL, row=(my-gOY)/gCELL;
        if(row<0||row>7||col<0||col>7){ pieceSelected=false; InvalidateRect(hwnd,NULL,FALSE); break; }
        if(pieceSelected && row==selRow && col==selCol){ pieceSelected=false; InvalidateRect(hwnd,NULL,FALSE); break; }
        if(!pieceSelected){
            char sym=gBoard->getSymbolAt(row,col); if(!sym) break;
            bool iw=(sym>='A'&&sym<='Z');
            if(whiteTurn&&!iw) break; if(!whiteTurn&&iw) break;
            pieceSelected=true; selRow=row; selCol=col;
        } else {
            bool moved=gBoard->movePiece(selRow,selCol,row,col);
            pieceSelected=false;
            if(moved){
                if(gBoard->kingCaptured){
                    gameOver=true;
                    statusMsg=whiteTurn?"WHITE WINS! Restarting in 3s...":"BLACK WINS! Restarting in 3s...";
                    SetTimer(hwnd,TIMER_ID,3000,NULL);
                } else whiteTurn=!whiteTurn;
            }
        }
        InvalidateRect(hwnd,NULL,FALSE); break;
    }
    case WM_DESTROY: PostQuitMessage(0); break;
    default: return DefWindowProc(hwnd,msg,wp,lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int){
    gBoard=new Board();
    gSW=GetSystemMetrics(SM_CXSCREEN);
    gSH=GetSystemMetrics(SM_CYSCREEN);

    int bp=(int)(min(gSW,gSH)*0.80);
    gCELL=bp/8; bp=gCELL*8;
    gOX=(gSW-bp)/2;
    gOY=(gSH-bp)/2;

    WNDCLASSEXA wc={};
    wc.cbSize=sizeof(wc); wc.lpfnWndProc=WndProc;
    wc.hInstance=hi; wc.lpszClassName="Chess";
    wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    RegisterClassExA(&wc);

    HWND hwnd=CreateWindowExA(
        WS_EX_TOPMOST,"Chess","Chess",
        WS_POPUP,
        0,0,gSW,gSH,
        NULL,NULL,hi,NULL);

    ShowWindow(hwnd,SW_SHOW);
    UpdateWindow(hwnd);

    MSG m;
    while(GetMessage(&m,NULL,0,0)){
        TranslateMessage(&m); DispatchMessage(&m);
    }
    delete gBoard;
    return (int)m.wParam;
}
