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
	fastForwardModel = glm::mat4(1.0f);
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(305.f, 30.f / 2.f, 0.f));
	fastForwardModel = glm::translate(fastForwardModel, glm::vec3(-25.f / 2.f, -25.f / 2.f, 0.f));

	spawnModel = glm::mat4(1.0f); 
	spawnModel = glm::translate(spawnModel, glm::vec3(spawnPoint[0]+12,spawnPoint[1], 0.f));
	
	exitModel = glm::mat4(1.0f);
	exitModel = glm::translate(exitModel, glm::vec3(exitPoint[0] + 12, exitPoint[1], 0.f));

	pausedMatrix = glm::mat4(1.0f);
	pausedMatrix = glm::translate(pausedMatrix, glm::vec3(150.f, 80.f, 0.f));
	pausedMatrix = glm::translate(pausedMatrix, glm::vec3(-100.f / 2.f, -50.f / 2.f, 0.f));

	for (int i = 0; i < 2; ++i) {
		loseMatrix[i] = glm::mat4(1.0f);
		loseMatrix[i] = glm::translate(loseMatrix[i], glm::vec3(150.f, 80.f + i*40.f, 0.f));
		loseMatrix[i] = glm::translate(loseMatrix[i], glm::vec3(-100.f / 2.f, -40.f / 2.f, 0.f));
	}

	float centreX = float(CAMERA_WIDTH)*16.f / 648.f;
	float centreY = 28.f*35.f / 84.f;
	for (int i = 0; i < 3; ++i) {
		timeMatrix[i] = glm::mat4(1.0f);
		timeMatrix[i] = glm::translate(timeMatrix[i], glm::vec3(((576.f + 16.f*i) / 648.f)*320.f, 28.f*(14.f / 84.f) + 160.f, 0.f));
		timeMatrix[i] = glm::translate(timeMatrix[i], glm::vec3(-centreX / 2.f, -centreY / 2.f, 0.f));
		outLemmingsMatrix[i] = glm::mat4(1.0f);
		outLemmingsMatrix[i] = glm::translate(outLemmingsMatrix[i], glm::vec3(((290.f+16.f*i)/648.f)*320.f, 28.f*(14.f/84.f)+160.f, 0.f));
		outLemmingsMatrix[i] = glm::translate(outLemmingsMatrix[i], glm::vec3(-centreX / 2.f, -centreY / 2.f, 0.f));
		inLemmingsMatrix[i] = glm::mat4(1.0f);
		inLemmingsMatrix[i] = glm::translate(inLemmingsMatrix[i], glm::vec3(((415.f + 16.f*i) / 648.f)*320.f, 28.f*(14.f / 84.f) + 160.f, 0.f));
		inLemmingsMatrix[i] = glm::translate(inLemmingsMatrix[i], glm::vec3(-centreX/2.f,-centreY/ 2.f, 0.f));
	}
}

void Level::keypressed(int key) {
	switch (key) {
		case 101: //EXPLOSION, Key: e
			for (int i = 0; i < vPik.size(); ++i) vPik[i].keyPressed(key);
			break;
		case 49: //BASH, Key: 1
			for (int i = 0; i < vPik.size(); ++i) vPik[i].keyPressed(key);
			break;
		case 50: //BUILD, Key: 2
			for (int i = 0; i < vPik.size(); ++i) vPik[i].keyPressed(key);
			break;
		case 51: //DIG, Key: 3
			for (int i = 0; i < vPik.size(); ++i) vPik[i].keyPressed(key);
			break;
		case 122: //SPAWN RED, KEY: z
			spawnPikmin(0);
			break;
		case 120: //SPAWN BLUE, KEY: x
			spawnPikmin(1);
			break;
		case 99: //SPAWN YELLOW, KEY: c
			spawnPikmin(2);
			break;
		case 118: //SPAWN PURPLE, KEY: v
			spawnPikmin(3);
			break;
	}
}

