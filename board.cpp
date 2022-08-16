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

#include "move.h"
#include "piece.h"
#include "board.h"
#include "game.h"
#include "player.h"

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



Move::Move(Point a, Point b, Piece* p) : origin(a), destination(b), piece(p) {
		delta = b - a;
}

int Move::distance() {
		vector<int> dv = delta.abs().vec();
		return *std::max_element(dv.begin(), dv.end());
}


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


