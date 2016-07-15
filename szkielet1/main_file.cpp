/*
Niniejszy program jest wolnym oprogramowaniem; mo�esz go
rozprowadza� dalej i / lub modyfikowa� na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacj� Wolnego
Oprogramowania - wed�ug wersji 2 tej Licencji lub(wed�ug twojego
wyboru) kt�rej� z p�niejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej�, i� b�dzie on
u�yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domy�lnej
gwarancji PRZYDATNO�CI HANDLOWEJ albo PRZYDATNO�CI DO OKRE�LONYCH
ZASTOSOWA�.W celu uzyskania bli�szych informacji si�gnij do
Powszechnej Licencji Publicznej GNU.

Z pewno�ci� wraz z niniejszym programem otrzyma�e� te� egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
je�li nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include <iostream>
#include "allmodels.h"
#include "bullet.h"
#include "shipNextMove.h"
#include "shipState.h"
#include "bullet.h"
#include "target.h"

Models::Torus myTorus(1.7f, 0.3f, 36, 36);
shipNextMove nextMove;
shipState ship;
const int howManyTargets = 15;

const int randTargetRange = 50;
Target targets[howManyTargets];



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
	nextMove.setMove(key, action);
}

//Procedura obs�ugi b��d�w
void error_callback(int error, const char* description) {
	fputs(description, stderr);}

//Procedura inicjuj�ca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************
	glClearColor(0, 0, 0, 1); //Czy�� ekran na czarno
	//glEnable(GL_LIGHTING); //W��cz tryb cieniowania
	glEnable(GL_LIGHT0); //W��cz domyslne �wiat�o
	glEnable(GL_DEPTH_TEST); //W��cz u�ywanie Z-Bufora
	glEnable(GL_COLOR_MATERIAL); //glColor3d ma modyfikowa� w�asno�ci materia�u
	
}


void addObjects() {
	if (nextMove.shoot) {
		ship.shoot();
		nextMove.shoot = false;
	}
}

void moveObjects() {
	for (int i = 0; i < ship.howManyBullets; i++) {
		if (ship.bullets[i].alive) {
			ship.bullets[i].m = translate(ship.bullets[i].m, glm::vec3(0.f,0.f,-1.f) * 0.9f);
		}
	}
}

//Procedura rysuj�ca zawarto�� sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************l
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolor�w
	glm::mat4 P = glm::perspective(50 * PI / 180, 1.0f, 0.05f, 50.0f); //Wylicz macierz rzutowania

	//Za�aduj macierze do OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	glColor3d(0, 1, 0); //Rysuj na zielono	

	glm::mat4 mt0 = glm::mat4(1.0f);
	mt0 = translate(mt0, glm::vec3(2, 0, 0));

	glLoadMatrixf(glm::value_ptr(ship.view*mt0));

	myTorus.drawWire(); //Narysuj torus

	glm::mat4 mt1 = glm::mat4(1.0f);
	mt1 = translate(mt1, glm::vec3(-2, 0, 0));

	glLoadMatrixf(glm::value_ptr(ship.view*mt1));

	myTorus.drawWire(); //Narysuj torus
	for (int i = 0; i < ship.howManyBullets; i++) {
		if (ship.bullets[i].alive) {
			glLoadMatrixf(glm::value_ptr(ship.view*ship.bullets[i].m));
			myTorus.drawWire();		
		}
	}

	for (int i = 0; i < howManyTargets; i++) {
			glLoadMatrixf(glm::value_ptr(ship.view*targets[i].m));
			myTorus.drawWire();
	}


	//Przerzu� tylny bufor na przedni
	glfwSwapBuffers(window);

}

void initTargets() {
	for (int i = 0; i < howManyTargets; i++) {
		glm::mat4 m = glm::mat4(1.0f);
		glm::vec3 pos = glm::vec3((rand() % randTargetRange) - randTargetRange/2, (rand() % randTargetRange) - randTargetRange / 2, (rand() % randTargetRange) - randTargetRange / 2);
		targets[i].m = rotate(targets[i].m, (float) rand(), pos);
		targets[i].m = translate(targets[i].m, pos);
	}
}

void rotateTargets(double time) {
	for (int i = 0; i < howManyTargets; i++) {
		targets[i].m = rotate(targets[i].m, (float)time, glm::vec3(1,0,0));
	}
}

int main(void)
{
	GLFWwindow* window; //Wska�nik na obiekt reprezentuj�cy okno
	std::cout << "lol"<<std::endl;

	glfwSetErrorCallback(error_callback);//Zarejestruj procedur� obs�ugi b��d�w

	if (!glfwInit()) { //Zainicjuj bibliotek� GLFW
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE); 
	}

	window = glfwCreateWindow(700, 700, "OpenGL", NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) //Je�eli okna nie uda�o si� utworzy�, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
	glfwSwapInterval(1); //Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora
	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotek� GLEW
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj�ce

	float angle = 0; //K�t obrotu torusa
	glfwSetTime(0); //Wyzeruj licznik czasu

	initTargets();

	//G��wna p�tla
	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		//std::cout << glfwGetTime()<<std::endl;
		ship.moveCamera(glfwGetTime(), nextMove);
		rotateTargets(glfwGetTime());
		addObjects();
		moveObjects();
		glfwSetTime(0);
		drawScene(window,angle); //Wykonaj procedur� rysuj�c�
		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
		//Wyzeruj licznik czasu
	}

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}