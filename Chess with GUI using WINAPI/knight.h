#ifndef knight_h
#define knight_h
#include<cmath>
#include "peice.h"

class Knight :public Peice {
public:
	Knight(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'N';

		return 'n';
	}

	bool isValidMove(int sr, int sc, int dr, int dc){
		int r = abs(sr - dr);
		int c = abs(sc - dc);
		if((r==1 && c==2) || (r==2 && c==1)) return true;
		return false;
	}
};

#endif knight.h
