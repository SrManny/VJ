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

	insMatrix = glm::mat4(1.0f);
	insMatrix = glm::translate(insMatrix, glm::vec3(120.f, 110.f + (72.f / 2.f), 0.f));
	insMatrix = glm::translate(insMatrix, glm::vec3(-50.f / 2.f, -72.f / 8.f, 0.f));

	exitInsMatrix = glm::mat4(1.0f);
	exitInsMatrix = glm::translate(exitInsMatrix, glm::vec3((239.f / 960.f)*320.f , (502.f / 564.f)*188.f, 0.f));
	exitInsMatrix = glm::translate(exitInsMatrix, glm::vec3(-320.f*(202.f / 960.f) / 2.f, -((538 - 465) / 564.f)*188.f / 2.f, 0.f));

}
void Menu::init(glm::mat4 projection) {

	initShaders();
	startSprite = 0;
	selectSprite = 0;
	insSprite = 0;
	overStart = false;
	overSelect = false;
	overIns = false;
	insPressed = false;
	osts.openFromFile("soundTrack/01.pikmin.wav");
	osts.setLoop(true);
	osts.play();
	this->projection = projection;
	startButtonMatrixs();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT + 28)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	backQuad = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	background.loadFromFile("images/TitleScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background.setMinFilter(GL_NEAREST);
	background.setMagFilter(GL_NEAREST);

	instructions.loadFromFile("images/Menu/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions.setMinFilter(GL_NEAREST);
	instructions.setMagFilter(GL_NEAREST);

	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(50.f, 72.f / 4.f);

	glm::vec2 geomExit[2] = { glm::vec2(0.f, 0.f), glm::vec2(320.f*(202.f / 960.f), ((538 - 465) / 564.f)*188.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f / 3.f);
	startQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	selectQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	insQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	exitInsQuad[0] = TexturedQuad::createTexturedQuad(geomExit, texCoords, zetaTextProgram);

	start[0].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[0].setMinFilter(GL_NEAREST);
	start[0].setMagFilter(GL_NEAREST);
	select[0].loadFromFile("images/selectStage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	select[0].setMinFilter(GL_NEAREST);
	select[0].setMagFilter(GL_NEAREST);

	texCoords[0] = glm::vec2(0.f, 1.f / 3.f); texCoords[1] = glm::vec2(1.f, 2.f / 3.f);
	startQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	selectQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	insQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	exitInsQuad[1] = TexturedQuad::createTexturedQuad(geomExit, texCoords, zetaTextProgram);
	start[1].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[1].setMinFilter(GL_NEAREST);
	start[1].setMagFilter(GL_NEAREST);
	select[1].loadFromFile("images/selectStage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	select[1].setMinFilter(GL_NEAREST);
	select[1].setMagFilter(GL_NEAREST);

	texCoords[0] = glm::vec2(0.f, 2.f / 3.f); texCoords[1] = glm::vec2(1.f, 1.f);
	startQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	selectQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	insQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, zetaTextProgram);
	exitInsQuad[2] = TexturedQuad::createTexturedQuad(geomExit, texCoords, zetaTextProgram);
	start[2].loadFromFile("images/startButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start[2].setMinFilter(GL_NEAREST);
	start[2].setMagFilter(GL_NEAREST);
	select[2].loadFromFile("images/selectStage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	select[2].setMinFilter(GL_NEAREST);
	select[2].setMagFilter(GL_NEAREST);
	insButton.loadFromFile("images/selectStage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	insButton.setMinFilter(GL_NEAREST);
	insButton.setMagFilter(GL_NEAREST);
	exitIns.loadFromFile("images/selectStage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	insButton.setMinFilter(GL_NEAREST);
	insButton.setMagFilter(GL_NEAREST);
}

void Menu::render() {
	glm::mat4 modelview;
	zetaTextProgram.use();
	zetaTextProgram.setUniformMatrix4f("projection", projection);
	zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	zetaTextProgram.setUniformMatrix4f("modelview", modelview);
	zetaTextProgram.setUniform2f("zeta", 0.f, 1.f);
	if (!insPressed) {
		backQuad->render(background);
		zetaTextProgram.setUniformMatrix4f("modelview", startModel);
		zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
		startQuad[startSprite]->render(start[startSprite]);
		zetaTextProgram.setUniformMatrix4f("modelview", selectModel);
		selectQuad[selectSprite]->render(select[selectSprite]);
		zetaTextProgram.setUniformMatrix4f("modelview", insMatrix);
		insQuad[insSprite]->render(insButton);
	}
	else {
		backQuad->render(instructions);
		zetaTextProgram.setUniformMatrix4f("modelview", exitInsMatrix);
		exitInsQuad[exitInsSprite]->render(exitIns);
	}
}

//Devuelve si intersecta el punto xy con el Quad de coordenadas min max
bool Menu::intersecta(int x, int y, glm::vec4 min, glm::vec4 max) {
	return (((x < max[0]) && (x > min[0])) && ((y < max[1]) && (y > min[1])));
}
void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton) {

	if (!insPressed) {
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

		bool aux3 = insQuad[0]->intersecta(mouseX, mouseY, insMatrix);
		if (aux3) {
			if (bLeftButton) {
				insSprite = 2;
				overIns = true;
			}
			else {
				insSprite = 1;
				overIns = false;
			}
		}
		else {
			insSprite = 0;
			overIns = false;
		}
	}
	else {
		bool aux = exitInsQuad[0]->intersecta(mouseX, mouseY, exitInsMatrix);
		if (aux) {
			if (bLeftButton) {
				exitInsSprite = 2;
				overExit = true;
			}
			else {
				exitInsSprite = 1;
				overExit = false;
			}
		}
		else {
			exitInsSprite = 0;
			overExit = false;
		}
	}
}

int Menu::mouseRelease(int button) {
	if (overStart) {
		startSprite = 0;
		overStart = false;
		osts.stop();
		return 2;
	}
	else if (overSelect) {
		selectSprite = 0;
		overSelect = false;
		osts.stop();
		return 1;
	}
	else if (overIns) {
		insSprite = 0;
		overIns = false;
		insPressed = true;
	}
	else if (overExit) {
		exitInsSprite = 0;
		overExit = false;
		insPressed = false;
	}
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