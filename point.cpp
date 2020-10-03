#include "point.h"


template<class T>
Point<T>::Point(T x, T y)
{
	p_coords.push_back(x);
	p_coords.push_back(y);
}

template<class T>
Point<T>::Point()
{
}

template<class T>
Point<T>::Point(T x, T y, T z)
{
	p_coords.push_back(x);
	p_coords.push_back(y);
	p_coords.push_back(z);
}

template<class T>
ostream& operator<<(ostream& os, const Point<T>& cp)
{
	os << "( ";
	for (int i = 0; i < cp.p_coords.size()-1; i++)
		os << cp.p_coords[i] << ",";
	os << cp.p_coords[cp.p_coords.size() - 1] << " )";
	return os;
}