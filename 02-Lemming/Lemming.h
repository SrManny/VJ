#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "TexturedQuad.h"
#include "ShaderProgram.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime, float centreX);
	void render(glm::mat4 projection);
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	int  mouseRelease(int mouseX, int mouseY, int button);
	void setMapMask(VariableTexture *mapMask);
	bool intersecta(int mouseX, int mouseY);
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	int* getboundingBox();
	void initShaders();
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE
	};

	LemmingState state;
	Texture spritesheet, selected[2];
	TexturedQuad *selectedLemmingQuad;
	Sprite *sprite;
	VariableTexture *mask;
	ShaderProgram zetaTextProgram;
	glm::mat4 modelView;
	int lemmingClicked;
	int boundingBox[4];
	float centreX;
	bool bselected, primero;
};


#endif // _LEMMING_INCLUDE


