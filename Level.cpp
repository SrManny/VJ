#include "Level.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

Level::Level()
{
	map = NULL;
}


Level::~Level()
{
	if (map != NULL)
		delete map;
}

void Level::initMatrixs() {
	float centreX = float(CAMERA_WIDTH)*16.f / 648.f;
	float centreY = 28.f*35.f / 84.f;
	float centreX2 = float(CAMERA_WIDTH)*20.f / 648.f;
	float centreY2 = 28.f*40.f / 84.f;
	fastForwardModel = glm::mat4(1.0f);
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(305.f, 30.f / 2.f, 0.f));
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(-25.f / 2.f, -25.f / 2.f, 0.f));

	spawnModel = glm::mat4(1.0f);
	spawnModel = glm::translate(spawnModel, glm::vec3(spawnPoint[0] - 8, spawnPoint[1] - 8, 0.f));

	if (nLevel == 3) {
		trampaModel = glm::mat4(1.0f);
		trampaModel = glm::translate(trampaModel, glm::vec3(200.f, 30.f, 0.f));
	}
	exitModel = glm::mat4(1.0f);
	exitModel = glm::translate(exitModel, glm::vec3(exitPoint[0] + 12, exitPoint[1], 0.f));

	pausedMatrix = glm::mat4(1.0f);
	pausedMatrix = glm::translate(pausedMatrix, glm::vec3(150.f, 80.f, 0.f));
	pausedMatrix = glm::translate(pausedMatrix, glm::vec3(-100.f / 2.f, -50.f / 2.f, 0.f));
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			resultMatrix[i][j] = glm::mat4(1.0f);
			resultMatrix[i][j] = glm::translate(resultMatrix[i][j], glm::vec3((650.f + j * 25)*320.f / 960.f, (193.f + i * 84.f) / 564.f*188.f, 0.f));
			resultMatrix[i][j] = glm::translate(resultMatrix[i][j], glm::vec3(-centreX2 / 2.f, -centreY2 / 2.f, 0.f));
		}
	for (int i = 0; i < 2; ++i) {
		loseMatrix[i] = glm::mat4(1.0f);
		loseMatrix[i] = glm::translate(loseMatrix[i], glm::vec3((239.f / 960.f)*320.f + i * (480.f / 960.f)*320.f, (502.f / 564.f)*188.f, 0.f));
		loseMatrix[i] = glm::translate(loseMatrix[i], glm::vec3(-320.f*(202.f / 960.f) / 2.f, -((538 - 465) / 564.f)*188.f / 2.f, 0.f));
	}

	for (int i = 0; i < 3; ++i) {
		timeMatrix[i] = glm::mat4(1.0f);
		timeMatrix[i] = glm::translate(timeMatrix[i], glm::vec3(((576.f + 16.f*i) / 648.f)*320.f, 28.f*(14.f / 84.f) + 160.f, 0.f));
		timeMatrix[i] = glm::translate(timeMatrix[i], glm::vec3(-centreX / 2.f, -centreY / 2.f, 0.f));
		outLemmingsMatrix[i] = glm::mat4(1.0f);
		outLemmingsMatrix[i] = glm::translate(outLemmingsMatrix[i], glm::vec3(((290.f + 16.f*i) / 648.f)*320.f, 28.f*(14.f / 84.f) + 160.f, 0.f));
		outLemmingsMatrix[i] = glm::translate(outLemmingsMatrix[i], glm::vec3(-centreX / 2.f, -centreY / 2.f, 0.f));
		inLemmingsMatrix[i] = glm::mat4(1.0f);
		inLemmingsMatrix[i] = glm::translate(inLemmingsMatrix[i], glm::vec3(((415.f + 16.f*i) / 648.f)*320.f, 28.f*(14.f / 84.f) + 160.f, 0.f));
		inLemmingsMatrix[i] = glm::translate(inLemmingsMatrix[i], glm::vec3(-centreX / 2.f, -centreY / 2.f, 0.f));
	}
}

