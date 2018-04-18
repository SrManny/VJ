#pragma once
#include "PowersBar.h"
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include <list>
#include "Lemming.h"

class Level 
{
public:
	Level();
	~Level();

	void init(int nLevel);
	void setValues();
	void render();
	void update(int deltaTime);


	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void mouseRelease(int mouseX, int mouseY, int button);
	void keypressed(int key);
	void keyreleased(int key);
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void renderScore();
	int WantToGoBack();

	void initMatrixs();
	void initShaders();
	int getSpeed();

	void spawnPikmin(int tipus); 

public:
	Texture  powersTexture, fastForwardButton, numbers, spawn, exit, pausedTexture, loseTextures[2], winTexture;
	VariableTexture colorTexture, maskTexture;
	MaskedTexturedQuad *map;
	TexturedQuad *powersQuad, *fastForwardQuad, *numbersQuad[10], *spawnQuad, *exitQuad, *pausedQuad, *loseQuads[3];
	int inCentreX, speed, stateBackMenu, stateRetry;
	bool overBackMenu, overRetry;
	Lemming lemmings[1];
	Lemming PikminAux; 
	vector<Lemming> vPik;
	bool lemmingsSelected[1];
	int maxPikmins, winPikmins, paused, weLost, weWantToGoBack, weWin;
	vector <int> actualment{ 0,0,0,0 };
	int nLevel, survived, Time, out;
	float second, winTime;
	float offsetxLevel, sizeOfLevel;
	glm::vec2 spawnPoint;
	glm::vec2 exitPoint;
	bool mapPressed;
	string LevelTextureLocation;
	string LevelMaskLocation;
	PowersBar powersBar;
	glm::mat4 projection, projection2, fastForwardModel, projectionButtons, timeMatrix[3], outLemmingsMatrix[3], inLemmingsMatrix[3], spawnModel, exitModel;
	glm::mat4 pausedMatrix, loseMatrix[2];
	ShaderProgram simpleTexProgram, maskedTexProgram, zetaTextProgram;
};
