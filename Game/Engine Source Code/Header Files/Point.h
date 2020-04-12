#ifndef POINT_H
#define POINT_H

#include <iostream>

using namespace std;

class Point
{
public:
	Point(float newX = 0.0f, float newY = 0.0f, float newZ = 0.0f);
	~Point();

	float x;
	float y;
	float z;

	Point& operator = (Point &p);

};

ostream &operator <<(ostream &stream, Point &p);

#endif
