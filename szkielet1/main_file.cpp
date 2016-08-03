/*
Niniejszy program jest wolnym oprogramowaniem; mo¿esz go
rozprowadzaæ dalej i / lub modyfikowaæ na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacjê Wolnego
Oprogramowania - wed³ug wersji 2 tej Licencji lub(wed³ug twojego
wyboru) którejœ z póŸniejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej¹, i¿ bêdzie on
u¿yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyœlnej
gwarancji PRZYDATNOŒCI HANDLOWEJ albo PRZYDATNOŒCI DO OKREŒLONYCH
ZASTOSOWAÑ.W celu uzyskania bli¿szych informacji siêgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnoœci¹ wraz z niniejszym programem otrzyma³eœ te¿ egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeœli nie - napisz do Free Software Foundation, Inc., 59 Temple
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

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);}

//Procedura inicjuj¹ca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 0, 1); //Czyœæ ekran na czarno
	//glEnable(GL_LIGHTING); //W³¹cz tryb cieniowania
	glEnable(GL_LIGHT0); //W³¹cz domyslne œwiat³o
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora
	glEnable(GL_COLOR_MATERIAL); //glColor3d ma modyfikowaæ w³asnoœci materia³u
	glEnable(GL_TEXTURE_2D);
	
								 //Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
							  //Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, "brick.png");

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
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

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************l
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolorów
	glm::mat4 P = glm::perspective(50 * PI / 180, 1.0f, 0.05f, 50.0f); //Wylicz macierz rzutowania

	//Za³aduj macierze do OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	glColor3d(1, 1, 1); //Rysuj na bia³o
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

	//Przerzuæ tylny bufor na przedni
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
	GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno
	std::cout << "lol"<<std::endl;

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE); 
	}

	window = glfwCreateWindow(700, 700, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora
	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj¹ce

	float angle = 0; //K¹t obrotu torusa
	glfwSetTime(0); //Wyzeruj licznik czasu

	initTargets();

	//G³ówna pêtla
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		//std::cout << glfwGetTime()<<std::endl;
		ship.moveCamera(glfwGetTime(), nextMove);
		rotateTargets(glfwGetTime());
		addObjects();
		moveObjects();
		checkForCollision();
		glfwSetTime(0);
		drawScene(window,angle); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
		//Wyzeruj licznik czasu
	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}