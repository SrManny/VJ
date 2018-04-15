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

	void init(glm::mat4 projection);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	int mouseRelease(int button);

private:
	void initShaders();
	void startButtonMatrixs();
	bool intersecta(int x, int y, glm::vec4 min, glm::vec4 max);

private:
	Texture background, start[3], select[3];
	TexturedQuad *backQuad, *startQuad[3], *selectQuad[3];
	ShaderProgram zetaTextProgram;
	glm::mat4 projection;
	//0 normal, 1 raton encima, 2 clik en el boton
	int startSprite, selectSprite;
	//start is pressed
	bool overStart, overSelect;
	glm::mat4 startInvMatrix, startModel, selectMatrix, selectModel;
};

#endif