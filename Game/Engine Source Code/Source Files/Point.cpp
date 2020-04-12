#include "Point.h"
#include <iostream>

using namespace std;

Point::Point(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

Point::~Point()
{
	
}

ostream &operator <<(ostream &stream, Point &p)
{
	stream << p.x << +" " << p.y << " " << p.z;
	return stream;
}

Point& Point::operator=(Point &p)
{
	x = p.x;
	y = p.y;
	z = p.z;

	return *this;
}