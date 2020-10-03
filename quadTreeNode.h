#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <vector>
#include <iostream>
#include "point.h"
#include "point.cpp"

using namespace std;

template<class T>
class QTNode
{
public:
	double qtn_min_x, qtn_min_y, qtn_max_x, qtn_max_y, qtn_min_z,qtn_max_z;
	QTNode<T> *qtn_sons[8];
	vector<Point<T>> qtn_points;
	bool qtn_is_leaf;

	QTNode(double, double, double, double, double, double);

	template<class U>friend ostream& operator<<(ostream &, const QTNode<U>&);

};


#endif