void Level::keypressed(int key) { // NEW
	if (key == 49 || key == 51 || key == 52 || key == 54 || key == 50) {
		int request;
		if (key == 49) request = 8;
		else if (key == 50) request = 6;
		else if (key == 51) request = 9;
		else if (key == 52) request = 5;
		else if (key == 53) request = 4;
		else if (key == 54) request = 2;
		else if (key == 3) request = 3;
		cout << request << endl;
		int stateAction = powersBar.getPowersBarState(request);
		int total = 0;
		for (int i = 0; i < vPik.size(); ++i) {
			if (vPik[i].getIfSelected() && vPik[i].canDoAction(request)) {
				lemmingsSelected[i] = i;
				if (vPik[i].getState() < 17 && vPik[i].getState() < 20)
					++total;
			}
		}
		//fallamos intentando realizar un poder
		if (total > stateAction) {
			//Ruido de fallo
		}
		else {
			for (int i = 0; i < vPik.size(); ++i) if (lemmingsSelected[i] != -1) {
				vPik[lemmingsSelected[i]].doAction(request);
				lemmingsSelected[i] = -1;
			}
			powersBar.setSpendPowers(request, total);
		}
		powersBar.finishRequest();
	}
	else if (key == 110) //NUKE, Key: n 
		Nuke();
	else if (key == 122) //SPAWN RED, KEY: z
		spawnPikmin(0);
	else if(key == 120) //SPAWN BLUE, KEY: x
		spawnPikmin(1);
	else if (key == 99) //SPAWN YELLOW, KEY: c
		spawnPikmin(2);
	else if (key == 118) //SPAWN PURPLE, KEY: v
		spawnPikmin(3);
}

void Level::keyreleased(int key) {

	lemmings[0].keyReleased(key);
}
void Level::renderFinalScore() {
	int aux0;
	for (int i = 0; i < 3; ++i) {
		if (i == 0) aux0 = survived;
		else if (i == 1) aux0 = survived;
		else aux0 = (float(survived) / float(winPikmins)) * 100;
		for (int j = 0; j < 3; ++j) {
			int div = pow(10, 3 - j - 1);
			int digit = aux0 / div;
			zetaTextProgram.setUniformMatrix4f("modelview", resultMatrix[i][j]);
			numbersResultQuad[digit]->render(numbers);
			aux0 = aux0 % div;
		}
	}
}
void Level::renderScore() {
	int aux0 = out;
	for (int i = 0; i < 3; ++i) {
		int div = pow(10, 3 - i - 1);
		int digit = aux0 / div;
		zetaTextProgram.setUniformMatrix4f("modelview", outLemmingsMatrix[i]);
		numbersQuad[digit]->render(numbers);
		aux0 = aux0 % div;
	}
	int aux1 = (float(survived) / float(winPikmins)) * 100;
	for (int i = 0; i < 3; ++i) {
		int div = pow(10, 3 - i - 1);
		int digit = aux1 / div;
		zetaTextProgram.setUniformMatrix4f("modelview", inLemmingsMatrix[i]);
		numbersQuad[digit]->render(numbers);
		aux1 = aux1 % div;
	}

	int aux2 = Time;
	for (int i = 0; i < 3; ++i) {
		if (Time >= 0) {
			int div = pow(10, 3 - i - 1);
			int digit = aux2 / div;
			zetaTextProgram.setUniformMatrix4f("modelview", timeMatrix[i]);
			numbersQuad[digit]->render(numbers);
			aux2 = aux2 % div;
		}
	}
}

