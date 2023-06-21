#pragma once

#include "pgr.h"
#include "object.h"
#include <ft2build.h>
#include FT_FREETYPE_H 

typedef struct _Character{
	GLuint texture;
	GLuint bearingX;
	GLuint bearingY;
	GLuint wigth;
	GLuint height; 
	GLuint advance;
}Character;

class CharactersDraw
{
public:

	CharactersDraw(ShaderProgram* shdrPrg, float monitorWidth=500.0f, float monitorHeight= 500.0f);
	~CharactersDraw();

	void update();
	void draw(std::string text, float posX, float posY, float scale, glm::vec3 color);
	void mapAllCharacters();

private:
	FT_Library ft;
	FT_Face face;
	Character characters[128];
	ObjectGeometry* geometry;
	ShaderProgram* shaderProgram;
	glm::mat4 projection;
};