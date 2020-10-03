#ifndef POINT_H
#define POINT_H
#include <vector>
#include <iostream>

using namespace std;

template<class T>
class Point
{
public:
	vector<T> p_coords; //x,y,z...
	Point(T, T);
	Point(T, T, T);
	Point();
	template<class U>friend ostream& operator<<(ostream &, const Point<U>&);
};

#endif
