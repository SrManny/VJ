#pragma once
#include "PowersBar.h"
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include <list>
#include "Lemming.h"
#include <SFML\Audio.hpp>

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
	int mouseRelease(int mouseX, int mouseY, int button);
	void keypressed(int key);
	void keyreleased(int key);
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void renderScore();
	void renderFinalScore();
	int WantToGoBack();

	void gameFinish(); //NEW
	void Nuke(); //NEW
	void setTrampa();
	void eraseTrampa();
	void deleteDeadPikmins(); //NEW

	void collisionLevel(); //NEW

	void initMatrixs();
	void initShaders();
	int getSpeed();

	void spawnPikmin(int tipus);

private:
	Texture  powersTexture, fastForwardButton, numbers, spawn, exit, pausedTexture, loseTextures[2], gameOverTexture, winTexture, winTextures[2];
	VariableTexture colorTexture, maskTexture;
	MaskedTexturedQuad *map;
	TexturedQuad *powersQuad, *fastForwardQuad, *numbersQuad[10], *numbersResultQuad[10], *spawnQuad, *exitQuad, *pausedQuad, *gameOverAndWinQuad, *loseQuads[3], *trampaQuad, *interruptorQuad;
	int inCentreX, speed, stateBackMenu, stateRetry;
	bool overBackMenu, overRetry;
	Lemming lemmings[1];
	Lemming PikminAux;
	vector<Lemming> vPik;
	int lemmingsSelected[50];
	int maxPikmins, winPikmins, paused, weLost, weWantToGoBack, weWin;
	vector <int> actualment{ 0,0,0,0 };
	int nLevel, survived, Time, out;
	float second, winTime;
	float offsetxLevel, sizeOfLevel, spawnTime;
	glm::vec2 spawnPoint;
	glm::vec2 exitPoint;
	sf::Music ost, winSong, gameOverSong;
	bool mapPressed;
	string LevelTextureLocation;
	string LevelMaskLocation;
	PowersBar powersBar;
	glm::mat4 projection, projection2, fastForwardModel, projectionButtons, timeMatrix[3], outLemmingsMatrix[3], inLemmingsMatrix[3], spawnModel, exitModel;
	glm::mat4 pausedMatrix, loseMatrix[2], resultMatrix[3][3];
	ShaderProgram simpleTexProgram, maskedTexProgram, zetaTextProgram;
	//NEW
	int requiredPercent;
	Texture trampa, interruptor;
	glm::vec4 trampaBox, exitBox, interruptorBox;
	glm::mat4 trampaModel, interruptorModel;
	int tipusTrampa;
	bool interruptorON;
};