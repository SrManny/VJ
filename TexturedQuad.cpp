#include <GL/glew.h>
#include <GL/gl.h>
#include "TexturedQuad.h"


TexturedQuad *TexturedQuad::createTexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program)
{
	TexturedQuad *quad = new TexturedQuad(geom, texCoords, program);

	return quad;
}


TexturedQuad::TexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program)
{
	float vertices[24] = { geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y };

	boundingBox[0] = geom[0][0];
	boundingBox[1] = geom[0][1];
	boundingBox[2] = geom[1][0];
	boundingBox[3] = geom[1][1];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

void TexturedQuad::render(const Texture &tex) const
{
	glEnable(GL_TEXTURE_2D);
	tex.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}


bool TexturedQuad::intersecta(int mouseX, int mouseY, glm::mat4 model) {
	int* boundingBox = getboundingBox();
	float xratio = 960 / CAMERA_WIDTH;
	float yratio = (480 + 84) / (CAMERA_HEIGHT + 28);
	glm::vec4 min = model * glm::vec4(boundingBox[0], boundingBox[1], 0.5f, 1.0f);
	glm::vec4 max = model * glm::vec4(boundingBox[2], boundingBox[3], 0.5f, 1.0f);
	min.x = min.x*xratio;
	min.y = min.y*yratio;
	max.x = max.x*xratio;
	max.y = max.y*yratio;
	return (((mouseX < max[0]) && (mouseX > min[0])) && ((mouseY < max[1]) && (mouseY > min[1])));
}
void TexturedQuad::free()
{
	glDeleteBuffers(1, &vbo);
}

int* TexturedQuad::getboundingBox() {
	return boundingBox;
}
