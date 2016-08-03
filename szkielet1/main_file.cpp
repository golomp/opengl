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
#include "lodepng.h"

GLuint tex;


Models::Torus myTorus(1.7f, 0.3f, 36, 36);
Models::Torus miniTorus(0.3f, 0.3f, 10, 10);
shipNextMove nextMove;
shipState ship;
const int howManyTargets = 15;
int points = 0;

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
	glEnable(GL_TEXTURE_2D);
	
								 //Wczytanie do pami�ci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do kt�rych wczytamy wymiary obrazka
							  //Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, "brick.png");

	//Import do pami�ci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_SPHERE_MAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_SPHERE_MAP);
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
	glColor3d(1, 1, 1); //Rysuj na bia�o
	glBindTexture(GL_TEXTURE_2D, tex);

	glm::mat4 mt0 = glm::mat4(1.0f);
	mt0 = translate(mt0, glm::vec3(2, 0, 0));

	glLoadMatrixf(glm::value_ptr(ship.view*mt0));

	myTorus.drawSolid(); //Narysuj torus

	glm::mat4 mt1 = glm::mat4(1.0f);
	mt1 = translate(mt1, glm::vec3(-2, 0, 0));

	glLoadMatrixf(glm::value_ptr(ship.view*mt1));

	myTorus.drawSolid(); //Narysuj torus
	for (int i = 0; i < ship.howManyBullets; i++) {
		if (ship.bullets[i].alive) {
			glLoadMatrixf(glm::value_ptr(ship.view*ship.bullets[i].m));
			miniTorus.drawSolid();
		}
	}

	for (int i = 0; i < howManyTargets; i++) {
		if (targets[i].alive) {
			glLoadMatrixf(glm::value_ptr(ship.view*targets[i].m));
			myTorus.drawSolid();
			miniTorus.drawSolid();
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//Przerzu� tylny bufor na przedni
	glfwSwapBuffers(window);

}

void initTargets() {
	for (int i = 0; i < howManyTargets; i++) {
		glm::mat4 m = glm::mat4(1.0f);
		glm::vec3 pos = glm::vec3((rand() % randTargetRange) - randTargetRange/2, (rand() % randTargetRange) - randTargetRange / 2, (rand() % randTargetRange) - randTargetRange / 2);
		targets[i].position = glm::vec4(pos, 1.0f);
		//targets[i].m = rotate(targets[i].m, (float) rand(), pos);
		targets[i].m = translate(targets[i].m, pos);
	}
}

void rotateTargets(double time) {
	for (int i = 0; i < howManyTargets; i++) {
		targets[i].m = rotate(targets[i].m, (float)time, glm::vec3(1,0,0));
	}
}

float calcDistance(glm::vec4 a, glm::vec4 b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
 }

void checkForCollision() {
	glm::vec4 zero = glm::vec4(0, 0, 0, 0);
	glm::vec4 targetPosition;
	glm::vec4 bulletPosition;
	float dist;
	for (int t = 0; t < howManyTargets; t++) {
		for (int b = 0; b < ship.howManyBullets; b++) {
			if (targets[t].alive && ship.bullets[b].alive) {
				targetPosition = glm::vec4(targets[t].m[3][0], targets[t].m[3][1], targets[t].m[3][2], 1.0f);
				bulletPosition = glm::vec4(ship.bullets[b].m[3][0], ship.bullets[b].m[3][1], ship.bullets[b].m[3][2], 1.0f);
				dist = calcDistance(targetPosition, bulletPosition);
				if (dist < 1.0f) {
					targets[t].alive = false;
					ship.bullets[b].alive = false;
					++points;
					std::cout << "points " << points << std::endl;
				}
			}
		}
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
		checkForCollision();
		glfwSetTime(0);
		drawScene(window,angle); //Wykonaj procedur� rysuj�c�
		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
		//Wyzeruj licznik czasu
	}

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}