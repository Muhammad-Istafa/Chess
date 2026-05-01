#ifndef knight_h
#define knight_h
#include "peice.h"

class Knight :public Peice {
public:
	Knight(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'N';

		return 'n';
	}
};

#endif knight.h