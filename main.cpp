#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "quadTree.h"
#include "QuadTree.cpp"
#include <fstream>
#include <string>
#define KEY_ESC 27

	using namespace std;

int mx = 0;
int my = 0;

float ax = 0.0;
float ay = 0.0;
float speed = 0.1;

QTree<double> *tree = new QTree<double>(-100, 100, 100, -100, 100, -100, 1);

/*
void t1()
{
	ifstream fe("bunnyPoints.txt");
	ofstream fs("bunnyCoords.txt");
	string cadena;
	int cont = 0;
	while (!fe.eof())
	{
		fe >> cadena;
		for (int i = cadena.size() - 1; i >= 0; i--)
		{
			if (cadena[i] == '[' or cadena[i] == ']' or cadena[i] == ',')
				cadena.erase(i, 1);

		}
		cont++;
		fs << cadena << " ";
		if (cont == 3)
		{
			fs << "\n";
			cont = 0;
		}
	}
	fe.close();
	fs.close();
}
*/

void t2()
{
	ifstream fe("bunnyCoords.txt");
	double cx,cy,cz;

	while (!fe.eof())
	{
		fe >> cx >> cy >> cz;
		cx *= 50; cy *= 50; cz *= 50;
		Point<double> temp(cx, cy, cz);
		tree->insert(temp);
	}
	fe.close();
}

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255, 0, 0);
	glVertex3d(-30, 0, 0);
	glVertex3d(30, 0, 0);
	glColor3d(0, 255, 0);
	glVertex3d(0, -30, 0);
	glVertex3d(0, 30, 0);
	glColor3d(0, 0, 255);
	glVertex3d(0, 0, -30);
	glVertex3d(0, 0, 30);
	glEnd();
}

bool r = false;

void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mx = x;
		my = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		r = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		r = false;
	}
}

void OnMouseMotion(int x, int y)
{
	int dx = mx - x;
	int dy = my - y;

	mx = x;
	my = y;

	ax += dx * speed;
	ay += dy * speed;
}



void idle() { // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	gluPerspective(100.0, 1.0, 1.0, 1000.0);

	glTranslatef(0, 0, -100.0);
	glRotatef(ax, 0, 1, 0);
	glRotatef(ay, 1, 0, 0);

	//displayGizmo();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glutSolidTeapot(7);

	glTranslatef(20, 20, 0);
	//glutSolidCube(10);
	//dibujar quadTree (qt->draw())
	tree->draw1();
	tree->draw(tree->qt_root);
	/*glBegin(GL_POINTS);
	for()
		glVertex2f(v.at(i)[0],v.at(i)[1])
	glEnd();*/
	//dibuja el gizmo

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glEnable(GL_DEPTH_TEST);
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {
	
	/*int limite;
	cout << "ingrese el limite pr cuadrante: ";
	cin >> limite;*/
	tree->qt_point_limit = 100;/*limite;*/
	t2();

	/*tree->insert(Point<double>(2, 4, 5));
	tree->insert(Point<double>(52, 64, 33)); 
	tree->print(tree->qt_root);
	system("pause");
*/
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("OctTree"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);


	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	

	return 0;
}