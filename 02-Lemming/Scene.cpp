#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{

	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::initMatrixs() {
	fastForwardModel = glm::mat4(1.0f);
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(305.f, 30.f / 2.f, 0.f));
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(-25.f / 2.f, -25.f / 2.f, 0.f));
}
void Scene::init()
{
	//La pantalla de juego es de 320 x 160 sin panel de poderes sino es 320x188
	centreX = 0.f;
	speed = 1;
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(512), float(CAMERA_HEIGHT))};
	glm::vec2 geomFastButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(25), float(25)) };
	cout << "amplada " << CAMERA_WIDTH << endl << "altura " << CAMERA_HEIGHT << endl;
	//glm::vec2 texCoords[2] = {glm::vec2((120.f + centreX) / 512.0, 0.f), glm::vec2((120.f + 320.f + centreX) / 512.0f, 160.f / 256.0f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f,160.f/256.f)};
	initShaders();
	initMatrixs();
	escenario = 0;
	numberOfLemmings = 1;
	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f,1.f) };
	fastForwardQuad = TexturedQuad::createTexturedQuad(geomFastButton, texCoords2, zetaTextProgram);	//Inicializamos el menu y el powerBar
	powersBar.init();

	colorTexture.loadFromFile("images/fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/fun1_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	fastForwardButton.loadFromFile("images/Buttons/fastForwardButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fastForwardButton.setMinFilter(GL_NEAREST);
	fastForwardButton.setMagFilter(GL_NEAREST);

	projection2 = glm::ortho(120.f, float(120.f +320.f - 1), float(160.f-1+28), 0.f);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	projectionButtons = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	menu.init(projection);
	selectLevels.init(projection);
	currentTime = 0.0f;
	
	lemming.init(glm::vec2(60, 30), simpleTexProgram);
	lemming.setMapMask(&maskTexture);
	bmenu = true;
	bselectLevels = false;
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	lemming.update(deltaTime, centreX);
}

void Scene::render()
{
	glm::mat4 modelview;
//Si no estamos en el menu cargo lo que sea
	if (escenario == 2) {
		powersBar.render();
		maskedTexProgram.use();

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
	}
	//Cargo el Menu: la imagen de fondo, (en un futuro los botones)...
	else if (escenario == 0){
		menu.render();
	}
	else if (escenario == 1) {
		selectLevels.render();
	}
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{

	if (escenario == 0) {
		menu.mouseMoved(mouseX, mouseY, bLeftButton);
	}
	else if (escenario == 1) {
		selectLevels.mouseMoved(mouseX, mouseY, bLeftButton);
	}
	else {
		lemming.mouseMoved(mouseX, mouseY, bLeftButton);
		mapPressed = powersBar.mouseMoved(mouseX, mouseY, bLeftButton);
		if (mapPressed) {
			centreX = mouseX - (524.f / 648.f)*960.f;
			projection2 = glm::ortho(120.f + centreX, float(120.f + 320.f - 1+ centreX), float(160.f - 1 + 28), 0.f);
			projection = glm::ortho(0.f+ centreX, float(CAMERA_WIDTH - 1+ centreX), float(CAMERA_HEIGHT - 1 + 28), 0.f);
		}
		bool intersectaFast = fastForwardQuad->intersecta(mouseX, mouseY, fastForwardModel);
		if (bLeftButton && !intersectaFast) {
			eraseMask(mouseX, mouseY);
		}
		else if (bRightButton && !intersectaFast) {
			applyMask(mouseX, mouseY);
		}
	}
}

int Scene::mouseRelease(int mouseX, int mouseY, int button) {
	if (escenario == 2) {
		bool intersectaFast = fastForwardQuad->intersecta(mouseX, mouseY, fastForwardModel);
		if (intersectaFast) {
			if (speed == 4) speed = 1;
			else speed *= 2;
		}
		escenario = powersBar.mouseRelease(button);
		lemming.mouseRelease(mouseX, mouseY, button);
	}
	else if (escenario == 1) escenario = selectLevels.mouseRelease(button);
	else if (escenario == 0) escenario = menu.mouseRelease(button);
	return speed;
}
void Scene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120.f + centreX;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 0);
}

void Scene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120 + centreX;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
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



