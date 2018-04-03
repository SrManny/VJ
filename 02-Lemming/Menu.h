#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <iostream>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"

class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	bool mouseRelease(int button);

private:
	void initShaders();
	void ButtonsMatrixs();

private:
	Texture background, start[3], selectLevels[3];
	TexturedQuad *backQuad, *startQuad[3], *selectQuad[3];
	ShaderProgram zetaTextProgram;
	glm::mat4 projection;
	//0 normal, 1 raton encima, 2 clik en el boton
	int startSprite, selectSprite;
	//start is pressed
	bool overStart, overSelect;
	glm::mat4 startInvMatrix, startModel;
};

#endif