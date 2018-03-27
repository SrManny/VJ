#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "TexturedQuad.h"


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

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);

private:

	//menuTexture es donde cargo el fondo del menu
	Texture colorTexture, menuTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	//menuText tendra tantos elementos como size (pos0 el fondo, pos1 el boton start...)
	TexturedQuad *menuText[2];
	//zetaTextProgram es un shader para poder cargar cada elemento del menu con una zeta diferente en funcion de la distancia
	//y que no haya problemas de zflighting
	ShaderProgram simpleTexProgram, maskedTexProgram, zetaTextProgram;
	float currentTime;
	glm::mat4 projection;
	bool menu;
	Lemming lemming;

};


#endif // _SCENE_INCLUDE

