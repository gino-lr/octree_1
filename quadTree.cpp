#include "quadTree.h"
#include<GL/glut.h>

template<class T>
QTree<T>::QTree(double min_x, double min_y, double max_x, double max_y, double min_z, double max_z, int limit)
{
	qt_min_x = min_x;
	qt_min_y = min_y;
	qt_max_x = max_x;
	qt_max_y = max_y;
	qt_min_z = min_z;
	qt_max_z = max_z;

	qt_point_limit = limit;

	qt_root = new QTNode<T>(min_x, min_y, max_x, max_y,min_z,max_z);
}

template<class T>
bool QTree<T>::find(Point<T> x,QTNode<T> * &p)
{
	int aux_hijo;
	for (p = qt_root; !p->qtn_is_leaf; p = p->qtn_sons[aux_hijo])
	{
		if (x.p_coords[0] <= p->qtn_sons[0]->qtn_max_x)
		{
			if (x.p_coords[1] >= p->qtn_sons[0]->qtn_max_y)
				aux_hijo = 0;
			else
				aux_hijo = 2;
		}
		else
		{
			if (x.p_coords[1] >= p->qtn_sons[1]->qtn_max_y)
				aux_hijo = 1;
			else
				aux_hijo = 3;
		}
		if (x.p_coords[2] < p->qtn_sons[3]->qtn_max_z)
			aux_hijo += 4;
	}

	for (unsigned i = 0; i < p->qtn_points.size(); i++)
	{
		if (x.p_coords[0] == p->qtn_points[i].p_coords[0] and x.p_coords[1] == p->qtn_points[i].p_coords[1]
			and x.p_coords[2] == p->qtn_points[i].p_coords[2]) return 1;
	}
	return 0;
}

template<class T>
bool QTree<T>::insert(Point<T> x)
{
	QTNode<T> *p;
	if (find(x,p)) return 0; 

	p->qtn_points.push_back(x);

	if (p->qtn_points.size() > qt_point_limit)
		split(p);

	return 1;
}

template<class T>
void QTree<T>::split(QTNode<T> *p)
{
	p->qtn_is_leaf = false;
	double rx, ry, rz;
	int aux_hijo;
	rx = (p->qtn_max_x + p->qtn_min_x) / 2;
	ry = (p->qtn_max_y + p->qtn_min_y) / 2;
	rz = (p->qtn_max_z + p->qtn_min_z) / 2;

	p->qtn_sons[0] = new QTNode<T>(p->qtn_min_x,p->qtn_min_y,rx,ry,p->qtn_min_z,rz);
	p->qtn_sons[1] = new QTNode<T>(rx,p->qtn_min_y, p->qtn_max_x, ry, p->qtn_min_z, rz);
	p->qtn_sons[2] = new QTNode<T>(p->qtn_min_x, ry, rx, p->qtn_max_y, p->qtn_min_z, rz);
	p->qtn_sons[3] = new QTNode<T>(rx, ry, p->qtn_max_x, p->qtn_max_y, p->qtn_min_z, rz);
	p->qtn_sons[4] = new QTNode<T>(p->qtn_min_x, p->qtn_min_y, rx, ry, rz,p->qtn_max_z);
	p->qtn_sons[5] = new QTNode<T>(rx, p->qtn_min_y, p->qtn_max_x, ry, rz, p->qtn_max_z);
	p->qtn_sons[6] = new QTNode<T>(p->qtn_min_x, ry, rx, p->qtn_max_y, rz, p->qtn_max_z);
	p->qtn_sons[7] = new QTNode<T>(rx, ry, p->qtn_max_x, p->qtn_max_y, rz, p->qtn_max_z);


	for (unsigned i = 0; i < p->qtn_points.size(); i++)
	{
		if (p->qtn_points[i].p_coords[0] <= p->qtn_sons[0]->qtn_max_x)
		{
			if (p->qtn_points[i].p_coords[1] >= p->qtn_sons[0]->qtn_max_y)
				aux_hijo = 0;
			else
				aux_hijo = 2;
		}
		else
		{
			if (p->qtn_points[i].p_coords[1] >= p->qtn_sons[1]->qtn_max_y)
				aux_hijo = 1;
			else
				aux_hijo = 3;
		}
		if (p->qtn_points[i].p_coords[2] < p->qtn_sons[3]->qtn_max_z)
			aux_hijo += 4;
		p->qtn_sons[aux_hijo]->qtn_points.push_back(p->qtn_points[i]);
	}

	p->qtn_points.clear();
	//si todos los puntos se van para un lado
	for (unsigned i = 0; i < 8; i++)
	{
		if (p->qtn_sons[i]->qtn_points.size() > qt_point_limit)
		{
			split(p->qtn_sons[i]);
			break;
		}
	}
}


