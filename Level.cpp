#include "Level.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

Level::Level()
{
	map = NULL;
}


Level::~Level()
{
	if (map != NULL)
		delete map;
}

void Level::initMatrixs() {
	fastForwardModel = glm::mat4(1.0f);
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(305.f, 30.f / 2.f, 0.f));
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(-25.f / 2.f, -25.f / 2.f, 0.f));
}

void Level::keypressed(int key) {
	lemming.keyPressed(key);
}

void Level::init(int nLevel)
{
	inCentreX = 0.f;
	speed = 1;
	this->nLevel = nLevel;
	setValues();
	initShaders();
	initMatrixs();
	setValues();
	powersBar.init();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(sizeOfLevel, float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f,160.f / 256.f) };
	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(LevelTextureLocation, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(LevelMaskLocation, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	fastForwardButton.loadFromFile("images/Buttons/fastForwardButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fastForwardButton.setMinFilter(GL_NEAREST);
	fastForwardButton.setMagFilter(GL_NEAREST);
	glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f,1.f) };
	glm::vec2 geomFastButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(25), float(25)) };
	fastForwardQuad = TexturedQuad::createTexturedQuad(geomFastButton, texCoords2, zetaTextProgram);	//Inicializamos el menu y el powerBar

	projectionButtons = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	projection2 = glm::ortho(offsetxLevel, float(offsetxLevel + 320.f - 1), float(160.f - 1 + 28), 0.f);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	lemming.init(glm::vec2(60, 30), simpleTexProgram);
	++actualment[lemming.getTipus()];
	lemming.setMapMask(&maskTexture);
	lemmings.push_back(lemming);
}

void Level::setValues() {
	if (nLevel == 1) {
		maxPikmins = 50;
		LevelTextureLocation = "images/fun1.png";
		LevelMaskLocation = "images/fun1_mask.png";
		spawnPoint = glm::vec2(60, 30);
		exitPoint = glm::vec2(60, 80);
		Time = 650;
		survived = 0;
		maxPikmins = 50;
		offsetxLevel = 120.f;
		sizeOfLevel = 512.f;
	}
	else if (nLevel == 2) {

	}

	else if (nLevel == 3) {

	}
}

void Level::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120.f + inCentreX;
	posY = mouseY / 3;

	for (int y = max(0, posY - 4); y <= min(maskTexture.height() - 1, posY + 4); y++)
		for (int x = max(0, posX - 4); x <= min(maskTexture.width() - 1, posX + 4); x++) {
			float distance = sqrt(pow((x - posX), 2) + pow((y - posY), 2));
			if (distance < 5)maskTexture.setPixel(x, y, 0);
		}
}

void Level::applyMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120 + inCentreX;
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Level::render() {
	powersBar.render();
	maskedTexProgram.use();
	glm::mat4 modelview;
	//projection2 = glm::translate(projection2, glm::vec3(currentTime / 2000.f, 0.f, 0.f));
	maskedTexProgram.setUniformMatrix4f("projection", projection2);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	//Hay que trasladar en x 524 pixeles (81.01 % de la imagen) en 320 son 259.26
	//modelview = glm::translate(modelview, glm::vec3(currentTime/1000.f, 0.f, 0.f));
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);

	simpleTexProgram.use();

	//projection = glm::translate(projection, glm::vec3(currentTime / 2000.f, 0.f, 0.f));
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	//modelview = glm::translate(modelview, glm::vec3(currentTime / 1000.f, 0.f, 0.f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	lemming.render(projection);
	zetaTextProgram.use();
	zetaTextProgram.setUniformMatrix4f("projection", projectionButtons);
	zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
	//modelview = glm::translate(modelview, glm::vec3(currentTime / 1000.f, 0.f, 0.f));
	zetaTextProgram.setUniformMatrix4f("modelview", fastForwardModel);
	fastForwardQuad->render(fastForwardButton);
	//for (unsigned int i = 0; i < lemmings.size(); ++i);
		//lemmings[i].render();
}

void Level::update(int deltaTime)
{
	lemming.update(deltaTime, inCentreX);
	for (unsigned int i = 0; i < lemmings.size(); ++i);
		//lemmings[i].update(deltaTime);
}

void Level::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	lemming.mouseMoved(mouseX, mouseY, bLeftButton);
	mapPressed = powersBar.mouseMoved(mouseX, mouseY, bLeftButton);
	mapPressed = powersBar.mouseMoved(mouseX, mouseY, bLeftButton);
	if (mapPressed) {
		inCentreX = mouseX - (524.f / 648.f)*960.f;
		projection2 = glm::ortho(120.f + inCentreX, float(120.f + 320.f - 1 + inCentreX), float(160.f - 1 + 28), 0.f);
		projection = glm::ortho(0.f + inCentreX, float(CAMERA_WIDTH - 1 + inCentreX), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	}
	bool intersectaFast = fastForwardQuad->intersecta(mouseX, mouseY, fastForwardModel);
	if (bLeftButton && !intersectaFast) {
		eraseMask(mouseX, mouseY);
	}
	else if (bRightButton && !intersectaFast) {
		applyMask(mouseX, mouseY);
	}
}

void Level::mouseRelease(int mouseX, int mouseY, int button)
{
	powersBar.mouseRelease(button);
	bool intersectaFast = fastForwardQuad->intersecta(mouseX, mouseY, fastForwardModel);
	if (intersectaFast) {
		if (speed == 4) speed = 1;
		else speed *= 2;
	}
	lemming.mouseRelease(mouseX, mouseY, button);
}

void Level::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if (!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

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
int Level::getSpeed() {
	return speed;
}