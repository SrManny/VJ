#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <iostream>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include <SFML/Audio.hpp>

class Menu
{
public:
	Menu();
	~Menu();

	void init(glm::mat4 projection);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	int mouseRelease(int button);

private:
	void initShaders();
	void startButtonMatrixs();
	bool intersecta(int x, int y, glm::vec4 min, glm::vec4 max);

private:
	Texture background, start[3], select[3], insButton, instructions, exitIns, creditosTexture, creditosBut;
	TexturedQuad *backQuad, *startQuad[3], *selectQuad[3], *insQuad[3], *exitInsQuad[3];
	ShaderProgram zetaTextProgram;
	glm::mat4 projection;
	sf::Music osts;
	//0 normal, 1 raton encima, 2 clik en el boton
	int startSprite, selectSprite, insSprite, exitInsSprite, creditosSprite;
	//start is pressed
	bool overStart, overSelect, overIns, overExit, insPressed, overCreditos, creditosPressed;
	glm::mat4 startInvMatrix, startModel, selectMatrix, selectModel, insMatrix, exitInsMatrix, creditosMatrix;
};

#endif