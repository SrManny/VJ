#include "Menu.h"
#include "TexturedQuad.h"
#include <glm/gtc/matrix_transform.hpp>


Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::startButtonMatrixs() {

	startModel = glm::mat4(1.0f);
	startModel = glm::translate(startModel, glm::vec3(120.f, 110.f, 0.f));
	startModel = glm::translate(startModel, glm::vec3(-50.f / 2.f, -72.f / 8.f, 0.f));
	startInvMatrix = glm::inverse(startModel);
}
void Menu::init() {

	initShaders();
	startSprite = 0;
	overStart = false;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	startButtonMatrixs();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT + 28)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	backQuad = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	background.loadFromFile("images/fondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background.setMinFilter(GL_NEAREST);
	background.setMagFilter(GL_NEAREST);

	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(50.f, 72.f / 4.f);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f / 3.f);
	startQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	start[0].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[0].setMinFilter(GL_NEAREST);
	start[0].setMagFilter(GL_NEAREST);

	texCoords[0] = glm::vec2(0.f, 1.f / 3.f); texCoords[1] = glm::vec2(1.f, 2.f / 3.f);
	startQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	start[1].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[1].setMinFilter(GL_NEAREST);
	start[1].setMagFilter(GL_NEAREST);

	texCoords[0] = glm::vec2(0.f, 2.f / 3.f); texCoords[1] = glm::vec2(1.f, 1.f);
	startQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	start[2].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[2].setMinFilter(GL_NEAREST);
	start[2].setMagFilter(GL_NEAREST);
}

void Menu::render() {
	glm::mat4 modelview;

	zetaTextProgram.use();
	zetaTextProgram.setUniformMatrix4f("projection", projection);
	zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	zetaTextProgram.setUniformMatrix4f("modelview", modelview);
	zetaTextProgram.setUniform2f("zeta", 0.f, 1.f);
	backQuad->render(background);

	zetaTextProgram.setUniformMatrix4f("modelview", startModel);
	zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
	startQuad[startSprite]->render(start[startSprite]);
}

//Devuelve si intersecta el punto xy con el Quad de coordenadas min max
bool Menu::intersecta(int x, int y, glm::vec4 min, glm::vec4 max) {
	return (((x < max[0]) && (x > min[0])) && ((y < max[1]) && (y > min[1])));
}
void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton) {
	int* boundingBox = startQuad[0]->getboundingBox();
	float xratio = 960 / CAMERA_WIDTH;
	float yratio = (480+84) / (CAMERA_HEIGHT+28);
	glm::vec4 min =  startModel * glm::vec4(boundingBox[0], boundingBox[1], 0.5f, 1.0f);
	glm::vec4 max =  startModel * glm::vec4(boundingBox[2], boundingBox[3], 0.5f, 1.0f);
	min.x = min.x*xratio;
	min.y = min.y*yratio;
	max.x = max.x*xratio;
	max.y = max.y*yratio;
	bool aux = intersecta(mouseX, mouseY, min , max);
	if (aux) {
		if (bLeftButton) {
			startSprite = 2;
			overStart = true;
		}
		else {
			startSprite = 1;
			overStart = false;
		}
	}
	else {
		startSprite = 0;
		overStart = false;
	}
}

bool Menu::mouseRelease(int button) {
	if (overStart) return false;
	return true;
}
void Menu::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/zetaVariable.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/zetaVariable.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	zetaTextProgram.init();
	zetaTextProgram.addShader(vShader);
	zetaTextProgram.addShader(fShader);
	zetaTextProgram.link();
	if (!zetaTextProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << zetaTextProgram.log() << endl << endl;
	}
	zetaTextProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}