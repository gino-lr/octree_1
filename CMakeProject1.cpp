#include <iostream> 
#include <vector> 

using namespace std;

#define arrIzqDel 0 
#define arrDerDel 1 
#define abaDerDel 2 
#define abaIzqDel 3 
#define arrIzqAba 4 
#define arrDerAba 5 
#define abaDerAtr 6 
#define abaIzqAtr 7 

struct Punto {
	int x;
	int y;
	int z;
	Punto() : x(-1), y(-1), z(-1) {}
	Punto(int a, int b, int c) : x(a), y(b), z(c) {}
};

class Octree {
	Punto* punto;
	Punto* ArrIzqDel, * AbaDerAtr;
	vector<Octree*> hijo;
public:	
	Octree(){		
		punto = new Punto();
	}	
	Octree(int x, int y, int z)	{		
		punto = new Punto(x, y, z);
	}	
	Octree(int x1, int y1, int z1, int x2, int y2, int z2){		
		if (x2 < x1 || y2 < y1 || z2 < z1) {
			cout << "pts limites no validos" << endl;
			return;
		}
		punto = nullptr;
		ArrIzqDel = new Punto(x1, y1, z1);
		AbaDerAtr = new Punto(x2, y2, z2);
		
		hijo.assign(8, nullptr);
		for (int i = arrIzqDel; i <= abaIzqAtr;	++i)
			hijo[i] = new Octree();
	}
	
	void insertar(int x,int y,int z){		
		if (buscar(x, y, z)) {
			cout << "El pt ya existe" << endl;
			return;
		}		
		if (x < ArrIzqDel->x || x > AbaDerAtr->x || y < ArrIzqDel->y || y > AbaDerAtr->y || z < ArrIzqDel->z || z > AbaDerAtr->z){
			cout << "Pt fuera del limite" << endl;
			return;
		}		
		int midx = (ArrIzqDel->x + AbaDerAtr->x)/2;
		int midy = (ArrIzqDel->y + AbaDerAtr->y)/2;
		int midz = (ArrIzqDel->z	+ AbaDerAtr->z)/2;
		int pos = -1;		
		if (x <= midx) {
			if (y <= midy) {
				(z <= midz) ? pos = arrIzqDel : pos = arrIzqAba;
			}
			else {
				(z <= midz) ? pos = abaIzqDel : pos = abaIzqAtr;
			}
		}
		else {
			if (y <= midy) {
				(z <= midz) ? pos = arrDerDel : pos = arrDerAba;
			}
			else {
				(z <= midz) ? pos = abaDerDel : pos = abaDerAtr;
			}
		}		
		if (hijo[pos]->punto == nullptr) {
			hijo[pos]->insertar(x, y, z);
			return;
		}
		else if (hijo[pos]->punto->x == -1) {
			delete hijo[pos];
			hijo[pos] = new Octree(x, y, z);
			return;
		}
		else {
			int x_ = hijo[pos]->punto->x,
				y_ = hijo[pos]->punto->y,
				z_ = hijo[pos]->punto->z;
			delete hijo[pos];
			hijo[pos] = nullptr;
			if (pos == arrIzqDel) {
				hijo[pos] = new Octree(ArrIzqDel->x,
					ArrIzqDel->y,
					ArrIzqDel->z,
					midx,
					midy,
					midz);
			}

			else if (pos == arrDerDel) {
				hijo[pos] = new Octree(midx + 1,
					ArrIzqDel->y,
					ArrIzqDel->z,
					AbaDerAtr->x,
					midy,
					midz);
			}
			else if (pos == abaDerDel) {
				hijo[pos] = new Octree(midx + 1,
					midy + 1,
					ArrIzqDel->z,
					AbaDerAtr->x,
					AbaDerAtr->y,
					midz);
			}
			else if (pos == abaIzqDel) {
				hijo[pos] = new Octree(ArrIzqDel->x,
					midy + 1,
					ArrIzqDel->z,
					midx,
					AbaDerAtr->y,
					midz);
			}
			else if (pos == arrIzqAba) {
				hijo[pos] = new Octree(ArrIzqDel->x,
					ArrIzqDel->y,
					midz + 1,
					midx,
					midy,
					AbaDerAtr->z);
			}
			else if (pos == arrDerAba) {
				hijo[pos] = new Octree(midx + 1,
					ArrIzqDel->y,
					midz + 1,
					AbaDerAtr->x,
					midy,
					AbaDerAtr->z);
			}
			else if (pos == abaDerAtr) {
				hijo[pos] = new Octree(midx + 1,
					midy + 1,
					midz + 1,
					AbaDerAtr->x,
					AbaDerAtr->y,
					AbaDerAtr->z);
			}
			else if (pos == abaIzqAtr) {
				hijo[pos] = new Octree(ArrIzqDel->x,
					midy + 1,
					midz + 1,
					midx,
					AbaDerAtr->y,
					AbaDerAtr->z);
			}
			hijo[pos]->insertar(x_, y_, z_);
			hijo[pos]->insertar(x, y, z);
		}
	}	
	bool buscar(int x, int y, int z){		
		if (x < ArrIzqDel->x	|| x > AbaDerAtr->x || y < ArrIzqDel->y || y > AbaDerAtr->y || z < ArrIzqDel->z || z > AbaDerAtr->z)
			return 0;		
		int midx = (ArrIzqDel->x	+ AbaDerAtr->x)/2;
		int midy = (ArrIzqDel->y	+ AbaDerAtr->y)/2;
		int midz = (ArrIzqDel->z + AbaDerAtr->z)/2;
		int pos = -1;
		
		if (x <= midx) {
			if (y <= midy) {
				(z <= midz) ? pos = arrIzqDel : pos = arrIzqAba;
			}
			else {
				(z <= midz) ? pos = abaIzqDel : pos = abaIzqAtr;
			}
		}
		else {
			if (y <= midy) {
				(z <= midz) ? pos = arrDerDel : pos = arrDerAba;
			}
			else {
				(z <= midz) ? pos = abaDerDel : pos = abaDerAtr;
			}
		}		
		if (hijo[pos]->punto == nullptr) {
			return hijo[pos]->buscar(x, y, z);
		}		
		else if (hijo[pos]->punto->x == -1) {
			return 0;
		}
		else {
			if (x == hijo[pos]->punto->x && y == hijo[pos]->punto->y && z == hijo[pos]->punto->z)
				return 1;
		}
		return 0;
	}
};

int main(){	
	return 0;
}
