#include "move.h"
#include "point.h"
#include "player.h"

#ifndef piece_h
#define piece_h

// Represents a discrete unit that exists on a Board; may or may not be able to
// move or interact with other pieces, depending on the rules of the game
class Piece {
		public:

		string name; // The piece's name (specifically, its type)
		string symbol; // A symbol (ideally a single character) used to represent this piece, e.g. when rendering ASCII representations of game boards
		Point position; // Location of the piece on the board
		vector<Move*> history;

		Board* board; // The game board this piece "exists" within
		Player* player; // A reference to the piece's owner or the player associated with it in some sense

		Piece(string n, string s, Point pos); // Construct a piece not associated with a player
		Piece(string n, string s, Point pos, Player* p); // Construct a piece associated with a player
		vector<Move*> traverse(Point pos, Point delta, vector<Move*> movelist); // Attempts a "virtual move" in the direction specified by `delta` until a stopping condition is met; each intermediate position is added to `movelist`
		vector<Move*> moves(); // Returns a list of (references to) available moves for this piece
		string tostring(); // Returns a string summarizing an instance of this class
		Piece clone(); // Returns a copy of this instance
};

#endif
