#include "QuadTreeNode.h"


template<class T>
QTNode<T>::QTNode(double min_x, double min_y, double max_x, double max_y, double min_z, double max_z)
{
	qtn_min_x = min_x;
	qtn_min_y = min_y;
	qtn_max_x = max_x;
	qtn_max_y = max_y;
	qtn_min_z = min_z;
	qtn_max_z = max_z;

	qtn_is_leaf = true;

	for(int i=0;i<8;i++)
		qtn_sons[i]= NULL;

}



template<class T>
ostream& operator<<(ostream& os, const QTNode<T>& cp)
{
	os << "[ ";
	os << cp.qtn_min_x << " " << cp.qtn_min_y << " " << cp.qtn_max_x << " " << cp.qtn_max_y << " " << cp.qtn_min_z << " " << cp.qtn_max_z << " ]";
	return os;
}