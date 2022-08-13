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

class Point {
		vector<int> position;

		public:

		Point(vector<int> pos);
		Point();
		Point(int x, int y);

		int& operator[] (int index);
		bool operator== (Point& other);
		Point operator+ (Point other);
		Point operator- (Point other);
		Point clone();
		Point abs();
		string tostring();
		vector<int> vec();
};

class Player {
		string name;
		string color;
		vector<Piece*> pieces;

		public:

		vector<Move*> moves();
		
		Player(string n, string c);
		Player* add(Piece* p);
		string tostring();
};

class Piece {
		public:

		string name;
		string symbol;
		Point position;
		vector<Move*> history;

		Board* board;
		Player* player;

		Piece(string n, string s, Point pos);
		Piece(string n, string s, Point pos, Player* p);
		vector<Move*> traverse(Point pos, Point delta, vector<Move*> movelist);
		vector<Move*> moves();
		string tostring();
		Piece clone();
};

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

class Range {
		int start, stop;
		Range(int a, int b) : start(a), stop(b) {}
};


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
