#ifndef rook_h
#define rook_h
#include "peice.h"

class Rook :public Peice {
public:
	Rook(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'R';

		return 'r';
	}
};

#endif rook.h