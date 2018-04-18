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

	for (int i = 1; i < 4; ++i) {
		levelsButtonsModel[i] = glm::mat4(1.0f);
		levelsButtonsModel[i] = glm::translate(levelsButtonsModel[i], glm::vec3(100 + (i-1)*60, 100.f, 0.f));
		levelsButtonsModel[i] = glm::translate(levelsButtonsModel[i], glm::vec3(-60.f / 2.f, -50.f / 2.f, 0.f));
	}
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

	imageLoc[0] = "images/Buttons/Back.png";
	imageLoc[1] = "images/SelectLevels/fun1.png";
	imageLoc[2] = "images/SelectLevels/fun1.png";
	imageLoc[3] = "images/SelectLevels/fun1.png";

	for (int i = 1; i < 4; ++i) {
		geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(60.f, 50.f);
		texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
		levelsButtonsQuads[i] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
		levelsButtons[i].loadFromFile(imageLoc[i], TEXTURE_PIXEL_FORMAT_RGBA);
		levelsButtons[i].setMinFilter(GL_NEAREST);
		levelsButtons[i].setMagFilter(GL_NEAREST);
	}

	glm::vec2 geomBack[2] = { glm::vec2(0.f, 0.f), glm::vec2(40.f, 30.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	//startQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	levelsButtonsQuads[0] = TexturedQuad::createTexturedQuad(geomBack, texCoords, zetaTextProgram);
	levelsButtons[0].loadFromFile(imageLoc[0], TEXTURE_PIXEL_FORMAT_RGBA);
	levelsButtons[0].setMinFilter(GL_NEAREST);
	levelsButtons[0].setMagFilter(GL_NEAREST);
}


void SelectLevels::mouseMoved(int mouseX, int mouseY, bool bLeftButton) {
	for (int i = 0; i < 4; ++i) {
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
	
	for (int i = 0; i < 4; ++i) {
		zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
		zetaTextProgram.setUniformMatrix4f("modelview", levelsButtonsModel[i]);
		levelsButtonsQuads[i]->render(levelsButtons[i]);
	}
	/*zetaTextProgram.setUniformMatrix4f("modelview", startModel);
	zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
	startQuad[startSprite]->render(start[startSprite]);
	zetaTextProgram.setUniformMatrix4f("modelview", selectModel);
	selectQuad[selectSprite]->render(select[selectSprite]);*/
}

int SelectLevels::mouseRelease(int button) {
	for (int i = 0; i < 4; ++i) {
		if (pressedButton[i] == 2 && (i ==0)) return i;
		else if (pressedButton[i] == 2 && (i != 0)) return i + 1;
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