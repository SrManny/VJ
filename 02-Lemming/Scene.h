#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Menu.h"
#include "PowersBar.h"
#include "SelectLevels.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	int mouseRelease(int x, int y, int button);

private:
	void initShaders();
	void initMatrixs();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);

private:

	//menuTexture es donde cargo el fondo del menu
	Texture colorTexture, powersTexture, fastForwardButton;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	TexturedQuad *powersQuad, *fastForwardQuad;
	//menuText tendra tantos elementos como size (pos0 el fondo, pos1 el boton start...)
	//zetaTextProgram es un shader para poder cargar cada elemento del menu con una zeta diferente en funcion de la distancia
	//y que no haya problemas de zflighting
	ShaderProgram simpleTexProgram, maskedTexProgram, zetaTextProgram;
	float currentTime, centreX;
	//0 menu, 1 selectLevels, 2 niveles, 3 creditos;
	int escenario, speed;
	glm::mat4 projection, projection2, fastForwardModel, projectionButtons;
	bool bmenu, mapPressed, bselectLevels;
	int selectedLemmingsState[1];
	int numberOfLemmings;
	Lemming lemming;
	Menu menu;
	SelectLevels selectLevels;
	PowersBar powersBar;

};

#endif // _SCENE_INCLUDE

