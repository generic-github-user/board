#ifndef move_h
#define move_h

#include "point.h"
#include "piece.h"

class Move {
		public:

		Point origin;
		Point destination;
		bool capture;
		Piece* piece;
		Point delta;

		Move(Point a, Point b, Piece* p);
		int distance();
};

#endif
