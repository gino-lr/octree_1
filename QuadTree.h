#ifndef QUADTREE_H
#define QUADTREE_H
#include "quadTreeNode.h"
#include "quadTreeNode.cpp"


template<class T>
class QTree
{
public:
	double qt_min_x, qt_min_y, qt_max_x, qt_max_y,qt_min_z,qt_max_z;
	int qt_point_limit;
	QTNode<T> *qt_root;

	QTree(double,double,double,double, double, double, int);
	bool find(Point<T>,QTNode<T> * &);
	bool insert(Point<T>);
	void split(QTNode<T> *);
	void find_by_region(QTNode<T>*, Point<T>, Point<T>, vector<Point<T>> &);

	void draw1();
	void print(QTNode<T> *);
	void draw(QTNode<T> *);
};



#endif