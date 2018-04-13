#include "PowersBar.h"
#include <glm/gtc/matrix_transform.hpp>


PowersBar::PowersBar()
{
}


PowersBar::~PowersBar()
{
}

void PowersBar::initPowerMatrixs() {
	powerModelMatrix = glm::mat4(1.0f);
	powerModelMatrix = glm::translate(powerModelMatrix, glm::vec3(CAMERA_WIDTH / 2.f, CAMERA_HEIGHT + (28.f / 2.f), 0.f));
	powerModelMatrix = glm::translate(powerModelMatrix, glm::vec3(-CAMERA_WIDTH / 2.f, -28.f / 2.f, 0.f));
	//startInvMatrix = glm::inverse(startModel);
	mapModelMatrix = glm::mat4(1.0f);
	//Hay que trasladar en x 524 pixeles (81.01 % de la imagen) en 320 son 259.26
	mapModelMatrix = glm::translate(mapModelMatrix, glm::vec3((524.f/648.f)*320.f,(55.f/84.f)*28.f+160.f, 0.f));
	mapModelMatrix = glm::translate(mapModelMatrix, glm::vec3(-mapsizeW / 2.f, -mapsizeH/2.f, 0.f));

	mapSelectedMatrix = glm::mat4(1.0f);
	//Hay que trasladar en x un numero a ojo de pixeles
	mapSelectedMatrix = glm::translate(mapSelectedMatrix, glm::vec3((524.f / 648.f)*float(CAMERA_WIDTH), (55.f / 84.f)*28.f + 160.f, 0.f));
	mapSelectedMatrix = glm::translate(mapSelectedMatrix, glm::vec3(-((48.f / 648.f)*float(CAMERA_WIDTH))/2.0f, -mapsizeH / 2.f, 0.f));
	for (int i = 0; i < 12; ++i) {
		buttonsMatrix[i] = glm::mat4(1.0f);
		buttonsMatrix[i] = glm::translate(buttonsMatrix[i], glm::vec3(((6.f + 15.f+ i * 32.f) / 648.f)*float(CAMERA_WIDTH), (55.f / 84.f)*28.f + 160.f, 0.f));
		buttonsMatrix[i] = glm::translate(buttonsMatrix[i], glm::vec3(-((30.f / 648.f)*float(CAMERA_WIDTH)) / 2.0f, -(float(28.f)* (46.f / 84.f)) / 2.f, 0.f));
	}
}

