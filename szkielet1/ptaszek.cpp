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

#include "ptaszek.h"

namespace Models {

	Ptaszek ptaszek;

	Ptaszek::Ptaszek() {
		vertices = CubeInternal::vertices;
		normals = CubeInternal::normals;
		vertexNormals = CubeInternal::vertexNormals;
		texCoords = CubeInternal::texCoords;
		colors = CubeInternal::colors;
		vertexCount = CubeInternal::vertexCount;
	}

	Ptaszek::~Ptaszek() {
	}

	void Ptaszek::drawSolid() {
		glEnable(GL_NORMALIZE);

		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(4, GL_FLOAT, 0, vertices);
		//glColorPointer(4,GL_FLOAT,0,colors);
		glNormalPointer(GL_FLOAT, sizeof(float) * 4, vertexNormals);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	namespace CubeInternal {
		unsigned int vertexCount = 36;

		float vertices[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, -1.0f,
			0.0f, 1.0f, 1.0f,
			-1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, -1.0f,
			0.0f, 1.0f, 1.0f,
		};

		float colors[] = {
			1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,

			1.0f,1.0f,0.0f,
			1.0f,1.0f,0.0f,
			1.0f,1.0f,0.0f
		};

	}
}