#ifndef _SELECTLEVELS_INCLUDE
#define _SELECTLEVELS_INCLUDE

#include <iostream>
#include <list>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"

class SelectLevels
{
public:
	SelectLevels();
	~SelectLevels();

	void init(glm::mat4 projection);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	int mouseRelease(int button);

private:
	void initShaders();
	void startButtonMatrixs();

private:
	Texture background, levelsButtons[4];
	TexturedQuad *backQuad, *levelsButtonsQuads[4];
	ShaderProgram zetaTextProgram;
	glm::mat4 projection;
	//0 normal, 1 raton encima, 2 clik en el boton
	string imageLoc[4];
	//-1 nada, 0 nivel 1, 1 nivel 2, 2 nivel 3, 3 volver al menu
	int pressedButton[4];
	//start is pressed
	glm::mat4 levelsButtonsModel[4];
};

#endif