void Level::keyreleased(int key) {

		lemmings[0].keyReleased(key);
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
	int aux1 = (survived / winPikmins)*100;
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
	paused = 1; weLost = 0, weWin = 0, weWantToGoBack = 0; vPik.clear();
	this->nLevel = nLevel;
	stateBackMenu = 0;
	stateRetry = 0;overBackMenu = false; overRetry = false;
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
	maskTexture.loadFromFile(LevelMaskLocation, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	//Incializamos imagen del spawn de los Pikmn
	spawn.loadFromFile("images/Inici.png",TEXTURE_PIXEL_FORMAT_RGBA);
	spawn.setMinFilter(GL_NEAREST);
	spawn.setMagFilter(GL_NEAREST);
	glm::vec2 texCoordsSpawn[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
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
	glm::vec2 geomLose[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(100), float(40)) };
	loseTextures[0].loadFromFile("images/Buttons/Retry.png", TEXTURE_PIXEL_FORMAT_RGBA);
	loseTextures[0].setMinFilter(GL_NEAREST);
	loseTextures[0].setMagFilter(GL_NEAREST);
	loseTextures[1].loadFromFile("images/Buttons/BackToMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	loseTextures[1].setMinFilter(GL_NEAREST);
	loseTextures[1].setMagFilter(GL_NEAREST);
	for (int i = 0; i < 3; ++i) {
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, float(i) / 3.f), glm::vec2(1.f,float(i)/3.f+ 1.f/3.f) };
		loseQuads[i] = TexturedQuad::createTexturedQuad(geomLose, texCoords2, zetaTextProgram);
	}
	//Inicializamos el menu y el powerBar

	projectionButtons = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	projection2 = glm::ortho(offsetxLevel, float(offsetxLevel + 320.f - 1), float(160.f - 1 + 28), 0.f);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 28), 0.f);
	glm::vec2 geomNumbers[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH)*16.f / 648.f,28.f*35.f / 84.f) };
	for (int i = 0; i < 10; ++i) {
		glm::vec2 texCoordNum[2] = { glm::vec2(i*24.f/240.f, 0.f), glm::vec2((i*24.f+24.f) / 240.f,1.f) };
		numbersQuad[i] = TexturedQuad::createTexturedQuad(geomNumbers, texCoordNum, zetaTextProgram);	//Inicializamos el menu y el powerBar
	}
	numbers.loadFromFile("images/Buttons/numeros3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbers.setMinFilter(GL_NEAREST);
	numbers.setMagFilter(GL_NEAREST);

	/*for (int i = 0; i < 1; ++i) {
		int aleatorio = rand() % 4;
		lemmings[i].init(glm::vec2(80, 50), simpleTexProgram,aleatorio );
		++actualment[lemmings[i].getTipus()];
		lemmings[i].setMapMask(&maskTexture);
		lemmingsSelected[i] == false;
		++out;
	}*/
	int aleatorio = rand() % 4;
	spawnPikmin(aleatorio);

}

