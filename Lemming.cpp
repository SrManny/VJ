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
	WALKING_LEFT, WALKING_RIGHT, FALLING_LEFT, FALLING_RIGHT, DYING, DEATH, DIG, BASH_LEFT, BASH_RIGHT, CLIMB, BUILD_LEFT, BUILD_RIGHT, EXPLODE, BLOCK, STOPPER
};

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int tipus, float desplazamiento)
{
	lemmingClicked = -1;
	bselected = false;
	this->desplazamiento = desplazamiento;
	primero = true;primeroCaida = true;
	initShaders();
	state = FALLING_RIGHT_STATE;
	for (int i = 0; i < 12; ++i) powers[i] = 0;
	spritesheet.loadFromFile("images/Pikmin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125 / 2.f, 0.25 / 2), &spritesheet, &shaderProgram);
	//tipusLemming = rand() % 4;
	tipusLemming = tipus;
	sprite->setNumberAnimations(14);

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
	sprite->addKeyframe(CLIMB, glm::vec2(0.5625, tipusLemming / 4.0));

	sprite->setAnimationSpeed(BUILD_LEFT, 2);
	for (int i = 0; i < 2; i++)
		sprite->addKeyframe(BUILD_LEFT, glm::vec2(0.625 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(BUILD_RIGHT, 2);
	for (int i = 0; i < 2; i++)
		sprite->addKeyframe(BUILD_RIGHT, glm::vec2(0.5 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(EXPLODE, 4);
	for (int i = 0; i < 4; i++) {
		sprite->addKeyframe(EXPLODE, glm::vec2(0.75 + float(i) / 16.0, tipusLemming / 4.0));
	}

	sprite->setAnimationSpeed(DYING, 4);
	for (int i = 0; i < 3; i++)
		sprite->addKeyframe(DYING, glm::vec2(0.75 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(DEATH, 1);
	sprite->addKeyframe(DEATH, glm::vec2(0.9375, tipusLemming / 4.0 + 0.125));

	sprite->setAnimationSpeed(STOPPER, 1);
	sprite->addKeyframe(STOPPER, glm::vec2(0.75, tipusLemming / 4.0));

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
	int fall, fall2;
	this->centreX = centreX;
	//cout << sprite->position().x << " " << sprite->position().y << endl;
	if (primeroCaida && (sprite->position().y <= 0 || sprite->position().y >= 140)) {
		sprite->changeAnimation(DYING);
		state = DYING_STATE;
		dyngTime = 0.f;
		primeroCaida = false;
	}
	if (sprite->update(deltaTime) == 0)
		return;
	//cout << "me estoy actualizando" << endl;
	switch (state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		fall2 = collisionFloor(90);
		if (caidaMax < fall2) caidaMax = fall2;
		if (fall > 0) {
			sprite->position() += glm::vec2(0, fall);
			sprite->changeAnimation(FALLING_LEFT);
		}
		else {
			if (caidaMax > 82) {
				dyngTime = 0.f;
				caidaMax = 0.f;
				sprite->changeAnimation(DYING);
				state = DYING_STATE;
			}
			else {
				state = WALKING_LEFT_STATE;
				sprite->changeAnimation(WALKING_LEFT);
			}
		}
		break;
	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		fall2 = collisionFloor(90);
		if (caidaMax < fall2) caidaMax = fall2;
		if (fall > 0) {
			sprite->position() += glm::vec2(0, fall);
			sprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			if (caidaMax > 82){
				dyngTime = 0.f;
				caidaMax = 0.f;
				sprite->changeAnimation(DYING);
				state = DYING_STATE;
			}
			else {
				state = WALKING_RIGHT_STATE;
				sprite->changeAnimation(WALKING_RIGHT);
			}
		}
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if (collision(0))
		{
			if (powers[2] > 0) {
				if (!collisionCeiling(1)) {
					if (collision(-1)) {
						--powers[2];
						sprite->position() += glm::vec2(0, -1);
						sprite->changeAnimation(CLIMB);
						state = CLIMB_LEFT_STATE;
					}
					else if (collision(-2)) {
						--powers[2];
						sprite->position() += glm::vec2(-1, -1);
						sprite->changeAnimation(CLIMB);
						state = CLIMB_LEFT_STATE;
					}
				}
			}
			else if (powers[8] > 0) {
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
			if (fall > 2) {
				caidaMax = collisionFloor(20);
				state = FALLING_LEFT_STATE;
			}
		}
		break;
	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if (collision(0))
		{
			if (powers[2] > 0) {
				if (!collisionCeiling(1)) {
					if (collision(1)) {
						sprite->position() += glm::vec2(0, -1);
						--powers[2];
						sprite->changeAnimation(CLIMB);
						state = CLIMB_RIGHT_STATE;
					}
					else if (collision(2)) {
						sprite->position() += glm::vec2(1, -1);
						--powers[2];
						sprite->changeAnimation(CLIMB);
						state = CLIMB_RIGHT_STATE;
					}
				}
			}
			else if (powers[8] > 0) {
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
			else {
				caidaMax = collisionFloor(20);
				state = FALLING_RIGHT_STATE;
			}
		}
		break;
	case DIG_LEFT_STATE:
		fall = collisionFloor(3);

		if (fall > 0) {
			dig(sprite->position());
			sprite->position() -= glm::vec2(0, -1);
			dig(sprite->position());
			sprite->changeAnimation(DIG);
			caidaMax = collisionFloor(20);
			state = FALLING_LEFT_STATE;
		}
		else {

			dig(sprite->position());
			sprite->position() -= glm::vec2(0, -1);
		}
		break;
	case DIG_RIGHT_STATE:
		fall = collisionFloor(3);

		if (fall > 0) {
			dig(sprite->position());
			sprite->position() -= glm::vec2(0, -1);
			dig(sprite->position());
			sprite->changeAnimation(DIG);
			caidaMax = collisionFloor(20);
			state = FALLING_RIGHT_STATE;
		}
		else {

			dig(sprite->position());
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
				caidaMax = collisionFloor(20);
				state = FALLING_LEFT_STATE;
		}
		else {
			bash(sprite->position(), -1);
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
				caidaMax = collisionFloor(20);
				state = FALLING_RIGHT_STATE;
		}
		else {
			bash(sprite->position(), 1);
		}
		break;
	case BUILD_LEFT_STATE:
		if (!collision(3) && buildPixels < 50) {
			//sprite->changeAnimation(BUILD_LEFT);
			++buildPixels;
			build(sprite->position(), -1);
			sprite->position() += glm::vec2(-2, -1);
		}
		else {
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
		break;
	case BUILD_RIGHT_STATE:
		if (!collision(3) && buildPixels < 50) {
			++buildPixels;
			//sprite->changeAnimation(BUILD_RIGHT);
			build(sprite->position(), 1);
			sprite->position() += glm::vec2(2, -1);
		}
		else {
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		break;
	case STOP_LEFT_STATE:
		sprite->changeAnimation(STOPPER);
		stop(sprite->position());
		break;
	case STOP_RIGHT_STATE:
		sprite->changeAnimation(STOPPER);
		stop(sprite->position());
		break;
	case STOP_COLOR_LEFT_STATE:
		sprite->changeAnimation(STOPPER);
		stopColor(sprite->position());
		break;
	case STOP_COLOR_RIGHT_STATE:
		sprite->changeAnimation(STOPPER);
		stopColor(sprite->position());
		break;
	case CLIMB_LEFT_STATE:
		if (!collisionCeiling(1)) {
			if (collision(-1)) {
				sprite->position() += glm::vec2(0, -1);
			}
			else if (collision(-2)) {
				sprite->position() += glm::vec2(-1, -1);
			}
			else {
				state = WALKING_LEFT_STATE;
				sprite->changeAnimation(WALKING_LEFT);
			}
		}
		else {
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		break;
	case CLIMB_RIGHT_STATE:
		if (!collisionCeiling(1)) {
			if (collision(1)) {
				sprite->position() += glm::vec2(0, -1);
			}
			else if (collision(2)) {
				sprite->position() += glm::vec2(1, -1);
			}
			else {
				state = WALKING_RIGHT_STATE;
				sprite->changeAnimation(WALKING_RIGHT);
			}
		}
		else {
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
		break;
	case DYING_STATE:
		//sprite->changeAnimation(DYING);
		fall = collisionFloor(3);
		dyngTime += 1.f / float(deltaTime);
		if (dyngTime > 0.2) {
			if (fall > 0) {
				sprite->position() += glm::vec2(0, 1);
			}
			else {
				state = DEATH_STATE;
			}
		}
		break;
	case EXPLODE_STATE:
		explodeTime += 1.f/float(deltaTime);
		if (explodeTime > 0.1) {
			//sprite->changeAnimation(EXPLODE);
			explosion(sprite->position());
			sprite->changeAnimation(DYING);
			state = DYING_STATE;
			dyngTime = 0.f;
		}
		break;


	case DEATH_STATE:
		//kill lemming, erase, unrender
		sprite->changeAnimation(DEATH);
		die();
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

void Lemming::setMapColor(VariableTexture *mapColor)
{
	color = mapColor;
}

void Lemming::eraseMask(glm::vec2 pos)
{
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	for (int y = max(0, posY - 8); y <= min(mask->height() - 1, posY + 8); y++)
		for (int x = max(0, posX - 8); x <= min(mask->width() - 1, posX + 8); x++) {
			float distance = sqrt(pow((x - posX), 2) + pow((y - posY), 2));
			if (distance < 5) mask->setPixel(x, y, 0);
		}
}

void Lemming::explosion(glm::vec2 pos)
{
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	for (int i = -10; i < 10; ++i)
		for (int j = -10; j < 10; ++j) {
			float distance = sqrt(pow((i), 2) + pow((j), 2));
			if (distance < 10) mask->setPixel(posX + i, posY + j, 0);
		}

}

void Lemming::bash(glm::vec2 pos, int ind)
{
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	//ind indica si se excava a izquierda o derecha, 1 para derecha, -1 para izquierda
	if (ind == 1) {
		for (int i = 0; i < 10; ++i) {
			mask->setPixel(posX - 1, posY + (i*ind) - (8 * ind), 0);
			mask->setPixel(posX, posY + (i*ind) - (8 * ind), 0);
			mask->setPixel(posX + 1, posY + (i*ind) - (8 * ind), 0);
			mask->setPixel(posX + 2, posY + (i*ind) - (8 * ind), 0);
		}
	}
	else {
		for (int i = 0; i < 10; ++i) {
			mask->setPixel(posX + 1, posY + (i*ind) + 1, 0);
			mask->setPixel(posX, posY + (i*ind) + 1, 0);
			mask->setPixel(posX - 1, posY + (i*ind) + 1, 0);
			mask->setPixel(posX - 2, posY + (i*ind) + 1, 0);
		}
	}
}

void Lemming::dig(glm::vec2 pos)
{
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	for (int i = 0; i < 8; ++i) {
		mask->setPixel(posX + i - 4, posY, 0);
		mask->setPixel(posX + i - 4, posY + 1, 0);
		mask->setPixel(posX + i - 4, posY + 2, 0);
	}
}

void Lemming::build(glm::vec2 pos, int ind)
{
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	//ind indica si se construye a izquierda o derecha, 1 para derecha, -1 para izquierda
	if (ind == -1) {
		for (int i = 0; i < 4; ++i) {
			mask->setPixel(posX - 4 - i, posY + 1, 255);
			color->setPixel(posX - 4 - i, posY + 1, glm::ivec4(250, 250, 101, 255));
		}
	}
	else {
		for (int i = 0; i < 4; ++i) {
			mask->setPixel(posX + i, posY + 1, 255);
			color->setPixel(posX + i, posY + 1, glm::ivec4(250, 250, 101, 255));
		}
	}
}

void Lemming::stop(glm::vec2 pos) {
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 5; ++j) {
			if (mask->pixel(posX + j - 3, posY - i) == 0) {
				mask->setPixel(posX + j - 3, posY - i, 254);
			}
		}

}

void Lemming::stopColor(glm::vec2 pos) {
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 5; ++j)
			if (mask->pixel(posX + j, posY - i) == 0) {
				mask->setPixel(posX + j - 3, posY - i, 255 - 2 - tipusLemming);
			}
}

void Lemming::resumePikmin(glm::vec2 pos) {
	int posX = pos[0] + desplazamiento + 9;
	int posY = pos[1] + 14;
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j) {
			if (mask->pixel(posX + j - 8, posY - i + 4) == 254) {
				mask->setPixel(posX + j - 8, posY - i + 4, 0);
			}
			if (mask->pixel(posX + j - 8, posY - i + 4) == 255 - 2 - tipusLemming) {
				mask->setPixel(posX + j - 8, posY - i + 4, 0);

			}
		}
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(desplazamiento, 0); // Add the map displacement

	posBase += glm::ivec2(7, 16);
	while ((fall < maxFall) && !bContact)
	{
		if ((mask->pixel(posBase.x, posBase.y + fall) == 0) && (mask->pixel(posBase.x + 1, posBase.y + fall) == 0))
			fall += 1;
		else if ((mask->pixel(posBase.x, posBase.y + fall) == 255 - tipusLemming - 2) && (mask->pixel(posBase.x + 1, posBase.y + fall) == 255 - tipusLemming - 2))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collisionCeiling(int offset)
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(desplazamiento, 0); // Add the map displacement

	posBase += glm::ivec2(7, 15 - offset);
	if ((mask->pixel(posBase.x, posBase.y) == 0))
		return false;
	else if ((mask->pixel(posBase.x, posBase.y) == 255 - tipusLemming - 2))
		return false;

	return false;
}

bool Lemming::collision(int offset)
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(desplazamiento, 0); // Add the map displacement

	posBase += glm::ivec2(7 + offset, 15);
	int coutAux = mask->pixel(posBase.x + 1, posBase.y);
	if (mask->pixel(posBase.x, posBase.y) == 0 || mask->pixel(posBase.x, posBase.y) == 255 - tipusLemming - 2)
		if (mask->pixel(posBase.x + 1, posBase.y) == 0 || mask->pixel(posBase.x + 1, posBase.y) == 255 - tipusLemming - 2)
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
	//cout << "BoungdingBox del lemming: " << min[0] << " " << min[1] << " " << max[0] << " " << max[1] << endl;
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

	}

}

void Lemming::keyPressed(int key)
{
	if (bselected) {}
}

bool Lemming::canDoAction(int request) {
	if (request == 8) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) return true;
	}
	else if (request == 2)  return true;
	else if (request == 3) return true;
	else if (request == 4) {
		if (state == STOP_COLOR_LEFT_STATE || state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE || state == STOP_COLOR_RIGHT_STATE) return true;
	}
	else if (request == 5) {
		if (state == STOP_RIGHT_STATE || state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE || state == STOP_LEFT_STATE) return true;
	}
	else if (request == 6) {
		if (state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE) return true;
	}
	else if (request == 9) {
		if (state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE) return true;
	}
	return false;
}

void Lemming::doAction(int request) {
	if (request == 8 || request == 2) {
		++powers[request];
	}
	else if (request == 9) {
		if (state == WALKING_LEFT_STATE) state = DIG_LEFT_STATE;
		else if (state == WALKING_RIGHT_STATE) state = DIG_RIGHT_STATE;
		sprite->changeAnimation(DIG);
	}
	else if (request == 4) {
		if (state == WALKING_LEFT_STATE) {
			state = STOP_COLOR_LEFT_STATE;
			sprite->changeAnimation(STOPPER);
		}
		else if (state == WALKING_RIGHT_STATE) {
			state = STOP_COLOR_RIGHT_STATE;
			sprite->changeAnimation(STOPPER);
		}
		else if (state == STOP_COLOR_LEFT_STATE) {
			resumePikmin(sprite->position());
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		else if (state == STOP_COLOR_RIGHT_STATE) {
			resumePikmin(sprite->position());
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
	}
	else if (request == 5) {
		if (state == WALKING_LEFT_STATE) {
			state = STOP_LEFT_STATE;
			sprite->changeAnimation(STOPPER);
		}
		else if (state == WALKING_RIGHT_STATE) {
			state = STOP_RIGHT_STATE;
			sprite->changeAnimation(STOPPER);
		}
		else if (state == STOP_LEFT_STATE) {
			resumePikmin(sprite->position());
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		else if (state == STOP_RIGHT_STATE) {
			resumePikmin(sprite->position());
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
	}
	else if (request == 6) {
		buildPixels = 0;
		if (state == WALKING_LEFT_STATE) {
			state = BUILD_LEFT_STATE;
			sprite->changeAnimation(BUILD_LEFT);
		}
		else if (state == WALKING_RIGHT_STATE) {
			state = BUILD_RIGHT_STATE;
			sprite->changeAnimation(BUILD_RIGHT);
		}
		else if (state == BUILD_LEFT_STATE) {
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		else if (state == BUILD_RIGHT_STATE) {
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
	}
	else if (request == 3) {
		state = EXPLODE_STATE;
		explodeTime = 0.f;
		sprite->changeAnimation(EXPLODE);
	}
}

void Lemming::die()
{
	state = DEATH_STATE;
}

void Lemming::explode()
{
		explodeTime = 0.f;
		state = EXPLODE_STATE;
		sprite->changeAnimation(EXPLODE);
		explosion(sprite->position());
}

bool Lemming::hitLevel(glm::vec4 Box)
{
	int posXmin = Box[0];
	int posXmax = Box[1];
	int posYmin = Box[2];
	int posYmax = Box[3];
	glm::vec2 aux = sprite->position();
	int pikX = aux[0];
	int pikY = aux[1];
	if (pikX > posXmin && pikX < posXmax && pikY > posYmin && pikY < posYmax) {
		state = DEATH_STATE;
		return true;
	}
	return false;
}

bool Lemming::isDead() {
	if (state == DEATH_STATE) {
		return true;
	}
	return false;
}

int Lemming::getState() {
	return state;
}

