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
