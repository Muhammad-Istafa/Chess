#ifndef queen_h
#define queen_h
#include "peice.h"

class Queen :public Peice {
public:
	Queen(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'Q';

		return 'q';
	}
};

#endif queen.h