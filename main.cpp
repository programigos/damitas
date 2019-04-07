#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "include/Fichita.h"

#define KEY_ESC 27
#define KEY_NEXT 9
using namespace std;

bool player = true;
vector<Fichita *> fichas(24);
void loadFichitas(){
    bool color = 0;
    int cont =0;
    int ii;
    int jj;
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            if ((i+j+1)%2 == 0 && i != 3 && i != 4){
                ii=(75*i + 32)-300;
                jj=(75*j + 32)-300;
                fichas[cont] = new Fichita(jj,ii,color);
                cont++;
            }
        }
        if(i>=3)color = 1;
    }
}
void drawFicha(){
    ///cout<<fichas.size()<<endl;
    for(size_t i =0;i<fichas.size();++i){
        glPushMatrix();
            glPointSize(15.0f);
            if(fichas[i]->color)
                glColor3f(0.0f,0.0f,1.0f);
            else
                glColor3f(1.0f,0.0f,0.0f);
            glBegin(GL_POINTS);
                ///cout<<fichas[i]->posX<<" , "<<fichas[i]->posY<<" -> "<<fichas[i]->color <<endl;
                glVertex2i(fichas[i]->posX,fichas[i]->posY);
            glEnd();
        glPopMatrix();
    }
}
int l = 75;
void tablero(){
    int x=-300,y=300,X=300,Y=-300;
    for(int f=0;f<8;++f){
        for(int c=0;c<8;++c){
            if((f+c+1) % 2 == 0){
                glColor3f(0.0f,0.0f,0.0f);
                glBegin(GL_QUADS);
                    glVertex2i(x, y);
                    glVertex2i(x, y-l);
                    glVertex2i(x+l, y-l);
                    glVertex2i(x+l, y);
                glEnd();
            }
            else{
                glColor3f(255.0f,255.0f,255.0f);
                glBegin(GL_QUADS);
                    glVertex2i(x, y);
                    glVertex2i(x, y-l);
                    glVertex2i(x+l, y-l);
                    glVertex2i(x+l, y);
                glEnd();
            }
            x+=l;
        }
        y-=l;
        x=-300;
    }
}
void glPaint(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();
	glPushMatrix();
        glColor3f(0.7f,0.35f,0.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
            glVertex2i(-300,300);glVertex2i(-300,-300);
            glVertex2i(-300,-300);glVertex2i(300,-300);
            glVertex2i(300,-300);glVertex2i(300,300);
            glVertex2i(300,300);glVertex2i(-300,300);
        glEnd();
	glPopMatrix();
	tablero();
	drawFicha();
	glutSwapBuffers();
	glutPostRedisplay();
}

void parametrizar(int &x, int &y){
    if(x >= 400){
        x = x - 400;
    }
    else{
        x = (400-x)*(-1);
    }
    if(y <= 400){
        y = 400 - y;
    }
    else{
        y = (y-400)*(-1);
    }
}

float euclidiana(int x, int y, int a, int b){
    return sqrt( pow((x - a),2) + pow((y - b), 2) );
}

int buscar(int x, int y){
    for(int i = 0; i < fichas.size(); ++i){
        if(euclidiana(x,y, fichas[i]->posX, fichas[i]->posY) < 37.5){
            return i;
        }
    }
    return -1;
}

int ficha_seleccionada = -1;

void OnMouseClick(int button, int state, int x, int y){
    parametrizar(x, y);
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        int posicion  = buscar(x,y);
        if(posicion != -1){
            ficha_seleccionada = posicion;
        }
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        x += 300;
        y += 300;
        x /= 75;
        y /= 75;
        if(ficha_seleccionada != -1){
            if ((!fichas[ficha_seleccionada]->color && player)||(fichas[ficha_seleccionada]->color && !player)){
                fichas[ficha_seleccionada]->posX = (75*x + 32)-300;
                fichas[ficha_seleccionada]->posY = (75*y + 32)-300;
                player = !player;
            }
        }
    }
}

void init_GL(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
    glOrtho(-400.0f, 400.0f, -400.0f, 400.0f, -1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
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
	default:{
		break;
		}
	}
}
int main(int argc, char** argv) {
    loadFichitas();
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("DAMAS"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);

	glutMainLoop(); //bucle de rendering

	return 0;
}
