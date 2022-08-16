#ifndef board_h
#define board_h

#include <functional>
#include "piece.h"
#include "point.h"

// A space in which a game takes place; wraps a set of pieces and some topology
// that defines how they are able to move (along with the game's rules)
class Board {
		vector<int> dimensions; // Shape of the board (will eventually support non-rectangular topologies)
		vector<Piece> pieces; // List of pieces on the board
		//Piece*[][]

		public:

		Board(vector<int> dims, vector<Piece> p); // Board constructor
		Board* add(Piece p); // Add a piece to this board
		vector<Piece*> filter(function<bool(Piece)> f); // Return pointers to the pieces that match the predicate function `f`
		vector<Piece*> filter(Point p); // Return pointers to the pieces at position `p`
		bool free(Point p); // Check whether any pieces are present at `p`

		string summary(); // Returns an abridged string representation of the Board
		string tostring(); // Returns a string summarizing an instance of this class
		Board* print(); // Prints this Board instance to stdout

		Board* reflect(int axis); // Clones this board's pieces (into itself), reflecting over the given axis
		Board* reflect(int axis, Player* p); // Clones this board's pieces (into itself), reflecting over the given axis; sets the cloned piece's owner to `p`

		Board* move(Move m); // Executes move `m`
		Board clone(); // Returns a copy of this instance
};

#endif
