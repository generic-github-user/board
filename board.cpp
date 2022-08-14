#define FMT_HEADER_ONLY

#include <vector>
#include <string>
//#include <ranges>
#include <functional>
#include <iostream>

//#include <valarray>
#include <algorithm>
#include <iterator>

#include <assert.h>
#include "include/fmt/core.h"
#include "include/fmt/format.h"

using namespace std;

class Move;
class Piece;
class Board;
//class Board {
		//vector<int> dimensions;
		//vector<Piece> pieces;
//};
class Game;
//class Point;

template <class T>
class Vec;

// A location in space, assuming a typical Euclidean space and associated
// coordinate system; sometimes also used to represent an offset or motion
// through space (i.e., a delta)
class Point {
		vector<int> position; // Coordinates of the point

		public:

		Point(vector<int> pos); // Takes a vector and returns a new point
		Point(); // Default point constructor (i.e., (0, 0))
		Point(int x, int y); // Takes an x and y coordinate and returns a new point

		int& operator[] (int index); // Returns the component along the specified axis
		bool operator== (Point& other); // Test point equality (by value)
		Point operator+ (Point other); // Elementwise addition of points
		Point operator- (Point other); // Elementwise subtraction of points
		Point clone(); // Returns a copy of this point
		Point abs(); // Applies the absolute value function to each axis
		string tostring(); // Generate a short string representation of the point
		vector<int> vec(); // Convert this point to a vector
};

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

// A space in which a game takes place; wraps a set of pieces and some topology
// that defines how they are able to move (along with the game's rules)
class Board {
		vector<int> dimensions;
		vector<Piece> pieces;
		//Piece*[][]

		public:

		Board(vector<int> dims, vector<Piece> p);
		Board* add(Piece p);
		vector<Piece*> filter(function<bool(Piece)> f);
		vector<Piece*> filter(Point p);
		bool free(Point p);

		string summary();
		string tostring();
		Board* print();

		Board* reflect(int axis);
		Board* reflect(int axis, Player* p);

		Board* move(Move m);
		Board clone();
};

// A numeric range (start <= stop)
class Range {
		int start, stop;
		Range(int a, int b) : start(a), stop(b) {}
};


// A generic vector class that wraps `std::vector` and provides some additional
// functionality
template <class T>
class Vec {
		public:

		vector<T> v;
		Vec(vector<T> value) : v(value) {}
		Vec() : v(vector<T>()) {}

		Vec* push(T x) {
				v.push_back(x);
				return this;
		}

		Vec<T> abs() {
				Vec<T> output;
				for (T x : v) output.push(std::abs(x));
				return output;
		}
};

template <class T>
//Vec<T> operator+ (Vec<T>& a, Vec<T>& b) {
//Vec<T> add (Vec<T>& a, Vec<T>& b) {
Vec<T> operator+ (Vec<T> a, Vec<T> b) {
		Vec<T> output;
		for (int i=0; i<a.v.size(); i++) output.push(a.v[i] + b.v[i]);
		return output;
}

template <class T>
Vec<T> operator- (Vec<T> a, Vec<T> b) {
		Vec<T> output;
		for (int i=0; i<a.v.size(); i++) output.push(a.v[i] - b.v[i]);
		return output;
}



Point::Point(vector<int> pos) : position(pos) {}
Point::Point() : position({0, 0}) {}
Point::Point(int x, int y) : position({x, y}) {}

int& Point::operator[] (int index) { return position[index]; }
bool Point::operator== (Point& other) { return position == other.position; }
Point Point::operator+ (Point other) {
		//return vector<int>(valarray<int>(position) + valarray<int>(other.position));
		//return Point(x+other.x, y+other.y);
		//return (add(Vec<int>(position), Vec<int>(other.position))).v;
		return (Vec<int>(position) + Vec<int>(other.position)).v;
}
Point Point::operator- (Point other) {
		return (Vec<int>(position) - Vec<int>(other.position)).v;
}
