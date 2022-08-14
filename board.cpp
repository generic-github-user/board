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

Point Point::clone() { return Point(position); }
Point Point::abs() { return Point(Vec<int>(position).abs().v); }
vector<int> Point::vec() { return position; }

string Point::tostring() { return fmt::format("Point [{}]", fmt::join(position, ", ")); }
//string Point::tostring() { return ""; }


Player::Player(string n, string c) : name(n), color(c) {}

vector<Move*> Player::moves() {
		vector<Move*> m;
		for (Piece* p : pieces) {
				for (Move* x : p -> moves()) m.push_back(x);
		}
		return m;
}

Player* Player::add(Piece* p) {
		pieces.push_back(p);
		return this;
}

string Player::tostring() {
		return fmt::format("Player [{}, {}, {} moves, {} pieces: []]",
				name, color, moves().size(), pieces.size());
}

class Move {
		public:

		Point origin;
		Point destination;
		bool capture;
		Piece* piece;
		Point delta;

		Move(Point a, Point b, Piece* p) : origin(a), destination(b), piece(p) {
				delta = b - a;
		}
		
		int distance() {
				vector<int> dv = delta.abs().vec();
				return *std::max_element(dv.begin(), dv.end());
		}
};


Piece::Piece(string n, string s, Point pos) : name(n), symbol(s), position(pos) { }

Piece::Piece(string n, string s, Point pos, Player* p) : name(n), symbol(s), position(pos), player(p) {
		//p -> pieces.push_back(this);
		p -> add(this);
}

//Piece* add(Move m) {

vector<Move*> Piece::traverse(Point pos, Point delta, vector<Move*> movelist) {
		Point next = pos + delta;
		if (board->free(next)) {
				movelist.push_back(new Move(pos, next, this));
				traverse(next, delta, movelist);
		}
		return movelist;
}

vector<Move*> Piece::moves() {
		fmt::print("Getting moves for {}\n", tostring());
		vector<Move*> m;
		//int x = position[0];
		//int y = position[1];

		//if (name == "pawn") {
		//		//fmt::print("pawn");
		//		if (history.size() == 0) m.push_back(new Move(position, position+Point(0, 2), this));
		//		m.push_back(new Move(position, position+Point(0, 1), this));
		//}
		//if (name == "rook" || name == "queen") {
		//		//for (int i=x; i<board->dimensions[0]; i++) { }
		//		traverse(position, Point(1, 0), m);
		//		traverse(position, Point(-1, 0), m);
		//		traverse(position, Point(0, 1), m);
		//		traverse(position, Point(0, -1), m);
		//}
		//if (name == "knight") {}
		//if (name == "king") {}
		return m;
}

string Piece::tostring() {
		return fmt::format("Piece [{}, {}, {}]", name, symbol, position.tostring());
		//return "";
}

Piece Piece::clone() {
		return Piece(name, symbol, position.clone(), player);
}

//Board::Board(vector<int> dims)

Board::Board(vector<int> dims, vector<Piece> p = {}) : dimensions(dims), pieces(p) {

}

Board* Board::add(Piece p) {
		pieces.push_back(p);
		return this;
}

//vector<Piece*> filter(bool(*f)(Piece p)) {
vector<Piece*> Board::filter(function<bool(Piece)> f) { // ?
		vector<Piece*> result;
		//for (Piece p : pieces) if (f(p)) result.push_back(&p);
		for (Piece& p : pieces) if (f(p)) result.push_back(&p);
		return result;
}

vector<Piece*> Board::filter(Point p) {
		return filter([&](Piece x) -> bool{ return x.position == p; });
}

bool Board::free(Point p) {
		return filter(p).size() == 0;
}

string Board::summary() {
		return fmt::format("Board [{} pieces]\n", pieces.size());
}

string Board::tostring() {
		//for (int i : dimensions) std::cout << i;
		string result = "";
		vector<Piece*> matches;
		vector<int> D = dimensions;
		result += fmt::format("Board [{} pieces]\n", pieces.size());
		for (int y=0; y<D[1]; y++) {
				for (int x=0; x<D[0]; x++) {
						matches = filter(Point(x, y));
						//if (matches.size() > 0) std::cout << (*matches[0]).clone().symbol;
						//if (matches.size() > 0) { std::cout << (matches.size()); }
						//if (matches.size() > 0) { std::cout << std::string(matches[0] -> symbol); }
						//if (matches.size() > 0) { result.append(std::string(matches.at(0) -> symbol)); }

						if (matches.size() > 0) result += (matches.at(0) -> symbol);
						else result += " ";
				}
				result += "\n";
		}
		return result;
}

Board* Board::print() {
		std::cout << tostring();
		return this;
}

Board* Board::reflect(int axis) {
		//for (Piece& p : pieces) {
		int n = pieces.size();
		for (int i=0; i<n; i++) {
				//Piece p = pieces[i];
				Piece np = pieces[i].clone();
				np.position[axis] = dimensions[axis] - pieces[i].position[axis] - 1;
				add(np);
		}
		return this;
}

Board* Board::reflect(int axis, Player* p) {
		int n = pieces.size();
		for (int i=0; i<n; i++) {
				Piece np = pieces[i].clone();
				np.player = p;
				np.position[axis] = dimensions[axis] - pieces[i].position[axis] - 1;
				add(np);
		}
		return this;
}

Board* Board::move(Move m) {
		m.piece -> position = m.destination;
		return this;
}

Board Board::clone() {
		Board result = Board(vector<int>(dimensions), vector<Piece>(pieces));
		return result;
}


class Game {
		protected:

		Board board;
		vector<Move> history;

		int ply = 0;
		int turn = 0;
		vector<Player> players;
		Player* current;

		public:
};
};
