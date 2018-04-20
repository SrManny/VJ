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
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int tipus, float desplzamiento);
	void update(int deltaTime, float centreX);
	void render(glm::mat4 projection);

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	int  mouseRelease(int mouseX, int mouseY, int button);
	void setMapMask(VariableTexture *mapMask);
	void setMapColor(VariableTexture * mapColor);
	void eraseMask(glm::vec2 pos);
	bool intersecta(int mouseX, int mouseY);
	int getTipus();
	void keyReleased(int key);
	void keyPressed(int key);
	bool getIfSelected();
	void explosion(glm::vec2 pos);
	void bash(glm::vec2 pos, int ind);
	void dig(glm::vec2 pos);
	void build(glm::vec2 pos, int ind);
	void stop(glm::vec2 pos);
	void stopColor(glm::vec2 pos);
	void resumePikmin(glm::vec2 pos);
	bool canDoAction(int request);
	void doAction(int request);
	int getState();

	void die();

	//NEW
	void explode();
	bool hitLevel(glm::vec4 Box);
	bool isDead();

private:
	int collisionFloor(int maxFall);
	bool collisionCeiling(int offset);
	bool collision(int offset);
	int* getboundingBox();
	int buildPixels;
	void initShaders();

private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DYING_STATE, DEATH_STATE, DIG_LEFT_STATE, DIG_RIGHT_STATE, BASH_LEFT_STATE,
		BASH_RIGHT_STATE, CLIMB_LEFT_STATE, CLIMB_RIGHT_STATE, BUILD_LEFT_STATE, BUILD_RIGHT_STATE, EXPLODE_STATE, BLOCK_STATE, COLOR_BLOCK_STATE, STOP_LEFT_STATE, STOP_RIGHT_STATE,
		STOP_COLOR_LEFT_STATE, STOP_COLOR_RIGHT_STATE
	};

	LemmingState state;
	Texture spritesheet, selected[2];
	TexturedQuad *selectedLemmingQuad;
	Sprite *sprite;
	VariableTexture *mask, *color;
	ShaderProgram zetaTextProgram;
	glm::mat4 modelView;
	int lemmingClicked;
	int boundingBox[4], powers[12], alturaMaxima;
	float explodeTime, dyngTime;
	float centreX, desplazamiento;
	bool bselected, primero, primeroCaida;
	int tipusLemming;
};


#endif // _LEMMING_INCLUDE


