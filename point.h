#include <string>
#include <vector>
using namespace std;

#ifndef point_h
#define point_h

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

#endif
