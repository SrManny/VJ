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
	selectModel = glm::mat4(1.0f);
	selectModel = glm::translate(selectModel, glm::vec3(120.f, 110.f + (72.f / 4.f), 0.f));
	selectModel = glm::translate(selectModel, glm::vec3(-50.f / 2.f, -72.f / 8.f, 0.f));

}
void Menu::init(glm::mat4 projection) {

	initShaders();
	startSprite = 0;
	selectSprite = 0;
	overStart = false;
	overSelect = false;
	this->projection = projection;
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
	selectQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	start[0].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[0].setMinFilter(GL_NEAREST);
	start[0].setMagFilter(GL_NEAREST);
	select[0].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	select[0].setMinFilter(GL_NEAREST);
	select[0].setMagFilter(GL_NEAREST);

	texCoords[0] = glm::vec2(0.f, 1.f / 3.f); texCoords[1] = glm::vec2(1.f, 2.f / 3.f);
	startQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	selectQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	start[1].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[1].setMinFilter(GL_NEAREST);
	start[1].setMagFilter(GL_NEAREST);
	select[1].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	select[1].setMinFilter(GL_NEAREST);
	select[1].setMagFilter(GL_NEAREST);

	texCoords[0] = glm::vec2(0.f, 2.f / 3.f); texCoords[1] = glm::vec2(1.f, 1.f);
	startQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	selectQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	start[2].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[2].setMinFilter(GL_NEAREST);
	start[2].setMagFilter(GL_NEAREST);
	select[2].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	select[2].setMinFilter(GL_NEAREST);
	select[2].setMagFilter(GL_NEAREST);
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
	zetaTextProgram.setUniformMatrix4f("modelview", selectModel);
	selectQuad[selectSprite]->render(select[selectSprite]);
}

//Devuelve si intersecta el punto xy con el Quad de coordenadas min max
bool Menu::intersecta(int x, int y, glm::vec4 min, glm::vec4 max) {
	return (((x < max[0]) && (x > min[0])) && ((y < max[1]) && (y > min[1])));
}
void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton) {

	bool aux1 = startQuad[0]->intersecta(mouseX, mouseY, startModel);
	if (aux1) {
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

	bool aux = selectQuad[0]->intersecta(mouseX, mouseY, selectModel);
	if (aux) {
		if (bLeftButton) {
			selectSprite = 2;
			overSelect = true;
		}
		else {
			selectSprite = 1;
			overSelect = false;
		}
	}
	else {
		selectSprite = 0;
		overSelect = false;
	}
}

int Menu::mouseRelease(int button) {
	if (overStart) return 2;
	else if (overSelect) return 1;
	return 0;
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