void Level::setValues() {
	if (nLevel == 1) {
		maxPikmins = 50;
		out = 0;
		LevelTextureLocation = "images/fun1.png";
		LevelMaskLocation = "images/fun1_mask.png";
		spawnPoint = glm::vec2(60, 30);
		exitPoint = glm::vec2(216, 100);
		Time = 100;
		survived = 0;
		winPikmins = 10;
		offsetxLevel = 120.f;
		sizeOfLevel = 512.f;
	}
	else if (nLevel == 2) {
		maxPikmins = 50;
		out = 0;
		LevelTextureLocation = "images/fun1.png";
		LevelMaskLocation = "images/fun1_mask.png";
		spawnPoint = glm::vec2(60, 30);
		exitPoint = glm::vec2(216, 100);
		Time = 650;
		survived = 10;
		winPikmins = 10;
		offsetxLevel = 120.f;
		sizeOfLevel = 512.f;
	}

	else if (nLevel == 3) {
		maxPikmins = 50;
		out = 0;
		LevelTextureLocation = "images/fun1.png";
		LevelMaskLocation = "images/fun1_mask.png";
		spawnPoint = glm::vec2(60, 30);
		exitPoint = glm::vec2(216, 100);
		Time = 650;
		survived = 10;
		winPikmins = 10;
		offsetxLevel = 120.f;
		sizeOfLevel = 512.f;
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
	powersBar.render();
	maskedTexProgram.use();
	glm::mat4 modelview;
	//projection2 = glm::translate(projection2, glm::vec3(currentTime / 2000.f, 0.f, 0.f));
	maskedTexProgram.setUniformMatrix4f("projection", projection2);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	//Hay que trasladar en x 524 pixeles (81.01 % de la imagen) en 320 son 259.26
	//modelview = glm::translate(modelview, glm::vec3(currentTime/1000.f, 0.f, 0.f));
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);
	

	simpleTexProgram.use();

	//projection = glm::translate(projection, glm::vec3(currentTime / 2000.f, 0.f, 0.f));
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	//modelview = glm::translate(modelview, glm::vec3(currentTime / 1000.f, 0.f, 0.f));
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	//for (int i = 0; i < 1; ++i) lemmings[i].render(projection);
	for (int i = 0; i < vPik.size(); ++i) {
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
	else if (weLost == 1) {
		for (int i = 0; i < 2; ++i) {
			zetaTextProgram.setUniformMatrix4f("modelview", loseMatrix[i]);
			if (i == 0) loseQuads[stateRetry]->render(loseTextures[i]);
			else loseQuads[stateBackMenu]->render(loseTextures[i]);
		}
	}
	else if (weWin == 1) {
		zetaTextProgram.setUniformMatrix4f("modelview", pausedMatrix);
		pausedQuad->render(winTexture);
	}
	//Renderizamos el spaw
	zetaTextProgram.setUniformMatrix4f("projection", projection);
	zetaTextProgram.setUniformMatrix4f("modelview", spawnModel);
	spawnQuad->render(spawn);
	//Renderizamos la salida 
	zetaTextProgram.setUniformMatrix4f("modelview", exitModel);
	exitQuad->render(exit);

	//renderTime();
	//for (unsigned int i = 0; i < lemmings.size(); ++i);
		//lemmings[i].render();
}

void Level::update(int deltaTime)
{
	float aux = float(deltaTime) * float(speed);
	second += 1 / aux;
	if (winPikmins <= survived) {
		weWin = 1;
		if (second > 1) {
			second = 0;
			--winTime;
			if (winTime <= 0) {
				if (nLevel < 3) init(nLevel + 1);
				else weWantToGoBack = 1;
			}
		}
	}
	else if (paused == 1 && !weLost) {
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
				lemmingsSelected[i] = true;
				++total;
			}
		}
		//fallamos intentando realizar un poder
		if (total > stateAction) {
			//Ruido de fallo
		}
		else {
			for (int i = 0; i < vPik.size(); ++i) if (lemmingsSelected[i]) {
				lemmingsSelected[i] = false;
				vPik[i].doAction(request);
			}
			powersBar.setSpendPowers(request, total);
		}
		powersBar.finishRequest();
	}
	//ola.update(deltaTime, inCentreX);
	/*for (unsigned int i = 0; i < 1; ++i)
		lemmings[i].update(deltaTime,inCentreX);*/
	for (unsigned int i = 0; i < vPik.size(); ++i) {
		vPik[i].update(deltaTime, inCentreX);
	}
	 if (Time <= 0 || aliveLemmings <= 0) weLost = 1;
	}
}

void Level::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	//for (int i = 0; i < 1; ++i ) lemmings[i].mouseMoved(mouseX, mouseY, bLeftButton);
	if (weLost == 1) {
		cout << "OLA HE PERDIDO" << endl;
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
			inCentreX = mouseX - (524.f / 648.f)*960.f;
			projection2 = glm::ortho(120.f + inCentreX, float(120.f + 320.f - 1 + inCentreX), float(160.f - 1 + 28), 0.f);
			projection = glm::ortho(0.f + inCentreX, float(CAMERA_WIDTH - 1 + inCentreX), float(CAMERA_HEIGHT - 1 + 28), 0.f);
		}
	}
}

void Level::mouseRelease(int mouseX, int mouseY, int button)
{
	powersBar.mouseRelease(mouseX, mouseY,button);
	paused = powersBar.getPaused();
	if (weLost == 1) {
		for (int i = 0; i < 2; ++i) {
			bool intersecta = loseQuads[0]->intersecta(mouseX, mouseY, loseMatrix[i]);
			if (intersecta &&  i == 0) init(nLevel);
			else if (intersecta && i == 1) weWantToGoBack = 1;
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
		PikminAux.init(spawnPoint, simpleTexProgram, tipus);
		++actualment[tipus];
		PikminAux.setMapColor(&colorTexture);
		PikminAux.setMapMask(&maskTexture);
		lemmingsSelected[0] == false;
		++out;
	}
	vPik.push_back(PikminAux);
}

int Level::WantToGoBack() {
	return weWantToGoBack;
}