void Level::init(int nLevel)
{
	inCentreX = 0.f;
	speed = 1;
	second = 0.f, winTime = 3.f;
	//winSong.openFromFile("soundTrack/14.results.wav");
	//winSong.setLoop(true);
	//gameOverSong.openFromFile("soundTrack/28liveasapikmin.wav");
	//gameOverSong.setLoop(true);
	paused = 1; weLost = 0, weWin = 0, weWantToGoBack = 0; vPik.clear();
	this->nLevel = nLevel;
	stateBackMenu = 0;
	stateRetry = 0; overBackMenu = false; overRetry = false;
	setValues();
	initShaders();
	initMatrixs();
	powersBar.init();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(sizeOfLevel, float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f,160.f / 256.f) };
	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(LevelTextureLocation, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(LevelMaskLocation, TEXTURE_PIXEL_FORMAT_RGBA);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	//Incializamos imagen del spawn de los Pikmn
	spawn.loadFromFile("images/Inici.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spawn.setMinFilter(GL_NEAREST);
	spawn.setMagFilter(GL_NEAREST);
	glm::vec2 texCoordsSpawn[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomSpawn[2] = { glm::vec2(1.f,1.f), glm::vec2(32.f, 32.f) };
	spawnQuad = TexturedQuad::createTexturedQuad(geomSpawn, texCoordsSpawn, zetaTextProgram);

	//Incializamos imagen de la salida de los Pikmin del nivel
	exit.loadFromFile("images/Goal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	exit.setMinFilter(GL_NEAREST);
	exit.setMagFilter(GL_NEAREST);
	glm::vec2 texCoordsExit[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomExit[2] = { glm::vec2(1.f,1.f), glm::vec2(20.f, 20.f) };
	exitQuad = TexturedQuad::createTexturedQuad(geomSpawn, texCoordsSpawn, zetaTextProgram);

	//Incializamos imagen del boton de Fast Forward
	fastForwardButton.loadFromFile("images/Buttons/fastForwardButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fastForwardButton.setMinFilter(GL_NEAREST);
	fastForwardButton.setMagFilter(GL_NEAREST);
	glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f,1.f) };
	glm::vec2 geomFastButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(25), float(25)) };
	fastForwardQuad = TexturedQuad::createTexturedQuad(geomFastButton, texCoords2, zetaTextProgram);

	//Inicializamos imagen del boton de pause
	pausedTexture.loadFromFile("images/Buttons/pausedState.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pausedTexture.setMinFilter(GL_NEAREST);
	pausedTexture.setMagFilter(GL_NEAREST);
	winTexture.loadFromFile("images/Buttons/pausedState.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winTexture.setMinFilter(GL_NEAREST);
	winTexture.setMagFilter(GL_NEAREST);
	glm::vec2 geomPausedButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(100), float(50)) };
	pausedQuad = TexturedQuad::createTexturedQuad(geomPausedButton, texCoords2, zetaTextProgram);

	//Inicializamos botones Retry y BackToMenu
	glm::vec2 geomLose[2] = { glm::vec2(0.f, 0.f), glm::vec2(320.f*(202.f / 960.f), ((538 - 465) / 564.f)*188.f) };
	loseTextures[0].loadFromFile("images/Buttons/Retry.png", TEXTURE_PIXEL_FORMAT_RGBA);
	loseTextures[0].setMinFilter(GL_NEAREST);
	loseTextures[0].setMagFilter(GL_NEAREST);
	loseTextures[1].loadFromFile("images/Buttons/mainMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	loseTextures[1].setMinFilter(GL_NEAREST);
	loseTextures[1].setMagFilter(GL_NEAREST);

	winTextures[0].loadFromFile("images/Buttons/next.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winTextures[0].setMinFilter(GL_NEAREST);
	winTextures[0].setMagFilter(GL_NEAREST);
	winTextures[1].loadFromFile("images/Buttons/mainMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winTextures[1].setMinFilter(GL_NEAREST);
	winTextures[1].setMagFilter(GL_NEAREST);

	gameOverTexture.loadFromFile("images/winOrGameOverScreen/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOverTexture.setMinFilter(GL_NEAREST);
	gameOverTexture.setMagFilter(GL_NEAREST);
	winTexture.loadFromFile("images/winOrGameOverScreen/winTexture.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winTexture.setMinFilter(GL_NEAREST);
	winTexture.setMagFilter(GL_NEAREST);
	//geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(sizeOfLevel, float(CAMERA_HEIGHT)+28) };
	glm::vec2 geomGameOver[2] = { glm::vec2(0.f, 0.f), glm::vec2(320.f, 188.f) };
	gameOverAndWinQuad = TexturedQuad::createTexturedQuad(geomGameOver, texCoords2, zetaTextProgram);
	for (int i = 0; i < 3; ++i) {
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, float(i) / 3.f), glm::vec2(1.f,float(i) / 3.f + 1.f / 3.f) };
		loseQuads[i] = TexturedQuad::createTexturedQuad(geomLose, texCoords2, zetaTextProgram);
	}
	//Inicializamos el menu y el powerBar

	projectionButtons = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	projection2 = glm::ortho(offsetxLevel, float(offsetxLevel + 320.f - 1), float(160.f - 1 + 28), 0.f);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	glm::vec2 geomNumbers[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH)*16.f / 648.f,28.f*35.f / 84.f) };
	glm::vec2 geomNumbers2[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH)*20.f / 648.f,28.f*40.f / 84.f) };
	for (int i = 0; i < 10; ++i) {
		glm::vec2 texCoordNum[2] = { glm::vec2(i*24.f / 240.f, 0.f), glm::vec2((i*24.f + 24.f) / 240.f,1.f) };
		numbersQuad[i] = TexturedQuad::createTexturedQuad(geomNumbers, texCoordNum, zetaTextProgram);	//Inicializamos el menu y el powerBar
		numbersResultQuad[i] = TexturedQuad::createTexturedQuad(geomNumbers2, texCoordNum, zetaTextProgram);
	}
	numbers.loadFromFile("images/Buttons/numeros3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbers.setMinFilter(GL_NEAREST);
	numbers.setMagFilter(GL_NEAREST);
	int aleatorio = rand() % 4;
	spawnPikmin(aleatorio);

}

void Level::setValues() {
	if (nLevel == 1) {
		maxPikmins = 10;
		out = 0;
		LevelTextureLocation = "images/Levels/fun1.png";
		LevelMaskLocation = "images/Levels/fun1_mask.png";
		spawnPoint = glm::vec2(60, 30);
		exitPoint = glm::vec2(216, 100);
		Time = 100;
		survived = 0.f;
		winPikmins = 10.f;
		offsetxLevel = 120.f;
		sizeOfLevel = 512.f;
		requiredPercent = 50;
		exitBox = glm::vec4(exitPoint[0], exitPoint[0] + 32, exitPoint[1], exitPoint[1] + 32);
		//ost.openFromFile("soundTrack/06theimpactsite.wav");
		//ost.setLoop(true);
		//ost.play();
	}
	else if (nLevel == 2) {
		maxPikmins = 20;
		out = 0;
		LevelTextureLocation = "images/Levels/level2.png";
		LevelMaskLocation = "images/Levels/level2_mask.png";
		spawnPoint = glm::vec2(-120, 30);
		exitPoint = glm::vec2(216, 100);
		Time = 650;
		survived = 10;
		winPikmins = 10;
		offsetxLevel = 449.f;
		sizeOfLevel = 1218.f;
		requiredPercent = 70;
		exitBox = glm::vec4(216, 221, 100, 105);
		//ost.openFromFile("soundTrack/06theimpactsite.wav");
		//ost.setLoop(true);
		//ost.play();
	}

	else if (nLevel == 3) {
		maxPikmins = 30;
		out = 0;
		LevelTextureLocation = "images/Levels/mayhem2.png";
		LevelMaskLocation = "images/Levels/mayhem2_mask.png";
		spawnPoint = glm::vec2(60, 30);
		exitPoint = glm::vec2(216, 100);
		Time = 650;
		survived = 0;
		winPikmins = 10;
		offsetxLevel = 120.f;
		sizeOfLevel = 671.f;
		requiredPercent = 100;
		exitBox = glm::vec4(216, 221, 100, 105);
		//CARGAMOS LA TRAMPA
		trampaBox = glm::vec4(2, 2, 150, 150);
		interruptorBox = glm::vec4(0, 0, 5, 5);
		trampa.loadFromFile("images/Environment/Lava.png", TEXTURE_PIXEL_FORMAT_RGBA);
		trampa.setMinFilter(GL_NEAREST);
		trampa.setMagFilter(GL_NEAREST);
		glm::vec2 texCoordsTrampa[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		glm::vec2 geomTrampa[2] = { glm::vec2(1.f,1.f), glm::vec2(32.f, 32.f) };
		spawnQuad = TexturedQuad::createTexturedQuad(geomTrampa, texCoordsTrampa, zetaTextProgram);

		//ost.openFromFile("soundTrack/06theimpactsite.wav");
		//ost.setLoop(true);
		//ost.play();
	}
}

void Level::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120.f + inCentreX;
	posY = mouseY / 3;

	for (int y = max(0, posY - 4); y <= min(maskTexture.height() - 1, posY + 4); y++)
		for (int x = max(0, posX - 4); x <= min(maskTexture.width() - 1, posX + 4); x++) {
			float distance = sqrt(pow((x - posX), 2) + pow((y - posY), 2));
			if (distance < 5)maskTexture.setPixel(x, y, 0);
		}
}

void Level::applyMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120 + inCentreX;
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Level::render() {
	glm::mat4 modelview;
	if (weLost == 1) {
		cout << "no puedo entrar aqui" << endl;
		zetaTextProgram.use();
		zetaTextProgram.setUniformMatrix4f("projection", projectionButtons);
		zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
		modelview = glm::mat4(1.0f);
		zetaTextProgram.setUniformMatrix4f("modelview", modelview);
		gameOverAndWinQuad->render(gameOverTexture);
		for (int i = 0; i < 2; ++i) {
			zetaTextProgram.setUniformMatrix4f("modelview", loseMatrix[i]);
			if (i == 0) loseQuads[stateRetry]->render(loseTextures[i]);
			else loseQuads[stateBackMenu]->render(loseTextures[i]);
		}
	}
	else if (weWin == 1) {
		//cout << "Entro bien " << endl;
		zetaTextProgram.use();
		zetaTextProgram.setUniformMatrix4f("projection", projectionButtons);
		zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
		modelview = glm::mat4(1.0f);
		zetaTextProgram.setUniformMatrix4f("modelview", modelview);
		gameOverAndWinQuad->render(winTexture);
		for (int i = 0; i < 2; ++i) {
			zetaTextProgram.setUniformMatrix4f("modelview", loseMatrix[i]);
			if (i == 0) loseQuads[stateRetry]->render(winTextures[i]);
			else loseQuads[stateBackMenu]->render(winTextures[i]);
		}
		renderFinalScore();
	}
	else {
		cout << "pero k pasa if imposible " << endl;
		powersBar.render();
		maskedTexProgram.use();
		//projection2 = glm::translate(projection2, glm::vec3(currentTime / 2000.f, 0.f, 0.f));
		maskedTexProgram.setUniformMatrix4f("projection", projection2);
		maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		//Hay que trasladar en x 524 pixeles (81.01 % de la imagen) en 320 son 259.26
		//modelview = glm::translate(modelview, glm::vec3(currentTime/1000.f, 0.f, 0.f));
		maskedTexProgram.setUniformMatrix4f("modelview", modelview);
		map->render(maskedTexProgram, colorTexture, maskTexture);


		//modelview = glm::translate(modelview, glm::vec3(currentTime / 1000.f, 0.f, 0.f));
		//for (int i = 0; i < 1; ++i) lemmings[i].render(projection);
		for (int i = 0; i < vPik.size(); ++i) {
			simpleTexProgram.use();
			//projection = glm::translate(projection, glm::vec3(currentTime / 2000.f, 0.f, 0.f));
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			modelview = glm::mat4(1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			vPik[i].render(projection);
		}

		zetaTextProgram.use();
		zetaTextProgram.setUniformMatrix4f("projection", projectionButtons);
		zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
		//modelview = glm::translate(modelview, glm::vec3(currentTime / 1000.f, 0.f, 0.f));
		zetaTextProgram.setUniformMatrix4f("modelview", fastForwardModel);
		fastForwardQuad->render(fastForwardButton);
		renderScore();
		if (paused == 0) {
			zetaTextProgram.setUniformMatrix4f("modelview", pausedMatrix);
			pausedQuad->render(pausedTexture);
		}
		/*else if (weWin == 1) {
			zetaTextProgram.setUniformMatrix4f("modelview", pausedMatrix);
			pausedQuad->render(winTexture);
		}*/
		//Renderizamos el spaw
		zetaTextProgram.setUniformMatrix4f("projection", projection);
		zetaTextProgram.setUniformMatrix4f("modelview", spawnModel);
		zetaTextProgram.setUniformMatrix4f("trampa", trampaModel);
		spawnQuad->render(spawn);
		//Renderizamos la salida 
		zetaTextProgram.setUniformMatrix4f("modelview", exitModel);
		exitQuad->render(exit);
	}
}

void Level::update(int deltaTime)
{
	float aux = float(deltaTime) * float(speed);
	second += 1 / aux;
	/*if (weWin == 0 && winPikmins <= survived) {
		//ost.stop();
		//winSong.play();
		cout << "pero k pasa" << endl;
		//weWin = 1;
	}
	else*/ if (weWin != 1 && paused == 1 && weLost != 1) {
		if (second > 1) {
			second = 0;
			--Time;
		}
		//Modificar pls
		int aliveLemmings = 10;
		int request = powersBar.getPowersBarRequest();
		if (request != -1) {
			int stateAction = powersBar.getPowersBarState(request);
			int total = 0;
			for (int i = 0; i < vPik.size(); ++i) {
				if (vPik[i].getIfSelected() && vPik[i].canDoAction(request)) {
					lemmingsSelected[i] = i;
					if (vPik[i].getState() < 17 && vPik[i].getState() < 20)++total;
				}
			}
			//fallamos intentando realizar un poder
			if (total > stateAction) {
				//Ruido de fallo
			}
			else {
				for (int i = 0; i < vPik.size(); ++i) if (lemmingsSelected[i] != -1) {
					vPik[lemmingsSelected[i]].doAction(request);
					lemmingsSelected[i] = -1;
				}
				powersBar.setSpendPowers(request, total);
			}
			powersBar.finishRequest();
		}
		for (unsigned int i = 0; i < vPik.size(); ++i) {
			vPik[i].update(deltaTime, inCentreX);
		}

		collisionLevel();
		/*if (winPikmins <= survived) {
			cout << " entro en el cout del Win" << endl;
			weWin = 1;
		}
		else*//* if (Time <= 0 || aliveLemmings <= 0) {
			//ost.stop();
			//gameOverSong.play();
			cout << " entro en el cout del lost" << endl;
			weWin = 1;
		}*/
		//else if (winPikmins == 10) weWin = 1;
		cout << "estoy haciendo update" << endl;
		deleteDeadPikmins();
		gameFinish(); //susituyo el if por la funcion
	}
}

void Level::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if (weLost == 1 || weWin == 1) {
		for (int i = 0; i < 2; ++i) {
			bool intersecta = loseQuads[0]->intersecta(mouseX, mouseY, loseMatrix[i]);
			if (intersecta &&  i == 0 && bRightButton) {
				stateRetry = 2;
				overRetry = true;
			}
			else if (intersecta && i == 0) {
				stateRetry = 1;
				overRetry = false;
			}
			else if (i == 0) stateRetry = 0;
			else if (intersecta && i == 1 && bRightButton) {
				overBackMenu = true;
				stateBackMenu = 2;
			}
			else if (intersecta && i == 1) {
				stateBackMenu = 1;
				overBackMenu = false;
			}
			else if (i == 1) stateBackMenu = 0;
		}
	}
	else if (paused == 1) {
		for (int i = 0; i < vPik.size(); ++i) vPik[i].mouseMoved(mouseX, mouseY, bLeftButton);
		bool intersectaFast = fastForwardQuad->intersecta(mouseX, mouseY, fastForwardModel);
		mapPressed = powersBar.mouseMoved(mouseX, mouseY, bLeftButton);
		if (mapPressed) {
			inCentreX = (mouseX*(1218.f/960.f) - (524.f / 648.f)*1218.f)*2.5;
			projection2 = glm::ortho(offsetxLevel + inCentreX, float(offsetxLevel + 320.f - 1 + inCentreX), float(160.f - 1 + 28), 0.f);
			projection = glm::ortho(float(inCentreX), float(CAMERA_WIDTH - 1 + inCentreX), float(CAMERA_HEIGHT - 1 + 28), 0.f);
		}
	}
}

int Level::mouseRelease(int mouseX, int mouseY, int button)
{
	powersBar.mouseRelease(mouseX, mouseY, button);
	int aux = powersBar.getPowersBarRequest();
	if (aux == 11) Nuke();
	paused = powersBar.getPaused();
	if (weLost == 1) {
		for (int i = 0; i < 2; ++i) {
			bool intersecta = loseQuads[0]->intersecta(mouseX, mouseY, loseMatrix[i]);
			if (intersecta &&  i == 0) {
				//gameOverSong.stop();
				init(nLevel);
			}
			else if (intersecta && i == 1) {
				//ost.stop();
				//gameOverSong.stop();
				weWantToGoBack = 1;
			}
		}
	}
	else if (weWin == 1) {
		for (int i = 0; i < 2; ++i) {
			bool intersecta = loseQuads[0]->intersecta(mouseX, mouseY, loseMatrix[i]);
			if (intersecta &&  i == 0) {
				if (nLevel < 3) {
					//winSong.stop();
					init(nLevel + 1);
				}
				else {
					//winSong.stop();
					weWantToGoBack = 1;
				}
			}
			else if (intersecta && i == 1) {
				//winSong.stop();
				weWantToGoBack = 1;
			}
		}
	}
	else if (paused == 1) {
		bool intersectaFast = fastForwardQuad->intersecta(mouseX, mouseY, fastForwardModel);
		if (intersectaFast) {
			if (speed == 4) speed = 1;
			else speed *= 2;
		}
		for (int i = 0; i < vPik.size(); ++i) {
			vPik[i].mouseRelease(mouseX, mouseY, button);
		}
	}
	return nLevel + 1;
}

void Level::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if (!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/zetaVariable.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/zetaVariable.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	zetaTextProgram.init();
	zetaTextProgram.addShader(vShader);
	zetaTextProgram.addShader(fShader);
	zetaTextProgram.link();
	if (!zetaTextProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << zetaTextProgram.log() << endl << endl;
	}
	zetaTextProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

}
int Level::getSpeed() {
	return speed;
}

void Level::spawnPikmin(int tipus)
{
	if (out + survived < maxPikmins) {
		//PikminAux.init(glm::vec2(80, 50), simpleTexProgram, tipus);
		PikminAux.init(spawnPoint, simpleTexProgram, tipus, offsetxLevel);
		++actualment[tipus];
		PikminAux.setMapColor(&colorTexture);
		PikminAux.setMapMask(&maskTexture);
		lemmingsSelected[vPik.size()] = -1;
		++out;
		vPik.push_back(PikminAux);
	}
}

int Level::WantToGoBack() {
	return weWantToGoBack;
}

void Level::deleteDeadPikmins() { //NEW
	for (int i = 0; i < vPik.size(); ++i) {
		if (vPik[i].isDead())
			vPik.erase(vPik.begin() + i);
	}
}

void Level::collisionLevel() { //NEW
	for (int i = 0; i < vPik.size(); ++i) {
		bool doesHit = vPik[i].hitLevel(exitBox);
		if (doesHit) {
			++survived;
			vPik.erase(vPik.begin() + i);
		}
		if (nLevel == 3) {
			if (vPik[i].hitLevel(trampaBox)) {
				vPik.erase(vPik.begin() + i);
			}
			else if (vPik[i].hitLevel(trampaBox)) {
				vPik.erase(vPik.begin() + i);
			}
		}
	}
}

void Level::gameFinish() { //NEW
	if (vPik.size() == 0) {
		//if (out == maxPikmins) {
			if ((float(survived) / float(maxPikmins) * 100) > requiredPercent)
				weWin = 1;
			else
				weLost = 1;
		//}
	}
	if (Time == 0) {
		if ((float(survived) / float(maxPikmins) * 100) > requiredPercent) {
			weWin = 1;
		}
		else
			weLost = 1;
	}
}

void Level::Nuke() { //NEW
	for (int i = 0; i < vPik.size(); ++i) {
		vPik[i].explode();
	}
	gameFinish();
}
