#ifndef queen_h
#define queen_h
#include<cmath>
#include "peice.h"

class Queen :public Peice {
public:
	Queen(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'Q';

		return 'q';
	}

	bool isValidMove(int sr, int sc, int dr, int dc){
		if((sr == dr) || (sc == dc)) return true;

		if(abs(sr - dr) == abs(sc - dc)) return true;

		return false;
	}

	bool isSlider() {
     	return true;
	}
};

#endif queen.h