template<class T>
void QTree<T>::print(QTNode<T> *p)
{
	cout << *p << endl;

	if (!p->qtn_is_leaf)
		for (int i = 0; i < 8; i++)
			print(p->qtn_sons[i]);
}

template<class T>
void QTree<T>::draw1()
{
	QTNode<T> *p = qt_root;
	if (p->qtn_is_leaf)
	{
		for (unsigned i = 0; i < p->qtn_points.size(); i++)
		{
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glColor3f(0, 0, 255);
			glVertex3d(p->qtn_points[i].p_coords[0], p->qtn_points[i].p_coords[1], p->qtn_points[i].p_coords[2]);
			glEnd();
		}
		return;
	}
	double rx, ry, rz;
	rx = (p->qtn_max_x + p->qtn_min_x) / 2;
	ry = (p->qtn_max_y + p->qtn_min_y) / 2;
	rz = (p->qtn_max_z + p->qtn_min_z) / 2;

	glBegin(GL_LINES);
	//horizontales
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, p->qtn_max_z);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, p->qtn_max_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, p->qtn_max_z);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, p->qtn_max_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, p->qtn_min_z);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, p->qtn_min_z);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, p->qtn_min_z);


	//verticales
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, p->qtn_max_z);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, p->qtn_max_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, p->qtn_max_z);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, p->qtn_max_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, p->qtn_min_z);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, p->qtn_min_z);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, p->qtn_min_z);

	//diagonales
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, p->qtn_max_z);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, p->qtn_max_z);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, p->qtn_max_z);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, p->qtn_max_z);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, p->qtn_min_z);

	glEnd();
}

template<class T>
void QTree<T>::draw(QTNode<T> *p)
{
	if (p->qtn_is_leaf)
	{
		for (unsigned i = 0; i < p->qtn_points.size(); i++)
		{
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glColor3f(0, 0, 255);
			glVertex3d(p->qtn_points[i].p_coords[0], p->qtn_points[i].p_coords[1], p->qtn_points[i].p_coords[2]);
			glEnd();
		}
		return;
	}

	//*
	double rx, ry, rz;
	rx = (p->qtn_max_x + p->qtn_min_x) / 2;
	ry = (p->qtn_max_y + p->qtn_min_y) / 2;
	rz = (p->qtn_max_z + p->qtn_min_z) / 2;

	glBegin(GL_LINES);
	//horizontales

	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, rz);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, rz);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, rz);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, rz);

	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, ry, p->qtn_max_z);
	glVertex3d(p->qtn_max_x, ry, p->qtn_max_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, ry, rz);
	glVertex3d(p->qtn_max_x, ry, rz);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, ry, p->qtn_min_z);
	glVertex3d(p->qtn_max_x, ry, p->qtn_min_z);

	//verticales
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, p->qtn_min_y, rz);
	glVertex3d(p->qtn_min_x, p->qtn_max_y, rz);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_max_x, p->qtn_min_y, rz);
	glVertex3d(p->qtn_max_x, p->qtn_max_y, rz);

	glColor3d(255, 0, 0);
	glVertex3d(rx, p->qtn_min_y, p->qtn_max_z);
	glVertex3d(rx, p->qtn_max_y, p->qtn_max_z);
	glColor3d(255, 0, 0);
	glVertex3d(rx, p->qtn_min_y, rz);
	glVertex3d(rx, p->qtn_max_y, rz);
	glColor3d(255, 0, 0);
	glVertex3d(rx, p->qtn_min_y, p->qtn_min_z);
	glVertex3d(rx, p->qtn_max_y, p->qtn_min_z);

	//diagonales
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_min_x, ry, p->qtn_max_z);
	glVertex3d(p->qtn_min_x, ry, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(p->qtn_max_x, ry, p->qtn_max_z);
	glVertex3d(p->qtn_max_x, ry, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(rx, p->qtn_min_y, p->qtn_max_z);
	glVertex3d(rx, p->qtn_min_y, p->qtn_min_z);
	glColor3d(255, 0, 0);
	glVertex3d(rx, p->qtn_max_y, p->qtn_max_z);
	glVertex3d(rx, p->qtn_max_y, p->qtn_min_z);

	glEnd();
	//*/
	for (unsigned i = 0; i < 8; i++)
	{
		draw(p->qtn_sons[i]);
	}
}