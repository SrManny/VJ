#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
}

Scene::~Scene()
{

}



void Scene::init()
{
	//La pantalla de juego es de 320 x 160 sin panel de poderes sino es 320x188
	//cout << "amplada " << CAMERA_WIDTH << endl << "altura " << CAMERA_HEIGHT << endl;
	//glm::vec2 texCoords[2] = {glm::vec2((120.f + centreX) / 512.0, 0.f), glm::vec2((120.f + 320.f + centreX) / 512.0f, 160.f / 256.0f)};
	escenario = 0;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	menu.init(projection);
	selectLevels.init(projection);
	currentTime = 0.0f;
	speed = 1.0f;
	/*lemming.init(glm::vec2(60, 30), simpleTexProgram);
	lemming.setMapMask(&maskTexture);
	bmenu = true;
	bselectLevels = false;*/
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (escenario == 2) {
		level.update(deltaTime);
		int aux = level.WantToGoBack();
		if (aux == 1) escenario = 0;
	}
}
void Scene::keypressed(int key) {
	if (escenario == 2) level.keypressed(key);
}

void Scene::keyreleased(int key) {
	if (escenario == 2) level.keyreleased(key);
}

void Scene::render()
{
	//Cargo el Menu: la imagen de fondo, (en un futuro los botones)...
	//cout << "esto es antes del render " << escenario << endl;
	if (escenario == 0) {
		menu.render();
	}
	else if (escenario == 1) selectLevels.render();
	else level.render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{

	if (escenario == 0) {
		//cout << escenario << endl;
		menu.mouseMoved(mouseX, mouseY, bLeftButton);
	}
	else if (escenario == 1) {
		selectLevels.mouseMoved(mouseX, mouseY, bLeftButton);
	}
	else level.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
}

int Scene::mouseRelease(int mouseX, int mouseY, int button) {
	if (escenario == 2) {
		level.mouseRelease(mouseX, mouseY, button);
		speed = level.getSpeed();
	}
	if (escenario == 1) {
		escenario = selectLevels.mouseRelease(button);
		if (escenario == 2) level.init(escenario-1);
	}
	else if (escenario == 0) {
		escenario = menu.mouseRelease(button);
		if (escenario > 1) level.init(escenario-1);
	}
	return speed;
}