void PowersBar::loadActionButtonsImages() {
	actionButton[0].loadFromFile("images/Buttons/NukeButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[0].setMinFilter(GL_NEAREST);
	actionButton[0].setMagFilter(GL_NEAREST);
	
	actionButton[1].loadFromFile("images/Buttons/NukeButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[1].setMinFilter(GL_NEAREST);
	actionButton[1].setMagFilter(GL_NEAREST);
	
	actionButton[2].loadFromFile("images/Buttons/ClimbButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[2].setMinFilter(GL_NEAREST);
	actionButton[2].setMagFilter(GL_NEAREST);
	
	actionButton[3].loadFromFile("images/Buttons/Explodebutton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[3].setMinFilter(GL_NEAREST);
	actionButton[3].setMagFilter(GL_NEAREST);
	
	actionButton[4].loadFromFile("images/Buttons/ColorBlockButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[4].setMinFilter(GL_NEAREST);
	actionButton[4].setMagFilter(GL_NEAREST);
	
	actionButton[5].loadFromFile("images/Buttons/BlockButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[5].setMinFilter(GL_NEAREST);
	actionButton[5].setMagFilter(GL_NEAREST);
	
	actionButton[6].loadFromFile("images/Buttons/BuildIcon.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[6].setMinFilter(GL_NEAREST);
	actionButton[6].setMagFilter(GL_NEAREST);
	
	actionButton[7].loadFromFile("images/Buttons/NukeButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[7].setMinFilter(GL_NEAREST);
	actionButton[7].setMagFilter(GL_NEAREST);
	
	actionButton[8].loadFromFile("images/Buttons/BashButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[8].setMinFilter(GL_NEAREST);
	actionButton[8].setMagFilter(GL_NEAREST);
	
	actionButton[9].loadFromFile("images/Buttons/DigButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[9].setMinFilter(GL_NEAREST);
	actionButton[9].setMagFilter(GL_NEAREST);

	actionButton[10].loadFromFile("images/Buttons/NukeButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[10].setMinFilter(GL_NEAREST);
	actionButton[10].setMagFilter(GL_NEAREST);

	actionButton[11].loadFromFile("images/Buttons/NukeButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	actionButton[11].setMinFilter(GL_NEAREST);
	actionButton[11].setMagFilter(GL_NEAREST);
}

void PowersBar::init() {
	level = 0;
	overMap = false;
	initShaders();
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1 + 29), 0.f);
	mapsizeW = float(CAMERA_WIDTH)*208.f / 648.f;
	mapsizeH = float(28.f)* (42.f/ 84.f);
	initPowerMatrixs();
	//De los 28 que ocupa de altura el powerBar el map ocupa 13.3333f
	//De los 320 que ocupa de anchura el powerBar el map ocupa 69.333f
	glm::vec2 geomMaps[2] = { glm::vec2(0.f, 0.f), glm::vec2(mapsizeW, mapsizeH) };
	//Bars ocupa 320x28
	glm::vec2 geomPowers[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(28)) };
	glm::vec2 geomMapSelected[2] = { glm::vec2(0.f, 0.f), glm::vec2((48.f/ 648.f)*float(CAMERA_WIDTH), mapsizeH)};
	glm::vec2 geomActionButton[2] = { glm::vec2(0.f, 0.f), glm::vec2((30.f/648.f)*float(CAMERA_WIDTH), float(28.f)* (46.f / 84.f)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	barQuad = TexturedQuad::createTexturedQuad(geomPowers, texCoords, zetaTextProgram);
	mapQuad = TexturedQuad::createTexturedQuad(geomMaps, texCoords, zetaTextProgram);
	mapSelectedQuad = TexturedQuad::createTexturedQuad(geomMapSelected, texCoords, zetaTextProgram);
	for (int i = 0; i < 12; ++i) actionButtonQuad[i] = TexturedQuad::createTexturedQuad(geomActionButton, texCoords, zetaTextProgram);
	loadActionButtonsImages();

	barTexture.loadFromFile("images/powers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	barTexture.setMinFilter(GL_NEAREST);
	barTexture.setMagFilter(GL_NEAREST);
	map[0].loadFromFile("images/map1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	map[0].setMinFilter(GL_NEAREST);
	map[0].setMagFilter(GL_NEAREST);

	map[1].loadFromFile("images/map1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	map[1].setMinFilter(GL_NEAREST);
	map[1].setMagFilter(GL_NEAREST);

	map[2].loadFromFile("images/map1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	map[2].setMinFilter(GL_NEAREST);
	map[2].setMagFilter(GL_NEAREST);

	mapSelectedTexture.loadFromFile("images/mapSelected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mapSelectedTexture.setMinFilter(GL_NEAREST);
	mapSelectedTexture.setMagFilter(GL_NEAREST);

}

//Devuelve si intersecta el punto xy con el Quad de coordenadas min max
bool PowersBar::intersecta(int x, int y, glm::vec4 min, glm::vec4 max) {
	return (((x < max[0]) && (x > min[0])) && ((y < max[1]) && (y > min[1])));
}
int PowersBar::mouseMoved(int mouseX, int mouseY, bool bLeftButton) {
	int* boundingBox = mapQuad->getboundingBox();
	float xratio = 960 / CAMERA_WIDTH;
	float yratio = (480 + 84) / (CAMERA_HEIGHT + 28);
	glm::vec4 min = mapModelMatrix * glm::vec4(boundingBox[0], boundingBox[1], 0.5f, 1.0f);
	glm::vec4 max = mapModelMatrix * glm::vec4(boundingBox[2], boundingBox[3], 0.5f, 1.0f);
	//cout << "Coordenadas del quad selected: " << endl;
	//cout << "Minimos " << min.x << " " << min.y << " Maximos " << max.x << " " << max.y << endl;
	//float offset = ((48.f / 2.0f)/648.f)*CAMERA_WIDTH;
	float minMapX = (444.f / 648.f)*float(CAMERA_WIDTH)*xratio;
	float maxMapX = (605.f / 648.f)*float(CAMERA_WIDTH)*xratio;
	min.x = min.x*xratio;
	min.y = min.y*yratio;
	max.x = max.x*xratio;
	max.y = max.y*yratio;
	//if (!overMap) overMap = intersecta(mouseX, mouseY, min, max);
	bool aux = intersecta(mouseX, mouseY, min, max);
	if ((aux && bLeftButton) || (!aux && overMap)) {
		overMap = true;
		mapSelectedMatrix = glm::mat4(1.0f);
		if (mouseX > maxMapX) mouseX = maxMapX;
	    if (mouseX < minMapX) mouseX = minMapX;
		//Hay que trasladar en x un numero a ojo de pixeles
		mapSelectedMatrix = glm::translate(mapSelectedMatrix, glm::vec3(mouseX / 3.f, (55.f / 84.f)*28.f + 160.f, 0.f));
		mapSelectedMatrix = glm::translate(mapSelectedMatrix, glm::vec3(-((48.f / 648.f)*float(CAMERA_WIDTH)) / 2.0f, -mapsizeH / 2.f, 0.f));
		return true;
	}
	return false;
}
int PowersBar::mouseRelease(int button) {
	overMap = false;
	return 2.f;
}
void PowersBar::render() {
	zetaTextProgram.use();
	zetaTextProgram.setUniformMatrix4f("projection", projection);
	zetaTextProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	zetaTextProgram.setUniform2f("zeta", 0.f, 0.5f);
	zetaTextProgram.setUniformMatrix4f("modelview", powerModelMatrix);
	barQuad->render(barTexture);
	zetaTextProgram.setUniformMatrix4f("modelview", mapModelMatrix);
	mapQuad->render(map[level]);
	zetaTextProgram.setUniformMatrix4f("modelview", mapSelectedMatrix);
	mapSelectedQuad->render(mapSelectedTexture);
	zetaTextProgram.setUniform2f("zeta", 0.f, 0.0f);
	for (int i = 0; i < 12; ++i) {
		zetaTextProgram.setUniformMatrix4f("modelview", buttonsMatrix[i]);
		actionButtonQuad[i]->render(actionButton[i]);
	}
}
void PowersBar::initShaders() {
	Shader vShader, fShader;

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