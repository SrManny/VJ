#ifndef _POWERSBAR_INCLUDE
#define _POWERSBAR_INCLUDE

#include <iostream>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"

class PowersBar
{
public:
	PowersBar();
	~PowersBar();

	void init();
	void render();

	int mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	int mouseRelease(int button);

private:
	void initShaders();
	void initPowerMatrixs();
	bool intersecta(int x, int y, glm::vec4 min, glm::vec4 max);
	void loadActionButtonsImages();

private:
	Texture barTexture, start[3], map[3], mapSelectedTexture, actionButton[12];
	TexturedQuad *barQuad, *startQuad[3], *mapQuad, *mapSelectedQuad, *actionButtonQuad[12];
	ShaderProgram zetaTextProgram;
	glm::mat4 projection;
	int level;
	float mapsizeW, mapsizeH;
	bool overMap;
	//0 normal, 1 raton encima, 2 clik en el boton
	//int startSprite;
	//start is pressed
	//bool overStart;
	glm::mat4 powerModelMatrix, mapModelMatrix, mapSelectedMatrix, buttonsMatrix[12];

};

#endif
