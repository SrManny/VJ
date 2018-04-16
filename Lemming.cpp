#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, FALLING_LEFT, FALLING_RIGHT, DYING, DEATH, DIG, BASH_LEFT, BASH_RIGHT, CLIMB, BUILD_LEFT, BUILD_RIGHT, EXPLODE, BLOCK
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	lemmingClicked = -1;
	bselected = false;
	primero = true;
	initShaders();
	state = FALLING_RIGHT_STATE;
	for (int i = 0; i < 12; ++i) powers[i] = 0;
	spritesheet.loadFromFile("images/Pikmin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125 / 2.f, 0.25 / 2), &spritesheet, &shaderProgram);
	tipusLemming = rand() % 4;
	sprite->setNumberAnimations(12);

	sprite->setAnimationSpeed(WALKING_RIGHT, 12);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 16, tipusLemming / 4.f + 0.0f));

	sprite->setAnimationSpeed(WALKING_LEFT, 12);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 16, tipusLemming / 4.0f + 0.125f));
	sprite->setAnimationSpeed(FALLING_LEFT, 12);
	sprite->addKeyframe(FALLING_LEFT, glm::vec2(0.5, tipusLemming / 4.0));

	sprite->setAnimationSpeed(FALLING_RIGHT, 12);
	sprite->addKeyframe(FALLING_RIGHT, glm::vec2(0.5, tipusLemming / 4.0));

	sprite->setAnimationSpeed(DIG, 4);
	for (int i = 0; i < 2; i++)
		sprite->addKeyframe(DIG, glm::vec2((10.f + float(i)) / 16.0f, tipusLemming / 4.0));

	sprite->setAnimationSpeed(BASH_LEFT, 2);
	for (int i = 0; i < 2; i++)
		sprite->addKeyframe(BASH_LEFT, glm::vec2((10 + float(i)) / 16.0, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(BASH_RIGHT, 2);
	for (int i = 0; i < 2; i++)
		sprite->addKeyframe(BASH_RIGHT, glm::vec2((8 + float(i)) / 16.0, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(CLIMB, 2);
	sprite->addKeyframe(EXPLODE, glm::vec2(0.625, tipusLemming / 4.0));

	sprite->setAnimationSpeed(BUILD_LEFT, 2);
	for (int i = 0; i < 2; i++)
		sprite->addKeyframe(EXPLODE, glm::vec2(0.5 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(BUILD_RIGHT, 2);
	for (int i = 0; i < 2; i++)
		sprite->addKeyframe(EXPLODE, glm::vec2(0.625 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));


	sprite->setAnimationSpeed(EXPLODE, 2);
	for (int i = 0; i < 4; i++)
		sprite->addKeyframe(EXPLODE, glm::vec2(0.75 + float(i) / 16.0, tipusLemming / 4.0));

	sprite->setAnimationSpeed(DYING, 4);
	for (int i = 0; i < 4; i++)
		sprite->addKeyframe(DYING, glm::vec2(0.75 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(DEATH, 4);
	sprite->addKeyframe(DYING, glm::vec2(0.9375, tipusLemming / 4.0 + 0.125));

	sprite->changeAnimation(WALKING_RIGHT);
	sprite->setPosition(initialPosition);

	glm::vec2 geomLemming[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(16), float(16)) };
	glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f,1.f) };
	selectedLemmingQuad = TexturedQuad::createTexturedQuad(geomLemming, texCoords2, zetaTextProgram);
	selected[0].loadFromFile("images/overLemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	selected[0].setMinFilter(GL_NEAREST);
	selected[0].setMagFilter(GL_NEAREST);

	selected[1].loadFromFile("images/mapSelected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	selected[1].setMinFilter(GL_NEAREST);
	selected[1].setMagFilter(GL_NEAREST);
}

void Lemming::update(int deltaTime, float centreX)
{
	int fall;
	this->centreX = centreX;

	if (sprite->update(deltaTime) == 0)
		return;
	//cout << "me estoy actualizando" << endl;
	switch (state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if (fall > 0) {
			sprite->position() += glm::vec2(0, fall);
			sprite->changeAnimation(FALLING_LEFT);
		}
		else {
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		break;
	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if (fall > 0) {
			sprite->position() += glm::vec2(0, fall);
			sprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if (collision(0))
		{
			if (powers[8] > 0) {
				--powers[8];
				sprite->changeAnimation(BASH_LEFT);
				state = BASH_LEFT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(-1, -1);
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2)
				state = FALLING_LEFT_STATE;
		}
		break;
	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if (collision(0))
		{
			if (powers[8] > 0) {
				--powers[8];
				sprite->changeAnimation(BASH_RIGHT);
				state = BASH_RIGHT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(1, -1);
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
		}
		else
		{
			fall = collisionFloor(3);
			if (fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else
				state = FALLING_RIGHT_STATE;
		}
		break;
	case DIG_LEFT_STATE:
		fall = collisionFloor(3);

		if (fall > 2) {
			eraseMask(sprite->position());
			sprite->changeAnimation(DIG);
			state = FALLING_LEFT_STATE;
		}
		else {

			eraseMask(sprite->position());
			sprite->position() -= glm::vec2(0, -1);
		}
		break;
	case DIG_RIGHT_STATE:
		fall = collisionFloor(3);

		if (fall > 2) {
			eraseMask(sprite->position());
			sprite->changeAnimation(DIG);
			state = FALLING_RIGHT_STATE;
		}
		else {

			eraseMask(sprite->position());
			sprite->position() -= glm::vec2(0, -1);
		}
		break;
	case BASH_LEFT_STATE:
		sprite->position() += glm::vec2(-1, 0);
		if (!collision(-2))
		{
			fall = collisionFloor(3);
			if (fall > 0) {
				state = WALKING_LEFT_STATE;
				sprite->changeAnimation(WALKING_LEFT);
			}
			if (fall > 1) {
				state = WALKING_LEFT_STATE;
				sprite->changeAnimation(WALKING_LEFT);
			}
			if (fall > 2)
				state = FALLING_LEFT_STATE;
		}
		else {
			eraseMask(sprite->position());
		}
		break;

	case BASH_RIGHT_STATE:
		sprite->position() += glm::vec2(1, 0);
		if (!collision(2))
		{
			fall = collisionFloor(3);
			if (fall > 0) {
				state = WALKING_RIGHT_STATE;
				sprite->changeAnimation(WALKING_RIGHT);
			}
			if (fall > 1) {
				state = WALKING_RIGHT_STATE;
				sprite->changeAnimation(WALKING_RIGHT);
			}
			if (fall > 2)
				state = FALLING_RIGHT_STATE;
		}
		else {
			eraseMask(sprite->position());
		}
		break;
	case BUILD_LEFT_STATE:
		sprite->changeAnimation(BUILD_LEFT);
		build(sprite->position(), -1);
		break;
	case BUILD_RIGHT_STATE:
		sprite->changeAnimation(BUILD_RIGHT);
		build(sprite->position(), 1);
		break;
	case DYING_STATE:
		sprite->changeAnimation(DYING);
		sprite->changeAnimation(DEATH);
		state = DEATH_STATE;
		break;
	case EXPLODE_STATE:
		sprite->changeAnimation(EXPLODE);
		explosion(sprite->position());
		state = DYING_STATE;
		break;
	case DEATH_STATE:
		//kill lemming, erase, unrender
		break;
	}
}

void Lemming::render(glm::mat4 projection)
{
	sprite->render();
	if (lemmingClicked != -1) {
		zetaTextProgram.use();
		zetaTextProgram.setUniformMatrix4f("projection", projection);
		zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
		//modelview = glm::translate(modelview, glm::vec3(currentTime / 1000.f, 0.f, 0.f));
		modelView = glm::mat4(1.0f);
		glm::vec2 pos = sprite->position();
		modelView = glm::translate(modelView, glm::vec3(pos.x + 8.f, pos.y + 8.f, 0.f));
		modelView = glm::translate(modelView, glm::vec3(-16.f / 2.f, -16.f / 2.f, 0.f));
		zetaTextProgram.setUniformMatrix4f("modelview", modelView);
		selectedLemmingQuad->render(selected[lemmingClicked]);
	}

}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

void Lemming::eraseMask(glm::vec2 pos)
{
	int posX = pos[0] + 120 + 9;
	int posY = pos[1] + 14;
	for (int y = max(0, posY - 8); y <= min(mask->height() - 1, posY + 8); y++)
		for (int x = max(0, posX - 8); x <= min(mask->width() - 1, posX + 8); x++) {
			float distance = sqrt(pow((x - posX), 2) + pow((y - posY), 2));
			if (distance < 5) mask->setPixel(x, y, 0);
			}
}

void Lemming::explosion(glm::vec2 pos)
{
	int x = pos[0];
	int y = pos[1];
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
			if (i + j < 16)
				mask->setPixel(x + i, y + j, 255);

}

void Lemming::bash(glm::vec2 pos, int ind)
{
	int x = pos[0];
	int y = pos[1];
	//ind indica si se excava a izquierda o derecha, 1 para derecha, -1 para izquierda
	for (int i = 0; i < 10; ++i) {
		mask->setPixel(x, y + (i*ind), 0);
	}
}

void Lemming::build(glm::vec2 pos, int ind)
{
	int x = pos[0];
	int y = pos[1];
	//ind indica si se construye a izquierda o derecha, 1 para derecha, -1 para izquierda
	mask->setPixel(x + 4 * ind, y + 1, 0);
	mask->setPixel(x + 5 * ind, y + 1, 0);
	//Hace falta ahora pintar la textura real con algun color definido

}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement

	posBase += glm::ivec2(7, 16);
	while ((fall < maxFall) && !bContact)
	{
		if ((mask->pixel(posBase.x, posBase.y + fall) == 0) && (mask->pixel(posBase.x + 1, posBase.y + fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision(int offset)
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement

	posBase += glm::ivec2(7+offset, 15);
	if ((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x + 1, posBase.y) == 0))
		return false;

	return true;
}

int* Lemming::getboundingBox() {
	glm::vec2 center = sprite->position();
	boundingBox[0] = center.x;
	boundingBox[1] = center.y;
	boundingBox[2] = center.x + 16;
	boundingBox[3] = center.y + 16;
	//cout << "BoungdingBox del lemming: " << boundingBox[0] << " " << boundingBox[1] << " " << boundingBox[2] << " " << boundingBox[3] << endl;
	return boundingBox;
}
void Lemming::mouseMoved(int mouseX, int mouseY, bool bLeftButton) {
	bool inter = intersecta(mouseX, mouseY);
	if (inter && bLeftButton) {
		lemmingClicked = 1;
		bselected = true;
	}
	else if (inter && !bselected && !bLeftButton) {
		lemmingClicked = 0;
	}
	else if (!inter && !bselected) lemmingClicked = -1;
	/*else if (inter && (lemmingClicked == 0) && !bLeftButton) {
	lemmingClicked = 1;
	}*/
	//else if (inter && (lemmingClicked == 1) && bLeftButton) lemmingClicked = 1;
	//else if (inter && (lemmingClicked == 1) && !bLeftButton) lemmingClicked = -1;
	else if (!inter && (lemmingClicked == 0)) lemmingClicked = -1;
}

int Lemming::mouseRelease(int mouseX, int mouseY, int button) {
	bool inter = intersecta(mouseX, mouseY);
	if (inter && bselected && !primero) {
		lemmingClicked = 0;
		bselected = false;
		primero = true;
	}
	else if (inter && bselected && primero) {
		primero = false;
	}
	return 0;
}
bool Lemming::getIfSelected() {
	return bselected;
}

bool Lemming::intersecta(int mouseX, int mouseY) {
	int* boundingBox = getboundingBox();
	//cout << "BoungdingBox del lemming: " << boundingBox[0] << " " << boundingBox[1] << " " << boundingBox[2] << " " << boundingBox[3] << endl;

	float xratio = 960 / CAMERA_WIDTH;
	float yratio = (480 + 84) / (CAMERA_HEIGHT + 28);
	mouseX = mouseX / xratio + centreX;
	mouseY = mouseY / yratio;
	glm::vec2 min = glm::vec2(float(boundingBox[0]), float(boundingBox[1]));
	glm::vec2 max = glm::vec2(float(boundingBox[2]), float(boundingBox[3]));
	/*min[0] = min[0]*xratio ;
	min[1] = min[1]*yratio;
	max[0] = max[0]*xratio + centreX;
	max[1] = max[1]*yratio;*/
	cout << "BoungdingBox del lemming: " << min[0] << " " << min[1] << " " << max[0] << " " << max[1] << endl;
	return (((mouseX < max[0]) && (mouseX > min[0])) && ((mouseY < max[1]) && (mouseY > min[1])));
}

void Lemming::initShaders()
{
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

int Lemming::getTipus()
{
	return tipusLemming;
}

void Lemming::keyReleased(int key)
{
	if (bselected) {
		if (key == 51)
			state = WALKING_RIGHT_STATE;
		if (key == 49) {
			if (state == BASH_LEFT_STATE)
				state = WALKING_LEFT_STATE;
			if (state == BASH_RIGHT_STATE)
				state = WALKING_RIGHT_STATE;
		}
		if (key == 50) {
			if (state == BUILD_LEFT_STATE)
				state = WALKING_LEFT_STATE;
			if (state == WALKING_RIGHT_STATE)
				state = BUILD_RIGHT_STATE;
		}
	}

}

void Lemming::keyPressed(int key)
{
	if (bselected) {
		if (key == 51) {
			cout << "tecla81" << endl;
			if (state == WALKING_LEFT_STATE)state = DIG_LEFT_STATE;
			else if (state == WALKING_RIGHT_STATE) state = DIG_RIGHT_STATE;
			sprite->changeAnimation(DIG);
		}
		if (key == 69)
			state = EXPLODE_STATE;
		if (key == 49) {
			if (state == WALKING_LEFT_STATE)
				state = BASH_LEFT_STATE;
			if (state == WALKING_RIGHT_STATE)
				state = BASH_RIGHT_STATE;
		}
		if (key == 50) {
			if (state == WALKING_LEFT_STATE)
				state = BUILD_LEFT_STATE;
			if (state == WALKING_RIGHT_STATE)
				state = BUILD_RIGHT_STATE;
		}
	}
}

bool Lemming::canDoAction(int request) {
	if (request == 8) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) return true;
	}
	else if (request == 9) {
		if (state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE) return true;
	}
	return false;
}
void Lemming::doAction(int request){
	if (request == 8) {
		++powers[request];
	}
	else if (request == 9) {
		if (state == WALKING_LEFT_STATE)state = DIG_LEFT_STATE;
		else if (state == WALKING_RIGHT_STATE) state = DIG_RIGHT_STATE;
		sprite->changeAnimation(DIG);
	}
}