#include "SelectLevels.h"
#include <glm/gtc/matrix_transform.hpp>



SelectLevels::SelectLevels()
{
}


SelectLevels::~SelectLevels()
{
}

void SelectLevels::startButtonMatrixs() {

	levelsButtonsModel[0] = glm::mat4(1.0f);
	levelsButtonsModel[0] = glm::translate(levelsButtonsModel[0], glm::vec3(45.f / 2.f, 30.f / 2.f, 0.f));
	levelsButtonsModel[0] = glm::translate(levelsButtonsModel[0], glm::vec3(-40.f / 2.f, -30.f / 2.f, 0.f));

	levelsButtonsModel[1] = glm::mat4(1.0f);
	levelsButtonsModel[1] = glm::translate(levelsButtonsModel[1], glm::vec3(1000+45.f / 2.f, 30.f / 2.f, 0.f));
	levelsButtonsModel[1] = glm::translate(levelsButtonsModel[1], glm::vec3(-40.f / 2.f, -30.f / 2.f, 0.f));

	levelsButtonsModel[2] = glm::mat4(1.0f);
	levelsButtonsModel[2] = glm::translate(levelsButtonsModel[2], glm::vec3(1000+45.f / 2.f, 30.f / 2.f, 0.f));
	levelsButtonsModel[2] = glm::translate(levelsButtonsModel[2], glm::vec3(-40.f / 2.f, -30.f / 2.f, 0.f));

	levelsButtonsModel[3] = glm::mat4(1.0f);
	levelsButtonsModel[3] = glm::translate(levelsButtonsModel[3], glm::vec3(1000+45.f / 2.f, 30.f / 2.f, 0.f));
	levelsButtonsModel[3] = glm::translate(levelsButtonsModel[3], glm::vec3(-40.f / 2.f, -30.f / 2.f, 0.f));
}

void SelectLevels::init(glm::mat4 projection) {

	initShaders();
	for (int i = 0; i < 4; ++i) pressedButton[i] = 0;
	this->projection = projection;
	startButtonMatrixs();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT + 28)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	backQuad = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	levelsButtonsQuads[0] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	levelsButtonsQuads[1] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	levelsButtonsQuads[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	levelsButtonsQuads[3] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	background.loadFromFile("images/fondo_select.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	background.setMinFilter(GL_NEAREST);
	background.setMagFilter(GL_NEAREST);

	/*geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(50.f, 72.f / 4.f);
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
	select[1].setMagFilter(GL_NEAREST);*/

	glm::vec2 geomBack[2] = { glm::vec2(0.f, 0.f), glm::vec2(40.f, 30.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	//startQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	levelsButtonsQuads[0] = TexturedQuad::createTexturedQuad(geomBack, texCoords, zetaTextProgram);
	levelsButtons[0].loadFromFile("images/Buttons/Back.png", TEXTURE_PIXEL_FORMAT_RGBA);
	levelsButtons[0].setMinFilter(GL_NEAREST);
	levelsButtons[0].setMagFilter(GL_NEAREST);
}


void SelectLevels::mouseMoved(int mouseX, int mouseY, bool bLeftButton) {
	cout << "ola" << endl;
	for (int i = 0; i < 1; ++i) {
		bool aux1 = levelsButtonsQuads[i]->intersecta(mouseX, mouseY, levelsButtonsModel[i]);
		if (aux1) {
			if (bLeftButton) {
				pressedButton[i] = 2;
			}
			else {
				pressedButton[i] = 1;
			}
		}
		else {
			pressedButton[i] = 0;
		}
	}
}


void SelectLevels::render() {
	cout << "render de selectLevels" << endl;
	glm::mat4 modelview;
	zetaTextProgram.use();
	zetaTextProgram.setUniformMatrix4f("projection", projection);

	zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	zetaTextProgram.setUniformMatrix4f("modelview", modelview);
	zetaTextProgram.setUniform2f("zeta", 0.f, 1.f);
	backQuad->render(background);
	
	for (int i = 0; i < 1; ++i) {
		zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
		zetaTextProgram.setUniformMatrix4f("modelview", levelsButtonsModel[i]);
		levelsButtonsQuads[i]->render(levelsButtons[i]);
	}
	cout << "Saliendo del render SelectLevels" << endl;
	/*zetaTextProgram.setUniformMatrix4f("modelview", startModel);
	zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
	startQuad[startSprite]->render(start[startSprite]);
	zetaTextProgram.setUniformMatrix4f("modelview", selectModel);
	selectQuad[selectSprite]->render(select[selectSprite]);*/
}

int SelectLevels::mouseRelease(int button) {
	for (int i = 0; i < 4; ++i) {
		if ((pressedButton[i] == 2) && (i == 0)) return i;
		else if (pressedButton[i] == 2) return 3;
	}

	return 1;
}

void SelectLevels::initShaders() {
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