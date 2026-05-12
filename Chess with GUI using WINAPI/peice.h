#ifndef peice_h
#define peice_h

class Peice {
protected:
	bool white;
public:
	Peice(bool w) {
		white = w;
	}
	bool iswhite (){
		return white;
	}
	virtual bool isSlider() {
		return false;
	}
	virtual bool isValidMove(int sr, int sc, int dr, int dc) = 0;
	virtual char getsymbol() = 0;
	~Peice() {};
};

#endif 
