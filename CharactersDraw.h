#pragma once

#include "pgr.h"
#include "object.h"
#include <ft2build.h>
#include FT_FREETYPE_H 

// structure for character properties
typedef struct _Character{
	GLuint texture;
	int bearingX;
	int bearingY;
	int wigth;
	int height; 
	int advance;
}Character;

class CharactersDraw
{
public:

	// init alphabet to print on the screen
	CharactersDraw(ShaderProgram* shdrPrg, float monitorWidth=500.0f, float monitorHeight= 500.0f);
	// delete object
	~CharactersDraw();


	void update();
	// draw message on the screen
	void draw(std::string text, float posX, float posY, float scale, glm::vec3 color);
	// init alphabet
	void mapAllCharacters();

private:
	FT_Library ft;            
	FT_Face face;            ///< font
	Character characters[128];  ///<all characters
	ObjectGeometry* geometry;	///< geometry
	ShaderProgram* shaderProgram; ///< shader program
	glm::mat4 projection;        ///< projection matrix
};