#include "piece.h"

// An "agent" or independent actor participating in a game via moves
class Player {
		string name; // A unique name or identifier used to represent the player
		string color; // The player's color, where applicable
		vector<Piece*> pieces; // A list of references to pieces associated with this player (the actual `Piece` instances are stored in the `Board` class)

		public:

		vector<Move*> moves(); // Returns the set of legal moves currently available to the player
		
		Player(string n, string c); // Initializes a new `Player`
		Player* add(Piece* p); // Add an item to this player's piece list
		string tostring(); // Returns a string summarizing an instance of this class
};
