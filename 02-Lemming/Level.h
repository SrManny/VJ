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

	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);

	void initMatrixs();
	void initShaders();
	int getSpeed();

public:
	Texture colorTexture, powersTexture, fastForwardButton;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	TexturedQuad *powersQuad, *fastForwardQuad;
	int inCentreX, speed;
	vector<Lemming> lemmings;
	int maxPikmins;
	vector <int> actualment{ 0,0,0,0 };
	int nLevel;
	int survived;
	int Time;
	float offsetxLevel, sizeOfLevel;;
	Lemming lemming;
	glm::vec2 spawnPoint;
	glm::vec2 exitPoint;
	bool mapPressed;
	string LevelTextureLocation;
	string LevelMaskLocation;
	PowersBar powersBar;
	glm::mat4 projection, projection2, fastForwardModel, projectionButtons;
	ShaderProgram simpleTexProgram, maskedTexProgram, zetaTextProgram;
};
