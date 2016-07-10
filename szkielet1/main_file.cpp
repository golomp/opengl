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

float speed = 2.0f; // [radiany/s]
GLfloat cameraSpeed = 0.05f;
float cur_speed = 0;
float pitch = 0;
float roll = 0;
float yaw = 0;
float forward = 0;
float pitch_angle = 0;
float yaw_angle = 0;
float roll_angle = 0;


Models::Torus myTorus(1.7f, 0.3f, 36, 36);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
glm::vec3 direction;
glm::vec3 roll_vector;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_A:
			++yaw;
			break;
		case GLFW_KEY_D:
			--yaw;
			break;
		case GLFW_KEY_W:
			++pitch;
			break;
		case GLFW_KEY_S:
			--pitch;
			break;
		case GLFW_KEY_Q:
			--roll;
			break;
		case GLFW_KEY_E:
			++roll;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			++forward;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			--forward;
			break;
		}
	} 
	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_A:
			--yaw;
			break;
		case GLFW_KEY_D:
			++yaw;
			break;	
		case GLFW_KEY_W:
			--pitch;
			break;
		case GLFW_KEY_S:
			++pitch;
			break;
		case GLFW_KEY_Q:
			++roll;
			break;
		case GLFW_KEY_E:
			--roll;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			//--forward;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			//++forward;
			break;
		}
	}
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
	
}

void transform_camera_vectors(glm::mat4 rotation_mat) {
	cameraRight = glm::vec3(rotation_mat * glm::vec4(cameraRight, 0.0f));
	cameraUp = glm::vec3(rotation_mat * glm::vec4(cameraUp, 0.0f));
	cameraFront = glm::vec3(rotation_mat * glm::vec4(cameraFront, 0.0f));
}

void moveCamera(double time) {
	glm::mat4 M = glm::mat4(1.0f);

	double forward_speed = 5l * time;
	//pitch_angle = 4l * time * pitch;
	//yaw_angle = 4l * time * yaw;
	//roll_angle = 6l *time * roll;
	cameraSpeed = 1l * time  * forward;
	
	if (pitch == 1) {
		if (pitch_angle < 0.05) {
			pitch_angle += 0.04l * time;
		}
		if (pitch_angle < 0) {
			pitch_angle += 0.06l * time;
		}
	}
	else if (pitch == -1) {
		if (pitch_angle > -0.05) {
			pitch_angle -= 0.04l * time;
		}
		if (pitch_angle > 0) {
			pitch_angle -= 0.06l * time;
		}
	} else {
		pitch_angle /= 1.1l;
		if (abs(pitch_angle) < 0.00000000002) {
			pitch_angle = 0;
		}
	}


	if (roll == 1) {
		if (roll_angle < 0.05) {
			roll_angle += 0.04l * time;
		}
		if (roll_angle < 0) {
			roll_angle += 0.06l * time;
		}
	}
	else if (roll == -1) {
		if (roll_angle > -0.05) {
			roll_angle -= 0.04l * time;
		}
		if (roll_angle > 0) {
			roll_angle -= 0.06l * time;
		}
	}
	else {
		roll_angle /= 1.1l;
		if (abs(roll_angle) < 0.00000000002) {
			roll_angle = 0;
		}
	}

	if (yaw == 1) {
		if (yaw_angle < 0.05) {
			yaw_angle += 0.04l * time;
		}
		if (yaw_angle < 0) {
			yaw_angle += 0.06l * time;
		}
	}
	else if (yaw == -1) {
		if (yaw_angle > -0.05) {
			yaw_angle -= 0.04l * time;
		}
		if (yaw_angle > 0) {
			yaw_angle -= 0.06l * time;
		}
	}
	else {
		yaw_angle /= 1.1l;
		if (abs(yaw_angle) < 0.00000000002) {
			yaw_angle = 0;
		}
	}

	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, pitch_angle, cameraRight);
	transform_camera_vectors(rotationMat);

	
	rotationMat = glm::mat4(1);
	rotationMat = glm::rotate(rotationMat, roll_angle, cameraFront);
	transform_camera_vectors(rotationMat);
	

	rotationMat = glm::mat4(1);
	rotationMat = glm::rotate(rotationMat, yaw_angle, cameraUp);
	transform_camera_vectors(rotationMat);
	
	
	if (forward != 0) {
		cameraPos += cameraFront * cameraSpeed;
	}

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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
	glColor3d(0, 1, 0); //Rysuj na zielono	

	glm::mat4 mt0 = glm::mat4(1.0f);
	mt0 = translate(mt0, glm::vec3(2, 0, 0));
	mt0 = rotate(mt0, angle, glm::vec3(0, 0, 1));

	glLoadMatrixf(glm::value_ptr(view*mt0));

	myTorus.drawWire(); //Narysuj torus

	glm::mat4 mt1 = glm::mat4(1.0f);
	mt1 = translate(mt1, glm::vec3(-2, 0, 0));
	mt1 = rotate(mt1, -angle, glm::vec3(0, 0, 1));

	glLoadMatrixf(glm::value_ptr(view*mt1));

	myTorus.drawWire(); //Narysuj torus

	//Przerzuæ tylny bufor na przedni
	glfwSwapBuffers(window);

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

	//G³ówna pêtla
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		//std::cout << glfwGetTime()<<std::endl;
		moveCamera(glfwGetTime());
		glfwSetTime(0);
		drawScene(window,angle); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
		//Wyzeruj licznik czasu
	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}